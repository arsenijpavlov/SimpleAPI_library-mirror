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
struct ConfigTypeTraits<T, typename std::enable_if<is_container_as_vector<T>::value>::type>
{
    static bool load(const Config& config, const std::string& key, T& field)
    {
        // std::cout << "[debug] load container(as vector) key=\"" << key << "\"" << std::endl;

        using Type = typename T::value_type;

        if(config.isMapContainer() && config.containsKey(key)) {
            const Config& ck = config[key];
            field.resize(ck.size());
            size_t counter = 0;
            for(auto& item : field) {
                // рекурсивно вызываем traits(признаки) для каждого из элементов
                Type item_temp_value;

                if(!Loader(ck[counter++], item_temp_value)) {
                    return false;
                }

                item = item_temp_value;
            }
        }

        return true;
    }

    template<typename Lambda, typename... Args,
             typename std::enable_if<is_variadic_lambda_callable<Lambda, T&&, std::string&&>::value, int>::type = 0>
    static bool load(const Config& config, const std::string& key, T& field, Lambda lambda, Args&&... args)
    {
        // std::cout << "[debug] load container(as vector) key=\"" << key << "\"" << std::endl;

        using Type = typename T::value_type;

        if(config.isMapContainer() && config.containsKey(key)) {
            const Config& ck = config[key];
            T temp_value;
            temp_value.resize(ck.size());
            size_t counter = 0;
            for(auto& item : temp_value) {
                // рекурсивно вызываем traits(признаки) для каждого из элементов
                Type item_temp_value;

                if(!Loader(ck[counter++], item_temp_value)) {
                    return false;
                }

                item = item_temp_value;
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
        // std::cout << "[debug] save container(as vector) key=\"" << key << "\"" << std::endl;

        using Type = typename T::value_type;

        for(const auto& item : field) {
            // ключ дальше нельзя передать - создаст новую вложенность
            Config temp = Saver(item, prefix_comment, suffix_comment);
            config[key].push_back(temp);
        }
        config[key].setComment(prefix_comment, suffix_comment);
    }
};

} // namespace simpleapi
} // namespace tools
