#pragma once

#include <Core/Types/InputTypes.h>

namespace CE
{
    EKey ToEKey         (int glfwKey) noexcept;
    EKey ToEKeyMouse    (int glfwKey) noexcept;
}