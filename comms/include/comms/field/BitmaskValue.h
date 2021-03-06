//
// Copyright 2014 - 2015 (C). Alex Robenko. All rights reserved.
//

// This library is free software: you can redistribute it and/or modify
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

#include <limits>
#include "comms/Field.h"

#include "comms/util/SizeToType.h"
#include "details/AdaptBasicField.h"
#include "details/OptionsParser.h"
#include "comms/details/gen_enum.h"
#include "comms/details/bits_access.h"
#include "IntValue.h"
#include "tag.h"

namespace comms
{

namespace field
{

namespace details
{

template <bool THasFixedLength>
struct BitmaskUndertlyingType;

template <>
struct BitmaskUndertlyingType<true>
{
    template <typename TOptionsBundle>
    using Type = typename comms::util::SizeToType<TOptionsBundle::FixedLength, false>::Type;
};

template <>
struct BitmaskUndertlyingType<false>
{
    template <typename TOptionsBundle>
    using Type = unsigned;
};

template <typename TOptionsBundle>
using BitmaskUndertlyingTypeT =
    typename BitmaskUndertlyingType<TOptionsBundle::HasFixedLengthLimit>::template Type<TOptionsBundle>;


}  // namespace details

/// @brief Bitmask value field.
/// @details Quite often communication protocols specify bitmask values, where
///     any bit has a specific meaning. Although such masks are can be handled
///     as unsigned integer values using comms::field::IntValue field type,
///     using comms::field::Bitmask may be a bit more convenient.
/// @tparam TFieldBase Base class for this field, expected to be a variant of
///     comms::Field.
/// @tparam TOptions Zero or more options that modify/refine default behaviour
///     of the field. If no option is provided, the underlying type is assumed
///     to be "unsigned", which is usually 4 bytes long. To redefined the length
///     of the bitmask field, use comms::option::FixedLength option.
///     For example:
///     @code
///         using MyFieldBase = comms::Field<comms::option::BigEndian>;
///         using MyField =comms::field::EnumValue<MyFieldBase>;
///     @endcode
///     The serialised value of the field in the example above will consume
///     sizeof(unsigned) bytes, because the underlying type chosen to be "unsigned"
///     by default. Example below specifies simple bitmask value field with
///     2 bytes serialisation length:
///     @code
///         using MyFieldBase = comms::Field<comms::option::BigEndian>;
///         using MyField =comms::field::EnumValue<MyFieldBase, comms::option::FixedLength<2> >;
///     @endcode
///     Supported options are:
///     @li comms::option::FixedLength
///     @li comms::option::FixedBitLength
///     @li comms::option::DefaultValueInitialiser or comms::option::DefaultNumValue.
///     @li comms::option::ContentsValidator or comms::option::BitmaskReservedBits.
///     @li comms::option::ContentsRefresher
///     @li comms::option::FailOnInvalid
///     @li comms::option::IgnoreInvalid
/// @extends comms::Field
/// @headerfile comms/field/BitmaskValue.h
/// @see COMMS_BITMASK_BITS()
/// @see COMMS_BITMASK_BITS_ACCESS()
/// @see COMMS_BITMASK_BITS_SEQ()
template <typename TFieldBase, typename... TOptions>
class BitmaskValue : public TFieldBase
{
    using Base = TFieldBase;

    using OptionsBundle = details::OptionsParser<TOptions...>;

    using IntValueType = details::BitmaskUndertlyingTypeT<OptionsBundle>;

    using IntValueField =
        IntValue<
            TFieldBase,
            IntValueType,
            TOptions...
        >;

public:

    /// @brief All the options provided to this class bundled into struct.
    using ParsedOptions = OptionsBundle;

    /// @brief Tag indicating type of the field
    using Tag = tag::Bitmask;

    /// @brief Type of underlying integral value.
    /// @details Unsigned integral type, which depends on the length of the
    ///     mask determined by the comms::option::FixedLength option.
    using ValueType = typename IntValueField::ValueType;

    /// @brief Default constructor.
    /// @brief Initial bitmask has all bits cleared (equals 0)
    BitmaskValue() = default;

