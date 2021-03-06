//
// Copyright 2017 (C). Alex Robenko. All rights reserved.
//

// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


#pragma once

#include <tuple>
#include <type_traits>

#include "macro_common.h"
#include "gen_enum.h"


#define COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    template <typename... TArgs> \
    FUNC_ARGS_AUTO_REF_RETURN(COMMS_CONCATENATE(initField_, n_), TArgs&&... args, decltype(v_.template initField<COMMS_CONCATENATE(FieldIdx_, n_)>(std::forward<TArgs>(args)...))) { \
        return v_.template initField<COMMS_CONCATENATE(FieldIdx_, n_)>(std::forward<TArgs>(args)...); \
    } \
    FUNC_AUTO_REF_RETURN(COMMS_CONCATENATE(accessField_, n_), decltype(v_.template accessField<COMMS_CONCATENATE(FieldIdx_, n_)>())) { \
        return v_.template accessField<COMMS_CONCATENATE(FieldIdx_, n_)>(); \
    } \
    FUNC_AUTO_REF_RETURN_CONST(COMMS_CONCATENATE(accessField_, n_), decltype(v_.template accessField<COMMS_CONCATENATE(FieldIdx_, n_)>())) { \
        return v_.template accessField<COMMS_CONCATENATE(FieldIdx_, n_)>(); \
    }

#define COMMS_VARIANT_MEM_ACC_FUNC_1(v_, n_) COMMS_VARIANT_MEM_ACC_FUNC(v_, n_)
#define COMMS_VARIANT_MEM_ACC_FUNC_2(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_1(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_3(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_2(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_4(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_3(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_5(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_4(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_6(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_5(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_7(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_6(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_8(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_7(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_9(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_8(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_10(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_9(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_11(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_10(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_12(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_11(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_13(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_12(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_14(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_13(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_15(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_14(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_16(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_15(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_17(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_16(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_18(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_17(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_19(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_18(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_20(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_19(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_21(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_20(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_22(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_21(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_23(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_22(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_24(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_23(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_25(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_24(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_26(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_25(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_27(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_26(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_28(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_27(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_29(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_28(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_30(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_29(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_31(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_30(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_32(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_31(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_33(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_32(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_34(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_33(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_35(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_34(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_36(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_35(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_37(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_36(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_38(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_37(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_39(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_38(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_40(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_39(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_41(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_40(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_42(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_41(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_43(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_42(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_44(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_43(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_45(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_44(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_46(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_45(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_47(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_46(v_, __VA_ARGS__))
#define COMMS_VARIANT_MEM_ACC_FUNC_48(v_, n_, ...) \
    COMMS_VARIANT_MEM_ACC_FUNC(v_, n_) \
    COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_47(v_, __VA_ARGS__))

#define COMMS_CHOOSE_VARIANT_MEM_ACC_FUNC_(N, v_, ...) COMMS_EXPAND(COMMS_VARIANT_MEM_ACC_FUNC_ ## N(v_, __VA_ARGS__))
#define COMMS_CHOOSE_VARIANT_MEM_ACC_FUNC(N, v_, ...) COMMS_EXPAND(COMMS_CHOOSE_VARIANT_MEM_ACC_FUNC_(N, v_, __VA_ARGS__))
#define COMMS_DO_VARIANT_MEM_ACC_FUNC(v_, ...) \
    COMMS_EXPAND(COMMS_CHOOSE_VARIANT_MEM_ACC_FUNC(COMMS_NUM_ARGS(__VA_ARGS__), v_, __VA_ARGS__))
















