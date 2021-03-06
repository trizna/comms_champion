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

namespace comms
{

namespace field
{

namespace adapter
{

template <typename TInitialiser, typename TBase>
class DefaultValueInitialiser : public TBase
{
    using Base = TBase;
    using Initialiser = TInitialiser;
public:
    using ValueType = typename Base::ValueType;

    DefaultValueInitialiser()
    {
        Initialiser()(*this);
    }

    explicit DefaultValueInitialiser(const ValueType& val)
      : Base(val)
    {
    }

    explicit DefaultValueInitialiser(ValueType&& val)
      : Base(std::move(val))
    {
    }

    DefaultValueInitialiser(const DefaultValueInitialiser&) = default;
    DefaultValueInitialiser(DefaultValueInitialiser&&) = default;
    DefaultValueInitialiser& operator=(const DefaultValueInitialiser&) = default;
    DefaultValueInitialiser& operator=(DefaultValueInitialiser&&) = default;
};

}  // namespace adapter

}  // namespace field

}  // namespace comms



