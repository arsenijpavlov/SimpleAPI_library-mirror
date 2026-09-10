#pragma once

/** @file static_config.h
 * @if LANG_RU
 *      @details Хотя в данном файле есть только описание макросов, блок include всё равно остаётся, для удобства подключения
 *      этого файла в итоговой програамме, использующей SimpleAPI
 * @endif
 * @if LANG_EN
 *      @details Although this file contains only macro definitions, the include block is retained to make it easier to
 *      include the file in the final program using SimpleAPI
 * @endif
*/

// NOTE: "IWYU pragma: keep" спрячет лишнее предупреждение от clangd
#include "../config/Config.h"          // IWYU pragma: keep
#include "type_checkers.h"             // IWYU pragma: keep
#include "simple_types.h"              // IWYU pragma: keep
#include "struct_types.h"              // IWYU pragma: keep
#include "container_types_as_vector.h" // IWYU pragma: keep
#include "container_types_as_set.h"    // IWYU pragma: keep
#include "container_types_as_array.h"  // IWYU pragma: keep
#include "container_types_as_bitset.h" // IWYU pragma: keep
#include "container_types_as_queue.h"  // IWYU pragma: keep
#include "container_types_as_stack.h"  // IWYU pragma: keep
#include "container_types_as_map.h"    // IWYU pragma: keep
#include "container_type_as_config.h"  // IWYU pragma: keep
#include "pair_types.h"                // IWYU pragma: keep
#include "enum_types.h"                // IWYU pragma: keep
#include <functional>                  // IWYU pragma: keep


// NOTE: приставка SAPI_ - сокращение от SimpleAPI_ для избежания совпадений с пользовательским пространством

// макросы-счётчики аргументов в других макросах
#define SAPI_GETTER_MACRO_2(_1, _2, NAME, ...)                 NAME
#define SAPI_GETTER_MACRO_3(_1, _2, _3, NAME, ...)             NAME
#define SAPI_GETTER_MACRO_6(_1, _2, _3, _4, _5, _6, NAME, ...) NAME


// объявления
#define SAPI_DECLARE_FIELD_3(type, name, default_value)                                         \
    type name;
#define SAPI_DECLARE_FIELD_4(type, name, default_value, lambda)                                 \
    type name;
#define SAPI_DECLARE_FIELD_5(type, name, default_value, lambda, prefix_comment)                 \
    type name;
#define SAPI_DECLARE_FIELD_6(type, name, default_value, lambda, prefix_comment, suffix_comment) \
    type name;
// обёртка
#define SAPI_DECLARE_FIELD(...) \
    SAPI_GETTER_MACRO_6(__VA_ARGS__, SAPI_DECLARE_FIELD_6, SAPI_DECLARE_FIELD_5, SAPI_DECLARE_FIELD_4, SAPI_DECLARE_FIELD_3)(__VA_ARGS__)

// инициализаторы
#define SAPI_INIT_FIELD_3(type, name, default_value)                                         \
    name = default_value;
#define SAPI_INIT_FIELD_4(type, name, default_value, lambda)                                 \
    name = default_value;
#define SAPI_INIT_FIELD_5(type, name, default_value, lambda, prefix_comment)                 \
    name = default_value;
#define SAPI_INIT_FIELD_6(type, name, default_value, lambda, prefix_comment, suffix_comment) \
    name = default_value;
// обёртка
#define SAPI_INIT_FIELD(...) \
    SAPI_GETTER_MACRO_6(__VA_ARGS__, SAPI_INIT_FIELD_6, SAPI_INIT_FIELD_5, SAPI_INIT_FIELD_4, SAPI_INIT_FIELD_3)(__VA_ARGS__)