    /// @brief Constructor
    /// @param[in] val Value of the field to initialise it with.
    explicit BitmaskValue(const ValueType& val)
       : intValue_(val)
    {
    }

    /// @brief Copy constructor
    BitmaskValue(const BitmaskValue&) = default;

    /// @brief Destructor
    ~BitmaskValue() = default;

    /// @brief Copy assignment
    BitmaskValue& operator=(const BitmaskValue&) = default;

    /// @brief Get access to underlying mask value storage.
    /// @return Const reference to the underlying stored value.
    const ValueType& value() const
    {
        return intValue_.value();
    }

    /// @brief Get access to underlying mask value storage.
    /// @return Reference to the underlying stored value.
    ValueType& value()
    {
        return intValue_.value();
    }

    /// @brief Get length required to serialise the current field value.
    /// @return Number of bytes it will take to serialise the field value.
    constexpr std::size_t length() const
    {
        return intValue_.length();
    }

    /// @brief Get maximal length that is required to serialise field of this type.
    /// @return Maximal number of bytes required serialise the field value.
    static constexpr std::size_t maxLength()
    {
        return IntValueField::maxLength();
    }

    /// @brief Get minimal length that is required to serialise field of this type.
    /// @return Minimal number of bytes required serialise the field value.
    static constexpr std::size_t minLength()
    {
        return IntValueField::minLength();
    }

    /// @brief Read field value from input data sequence
    /// @param[in, out] iter Iterator to read the data.
    /// @param[in] size Number of bytes available for reading.
    /// @return Status of read operation.
    /// @post Iterator is advanced.
    template <typename TIter>
    ErrorStatus read(TIter& iter, std::size_t size)
    {
        return intValue_.read(iter, size);
    }

    /// @brief Write current field value to output data sequence
    /// @param[in, out] iter Iterator to write the data.
    /// @param[in] size Maximal number of bytes that can be written.
    /// @return Status of write operation.
    /// @post Iterator is advanced.
    template <typename TIter>
    ErrorStatus write(TIter& iter, std::size_t size) const
    {
        return intValue_.write(iter, size);
    }

    /// @brief Check validity of the field value.
    constexpr bool valid() const
    {
        return intValue_.valid();
    }

    /// @brief Refresh contents of the field
    /// @return @b true in case the field's value has been updated, @b false otherwise
    bool refresh()
    {
        return intValue_.refresh();
    }

    /// @brief Check whether all bits from provided mask are set.
    /// @param[in] mask Mask to check against
    /// @return true in case all the bits are set, false otherwise
    bool hasAllBitsSet(ValueType mask) const
    {
        return (value() & mask) == mask;
    }

    /// @brief Check whether any bits from provided mask are set.
    /// @param[in] mask Mask to check against
    /// @return true in case at least one of the bits is set, false otherwise.
    bool hasAnyBitsSet(ValueType mask) const
    {
        return (value() & mask) != 0;
    }

    /// @brief Set all the provided bits.
    /// @details Equivalent to @code value() |= mask; @endcode
    /// @param[in] mask Mask of bits to set.
    void setBits(ValueType mask)
    {
        value() |= mask;
    }

    /// @brief Set all the provided bits.
    /// @details Equivalent to @code value() &= (~mask); @endcode
    /// @param[in] mask Mask of bits to clear.
    void clearBits(ValueType mask)
    {
        value() &= (~mask);
    }

    /// @brief Get bit value
    bool getBitValue(unsigned bitNum) const
    {
        return hasAllBitsSet(
            static_cast<ValueType>(1U) << bitNum);
    }

    /// @brief Set bit value
    void setBitValue(unsigned bitNum, bool val)
    {
        auto mask = static_cast<ValueType>(1U) << bitNum;
        if (val) {
            setBits(mask);
        }
        else {
            clearBits(mask);
        }
    }

private:

