#pragma once

#include <algorithm>


namespace simpleapi {

template <typename T>
class Optional {
    T    m_value;
    bool m_status;
public:
    using type = T;

    Optional<T>()                                               noexcept
        : m_status(false)
    {}
    Optional<T>(const T& value, bool enable = true)             noexcept
    {
        set(value);

        // значение сохранится, но будет выключено
        if(!enable) unset();
    }
    Optional<T>(T&& value, bool enable = true)                  noexcept
    {
        set(std::move(value));

        // значение сохранится, но будет выключено
        if(!enable) unset();
    }
    Optional<T>(const Optional<T>& other, bool enable = true)   noexcept
    {
        set(other);

        // значение сохранится, но будет выключено
        if(!enable) unset();
    }
    Optional<T>(Optional<T>&& other, bool enable = true)        noexcept
    {
        set(std::move(other));

        // значение сохранится, но будет выключено
        if(!enable) unset();
    }

    Optional<T>& operator=(const T& value)                      noexcept
    {
        set(value);
        return *this;
    }
    Optional<T>& operator=(T&& value)                           noexcept
    {
        set(std::move(value));
        return *this;
    }
    Optional<T>& operator=(const Optional<T>& other)            noexcept
    {
        set(other);
        return *this;
    }
    Optional<T>& operator=(Optional<T>&& other)                 noexcept
    {
        set(std::move(other));
        return *this;
    }

    void set(const T& value)                                    noexcept
    {
        m_value  = value;
        m_status = true;
    }
    void set(T&& value)                                         noexcept
    {
        m_value  = std::move(value);
        m_status = true;
    }
    void set(const Optional<T>& other)                          noexcept
    {
        if(this != &other) {
            m_value  = other.m_value;
            m_status = other.m_status;
        }
    }
    void set(Optional<T>&& other)                               noexcept
    {
        if(this != &other) {
            m_value  = other.m_value;
            m_status = other.m_status;
        }
    }

    explicit operator bool()                            const noexcept  { return m_status; }
    // выключенное значение не затирает предыдущую реализацию
    void unset()                                        noexcept        { m_status = false; }
    bool isValid()                                      const noexcept  { return m_status; }
    T&   value()                                        noexcept        { return m_value; }
    T    value()                                        const noexcept  { return m_value; }

    bool operator==(const T& other)                     const noexcept
    {
        return m_status && m_value == other;
    }
    bool operator!=(const T& other)                     const noexcept  { return !(*this == other); }
    bool operator==(const Optional<T>& other)           const noexcept
    {
        bool b = m_status == other.m_status;
        if(b && m_status) {
            return m_value == other.m_value;
        }
        return b;
    }
    bool operator!=(const Optional<T>& other)           const noexcept  { return !(*this == other); }
};

} // namespace simpleapi
