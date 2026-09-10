#pragma once

// NOTE: "IWYU pragma: keep" спрячет лишнее предупреждение от clangd
#include "base.h"          // IWYU pragma: keep
#include "type_checkers.h" // IWYU pragma: keep
#include <string>
#include <type_traits>
#include "../config/Config.h"


namespace simpleapi {
namespace tools {

// правило для определения типов-контейнеров (vector, queue и т.д.)
// std::string не считается за контейнер и обрабатывается как цельный элемент
template<typename T>
struct ConfigTypeTraits<T, typename std::enable_if<is_container_as_queue<T>::value>::type>
{
    static bool load(const Config& config, const std::string& key, T& field)
    {
        // std::cout << "[debug] load container(as queue) key=\"" << key << "\"" << std::endl;

        using Type = typename T::value_type;

        if(config.isMapContainer() && config.containsKey(key)) {
            const Config& ck = config[key];
            T temp_value; // метода clear() нет, поэтому работаем со времянкой
            size_t counter = 0;
            for(const auto& c : ck.getRange()) {
                // рекурсивно вызываем traits(признаки) для каждого из элементов
                Type item_temp_value;

                if(!Loader((*c.get()), item_temp_value)) {
                    static_config_error_str += "inner loader for [" + key + "] failed\n";
                    return false;
                }

                temp_value.push(item_temp_value);
            }
            field.swap(temp_value);
        }

        return true;
    }

    template<typename Lambda, typename... Args,
             typename std::enable_if<is_variadic_lambda_callable<Lambda, T&&, std::string&&>::value, int>::type = 0>
    static bool load(const Config& config, const std::string& key, T& field, Lambda lambda, Args&&... args)
    {
        // std::cout << "[debug] load container(as queue) key=\"" << key << "\"" << std::endl;

        using Type = typename T::value_type;

        if(config.isMapContainer() && config.containsKey(key)) {
            const Config& ck = config[key];
            T temp_value; // метода clear() нет, поэтому работаем со времянкой
            size_t counter = 0;
            for(const auto& c : ck.getRange()) {
                // рекурсивно вызываем traits(признаки) для каждого из элементов
                Type item_temp_value;

                if(!Loader((*c.get()), item_temp_value)) {
                    static_config_error_str += "inner loader for [" + key + "] failed\n";
                    return false;
                }

                temp_value.push(item_temp_value);
            }

            if(!ExecuteValidator(lambda, temp_value, key)) {
                static_config_error_str = "validate for [" + key + "] failed\n";
                return false;
            }

            field.swap(temp_value);
        }

        return true;
    }

    // комментарии учитываются только при записи
    static void save(Config& config, const std::string& key, const T& field,
                     const std::string& prefix_comment = "",
                     const std::string& suffix_comment = "")
    {
        // std::cout << "[debug] save container(as queue) key=\"" << key << "\"" << std::endl;

        using Type = typename T::value_type;

        T temp_value = field; // итераторов нет, поэтому работаем со времянкой

        while(!temp_value.empty()) {
            Type item = temp_value.front();
            // ключ дальше нельзя передать - создаст новую вложенность
            Config temp = Saver(item, prefix_comment, suffix_comment);
            config[key].push_back(temp);
            temp_value.pop();
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
