#pragma once

#include <bitset>
#include <deque>
#include <forward_list>
#include <list>
#include <queue>
#include <set>
#include <stack>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include "../config/Config.h"


namespace simpleapi {
namespace tools {

// проверка: тип одиночный или список значений
//   получим ::value == false для SFINAE, если указанных методов не существует (нельзя вызвать)
template <typename T>
class is_config_struct {
    // очистка типа от const и ссылок
    using CleanT = typename std::decay<T>::type;

    // метод для SFINAE проверки наличия метода loadConfig у класса U
    template <typename U>
    static char test(
        decltype(
            std::declval<U>().loadConfig(
                std::declval<const Config&>()
            )
        )*
    );
    // метод для разрешения конфликта для поля value
    template <typename U> static long test(...);
public:
    static const bool value = sizeof(test<CleanT>(0)) == sizeof(char);
};
// ----------------------------------------------------------------------------

// проверка: является ли тип STL контейнером определённого формата
//   получим ::value == false для SFINAE, если указанных методов не существует (нельзя вызвать)
template <typename T>
class is_container_as_vector {
    // очистка типа от const и ссылок
    using CleanT = typename std::decay<T>::type;

    // метод для SFINAE проверки
    template <typename Dummy = CleanT,
              typename = typename std::enable_if<
                 std::is_same<Dummy, std::vector<typename Dummy::value_type,
                                                 typename Dummy::allocator_type>>::value
                 || std::is_same<Dummy, std::list<typename Dummy::value_type,
                                                  typename Dummy::allocator_type>>::value
                 || std::is_same<Dummy, std::forward_list<typename Dummy::value_type,
                                                          typename Dummy::allocator_type>>::value
                 || std::is_same<Dummy, std::deque<typename Dummy::value_type,
                                                   typename Dummy::allocator_type>>::value
                 >::type
             >
    static char test(int);
    // метод для разрешения конфликта для поля value
    template <typename U>
    static long test(...);
public:
    static const bool value = (sizeof(test<CleanT>(0)) == sizeof(char));
};
//----------------
template <typename T>
class is_container_as_set {
    // очистка типа от const и ссылок
    using CleanT = typename std::decay<T>::type;

    // метод для SFINAE проверки
    template <typename Dummy = CleanT,
              typename = typename std::enable_if<
                 std::is_same<Dummy, std::set<typename Dummy::value_type,
                                              typename Dummy::key_compare,
                                              typename Dummy::allocator_type>>::value
                 || std::is_same<Dummy, std::multiset<typename Dummy::value_type,
                                                      typename Dummy::key_compare,
                                                      typename Dummy::allocator_type>>::value
                 >::type
             >
    static char test(int);
    // метод для разрешения конфликта для поля value
    template <typename U>
    static long test(...);
public:
    static const bool value = (sizeof(test<CleanT>(0)) == sizeof(char));
};
//----------------
template <typename T>
class is_container_as_unordered_set {
    // очистка типа от const и ссылок
    using CleanT = typename std::decay<T>::type;

    // метод для SFINAE проверки
    template <typename Dummy = CleanT,
              typename = typename std::enable_if<
                 std::is_same<Dummy, std::unordered_set<typename Dummy::value_type,
                                                        typename Dummy::hasher,
                                                        typename Dummy::key_equal,
                                                        typename Dummy::allocator_type>>::value
                 || std::is_same<Dummy, std::unordered_multiset<typename Dummy::value_type,
                                                                typename Dummy::hasher,
                                                                typename Dummy::key_equal,
                                                                typename Dummy::allocator_type>>::value
                 >::type
             >
    static char test(int);
    // метод для разрешения конфликта для поля value
    template <typename U>
    static long test(...);
public:
    static const bool value = (sizeof(test<CleanT>(0)) == sizeof(char));
};
//----------------
template <typename T>
class is_container_as_queue {
    // очистка типа от const и ссылок
    using CleanT = typename std::decay<T>::type;

    // метод для SFINAE проверки
    template <typename Dummy = CleanT,
              typename = typename std::enable_if<
                 std::is_same<Dummy, std::queue<typename Dummy::value_type,
                                                typename Dummy::container_type>>::value
                 >::type
             >
    static char test(int);
    // метод для разрешения конфликта для поля value
    template <typename U>
    static long test(...);
public:
    static const bool value = (sizeof(test<CleanT>(0)) == sizeof(char));
};
//----------------
template <typename T>
class is_container_as_priority_queue {
    // очистка типа от const и ссылок
    using CleanT = typename std::decay<T>::type;

    // метод для SFINAE проверки
    template <typename Dummy = CleanT,
              typename = typename std::enable_if<
                 std::is_same<Dummy, std::priority_queue<typename Dummy::value_type,
                                                         typename Dummy::container_type,
                                                         typename Dummy::value_compare>>::value
                 >::type
             >
    static char test(int);
    // метод для разрешения конфликта для поля value
    template <typename U>
    static long test(...);
public:
    static const bool value = (sizeof(test<CleanT>(0)) == sizeof(char));
};
//----------------
template <typename T>
class is_container_as_stack {
    // очистка типа от const и ссылок
    using CleanT = typename std::decay<T>::type;

