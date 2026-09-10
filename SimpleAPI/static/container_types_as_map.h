#pragma once

// NOTE: "IWYU pragma: keep" спрячет лишнее предупреждение от clangd
#include "base.h"          // IWYU pragma: keep
#include "type_checkers.h" // IWYU pragma: keep
#include <string>
#include <type_traits>
#include "../config/Config.h"
#include "../utils/number_utils.h"


namespace simpleapi {
namespace tools {

// правило для определения типов-контейнеров (vector, queue и т.д.)
// std::string не считается за контейнер и обрабатывается как цельный элемент
template<typename T>
struct ConfigTypeTraits<T, typename std::enable_if<is_container_as_map<T>::value
                                                   || is_container_as_unordered_map<T>::value>::type>
{
    using KeyType   = typename T::key_type;
    using ValueType = typename T::value_type;

    // явно запрещаем любые неподдерживаемые форматы ключа
    static_assert(std::is_arithmetic<typename T::key_type>::value
                      || std::is_convertible<KeyType, std::string>::value,
                  "SimpleAPI error: in map containers, the key must be in numeric, bool or string format");

    //---------------
    // локальные обработчики-валидаторы для KeyType (геттеры из KeyType в Config)
    template <typename K>
    static typename std::enable_if<std::is_same<K, std::string>::value,
                                   std::string>::type
    extract_key(const K& key)
    {
        return key;
    }
    template <typename K>
    static typename std::enable_if<std::is_integral<K>::value && !std::is_same<K, bool>::value,
                                   std::string>::type
    extract_key(const K& key)
    {
        return utils::ToString(key);
    }
    template <typename K>
    static typename std::enable_if<std::is_floating_point<K>::value,
                                   std::string>::type
    extract_key(const K& key)
    {
        return utils::ToString(key);
    }
    template <typename K>
    static typename std::enable_if<std::is_same<K, bool>::value,
                                   std::string>::type
    extract_key(const K& key)
    {
        return utils::ToString(key);
    }
    //---------------
    // локальные парсеры для KeyType (сеттеры из Config в KeyType)
    template <typename K>
    static typename std::enable_if<std::is_same<K, std::string>::value,
                                   K>::type
    parse_key(const std::string& key_str)
    {
        return key_str;
    }
    template <typename K>
    static typename std::enable_if<std::is_integral<K>::value && !std::is_same<K, bool>::value,
                                   K>::type
    parse_key(const std::string& key_str)
    {
        try {
            return static_cast<K>(utils::CreateLLongFromString(key_str));
        } catch(...)
        {
            return 0; // невалидный ввод не должен ломать работу программы
        }
    }
    template <typename K>
    static typename std::enable_if<std::is_floating_point<K>::value,
                                   K>::type
    parse_key(const std::string& key_str)
    {
        try {
            return static_cast<K>(utils::CreateLDoubleFromString(key_str));
        } catch(...)
        {
            return 0.f; // невалидный ввод не должен ломать работу программы
        }
    }
    template <typename K>
    static typename std::enable_if<std::is_same<K, bool>::value,
                                   K>::type
    parse_key(const std::string& key_str)
    {
        try {
            return utils::CreateBoolFromString(key_str);
        } catch(...)
        {
            return false; // невалидный ввод не должен ломать работу программы
        }
    }
    //---------------

    static bool load(const Config& config, const std::string& key, T& field)
    {
        // std::cout << "[debug] load container(as map) key=\"" << key << "\"" << std::endl;

        using Type = typename T::mapped_type;

        if(config.isMapContainer() && config.containsKey(key)) {
            field.clear();
            const Config& ck = config[key];
            size_t counter = 0;
            for(auto& c : ck.getNamedRange()) {
                // рекурсивно вызываем traits(признаки) для каждого из элементов
                Type item_temp_value;

                if(!Loader((*c.second.get()), item_temp_value)) {
                    static_config_error_str += "inner loader for [" + key + "] failed\n";
                    return false;
                }

                field.insert(std::make_pair(parse_key<KeyType>(c.first), item_temp_value));
            }
        }

        return true;
    }

    template<typename Lambda, typename... Args,
             typename std::enable_if<is_variadic_lambda_callable<Lambda, T&&, std::string&&>::value, int>::type = 0>
    static bool load(const Config& config, const std::string& key, T& field, Lambda lambda, Args&&... args)
    {
        // std::cout << "[debug] load container(as map) key=\"" << key << "\"" << std::endl;

        using Type = typename T::mapped_type;

        if(config.isMapContainer() && config.containsKey(key)) {
            const Config& ck = config[key];
            size_t counter = 0;
            T temp_value;
            for(auto& c : ck.getNamedRange()) {
                // рекурсивно вызываем traits(признаки) для каждого из элементов
                Type item_temp_value;

                if(!Loader((*c.second.get()), item_temp_value)) {
                    static_config_error_str += "inner loader for [" + key + "] failed\n";
                    return false;
                }

                temp_value.insert(std::make_pair(parse_key<KeyType>(c.first), item_temp_value));
            }

            if(!ExecuteValidator(lambda, temp_value, key)) {
                static_config_error_str = "validate for [" + key + "] failed\n";
                return false;
            }

            field = temp_value;
        }

        return true;
    }

    // комментарии учитываются только при записи
    static void save(Config& config, const std::string& key, const T& field,
                     const std::string& prefix_comment = "",
                     const std::string& suffix_comment = "")
    {
        // std::cout << "[debug] save container(as map) key=\"" << key << "\"" << std::endl;

        using Type = typename T::mapped_type;

        for(const auto& item : field) {
            // ключ дальше нельзя передать - создаст новую вложенность
            Config temp = Saver(item.second, prefix_comment, suffix_comment);
            config[key].push_back(extract_key(item.first), temp);
        }
        config[key].setComment(prefix_comment, suffix_comment);
    }

    static bool compare(const T& field, const T& other, const std::string& key)
    {
        return field == other;
    }
};

} // namespace simpleapi
} // namespace tools
