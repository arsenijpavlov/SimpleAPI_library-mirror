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
struct ConfigTypeTraits<T, typename std::enable_if<is_container_as_bitset<T>::value>::type>
{
    static bool load(const Config& config, const std::string& key, T& field)
    {
        // std::cout << "[debug] load container(as bitset) key=\"" << key << "\"" << std::endl;

        if(config.isMapContainer() && config.containsKey(key)) {
            const Config& ck = config[key];
            for(size_t i = 0; i < field.size(); i++) {
                if(ck.size() > i)
                {
                    field[i] = ck[i].get<bool>();
                } else {
                    // массив меньше, заполняем дефолтом для этого типа
                    field[i] = false;
                }
            }
        }

        return true;
    }

    template<typename Lambda, typename... Args,
             typename std::enable_if<is_variadic_lambda_callable<Lambda, T&&, std::string&&>::value, int>::type = 0>
    static bool load(const Config& config, const std::string& key, T& field, Lambda lambda, Args&&... args)
    {
        // std::cout << "[debug] load container(as bitset) key=\"" << key << "\"" << std::endl;

        if(config.isMapContainer() && config.containsKey(key)) {
            const Config& ck = config[key];
            std::bitset<field.size()> temp_value;
            for(size_t i = 0; i < field.size(); i++) {
                if(ck.size() > i)
                {
                    temp_value[i] = ck[i].get<bool>();
                } else {
                    // массив меньше, заполняем дефолтом для этого типа
                    temp_value[i] = false;
                }
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
        // std::cout << "[debug] save container(as bitset) key=\"" << key << "\"" << std::endl;

        for(size_t i = 0; i < field.size(); i++) {
            // ключ дальше нельзя передать - создаст новую вложенность
            Config temp = Config(ValueType::eBool, field[i]);
            config[key].push_back(temp);
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
