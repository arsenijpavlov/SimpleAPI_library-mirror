#pragma once

// NOTE: "IWYU pragma: keep" спрячет лишнее предупреждение от clangd
#include "base.h"          // IWYU pragma: keep
#include "type_checkers.h" // IWYU pragma: keep
#include <type_traits>
#include "../config/Config.h"


namespace simpleapi {
namespace tools {

// правило для определения одиночных типов
template<typename T>
struct ConfigTypeTraits<T, typename std::enable_if<!is_config_struct<T>::value
                                                   && !is_container_as_vector<T>::value
                                                   && !is_container_as_set<T>::value
                                                   && !is_container_as_unordered_set<T>::value
                                                   && !is_container_as_array<T>::value
                                                   && !is_container_as_queue<T>::value
                                                   && !is_container_as_priority_queue<T>::value
                                                   && !is_container_as_stack<T>::value
                                                   && !is_container_as_bitset<T>::value
                                                   && !is_container_as_map<T>::value
                                                   && !is_container_as_unordered_map<T>::value
                                                   && !std::is_enum<T>::value
                                                   && !is_pair<T>::value
                                                   && !std::is_same<T, Config>::value
                                                   >::type>
{
    static bool load(const Config& config, const std::string& key, T& field)
    {
        // std::cout << "[debug] load simple key=\"" << key << "\"" << std::endl;

        if(config.isMapContainer() && config.containsKey(key)) {
            field = config[key].get<T>();
        }

        return true;
    }

    template<typename Lambda, typename... Args,
             typename std::enable_if<
                 is_variadic_lambda_callable<Lambda, const T&, const std::string&>::value, int>::type = 0>
    static bool load(const Config& config, const std::string& key, T& field, Lambda lambda, Args&&... args)
    {
        // std::cout << "[debug] load simple key=\"" << key << "\"" << std::endl;

        if(config.isMapContainer() && config.containsKey(key)) {
            T temp_value = config[key].get<T>();

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
        // std::cout << "[debug] save simple key=\"" << key << "\"" << std::endl;

        config[key] = field;
        config[key].setComment(prefix_comment, suffix_comment);
    }

    static bool compare(const T& field, const T& other)
    {
        return field == other;
    }
};

} // namespace simpleapi
} // namespace tools
