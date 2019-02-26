// Copyright 2018-2019 Elias Kosunen
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file is a part of ekutil:
//     https://github.com/eliaskosunen/ekutil

#ifndef EKUTIL_MEMORY_H
#define EKUTIL_MEMORY_H

#include "meta.h"

#include <iterator>
#include <new>
#include <type_traits>

namespace ekutil {
    template <typename... Types>
    struct aligned_union {
        static EKUTIL_CONSTEXPR const size_t alignment_value =
            constexpr_max(alignof(Types)...);
        static EKUTIL_CONSTEXPR const size_t size_value =
            constexpr_max(sizeof(Types)...);
        struct type {
            alignas(alignment_value) unsigned char _s[size_value];
        };
    };

    template <typename ForwardIt, typename T>
    void uninitialized_fill(ForwardIt first,
                            ForwardIt last,
                            const T& value) noexcept
    {
        using value_type = typename std::iterator_traits<ForwardIt>::value_type;
        ForwardIt current = first;
        for (; current != last; ++current) {
            ::new (static_cast<void*>(std::addressof(*current)))
                value_type(value);
        }
    }
    template <typename T, typename ForwardIt>
    void uninitialized_fill_default_construct(ForwardIt first,
                                              ForwardIt last) noexcept
    {
        using value_type = typename std::iterator_traits<ForwardIt>::value_type;
        ForwardIt current = first;
        for (; current != last; ++current) {
            ::new (static_cast<void*>(std::addressof(*current))) value_type();
        }
    }
    template <typename InputIt, typename ForwardIt>
    ForwardIt uninitialized_copy(InputIt first,
                                 InputIt last,
                                 ForwardIt d_first) noexcept
    {
        using value_type = typename std::iterator_traits<ForwardIt>::value_type;
        ForwardIt current = d_first;
        for (; first != last; ++first, (void)++current) {
            ::new (static_cast<void*>(std::addressof(*current)))
                value_type(*first);
        }
        return current;
    }
    template <typename InputIt, typename ForwardIt>
    ForwardIt uninitialized_move(InputIt first,
                                 InputIt last,
                                 ForwardIt d_first) noexcept
    {
        using value_type = typename std::iterator_traits<ForwardIt>::value_type;
        ForwardIt current = d_first;
        for (; first != last; ++first, (void)++current) {
            ::new (static_cast<void*>(std::addressof(*current)))
                value_type(std::move(*first));
        }
        return current;
    }

    template <typename T>
    class unique_ptr {
    public:
        using element_type = T;
        using pointer = T*;

        EKUTIL_CONSTEXPR unique_ptr() noexcept = default;
        EKUTIL_CONSTEXPR unique_ptr(std::nullptr_t) noexcept {}

        EKUTIL_CONSTEXPR explicit unique_ptr(pointer p) : m_ptr(p) {}

        EKUTIL_CONSTEXPR unique_ptr(const unique_ptr&) = delete;
        EKUTIL_CONSTEXPR unique_ptr& operator=(const unique_ptr&) = delete;

        EKUTIL_CONSTEXPR14 unique_ptr(unique_ptr&& p) noexcept
            : m_ptr(std::move(p.m_ptr))
        {
            p.m_ptr = nullptr;
        }
        unique_ptr& operator=(unique_ptr&& p) noexcept
        {
            if (m_ptr) {
                delete m_ptr;
            }
            m_ptr = p.m_ptr;
            p.m_ptr = 0;
        }

        ~unique_ptr() noexcept
        {
            if (m_ptr) {
                delete m_ptr;
            }
        }

        EKUTIL_CONSTEXPR explicit operator bool() const noexcept
        {
            return get() != nullptr;
        }

        EKUTIL_CONSTEXPR pointer get() const noexcept
        {
            return m_ptr;
        }

        EKUTIL_CONSTEXPR pointer operator->() const noexcept
        {
            return m_ptr;
        }
        EKUTIL_CONSTEXPR typename std::add_lvalue_reference<T>::type operator*()
            const
        {
            return *m_ptr;
        }

    private:
        pointer m_ptr{nullptr};
    };

    EKUTIL_CLANG_PUSH
    EKUTIL_CLANG_IGNORE("-Wpadded")

    template <typename T>
    class erased_storage {
    public:
        using value_type = T;
        using pointer = T*;
        using storage_type =
            typename std::aligned_storage<sizeof(T), alignof(T)>::type;

        erased_storage() noexcept = default;

        erased_storage(T val) noexcept(
            std::is_nothrow_move_constructible<T>::value)
            : m_ptr(::new (static_cast<void*>(&m_data)) T(std::move(val)))
        {
        }

        erased_storage(const erased_storage& other)
            : m_ptr(::new (static_cast<void*>(&m_data)) T(other.get()))
        {
        }
        erased_storage& operator=(const erased_storage& other)
        {
            _destruct();
            m_ptr = ::new (static_cast<void*>(&m_data)) T(other.get());
            return *this;
        }

        erased_storage(erased_storage&& other) noexcept
            : m_ptr(::new (static_cast<void*>(&m_data))
                        T(std::move(other.get())))
        {
        }
        erased_storage& operator=(erased_storage&& other) noexcept
        {
            _destruct();
            m_ptr =
                ::new (static_cast<void*>(&m_data)) T(std::move(other.get()));
            return *this;
        }

        ~erased_storage() noexcept
        {
            _destruct();
        }

        EKUTIL_CONSTEXPR14 T& get() noexcept
        {
            return _get();
        }
        EKUTIL_CONSTEXPR14 const T& get() const noexcept
        {
            return _get();
        }

        EKUTIL_CONSTEXPR14 T& operator*() noexcept
        {
            return _get();
        }
        EKUTIL_CONSTEXPR14 const T& operator*() const noexcept
        {
            return _get();
        }

        EKUTIL_CONSTEXPR14 T* operator->() noexcept
        {
            return std::addressof(_get());
        }
        EKUTIL_CONSTEXPR14 const T* operator->() const noexcept
        {
            return std::addressof(_get());
        }

    private:
        void _destruct()
        {
            if (m_ptr) {
                _get().~T();
            }
        }
        static pointer _toptr(storage_type& data)
        {
#if EKUTIL_HAS_LAUNDER
            return std::launder<T>(reinterpret_cast<T*>(std::addressof(data)));
#else
            return reinterpret_cast<T*>(std::addressof(data));
#endif
        }
        EKUTIL_CONSTEXPR14 T& _get() noexcept
        {
            return *m_ptr;
        }
        EKUTIL_CONSTEXPR14 const T& _get() const noexcept
        {
            return *m_ptr;
        }

        storage_type m_data{};
        pointer m_ptr{nullptr};
    };

    EKUTIL_CLANG_POP
}  // namespace ekutil

#endif  // EKUTIL_MEMORY_H
