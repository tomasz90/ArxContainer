#pragma once

#ifndef ARX_INITIALIZER_LIST_H
#define ARX_INITIALIZER_LIST_H

#include "config/size_t.h"
#include <stddef.h>

namespace std {
#if defined(__GNUC__)

    template<typename T>
    class initializer_list {
    public:
        using value_type = T;
        using reference = const T&;
        using const_reference = const T&;
        using size_type = size_t;
        using iterator = const T*;
        using const_iterator = const T*;

    private:
        iterator  m_array;
        size_type m_len;

        // The compiler can call a private constructor.
        constexpr initializer_list(const_iterator itr, size_type st)
                : m_array(itr), m_len(st) { }

    public:
        constexpr initializer_list() noexcept : m_array(0), m_len(0) { }
        constexpr size_type size() const noexcept { return m_len; }
        constexpr const_iterator begin() const noexcept { return m_array; }
        constexpr const_iterator end() const noexcept { return begin() + size(); }
    };
#elif defined(__clang__)
    template<typename T>
    class initializer_list {
    private:
        const T* m_first;
        const T* m_last;

    public:
        using value_type      = T;
        using reference       = const T&;
        using const_reference = const T&;
        using size_type       = size_t;
        using iterator        = const T*;
        using const_iterator  = const T*;

        initializer_list() noexcept : m_first(nullptr), m_last(nullptr) {}

        // Number of elements.
        size_t size() const noexcept { return m_last - m_first; }

        // First element.
        const T* begin() const noexcept { return m_first; }

        // One past the last element.
        const T* end() const noexcept { return m_last; }
    };
#elif defined(_MSC_VER)

    template<typename T>
    class initializer_list {
    public:
        using value_type = T;
        using reference = const T&;
        using const_reference = const T&;
        using size_type = size_t;
        using iterator = const T*;
        using const_iterator = const T*;

        constexpr initializer_list() noexcept : m_first(nullptr), m_last(nullptr) {}

        constexpr initializer_list(const T* first, const T* last) noexcept
            : m_first(first), m_last(last) {}

        constexpr const T* begin() const noexcept { return m_first; }
        constexpr const T* end() const noexcept { return m_last; }
        constexpr size_t size() const noexcept {
            return static_cast<size_t>(m_last - m_first);
        }

    private:
        const T* m_first;
        const T* m_last;
    };
#else
    #error "Initializer_list is not supported for this compiler"
#endif

    template<typename T>
    constexpr const T* begin(initializer_list<T> il) noexcept {
        return il.begin();
    }

    template<typename T>
    constexpr const T* end(initializer_list<T> il) noexcept {
        return il.end();
    }
}

#endif  // ARX_INITIALIZER_LIST_H
