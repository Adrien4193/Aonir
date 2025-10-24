#pragma once

#include <string>
#include <string_view>

namespace Aonir
{
    auto ToUtf8(std::wstring_view value) -> std::string;
    auto ToUtf16(std::string_view value) -> std::wstring;
}
