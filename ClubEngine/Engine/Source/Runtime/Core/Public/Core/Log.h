#pragma once

#include <iostream>

template<typename... Args>
void CE_LOG(const char* format, Args... args)
{
    std::printf(format, args...);
    std::printf("\n");
}