//
// Copyright 2015 - 2017 (C). Alex Robenko. All rights reserved.
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

#include "comms/ErrorStatus.h"
#include "comms/options.h"
#include "basic/Bitfield.h"
#include "details/AdaptBasicField.h"
#include "tag.h"

namespace comms
{

namespace field
{

/// @brief Bitfield field.
/// @details Sometimes one or several bytes can be logically split into two
///     or more independent values, which are packed together to save some
///     space. For example, one enum type that has only 4 possible values, i.e
///     only two bits are needed to encode such value. It would be a waste to
///     allocate full byte for it. Instead, it is packed with some other, say
///     unsigned counter that requires up to 6 bits to encode its valid
///     range of values. The following code defines such field:
///     @code
///         enum class MyEnumType : std::uint8_t
///         {
///             Value1,
///             Value2,
///             Value3,
///             Value4
///         };
///
///         using MyFieldBase = comms::Field<comms::option::BigEndian>;
///         using MyField =
///             comms::field::Bitfield<
///                 MyFieldBase,
///                 std::tuple<
///                     comms::field::EnumValue<
///                         MyFieldBase,
///                         MyEnumType,
///                         comms::option::FixedBitLength<2>
///                     >,
///                     comms::field::IntValue<
///                         MyFieldBase,
///                         std::uint8_t,
///                         comms::option::FixedBitLength<6>
///                     >
///                 >
///             >;
///     @endcode
///     Note, that bitfield members fields specify their length in bits using
///     comms::option::FixedBitLength option.
///     Also note, that all bitfield member's lengths in bits combined create
///     a round number of bytes, i.e all the bits must sum up to 8, 16, 24, 32, ...
///     bits.
///
///     Refer to @ref sec_field_tutorial_bitfield for tutorial and usage examples.
/// @tparam TFieldBase Base class for this field, expected to be a variant of
///     comms::Field.
/// @tparam TMembers All member fields bundled together in
///     <a href="http://en.cppreference.com/w/cpp/utility/tuple">std::tuple</a>.
/// @tparam TOptions Zero or more options that modify/refine default behaviour
///     of the field.@n
///     Supported options are:
///     @li comms::option::ContentsValidator - All field members may specify
///         their independent validators. The bitfield field considered to
///         be valid if all the field members are valid. This option though,
///         provides an ability to add extra validation logic that can
///         observe value of more than one bitfield member. For example,
///         protocol specifies that if one specific member has value X, than
///         other member is NOT allowed to have value Y.
///     @li comms::option::ContentsRefresher - The default refreshing
///         behaviour is to call the @b refresh() member function of every
///         member field. This option provides an ability to set a custom
///         "refreshing" logic.
/// @pre TMember is a variant of std::tuple, that contains other fields.
/// @pre Every field member specifies its length in bits using
///     comms::option::FixedBitLength option.
/// @extends comms::Field
/// @headerfile comms/field/Bitfield.h
template <typename TFieldBase, typename TMembers, typename... TOptions>
class Bitfield : public
        details::AdaptBasicFieldT<basic::Bitfield<TFieldBase, TMembers>, TOptions...>
{
    using Base = details::AdaptBasicFieldT<basic::Bitfield<TFieldBase, TMembers>, TOptions...>;

public:
    /// @brief All the options provided to this class bundled into struct.
    using ParsedOptions = details::OptionsParser<TOptions...>;

    /// @brief Tag indicating type of the field
    using Tag = tag::Bitfield;

    /// @brief Value type.
    /// @details Same as TMemebers template argument, i.e. it is std::tuple
    ///     of all the member fields.
    using ValueType = typename Base::ValueType;

    /// @brief Default constructor
    /// @details All field members are initialised using their default constructors.
    Bitfield() = default;

    /// @brief Constructor
    /// @param[in] val Value of the field to initialise it with.
    explicit Bitfield(const ValueType& val)
      : Base(val)
    {
    }

    /// @brief Constructor
    /// @param[in] val Value of the field to initialise it with.
    explicit Bitfield(ValueType&& val)
      : Base(std::move(val))
    {
    }

    /// @brief Retrieve number of bits specified member field consumes.
    /// @tparam TIdx Index of the member field.
    /// @return Number of bits, specified with comms::option::FixedBitLength option
    ///     used with the requested member.
    template <std::size_t TIdx>
    static constexpr std::size_t memberBitLength()
    {
        static_assert(
            TIdx < std::tuple_size<ValueType>::value,
            "Index exceeds number of fields");

        using FieldType = typename std::tuple_element<TIdx, ValueType>::type;
        using FieldOptions = typename FieldType::ParsedOptions;
        return FieldOptions::FixedBitLength;
    }


#ifdef FOR_DOXYGEN_DOC_ONLY
    /// @brief Get access to the stored tuple of fields.
    /// @return Const reference to the underlying stored value.
    const ValueType& value() const;

    /// @brief Get access to the stored tuple of fields.
    /// @return Reference to the underlying stored value.
    ValueType& value();

    /// @brief Get length required to serialise the current field value.
    /// @return Number of bytes it will take to serialise the field value.
    constexpr std::size_t length() const;

    /// @brief Get minimal length that is required to serialise field of this type.
    /// @return Minimal number of bytes required serialise the field value.
    static constexpr std::size_t minLength();

    /// @brief Get maximal length that is required to serialise field of this type.
    /// @return Maximal number of bytes required serialise the field value.
    static constexpr std::size_t maxLength();

    /// @brief Read field value from input data sequence
    /// @param[in, out] iter Iterator to read the data.
    /// @param[in] size Number of bytes available for reading.
    /// @return Status of read operation.
    /// @post Iterator is advanced.
    template <typename TIter>
    ErrorStatus read(TIter& iter, std::size_t size);

    /// @brief Write current field value to output data sequence
    /// @param[in, out] iter Iterator to write the data.
    /// @param[in] size Maximal number of bytes that can be written.
    /// @return Status of write operation.
    /// @post Iterator is advanced.
    template <typename TIter>
    ErrorStatus write(TIter& iter, std::size_t size) const;

    /// @brief Check validity of the field value.
    bool valid() const;

    /// @brief Refresh the field's contents
    /// @details Calls refresh() member function on every member field, will
    ///     return @b true if any of the calls returns @b true.
    bool refresh();

#endif // #ifdef FOR_DOXYGEN_DOC_ONLY
};

/// @brief Equality comparison operator.
/// @param[in] field1 First field.
/// @param[in] field2 Second field.
/// @return true in case fields are equal, false otherwise.
/// @related Bitfield
template <typename... TArgs>
bool operator==(
    const Bitfield<TArgs...>& field1,
    const Bitfield<TArgs...>& field2)
{
    return field1.value() == field2.value();
}

/// @brief Non-equality comparison operator.
/// @param[in] field1 First field.
/// @param[in] field2 Second field.
/// @return true in case fields are NOT equal, false otherwise.
/// @related Bitfield
template <typename... TArgs>
bool operator!=(
    const Bitfield<TArgs...>& field1,
    const Bitfield<TArgs...>& field2)
{
    return field1.value() != field2.value();
}

/// @brief Equivalence comparison operator.
/// @param[in] field1 First field.
/// @param[in] field2 Second field.
/// @return true in case value of the first field is lower than than the value of the second.
/// @related Bitfield
template <typename... TArgs>
bool operator<(
    const Bitfield<TArgs...>& field1,
    const Bitfield<TArgs...>& field2)
{
    return field1.value() < field2.value();
}

/// @brief Compile time check function of whether a provided type is any
///     variant of comms::field::Bitfield.
/// @tparam T Any type.
/// @return true in case provided type is any variant of @ref Bitfield
/// @related comms::field::Bitfield
template <typename T>
constexpr bool isBitfield()
{
    return std::is_same<typename T::Tag, tag::Bitfield>::value;
}

/// @brief Upcast type of the field definition to its parent comms::field::Bitfield type
///     in order to have access to its internal types.
/// @related comms::field::Bitfield
template <typename TFieldBase, typename TMembers, typename... TOptions>
inline
Bitfield<TFieldBase, TMembers, TOptions...>&
toFieldBase(Bitfield<TFieldBase, TMembers, TOptions...>& field)
{
    return field;
}

/// @brief Upcast type of the field definition to its parent comms::field::Bitfield type
///     in order to have access to its internal types.
/// @related comms::field::Bitfield
template <typename TFieldBase, typename TMembers, typename... TOptions>
inline
const Bitfield<TFieldBase, TMembers, TOptions...>&
toFieldBase(const Bitfield<TFieldBase, TMembers, TOptions...>& field)
{
    return field;
}

}  // namespace field

}  // namespace comms


