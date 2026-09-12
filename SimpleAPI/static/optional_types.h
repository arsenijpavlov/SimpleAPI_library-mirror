#pragma once

// NOTE: "IWYU pragma: keep" спрячет лишнее предупреждение от clangd
#include "base.h"          // IWYU pragma: keep
#include "type_checkers.h" // IWYU pragma: keep
#include <string>
#include <type_traits>
#include "../config/Config.h"


namespace simpleapi {
namespace tools {

template<typename T>
struct ConfigTypeTraits<T, typename std::enable_if<is_optional<T>::value>::type>
{
    /* данный класс работает сразу с двумя значениями
     *  1) <key>_optional - включение опции (true/false)
     *  2) <key>          - фактическое значение
     */
    static constexpr const char* opt_str = "_optional";

    static bool load(const Config& config, const std::string& key, T& field)
    {
        // std::cout << "[debug] load optional key=\"" << key << "\"" << std::endl;

        using Type = typename T::type; // извлекаем внутренний тип, хранящийся в Optional<>

        // считываем параметр опциональности
        if(config.isMapContainer() && config.containsKey(key + opt_str)) {
            if(config[key + opt_str].get<bool>()) {
                // считываем опциональный параметр
                if(config.containsKey(key)) {
                    field = config[key].get<Type>();
                }
            } else {
                // параметр отключён
                field.unset();
            }
        }

        return true;
    }

    template<typename Lambda, typename... Args,
             typename std::enable_if<is_variadic_lambda_callable<Lambda, T&&, std::string&&>::value, int>::type = 0>
    static bool load(const Config& config, const std::string& key, T& field, Lambda lambda, Args&&... args)
    {
        // std::cout << "[debug] load optional key=\"" << key << "\"" << std::endl;

        using Type = typename T::type; // извлекаем внутренний тип, хранящийся в Optional<>

        // считываем параметр опциональности
        if(config.isMapContainer() && config.containsKey(key + opt_str)) {
            if(config[key + opt_str].get<bool>()) {
                // считываем опциональный параметр
                if(config.containsKey(key)) {
                    Optional<Type> temp_value = config[key].get<Type>();

                    if(!ExecuteValidator(lambda, temp_value, key)) {
                        static_config_error_str = "validate for optional [" + key + "] failed\n";
                        return false;
                    }

                    field = temp_value;
                }
            } else {
                // параметр отключён
                field.unset();
            }
        }

        return true;
    }

    // комментарии учитываются только при записи
    static void save(Config& config, const std::string& key, const T& field,
                     const std::string& prefix_comment = "",
                     const std::string& suffix_comment = "")
    {
        // std::cout << "[debug] save optional key=\"" << key << "\"" << std::endl;

        using Type = typename T::type; // извлекаем внутренний тип, хранящийся в Optional<>

        // сохраняем параметр опциональности
        config[key + opt_str] = field.isValid();
        // сохраняем опциональные данные (null при отсутствии)
        // NOTE: нельзя обернуть в тернарный оператор из-за разных типов присваиваемого значения
        if(field.isValid()) {
            config[key] = field.value();
        } else {
            config[key] = Config(ValueType::eNull);
        }

        config[key].setComment(prefix_comment, suffix_comment);
    }

    static bool compare(const T& field, const T& other)
    {
        return field == other;
    }
};

} // namespace simpleapi
} // namespace tools
