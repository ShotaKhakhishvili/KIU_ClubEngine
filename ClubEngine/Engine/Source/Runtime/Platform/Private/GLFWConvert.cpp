#include "GLFWConvert.h"

#include <GLFW/glfw3.h>

namespace CE
{

namespace
{
    constexpr EKey Offset(const EKey first, const int delta) noexcept
    {
        return static_cast<EKey>(static_cast<uint16>(first) + delta);
    }

    constexpr int Span(const EKey first, const EKey last) noexcept
    {
        return static_cast<int>(last) - static_cast<int>(first);
    }

    static_assert(Span(A,          Z)           == GLFW_KEY_Z    - GLFW_KEY_A,    "A..Z must be contiguous");
    static_assert(Span(Zero,       Nine)        == GLFW_KEY_9    - GLFW_KEY_0,    "Zero..Nine must be contiguous");
    static_assert(Span(F1,         F12)         == GLFW_KEY_F12  - GLFW_KEY_F1,   "F1..F12 must be contiguous");
    static_assert(Span(NumPadZero, NumPadNine)  == GLFW_KEY_KP_9 - GLFW_KEY_KP_0, "NumPad0..9 must be contiguous");
}

EKey ToEKey(const int glfwKey) noexcept
{
    if (glfwKey >= GLFW_KEY_A    && glfwKey <= GLFW_KEY_Z)    return Offset(A,          glfwKey - GLFW_KEY_A);
    if (glfwKey >= GLFW_KEY_0    && glfwKey <= GLFW_KEY_9)    return Offset(Zero,       glfwKey - GLFW_KEY_0);
    if (glfwKey >= GLFW_KEY_F1   && glfwKey <= GLFW_KEY_F12)  return Offset(F1,         glfwKey - GLFW_KEY_F1);
    if (glfwKey >= GLFW_KEY_KP_0 && glfwKey <= GLFW_KEY_KP_9) return Offset(NumPadZero, glfwKey - GLFW_KEY_KP_0);

    switch (glfwKey)
    {
        case GLFW_KEY_KP_MULTIPLY:      return Multiply;
        case GLFW_KEY_KP_ADD:           return Add;
        case GLFW_KEY_KP_SUBTRACT:      return Subtract;
        case GLFW_KEY_KP_DECIMAL:       return Decimal;
        case GLFW_KEY_KP_DIVIDE:        return Divide;
        case GLFW_KEY_KP_ENTER:         return NumPadEnter;
        case GLFW_KEY_KP_EQUAL:         return Equals;

        case GLFW_KEY_SPACE:            return SpaceBar;
        case GLFW_KEY_ENTER:            return Enter;
        case GLFW_KEY_ESCAPE:           return Escape;
        case GLFW_KEY_TAB:              return Tab;
        case GLFW_KEY_BACKSPACE:        return BackSpace;

        case GLFW_KEY_INSERT:           return Insert;
        case GLFW_KEY_DELETE:           return Delete;
        case GLFW_KEY_HOME:             return Home;
        case GLFW_KEY_END:              return End;
        case GLFW_KEY_PAGE_UP:          return PageUp;
        case GLFW_KEY_PAGE_DOWN:        return PageDown;

        case GLFW_KEY_LEFT:             return Left;
        case GLFW_KEY_UP:               return Up;
        case GLFW_KEY_RIGHT:            return Right;
        case GLFW_KEY_DOWN:             return Down;

        case GLFW_KEY_CAPS_LOCK:        return CapsLock;
        case GLFW_KEY_SCROLL_LOCK:      return ScrollLock;
        case GLFW_KEY_NUM_LOCK:         return NumLock;
        case GLFW_KEY_PRINT_SCREEN:     return PrintScreen;
        case GLFW_KEY_PAUSE:            return Pause;
        case GLFW_KEY_MENU:             return Menu;

        case GLFW_KEY_LEFT_SHIFT:       return LeftShift;
        case GLFW_KEY_RIGHT_SHIFT:      return RightShift;
        case GLFW_KEY_LEFT_CONTROL:     return LeftControl;
        case GLFW_KEY_RIGHT_CONTROL:    return RightControl;
        case GLFW_KEY_LEFT_ALT:         return LeftAlt;
        case GLFW_KEY_RIGHT_ALT:        return RightAlt;
        case GLFW_KEY_LEFT_SUPER:       return LeftSuper;
        case GLFW_KEY_RIGHT_SUPER:      return RightSuper;

        case GLFW_KEY_APOSTROPHE:       return Apostrophe;
        case GLFW_KEY_COMMA:            return Comma;
        case GLFW_KEY_MINUS:            return Hyphen;
        case GLFW_KEY_PERIOD:           return Period;
        case GLFW_KEY_SLASH:            return Slash;
        case GLFW_KEY_SEMICOLON:        return Semicolon;
        case GLFW_KEY_EQUAL:            return Equals;
        case GLFW_KEY_LEFT_BRACKET:     return LeftBracket;
        case GLFW_KEY_BACKSLASH:        return Backslash;
        case GLFW_KEY_RIGHT_BRACKET:    return RightBracket;
        case GLFW_KEY_GRAVE_ACCENT:     return Tilde;

        default:                        return None;
    }
}

EKey ToEKeyMouse(const int glfwKey) noexcept
{
    switch (glfwKey)
    {
        case GLFW_MOUSE_BUTTON_LEFT:    return LeftMouseButton;
        case GLFW_MOUSE_BUTTON_RIGHT:   return RightMouseButton;
        case GLFW_MOUSE_BUTTON_MIDDLE:  return MiddleMouseButton;
        case GLFW_MOUSE_BUTTON_4:       return ThumbMouseButton;
        case GLFW_MOUSE_BUTTON_5:       return ThumbMouseButton2;
        default:                        return None;
    }
}

}