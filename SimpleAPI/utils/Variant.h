#pragma once

#include <cstddef>
#include <cstdint>
#include <sys/types.h>
#include <type_traits>


namespace simpleapi {

namespace tools {

/* Макросы времени компиляции:
 *  - max_size_of_type      - расчёт максмального размера типа среди указанных
 *  - max_align_of_type     - расчёт максмального размера выравнивания типа
 *  - type_at_index         - получить тип на основе индекса
 *  - index_of_type         - получить индекс на основе типа
 *  - is_contains_type      - есть ли указанный тип среди списка
 *  - is_contains_duplicate - запрет создания Variant с дубликатами типов
 */

// ---------------------------------------------------------------------
// описатель рекурсивного поиска масксимального размера
// ---------------------------------------------------------------------
// базовое описание структуры для корректности выхода из SFINAE
template <typename... Types>
struct max_size_of_type;
// базовый случай, конец списка
template <>
struct max_size_of_type<> {
    static constexpr std::size_t size = 0;
};
// рекурсивное извлечение максимального размера
template <typename Head, typename... Tail>
struct max_size_of_type<Head, Tail...> {
    static constexpr std::size_t size = sizeof(Head) > max_size_of_type<Tail...>::size ? sizeof(Head)
                                                                                       : max_size_of_type<Tail...>::size;
};
// ---------------------------------------------------------------------

// ---------------------------------------------------------------------
// описатель рекурсивного поиска максимальной границы выравнивания
// ---------------------------------------------------------------------
// базовое описание структуры для корректности выхода из SFINAE
template <typename... Types>
struct max_align_of_type;
// базовый случай, конец списка
template <>
struct max_align_of_type<> {
    static constexpr std::size_t align_size = 0;
};
// рекурсивное извлечение максимального размера выравнивания
template <typename Head, typename... Tail>
struct max_align_of_type<Head, Tail...> {
    static constexpr std::size_t align_size = alignof(Head) > max_align_of_type<Tail...>::align_size ? alignof(Head)
                                                                                                     : max_align_of_type<Tail...>::align_size;
};
// ---------------------------------------------------------------------

// ---------------------------------------------------------------------
// описатель получения типа по индексу
// ---------------------------------------------------------------------
// базовое описание структуры для корректности выхода из SFINAE
template <std::size_t Index, typename... Types>
struct type_at_index;
// дошли до конца списка
template <typename Head, typename... Types>
struct type_at_index<0, Head, Types...> {
    using type = Head; // возвращаем тип
};
// рекурсивное извлечение типа по итерации индекса
template <std::size_t Index, typename Head, typename... Tail>
struct type_at_index<Index, Head, Tail...> {
    using type = typename type_at_index<Index - 1, Head, Tail...>::type;
};
// ---------------------------------------------------------------------

// ---------------------------------------------------------------------
// описатель получения индекса по типу
// ---------------------------------------------------------------------
// базовое описание структуры для корректности выхода из SFINAE
template <typename... Types>
struct index_of_type;
// тип совпал
template <typename FindType, typename... Tail>
struct index_of_type<FindType, Tail...> {
    static constexpr std::size_t value = 0;
};
// рекурсивное извлечение индекса по совпадению типа
template <typename FindType, typename Head, typename... Tail>
struct index_of_type<FindType, Head, Tail...> {
    static constexpr std::size_t value = 1 + index_of_type<FindType, Tail...>::value;
};
// ---------------------------------------------------------------------

// ---------------------------------------------------------------------
// описатель проверки наличия типа в списке
// ---------------------------------------------------------------------
// базовое описание структуры для корректности выхода из SFINAE
template <typename... Types>
struct is_contains_type;
// дошли до конца списка
template <>
struct is_contains_type<> {
    static constexpr bool value = false;
};
// список состоит из одного элемента
template <typename TemplateType, typename T>
struct is_contains_type<TemplateType, T> {
    static constexpr bool value = std::is_same<TemplateType, T>::value;
};
// рекурсивное сравнение типов из списка с искомым
template <typename TemplateType, typename Head, typename... Tail>
struct is_contains_type<TemplateType, Head, Tail...> {
    static constexpr bool value = std::is_same<TemplateType, Head>::value
                                  || is_contains_type<TemplateType, Tail...>::value;
};
// ---------------------------------------------------------------------

// ---------------------------------------------------------------------
// описатель проверки наличия дубликатов в списке типов
// ---------------------------------------------------------------------
// базовое описание структуры для корректности выхода из SFINAE
template <typename... Types>
struct is_contains_duplicate;
template <>
struct is_contains_duplicate<> {
    static constexpr bool value = false;
};
template <typename Head>
struct is_contains_duplicate<Head> {
    static constexpr bool value = false;
};
// рекурсивный поиск повторений наличия в списке
template <typename Head, typename... Tail>
struct is_contains_duplicate<Head, Tail...> {
    static constexpr bool value = is_contains_type<Head, Tail...>::value || is_contains_duplicate<Tail...>::value;
};
// ---------------------------------------------------------------------

} // namespace tools


template <typename... Types>
class Variant {
    static_assert(!tools::is_contains_duplicate<Types...>::value, "SimpleAPI: incorrect types list, found duplicates");

    template <std::size_t Index, typename... TypesList>
    struct Creator;

    template <std::size_t Index, typename... TypesList>
    struct Destroyer;

public:
    static constexpr std::size_t size = tools::max_size_of_type<Types...>::size;
    static constexpr std::size_t align_size = tools::max_align_of_type<Types...>::align_size;


    // по умолчанию проинициализируется первым типом (его значение по умолчанию)
    Variant() noexcept : m_current_type_index(0) {
        // создание объекта на указанном буфере с указанным типом
        new (data) typename tools::type_at_index<0, Types...>::type(0);
    }

    template <typename T, typename std::enable_if<tools::is_contains_type<T, Types...>::type, int>::type = 0>
    Variant(const T& value) {
        /* FIXME */
    }

    template <typename T, typename std::enable_if<tools::is_contains_type<T, Types...>::type, int>::type = 0>
    Variant(T&& value) {
        /* FIXME */
    }

    ~Variant() {
        /* FIXME */
    }

    template <typename T, typename std::enable_if<tools::is_contains_type<T, Types...>::type, int>::type = 0>
    Variant& operator=(const T& other) {
        /* FIXME */
        return {};
    }

    template <typename T, typename std::enable_if<tools::is_contains_type<T, Types...>::type, int>::type = 0>
    Variant& operator=(T&& other) {
        /* FIXME */
        return {};
    }

    template <typename T, typename std::enable_if<tools::is_contains_type<T, Types...>::type, int>::type = 0>
    void set(const T& other) {
        /* FIXME */
    }

    template <typename T, typename std::enable_if<tools::is_contains_type<T, Types...>::type, int>::type = 0>
    void set(T&& other) {
        /* FIXME */
    }

    template <typename T, typename std::enable_if<tools::is_contains_type<T, Types...>::type, int>::type = 0>
    Variant& get() {
        /* FIXME */
        return {};
    }

    template <typename T, typename std::enable_if<tools::is_contains_type<T, Types...>::type, int>::type = 0>
    Variant get() const {
        /* FIXME */
        return {};
    }

    /**
     * @brief index
     * @return Возвращает текущий индекс типа.
     */
    ssize_t index() const noexcept { return m_current_type_index; }

private:
    ssize_t m_current_type_index;
    // хранилище значения; аналог union, но через placement new
    alignas(align_size) uint8_t data[size];
};

} // namespace simpleapi
