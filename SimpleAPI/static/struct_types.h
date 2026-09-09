#pragma once

// NOTE: "IWYU pragma: keep" спрячет лишнее предупреждение от clangd
#include "base.h"          // IWYU pragma: keep
#include "type_checkers.h" // IWYU pragma: keep
#include <type_traits>


namespace simpleapi {
namespace tools {

// правило для определения структур
template<typename T>
struct ConfigTypeTraits<T, typename std::enable_if<is_config_struct<T>::value
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
        // std::cout << "[debug] load structure key=\"" << key << "\"" << std::endl;

        T temp_value;
        if(config.isMapContainer() && config.containsKey(key)) {
            const Config& ck = config[key];
            if(!temp_value.loadConfig(ck)) {
                return false;
            }
        }

        field = temp_value;
        return true;
    }

    template<typename Lambda, typename... Args,
             typename std::enable_if<is_variadic_lambda_callable<Lambda, T&&, std::string&&>::value, int>::type = 0>
    static bool load(const Config& config, const std::string& key, T& field, Lambda lambda, Args&&... args)
    {
        // std::cout << "[debug] load structure key=\"" << key << "\"" << std::endl;

        if(config.isMapContainer() && config.containsKey(key)) {
            const Config& ck = config[key];
            T temp_value;
            if(!temp_value.loadConfig(ck)) {
                return false;
            }

            if(ExecuteValidator(lambda, temp_value, key)) {
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
        // std::cout << "[debug] save structure key=\"" << key << "\"" << std::endl;

        config[key] = field.saveConfig();
        config[key].setComment(prefix_comment, suffix_comment);
    }
};

} // namespace simpleapi
} // namespace tools