    IntValueField intValue_;
};

// Implementation

/// @brief Equality comparison operator.
/// @param[in] field1 First field.
/// @param[in] field2 Second field.
/// @return true in case fields are equal, false otherwise.
/// @related BitmaskValue
template <typename... TArgs>
bool operator==(
    const BitmaskValue<TArgs...>& field1,
    const BitmaskValue<TArgs...>& field2)
{
    return field1.value() == field2.value();
}

/// @brief Non-equality comparison operator.
/// @param[in] field1 First field.
/// @param[in] field2 Second field.
/// @return true in case fields are NOT equal, false otherwise.
/// @related BitmaskValue
template <typename... TArgs>
bool operator!=(
    const BitmaskValue<TArgs...>& field1,
    const BitmaskValue<TArgs...>& field2)
{
    return field1.value() != field2.value();
}

/// @brief Equivalence comparison operator.
/// @param[in] field1 First field.
/// @param[in] field2 Second field.
/// @return true in case value of the first field is lower than than the value of the second.
/// @related BitmaskValue
template <typename... TArgs>
bool operator<(
    const BitmaskValue<TArgs...>& field1,
    const BitmaskValue<TArgs...>& field2)
{
    return field1.value() < field2.value();
}

/// @brief Compile time check function of whether a provided type is any
///     variant of comms::field::BitmaskValue.
/// @tparam T Any type.
/// @return true in case provided type is any variant of @ref BitmaskValue
/// @related comms::field::BitmaskValue
template <typename T>
constexpr bool isBitmaskValue()
{
    return std::is_same<typename T::Tag, tag::Bitmask>::value;
}

/// @brief Upcast type of the field definition to its parent comms::field::BitmaskValue type
///     in order to have access to its internal types.
/// @related comms::field::BitmaskValue
template <typename TFieldBase, typename... TOptions>
inline
BitmaskValue<TFieldBase, TOptions...>&
toFieldBase(BitmaskValue<TFieldBase, TOptions...>& field)
{
    return field;
}

/// @brief Upcast type of the field definition to its parent comms::field::BitmaskValue type
///     in order to have access to its internal types.
/// @related comms::field::BitmaskValue
template <typename TFieldBase, typename... TOptions>
inline
const BitmaskValue<TFieldBase, TOptions...>&
toFieldBase(const BitmaskValue<TFieldBase, TOptions...>& field)
{
    return field;
}


/// @brief Provide names for bits in comms::field::BitmaskValue field.
/// @details Defines BitIdx enum with all the provided values prefixed with
///     "BitIdx_". For example usage of
///     @code
///     COMMS_BITMASK_BITS(first, second, third, fourth);
///     @endcode
///     will generate the following enum type:
///     @code
///     enum BitIdx
///     {
///         BitIdx_first,
///         BitIdx_second,
///         BitIdx_third,
///         BitIdx_fourth,
///         BitIdx_numOfValues
///     };
///     @endcode
///     @b NOTE, that provided names @b first, @b second, @b third, and @b fourth have
///     found their way to the enum @b BitIdx. @n
///     Also note, that there is automatically added @b BitIdx_nameOfValues
///     value to the end of @b BitIdx enum.
///
///     It is possible to assign values to the provided names. It could be useful
///     when skipping some unused bits. For example
///     @code
///     COMMS_BITMASK_BITS(first=1, third=3, fourth);
///     @endcode
///     will generate the following enum type:
///     @code
///     enum BitIdx
///     {
///         BitIdx_first=1,
///         BitIdx_third=3,
///         BitIdx_fourth,
///         BitIdx_numOfValues
///     };
///     @endcode
///
///     The macro COMMS_BITMASK_BITS() should be used inside definition of the
///     bitmask field to provide names for the bits for external use:
///     @code
///     struct MyField : public comms::field::BitmaskValue<...>
///     {
///         COMMS_BITMASK_BITS(first, second, third, fourth);
///     }
///     @endcode
/// @related comms::field::BitmaskValue
#define COMMS_BITMASK_BITS(...) COMMS_DEFINE_ENUM(BitIdx, __VA_ARGS__)

/// @brief Generate access functions for bits in comms::field::BitmaskValue field.
/// @details The @ref COMMS_BITMASK_BITS() macro defines @b BitIdx enum to
///     be able to access internal bits. However, an ability to provide
///     values to the enumeration values using @b =val suffixes doesn't
///     allow generation of convenience access functions to the bits. That's
///     why COMMS_BITMASK_BITS_ACCESS() macro was introduced. For every name
///     listed in the parameters list, @b getBitValue_*() and @b setBitValue_*()
///     functions will be generated. For example, having the following definition
///     @code
///     struct MyField : public comms::field::BitmaskValue<...>
///     {
///         ...
///         COMMS_BITMASK_BITS_ACCESS(first, third, fourth);
///     }
///     @endcode
///     is equivalent to having following functions defined:
///     @code
///     struct MyField : public comms::field::BitmaskValue<...>
///     {
///         ...
///         bool getBitValue_first() const {
///             return getBitValue(BitIdx_first);
///         }
///
///         void setBitValue_first(bool value) {
///             setBitValue(BitIdx_first, value);
///         }
///
///         bool getBitValue_third() const {
///             return getBitValue(BitIdx_third);
///         }
///
///         void setBitValue_third(bool value) {
///             setBitValue(BitIdx_third, value);
///         }
///
///         bool getBitValue_fourth() const {
///             return getBitValue(BitIdx_fourth);
///         }
///
///         void setBitValue_fourth(bool value) {
///             setBitValue(BitIdx_fourth, value);
///         }
///     }
///     @endcode
///     @b NOTE, that generated @b getBitValue_*() and @b setBitValue_*()
///     functions use @b BitIdx_* enum values generated by
///     @ref COMMS_BITMASK_BITS(). It means that COMMS_BITMASK_BITS_ACCESS()
///     macro can NOT be used without @ref COMMS_BITMASK_BITS().
///     @code
///     struct MyField : public comms::field::BitmaskValue<...>
///     {
///         COMMS_BITMASK_BITS(first, third=2, fourth);
///         COMMS_BITMASK_BITS_ACCESS(first, third, fourth);
///     }
///     @endcode
/// @pre Must be used together with @ref COMMS_BITMASK_BITS()
/// @related comms::field::BitmaskValue
#define COMMS_BITMASK_BITS_ACCESS(...) \
    FUNC_AUTO_REF_RETURN(asBitmask, decltype(comms::field::toFieldBase(*this))) { \
        return comms::field::toFieldBase(*this); \
    }\
    FUNC_AUTO_REF_RETURN_CONST(asBitmask, decltype(comms::field::toFieldBase(*this))) { \
        return comms::field::toFieldBase(*this); \
    } \
    COMMS_DO_BIT_ACC_FUNC(asBitmask(), __VA_ARGS__)

/// @brief Combine usage of @ref COMMS_BITMASK_BITS() and @ref COMMS_BITMASK_BITS_ACCESS().
/// @details When assigned bit names start at bit 0 and go sequentially without
///     any gaps in the middle, i.e. don't have any @b =val suffixes, then use
///     COMMS_BITMASK_BITS_SEQ() macro to name the bits. It is defined to use
///     @ref COMMS_BITMASK_BITS() and @ref COMMS_BITMASK_BITS_ACCESS() with the
///     same bit names. For example
///     @code
///     struct MyField : public comms::field::BitmaskValue<...>
///     {
///         COMMS_BITMASK_BITS_SEQ(first, second, third, fourth);
///     }
///     @endcode
///     is equivalent to having
///     @code
///     struct MyField : public comms::field::BitmaskValue<...>
///     {
///         enum BitIdx
///         {
///             BitIdx_first,
///             BitIdx_second,
///             BitIdx_third,
///             BitIdx_fourth,
///             BitIdx_numOfValues
///         }
///
///         bool getBitValue_first() const {...}
///         void setBitValue_first(bool value) {...}
///         bool getBitValue_second() const {...}
///         void setBitValue_second(bool value) {...}
///         bool getBitValue_third() const {...}
///         void setBitValue_third(bool value) {...}
///         bool getBitValue_fourth() const {...}
///         void setBitValue_fourth(bool value) {...}
///     };
///     @endcode
/// @related comms::field::BitmaskValue
#define COMMS_BITMASK_BITS_SEQ(...) \
    COMMS_BITMASK_BITS(__VA_ARGS__) \
    COMMS_BITMASK_BITS_ACCESS(__VA_ARGS__)

}  // namespace field

}  // namespace comms


