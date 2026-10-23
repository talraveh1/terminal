// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.
#pragma once
#include <string_view>

// Only inspect global options. A shell argument or tab title named
// --background must not make the ordinary launcher wait for its window to exit.
inline bool IsBackgroundInvocation(int argc, const wchar_t* const* argv) noexcept
{
    for (int i = 1; i < argc; ++i)
    {
        const std::wstring_view arg{ argv[i] };
        if (arg == L"--background")
        {
            return true;
        }
        if (arg == L"-w" || arg == L"--window" || arg == L"--pos" ||
            arg == L"--size" || arg == L"-s" || arg == L"--saved")
        {
            ++i;
        }
        else if (arg.empty() || arg.front() != L'-' || arg == L"--")
        {
            break;
        }
    }
    return false;
}
