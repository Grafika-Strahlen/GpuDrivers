#pragma once

#include <String.hpp>

namespace gs {

WDynString FormatWindowsError(const unsigned long dwErr) noexcept;

C8ConstExprString StringifyFacility(const long hResult) noexcept;
C8ConstExprString StringifyHResultFacility(const long hResult) noexcept;
C8ConstExprString StringifyHResult(const long hResult) noexcept;

}