// загрузчики
#define SAPI_LOAD_FIELD_3(type, name, default_value)                                         \
    if(!simpleapi::tools::ConfigTypeTraits<type>::load(load_conf, #name, name)) return false;
#define SAPI_LOAD_FIELD_4(type, name, default_value, lambda)                                 \
    if(!simpleapi::tools::ConfigTypeTraits<type>::load(load_conf, #name, name, lambda)) return false;
#define SAPI_LOAD_FIELD_5(type, name, default_value, lambda, prefix_comment)                 \
    if(!simpleapi::tools::ConfigTypeTraits<type>::load(load_conf, #name, name, lambda)) return false;
#define SAPI_LOAD_FIELD_6(type, name, default_value, lambda, prefix_comment, suffix_comment) \
    if(!simpleapi::tools::ConfigTypeTraits<type>::load(load_conf, #name, name, lambda)) return false;
// обёртка
#define SAPI_LOAD_FIELD(...) \
    SAPI_GETTER_MACRO_6(__VA_ARGS__, SAPI_LOAD_FIELD_6, SAPI_LOAD_FIELD_5, SAPI_LOAD_FIELD_4, SAPI_LOAD_FIELD_3)(__VA_ARGS__)

// записыватели
#define SAPI_SAVE_FIELD_3(type, name, default_value)                                                                    \
    simpleapi::tools::ConfigTypeTraits<type>::save(save_conf, #name, name);
#define SAPI_SAVE_FIELD_4(type, name, default_value, lambda)                                                            \
    simpleapi::tools::ConfigTypeTraits<type>::save(save_conf, #name, name);
#define SAPI_SAVE_FIELD_5(type, name, default_value, lambda, prefix_comment)                                            \
    simpleapi::tools::ConfigTypeTraits<type>::save(save_conf, #name, name, prefix_comment);
#define SAPI_SAVE_FIELD_6(type, name, default_value, lambda, prefix_comment, suffix_comment)                            \
    simpleapi::tools::ConfigTypeTraits<type>::save(save_conf, #name, name, prefix_comment, suffix_comment);
// обёртка
#define SAPI_SAVE_FIELD(...) \
    SAPI_GETTER_MACRO_6(__VA_ARGS__, SAPI_SAVE_FIELD_6, SAPI_SAVE_FIELD_5, SAPI_SAVE_FIELD_4, SAPI_SAVE_FIELD_3)(__VA_ARGS__)

// сравнители
#define SAPI_COMMON_OPERATOR_EQUAL(type, name)                                   \
    simpleapi::tools::ConfigTypeTraits<type>::compare(name, other.name, #name) &&
    // simpleapi::tools::CompareValues(name, other.name) &&
//---
#define SAPI_OPERATOR_EQUAL_MACRO_3(type, name, default_value)                                         \
    SAPI_COMMON_OPERATOR_EQUAL(type, name)
#define SAPI_OPERATOR_EQUAL_MACRO_4(type, name, default_value, lambda)                                 \
    SAPI_COMMON_OPERATOR_EQUAL(type, name)
#define SAPI_OPERATOR_EQUAL_MACRO_5(type, name, default_value, lambda, prefix_comment)                 \
    SAPI_COMMON_OPERATOR_EQUAL(type, name)
#define SAPI_OPERATOR_EQUAL_MACRO_6(type, name, default_value, lambda, prefix_comment, suffix_comment) \
    SAPI_COMMON_OPERATOR_EQUAL(type, name)
// обёртка
#define SAPI_OPERATOR_EQUAL_MACRO(...) \
    SAPI_GETTER_MACRO_6(__VA_ARGS__, SAPI_OPERATOR_EQUAL_MACRO_6, SAPI_OPERATOR_EQUAL_MACRO_5, SAPI_OPERATOR_EQUAL_MACRO_4, SAPI_OPERATOR_EQUAL_MACRO_3)(__VA_ARGS__)

// enum
#define SAPI_ENUM_ELEMENT_1(element)                                             \
    element,
#define SAPI_ENUM_ELEMENT_2(element, value)                                      \
    element = value,
#define SAPI_ENUM_ELEMENT(...)                                                   \
    SAPI_GETTER_MACRO_2(__VA_ARGS__, SAPI_ENUM_ELEMENT_2, SAPI_ENUM_ELEMENT_1)(__VA_ARGS__)

#define SAPI_ENUM_TO_STRING_1(element)                                           \
    case _CURRENT_ENUM_NAME_::element: return #element;
#define SAPI_ENUM_TO_STRING_2(element, value)                                    \
    case _CURRENT_ENUM_NAME_::element: return #element;
#define SAPI_ENUM_TO_STRING(...)                                                 \
    SAPI_GETTER_MACRO_2(__VA_ARGS__, SAPI_ENUM_TO_STRING_2, SAPI_ENUM_TO_STRING_1)(__VA_ARGS__)

#define SAPI_ENUM_FROM_STRING_1(element)                                         \
    if(str == #element) { out_value = _CURRENT_ENUM_NAME_::element; return; }
#define SAPI_ENUM_FROM_STRING_2(element, value)                                  \
    if(str == #element) { out_value = _CURRENT_ENUM_NAME_::element; return; }
#define SAPI_ENUM_FROM_STRING(...)                                               \
    SAPI_GETTER_MACRO_2(__VA_ARGS__, SAPI_ENUM_FROM_STRING_2, SAPI_ENUM_FROM_STRING_1)(__VA_ARGS__)

#define SAPI_ENUM_POSSIBLE_VARIANTS_1(element)                                         \
    ret += std::string(#element) + ", ";
#define SAPI_ENUM_POSSIBLE_VARIANTS_2(element, value)                                  \
    ret += std::string(#element) + ", ";
#define SAPI_ENUM_POSSIBLE_VARIANTS(...)                                               \
    SAPI_GETTER_MACRO_2(__VA_ARGS__, SAPI_ENUM_POSSIBLE_VARIANTS_2, SAPI_ENUM_POSSIBLE_VARIANTS_1)(__VA_ARGS__)


// ====================================================================================================================
// ==================================================== USER SPACE ====================================================
// вариант описания enum без указания базового класса
// @TEST(STATIC, main)
#define SAPI_REGISTER_ENUM_2(EnumName, LIST)                                  \
    enum class EnumName {                                                     \
        LIST(SAPI_ENUM_ELEMENT)                                               \
        _UNDEFINED_STATE_                                                     \
    };                                                                        \
    inline std::string ToString(const EnumName& value) noexcept {             \
        using _CURRENT_ENUM_NAME_ = EnumName;                                 \
        switch(value) {                                                       \
            LIST(SAPI_ENUM_TO_STRING)                                         \
            default: break;                                                   \
        }                                                                     \
        return "UNKNOWN";                                                     \
    }                                                                         \
    inline void EnumFromString(const std::string& str, EnumName& out_value) { \
        using _CURRENT_ENUM_NAME_ = EnumName;                                 \
        LIST(SAPI_ENUM_FROM_STRING)                                           \
        out_value = EnumName::_UNDEFINED_STATE_;                              \
    }                                                                         \
    inline std::string EnumPossibleVariants(const EnumName value) {           \
        (void)value; /*hide warning unused parameter*/                        \
        std::string ret;                                                      \
        LIST(SAPI_ENUM_POSSIBLE_VARIANTS)                                     \
        if(!ret.empty() && ret.size() > 1) /* удаляем ", " */                 \
        {                                                                     \
            ret.pop_back();                                                   \
            ret.pop_back();                                                   \
        }                                                                     \
        return ret;                                                           \
    }

// вариант описания enum через указание базового класса
// @TEST(STATIC, main)
#define SAPI_REGISTER_ENUM_3(EnumName, LIST, UnderlyingType)                  \
    enum class EnumName : UnderlyingType {                                    \
        LIST(SAPI_ENUM_ELEMENT)                                               \
        _UNDEFINED_STATE_                                                     \
    };                                                                        \
    inline std::string ToString(const EnumName& value) noexcept {             \
        using _CURRENT_ENUM_NAME_ = EnumName;                                 \
        switch(value) {                                                       \
            LIST(SAPI_ENUM_TO_STRING)                                         \
            default: break;                                                   \
        }                                                                     \
        return "UNKNOWN";                                                     \
    }                                                                         \
    inline void EnumFromString(const std::string& str, EnumName& out_value) { \
        using _CURRENT_ENUM_NAME_ = EnumName;                                 \
        LIST(SAPI_ENUM_FROM_STRING)                                           \
        out_value = EnumName::_UNDEFINED_STATE_;                              \
    }                                                                         \
    inline std::string EnumPossibleVariants(const EnumName value) {           \
        (void)value; /*hide warning unused parameter*/                        \
        std::string ret;                                                      \
        LIST(SAPI_ENUM_POSSIBLE_VARIANTS)                                     \
        if(!ret.empty() && ret.size() > 1) /* удаляем ", " */                 \
        {                                                                     \
            ret.pop_back();                                                   \
            ret.pop_back();                                                   \
        }                                                                     \
        return ret;                                                           \
    }

#define SAPI_REGISTER_ENUM(...) \
    SAPI_GETTER_MACRO_3(__VA_ARGS__, SAPI_REGISTER_ENUM_3, SAPI_REGISTER_ENUM_2)(__VA_ARGS__)

/* Ограничения:
 * - базовая структура не может состоять из примитива Config,
 *      то есть пользователю нужно использовать стандартное "val = config;" без преобразований
 * - Config верхнего уровня обязан состоять из объекта Config, который сам по себе является ассоциативным массивом
 * - допустимые типы:
 *      - самостоятельный тип-примитив T
 *      - самостоятельный тип-структура T
 *      - индексные контейнеры:
 *          - std::vector<T>
 *          - std::list<T>
 *          - std::forward_list<T>
 *          - std::array<T, size>
 *          - std::bitset<T>
 *          - std::set<T>
 *          - std::multiset<T>
 *          - std::unordered_set<T>
 *          - std::unordered_multiset<T>
 *      - очереди:
 *          - std::stack<T>
 *          - std::queue<T>
 *          - std::deque<T>
 *          - std::priority_queue<T>
 *      - ассоциативные контейнеры:
 *          - std::map<K, T>
 *          - std::unordered_map<K, T>
 *          - std::multimap<K, T>           - сейчас нет реализации (Config не может содержать дублей ключей)
 *          - std::unordered_multimap<K, T> - сейчас нет реализации (Config не может содержать дублей ключей)
 * - запрещённые типы контейнеров:
 *      - std::basic_string - библиотека строго UTF-8, запрещаем пользователю выстрелить себе в ногу
*/
// @TEST(STATIC, main)
#define SAPI_REGISTER_CONFIG(StructName, SAPI_FIELDS_MACRO)           \
    struct StructName {                                               \
        SAPI_FIELDS_MACRO(SAPI_DECLARE_FIELD)                         \
                                                                      \
        StructName() {                                                \
            SAPI_FIELDS_MACRO(SAPI_INIT_FIELD)                        \
        }                                                             \
                                                                      \
        bool loadConfig(const simpleapi::Config& load_conf) {         \
            simpleapi::static_config_error_str.clear();               \
            SAPI_FIELDS_MACRO(SAPI_LOAD_FIELD)                        \
            return true;                                              \
        }                                                             \
                                                                      \
        simpleapi::Config saveConfig() const {                        \
            simpleapi::Config save_conf;                              \
            SAPI_FIELDS_MACRO(SAPI_SAVE_FIELD)                        \
            return save_conf;                                         \
        }                                                             \
                                                                      \
        bool operator==(const StructName& other) const {              \
            if(this == &other) return true;                           \
            return SAPI_FIELDS_MACRO(SAPI_OPERATOR_EQUAL_MACRO) true; \
        }                                                             \
                                                                      \
        bool operator!=(const StructName& other) const {              \
            if(this == &other) return false;                          \
                return !((*this) == other);                           \
        }                                                             \
    };

// EXAMPLE OF USAGE -------------------------------------------
//    #define STRUCT_FIELDS(X)       \
//        X(int, integer, 2)         \
//        X(uint8_t, u_integer, 3, []{ return (u_integer != 5); }) \
//        X(std::string, str, "asd")
//    SAPI_REGISTER_CONFIG(TestConfig, STRUCT_FIELDS)
// ------------------------------------------------------------
// enum:
//  #define MY_ENUM_FIELDS(X) \
//      X(eFirst, 0) \
//      X(eSecond, 5) \
//      X(eThird, 10)
//    SAPI_REGISTER_ENUM(MyEnum, MY_ENUM_FIELDS)
// EXAMPLE OF USAGE -------------------------------------------
