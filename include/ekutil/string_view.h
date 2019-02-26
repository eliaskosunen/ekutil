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

#ifndef EKUTIL_STRING_VIEW_H
#define EKUTIL_STRING_VIEW_H

#include "span.h"

#include <limits>

namespace ekutil {
    /**
     * A view over a (sub)string.
     */
    template <typename CharT, typename Traits = std::char_traits<CharT>>
    class basic_string_view {
    public:
        using traits_type = Traits;
        using value_type = CharT;
        using span_type = span<const value_type>;

        using pointer = value_type*;
        using const_pointer = const value_type*;
        using reference = value_type&;
        using const_reference = const value_type&;
        using iterator = typename span_type::const_iterator;
        using const_iterator = iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = reverse_iterator;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using span_index_type = typename span_type::index_type;

        static EKUTIL_CONSTEXPR_DECL const size_type npos = size_type(-1);

        EKUTIL_CONSTEXPR basic_string_view() noexcept = default;
        EKUTIL_CONSTEXPR basic_string_view(const_pointer s, size_type c)
            : m_data(s, static_cast<span_index_type>(c))
        {
        }
        EKUTIL_CONSTEXPR basic_string_view(const_pointer s)
            : m_data(s, static_cast<span_index_type>(Traits::length(s)))
        {
        }
        template <size_t N>
        EKUTIL_CONSTEXPR basic_string_view(const CharT (&s)[N]) : m_data(s, N)
        {
        }

        EKUTIL_CONSTEXPR const_iterator begin() const noexcept
        {
            return cbegin();
        }
        EKUTIL_CONSTEXPR const_iterator cbegin() const noexcept
        {
            return m_data.cbegin();
        }
        EKUTIL_CONSTEXPR const_iterator end() const noexcept
        {
            return cend();
        }
        EKUTIL_CONSTEXPR const_iterator cend() const noexcept
        {
            return m_data.cend();
        }

        EKUTIL_CONSTEXPR const_iterator rbegin() const noexcept
        {
            return crbegin();
        }
        EKUTIL_CONSTEXPR const_iterator crbegin() const noexcept
        {
            return m_data.crbegin();
        }
        EKUTIL_CONSTEXPR const_iterator rend() const noexcept
        {
            return crend();
        }
        EKUTIL_CONSTEXPR const_iterator crend() const noexcept
        {
            return m_data.crend();
        }

        EKUTIL_CONSTEXPR const_reference operator[](size_type pos) const
        {
            return m_data[static_cast<typename span_type::index_type>(pos)];
        }
        EKUTIL_CONSTEXPR14 const_reference at(size_type pos) const
        {
            return m_data.at(static_cast<typename span_type::index_type>(pos));
        }

        EKUTIL_CONSTEXPR const_reference front() const
        {
            return operator[](0);
        }
        EKUTIL_CONSTEXPR const_reference back() const
        {
            return operator[](size() - 1);
        }
        EKUTIL_CONSTEXPR const_pointer data() const noexcept
        {
            return m_data.data();
        }

        EKUTIL_CONSTEXPR size_type size() const noexcept
        {
            return static_cast<size_type>(m_data.size());
        }
        EKUTIL_CONSTEXPR size_type length() const noexcept
        {
            return size();
        }
        EKUTIL_CONSTEXPR size_type max_size() const noexcept
        {
            return std::numeric_limits<size_type>::max() - 1;
        }
        EKUTIL_NODISCARD EKUTIL_CONSTEXPR bool empty() const noexcept
        {
            return size() == 0;
        }

        EKUTIL_CONSTEXPR14 void remove_prefix(size_type n)
        {
            m_data = m_data.subspan(static_cast<std::ptrdiff_t>(n));
        }
        EKUTIL_CONSTEXPR14 void remove_suffix(size_type n)
        {
            m_data = m_data.first(size() - n);
        }

        EKUTIL_CONSTEXPR14 void swap(basic_string_view& v) noexcept
        {
            using std::swap;
            swap(m_data, v.m_data);
        }

        size_type copy(pointer dest, size_type count, size_type pos = 0) const
        {
            auto n = std::min(count, size() - pos);
            Traits::copy(dest, data() + pos, n);
            return n;
        }
        EKUTIL_CONSTEXPR14 basic_string_view
        substr(size_type pos = 0, size_type count = npos) const
        {
            auto n = std::min(count, size() - pos);
            return m_data.subspan(pos, n);
        }

        int compare(basic_string_view v) const noexcept
        {
            auto n = std::min(size(), v.size());
            auto cmp = Traits::compare(data(), v.data(), n);
            if (cmp == 0) {
                if (size() == v.size()) {
                    return 0;
                }
                if (size() > v.size()) {
                    return 1;
                }
                return -1;
            }
            return cmp;
        }
        int compare(size_type pos1, size_type count1, basic_string_view v) const
        {
            return substr(pos1, count1).compare(v);
        }
        int compare(size_type pos1,
                    size_type count1,
                    basic_string_view v,
                    size_type pos2,
                    size_type count2) const
        {
            return substr(pos1, count1).compare(v.substr(pos2, count2));
        }
        int compare(const_pointer s) const
        {
            return compare(basic_string_view(s));
        }
        int compare(size_type pos1, size_type count1, const_pointer s) const
        {
            return substr(pos1, count1).compare(basic_string_view(s));
        }
        int compare(size_type pos1,
                    size_type count1,
                    const_pointer s,
                    size_type count2) const
        {
            return substr(pos1, count1).compare(basic_string_view(s, count2));
        }

    private:
        span_type m_data{};
    };

    using string_view = basic_string_view<char>;
    using wstring_view = basic_string_view<wchar_t>;
    using u16string_view = basic_string_view<char>;
    using u32wstring_view = basic_string_view<wchar_t>;

}  // namespace ekutil

#endif  // EKUTIL_STRING_VIEW_H
