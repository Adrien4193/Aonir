#pragma once

#include <string>
#include <string_view>

namespace Aonir
{
    AONIR_PLATFORM_API auto ToUtf8(std::wstring_view value) -> std::string;
    AONIR_PLATFORM_API auto ToUtf16(std::string_view value) -> std::wstring;
}
