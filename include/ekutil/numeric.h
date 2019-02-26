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

#ifndef EKUTIL_NUMERIC_H
#define EKUTIL_NUMERIC_H

#include "compat.h"

#include <cstdint>
#include <limits>

namespace ekutil {
    template <typename Integral>
    int max_digits(int base) noexcept
    {
        using lim = std::numeric_limits<Integral>;

        static int base8_digits[4] = {3, 5, 11, 21};

        if (base == 10) {
            return lim::digits10;
        }
        if (base == 8) {
            return base8_digits[sizeof(Integral) - 1];
        }
        if (base == lim::radix) {
            return lim::digits;
        }

        auto i = lim::max();

        int digits = 0;
        while (i) {
            i /= static_cast<Integral>(base);
            digits++;
        }
        return digits;
    }

    uint64_t next_pow2(uint64_t x)
    {
        --x;
        x |= (x >> 1);
        x |= (x >> 2);
        x |= (x >> 4);
        x |= (x >> 8);
        x |= (x >> 16);
        x |= (x >> 32);
        return x + 1;
    }
    uint32_t next_pow2(uint32_t x)
    {
        --x;
        x |= (x >> 1);
        x |= (x >> 2);
        x |= (x >> 4);
        x |= (x >> 8);
        x |= (x >> 16);
        return x + 1;
    }
}  // namespace ekutil

#endif  // EKUTIL_NUMERIC_H

