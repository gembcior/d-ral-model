// =================================================================================
//
// MIT License
//
// Copyright (c) 2024 Gembcior
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// ==================================================================================

#ifndef DRAL_REGISTER_MODEL_H
#define DRAL_REGISTER_MODEL_H

#include "access_type.h"

#include <cstdint>
#include <utility>

namespace dral {

template<typename RegType, typename AddressPolicy, AccessType Access = AccessType::ReadWrite>
class RegisterModel
{
  static constexpr bool IsReadable{(Access & AccessType::ReadOnly) == AccessType::ReadOnly};
  static constexpr bool IsWritable{(Access & AccessType::WriteOnly) == AccessType::WriteOnly};

public:
  using SizeType = decltype(RegType::value);
  using RegValue = RegType;

  template<typename... Index>
    requires IsReadable
  [[nodiscard]] static auto read(Index&&... index)
  {
    volatile const auto* const regptr{
        reinterpret_cast<volatile const SizeType*>(AddressPolicy::getAddress(std::forward<Index>(index)...))};
    return RegValue{*regptr};
  }

  template<typename... Index>
    requires IsWritable
  static void write(const SizeType value, Index&&... index)
  {
    volatile auto* const regptr{
        reinterpret_cast<volatile SizeType*>(AddressPolicy::getAddress(std::forward<Index>(index)...))};
    *regptr = value;
  }

  template<typename... Index>
    requires IsWritable
  static void write(const RegValue& reg, Index&&... index)

  {
    write(reg.value, std::forward<Index>(index)...);
  }
};
}

#endif  // DRAL_REGISTER_MODEL_H
