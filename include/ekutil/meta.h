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

#ifndef EKUTIL_META_H
#define EKUTIL_META_H

#include "compat.h"

#include <cstddef>

namespace ekutil {
#if EKUTIL_HAS_VOID_T
    template <typename... Ts>
    using void_t = std::void_t<Ts...>;
#else
    template <typename... Ts>
    struct make_void {
        using type = void;
    };
    template <typename... Ts>
    using void_t = typename make_void<Ts...>::type;
#endif

    template <size_t I>
    struct priority_tag : priority_tag<I - 1> {
    };
    template <>
    struct priority_tag<0> {
    };

    template <typename T>
    EKUTIL_CONSTEXPR T constexpr_max(T val)
    {
        return val;
    }
    template <typename T, typename... Ts>
    EKUTIL_CONSTEXPR T constexpr_max(T val, Ts... a)
    {
        return val > constexpr_max(a...) ? val : constexpr_max(a...);
    }

}  // namespace ekutil

#endif  // EKUTIL_META_H