    // метод для SFINAE проверки
    template <typename Dummy = CleanT,
             typename = typename std::enable_if<
                 std::is_same<Dummy, std::stack<typename Dummy::value_type,
                                                typename Dummy::container_type>>::value
                 >::type
             >
    static char test(int);
    // метод для разрешения конфликта для поля value
    template <typename U>
    static long test(...);
public:
    static const bool value = (sizeof(test<CleanT>(0)) == sizeof(char));
};
//----------------
template <typename T>
class is_container_as_array {
    // очистка типа от const и ссылок
    using CleanT = typename std::decay<T>::type;

    // метод для SFINAE проверки
    template <typename Dummy = CleanT,
             typename = typename std::enable_if<
                 std::is_same<Dummy, std::array<typename Dummy::value_type,
                                                std::tuple_size<Dummy>::value>>::value
                 >::type
             >
    static char test(int);
    // метод для разрешения конфликта для поля value
    template <typename U>
    static long test(...);
public:
    static const bool value = (sizeof(test<CleanT>(0)) == sizeof(char));
};
//----------------
template <typename T>
class is_container_as_bitset {
    // очистка типа от const и ссылок
    using CleanT = typename std::decay<T>::type;

    // метод для SFINAE проверки
    template <typename Dummy = CleanT,
             typename = typename std::enable_if<
                 std::is_same<Dummy, std::bitset<Dummy().size()>>::value
                 >::type
             >
    static char test(int);
    // метод для разрешения конфликта для поля value
    template <typename U>
    static long test(...);
public:
    static const bool value = (sizeof(test<CleanT>(0)) == sizeof(char));
};
//----------------
template <typename T>
class is_container_as_map {
    // очистка типа от const и ссылок
    using CleanT = typename std::decay<T>::type;

    // метод для SFINAE проверки
    template <typename Dummy = CleanT,
             typename = typename std::enable_if<
                 std::is_same<Dummy, std::map<typename Dummy::key_type,
                                              typename Dummy::mapped_type,
                                              typename Dummy::key_compare,
                                              typename Dummy::allocator_type>>::value
                 || std::is_same<Dummy, std::multimap<typename Dummy::key_type,
                                                      typename Dummy::mapped_type,
                                                      typename Dummy::key_compare,
                                                      typename Dummy::allocator_type>>::value
                 >::type
             >
    static char test(int);
    // метод для разрешения конфликта для поля value
    template <typename U>
    static long test(...);
public:
    static const bool value = (sizeof(test<CleanT>(0)) == sizeof(char));
};
//----------------
template <typename T>
class is_container_as_unordered_map {
    // очистка типа от const и ссылок
    using CleanT = typename std::decay<T>::type;

    // метод для SFINAE проверки
    template <typename Dummy = CleanT,
             typename = typename std::enable_if<
                 std::is_same<Dummy, std::unordered_map<typename Dummy::key_type,
                                                        typename Dummy::mapped_type,
                                                        typename Dummy::hasher,
                                                        typename Dummy::key_equal,
                                                        typename Dummy::allocator_type>>::value
                 || std::is_same<Dummy, std::unordered_multimap<typename Dummy::key_type,
                                                                typename Dummy::mapped_type,
                                                                typename Dummy::hasher,
                                                                typename Dummy::key_equal,
                                                                typename Dummy::allocator_type>>::value
                 >::type
             >
    static char test(int);
    // метод для разрешения конфликта для поля value
    template <typename U>
    static long test(...);
public:
    static const bool value = (sizeof(test<CleanT>(0)) == sizeof(char));
};
//----------------
template <typename T>
class is_pair {
    // очистка типа от const и ссылок
    using CleanT = typename std::decay<T>::type;

    // метод для SFINAE проверки
    template <typename Dummy = CleanT,
             typename = typename std::enable_if<
                 std::is_same<Dummy, std::pair<typename Dummy::first_type,
                                               typename Dummy::second_type>>::value
                 >::type
             >
    static char test(int);
    // метод для разрешения конфликта для поля value
    template <typename U>
    static long test(...);
public:
    static const bool value = (sizeof(test<CleanT>(0)) == sizeof(char));
};
// ----------------------------------------------------------------------------

// разделение чтения переменной "value=get<T>()"/"value=T::loadConfig(cfg)"
// если T имеет метод loadConfig()
template <typename T>
static bool Loader(const Config& config, T& output_value,
                   typename std::enable_if<is_config_struct<T>::value>::type* = 0)
{
    if(!output_value.loadConfig(config))
        return false;
    return true;
}
// если T не имеет метода loadConfig()
template <typename T>
static bool Loader(const Config& config, T& output_value,
                   typename std::enable_if<!is_config_struct<T>::value>::type* = 0)
{
    output_value = config.get<T>();
    return true;
}
// ----------------------------------------------------------------------------

// разделение чтения переменной "value=get<T>()"/"value=T::loadConfig(cfg)"
// если T имеет метод saveConfig()
template <typename T>
static Config Saver(const T& input_value, const std::string prefix_comment, const std::string suffix_comment,
                    typename std::enable_if<is_config_struct<T>::value>::type* = 0)
{
    return input_value.saveConfig();
}
// если T не имеет метода saveConfig()
template <typename T>
static Config Saver(const T& input_value, const std::string prefix_comment, const std::string suffix_comment,
                   typename std::enable_if<!is_config_struct<T>::value>::type* = 0)
{
    return Config(input_value);
}
// ----------------------------------------------------------------------------

// проверка: является ли лямбда валидной
//   получим ::value == false для SFINAE, если указанных методов не существует (нельзя вызвать)
template <typename T, typename... Args>
class is_variadic_lambda_callable {
    // очистка типа от const и ссылок
    using CleanT = typename std::decay<T>::type;

