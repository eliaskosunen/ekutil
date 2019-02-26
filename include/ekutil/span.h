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

#ifndef EKUTIL_SPAN_H
#define EKUTIL_SPAN_H

#include "compat.h"

#include <iterator>
#include <type_traits>

namespace ekutil {
    /**
     * A view over a contiguous range.
     * Stripped-down version of `std::span`.
     */
    template <typename T>
    class span {
    public:
        using element_type = T;
        using value_type = typename std::remove_cv<T>::type;
        using index_type = std::ptrdiff_t;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using const_pointer = T*;
        using reference = T&;
        using iterator = pointer;
        using const_iterator = const_pointer;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        EKUTIL_CONSTEXPR span() noexcept = default;
        EKUTIL_CONSTEXPR span(pointer ptr, index_type count) noexcept
            : m_ptr(ptr), m_size(count)
        {
        }
        EKUTIL_CONSTEXPR span(pointer first, pointer last) noexcept
            : span(first, last - first)
        {
        }

        EKUTIL_CONSTEXPR iterator begin() const noexcept
        {
            return m_ptr;
        }
        EKUTIL_CONSTEXPR iterator end() const noexcept
        {
            return m_ptr + m_size;
        }
        EKUTIL_CONSTEXPR reverse_iterator rbegin() const noexcept
        {
            return reverse_iterator{end()};
        }
        EKUTIL_CONSTEXPR reverse_iterator rend() const noexcept
        {
            return reverse_iterator{begin()};
        }

        EKUTIL_CONSTEXPR const_iterator cbegin() const noexcept
        {
            return m_ptr;
        }
        EKUTIL_CONSTEXPR const_iterator cend() const noexcept
        {
            return m_ptr + m_size;
        }
        EKUTIL_CONSTEXPR const_reverse_iterator crbegin() const noexcept
        {
            return reverse_iterator{cend()};
        }
        EKUTIL_CONSTEXPR const_reverse_iterator crend() const noexcept
        {
            return reverse_iterator{cbegin()};
        }

        EKUTIL_CONSTEXPR14 reference operator[](index_type i) const noexcept
        {
            return *(m_ptr + i);
        }

        EKUTIL_CONSTEXPR pointer data() const noexcept
        {
            return m_ptr;
        }
        EKUTIL_CONSTEXPR index_type size() const noexcept
        {
            return m_size;
        }

        EKUTIL_CONSTEXPR14 span<T> subspan(index_type off) const
        {
            return span<T>(data() + off, size() - off);
        }
        EKUTIL_CONSTEXPR14 span<T> subspan(index_type off,
                                           difference_type count) const
        {
            return span<T>(data() + off, count);
        }

    private:
        pointer m_ptr{nullptr};
        index_type m_size{0};
    };

    template <typename T>
    EKUTIL_CONSTEXPR span<T> make_span(T* ptr, std::ptrdiff_t count) noexcept
    {
        return span<T>(ptr, count);
    }
    template <typename T>
    EKUTIL_CONSTEXPR span<T> make_span(T* first, T* last) noexcept
    {
        return span<T>(first, last);
    }
    template <typename T>
    EKUTIL_CONSTEXPR span<typename T::value_type> make_span(
        T& container) noexcept
    {
        return span<typename T::value_type>(
            std::addressof(*std::begin(container)),
            std::addressof(*(std::end(container) - 1)) + 1);
    }
}  // namespace ekutil

#endif  // EKUTIL_SPAN_H
