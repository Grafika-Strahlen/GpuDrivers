#pragma once

#include "Common.h"
#include <initializer_list>
#include <functional>

template<typename T, typename = ::std::enable_if_t<sizeof(T) == sizeof(UINT32) || sizeof(T) == sizeof(UINT64)>>
using UIntSelectorT = ::std::conditional_t<sizeof(T) == sizeof(UINT32), UINT32, UINT64>;

template<typename T, typename RetT = UIntSelectorT<T>>
RetT StructToUInt(const T& t) noexcept
{
    return reinterpret_cast<const RetT&>(t);
}