    // если это указатель, снимаем указатель
    // иначе оставляем как есть
    using CallableT = typename std::conditional<
        std::is_pointer<CleanT>::value,
        typename std::remove_pointer<CleanT>::type,
        CleanT
        >::type;

    // метод для SFINAE проверки наличия лямбы как nullptr
    template <typename U>
    static typename std::enable_if<std::is_same<U, std::nullptr_t>::value, char>::type
        test(int);

    // метод для SFINAE проверки наличия валидной лямбы у класса U
    template <typename U>
    static auto test(int) -> decltype(
        std::declval<U>()(std::declval<Args>()...),
        char()
    );

    // Если аргументов предложено много, но Перегрузка 2 не подошла,
    // мы проверяем, умеет ли лямбда принимать ТОЛЬКО ПЕРВЫЙ аргумент
    template <typename U, typename Dummy = void>
    static auto test(long) -> decltype(
        std::declval<U>()(std::declval<typename std::tuple_element<0, std::tuple<Args...>>::type>()),
        char()
        );

    // метод для разрешения конфликта для поля value
    template <typename U> static long test(...);
public:
    static const bool value = (sizeof(test<CallableT>(0)) == sizeof(char));
};
// ---------------
// ---------------
// если указатель — разыменовываем его
template <typename PtrT>
static inline typename std::remove_pointer<PtrT>::type&
UnwrapPointer(PtrT ptr, typename std::enable_if<std::is_pointer<PtrT>::value>::type* = 0) {
    return *ptr;
}
// если объект — отдаем как есть
template <typename ObjT>
static inline ObjT&
UnwrapPointer(ObjT& obj, typename std::enable_if<!std::is_pointer<ObjT>::value>::type* = 0) {
    return obj;
}
// ---------------
// ---------------
// описания перегрузок для лямбд/std::function() для вызова как через объект, так и через указатель на него
template <typename... Args>
static bool ExecuteValidator(std::nullptr_t, Args&&...) noexcept
{
    return true;
}
// описание лямбд по уменьшению возможного количества параметров
// 1. Запускается со всеми аргументами сразу
template <typename ValidatorT, typename ... Args,
          typename std::enable_if<!std::is_same<ValidatorT, std::nullptr_t>::value, int>::type = 0,
          typename = decltype(std::declval<ValidatorT>()(std::declval<Args>()...))
         >
static bool ExecuteValidator(ValidatorT validator, Args&&... args)
{
    // если это указатель на функцию/лямбду (или nullptr)
    auto&& not_ptr_validator = UnwrapPointer(validator);
    return not_ptr_validator(std::forward<Args>(args)...);
}
//----------------------------
// 2. Принимает T value и std::string key
template <typename ValidatorT, typename T, typename ... Args,
          typename std::enable_if<!std::is_same<ValidatorT, std::nullptr_t>::value, int>::type = 0,
          typename = decltype(std::declval<ValidatorT>()(std::declval<T>(), std::declval<std::string>()))
         >
static bool ExecuteValidator(ValidatorT validator, T&& t, std::string&& key, Args&&... args)
{
    // если это указатель на функцию/лямбду (или nullptr)
    auto&& not_ptr_validator = UnwrapPointer(validator);
    return not_ptr_validator(std::forward<T>(t), std::forward<std::string>(key));
}
//----------------------------
// 3. Принимает только T value
template <typename ValidatorT, typename T, typename ... Args,
          typename std::enable_if<!std::is_same<ValidatorT, std::nullptr_t>::value, int>::type = 0,
          typename = decltype(std::declval<ValidatorT>()(std::declval<T>()))
         >
static bool ExecuteValidator(ValidatorT validator, T&& t, Args&&... args)
{
    // если это указатель на функцию/лямбду (или nullptr)
    auto&& not_ptr_validator = UnwrapPointer(validator);
    return not_ptr_validator(std::forward<T>(t));
}
// ----------------------------------------------------------------------------

} // namespace tools
} // namespace simpleapi
