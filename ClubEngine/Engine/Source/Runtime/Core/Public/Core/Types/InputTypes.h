#pragma once

#include <Core/Types/CoreTypes.h>

#include <string_view>

#define CE_INPUT_KEYS(X)                                                                        \
    X(A) X(B) X(C) X(D) X(E) X(F) X(G) X(H) X(I) X(J) X(K) X(L) X(M)                            \
    X(N) X(O) X(P) X(Q) X(R) X(S) X(T) X(U) X(V) X(W) X(X) X(Y) X(Z)                            \
    X(Zero) X(One) X(Two) X(Three) X(Four) X(Five) X(Six) X(Seven) X(Eight) X(Nine)             \
    X(F1) X(F2) X(F3) X(F4) X(F5) X(F6) X(F7) X(F8) X(F9) X(F10) X(F11) X(F12)                  \
    X(NumPadZero) X(NumPadOne) X(NumPadTwo) X(NumPadThree) X(NumPadFour)                        \
    X(NumPadFive) X(NumPadSix) X(NumPadSeven) X(NumPadEight) X(NumPadNine)                      \
    X(Multiply) X(Add) X(Subtract) X(Decimal) X(Divide) X(NumPadEnter)                          \
    X(SpaceBar) X(Enter) X(Escape) X(Tab) X(BackSpace)                                          \
    X(Insert) X(Delete) X(Home) X(End) X(PageUp) X(PageDown)                                    \
    X(Left) X(Up) X(Right) X(Down)                                                              \
    X(CapsLock) X(ScrollLock) X(NumLock) X(PrintScreen) X(Pause) X(Menu)                        \
    X(LeftShift) X(RightShift) X(LeftControl) X(RightControl)                                   \
    X(LeftAlt) X(RightAlt) X(LeftSuper) X(RightSuper)                                           \
    X(Apostrophe) X(Comma) X(Hyphen) X(Period) X(Slash) X(Semicolon) X(Equals)                  \
    X(LeftBracket) X(Backslash) X(RightBracket) X(Tilde)                                        \
    X(LeftMouseButton) X(RightMouseButton) X(MiddleMouseButton)                                 \
    X(ThumbMouseButton) X(ThumbMouseButton2)                                                    \
    X(MouseX) X(MouseY) X(MouseWheelAxis)

namespace CE
{

enum EKey : uint16
{
    None,

#define CE_DEFINE_KEY(Name) Name,
    CE_INPUT_KEYS(CE_DEFINE_KEY)
#undef CE_DEFINE_KEY

    Count
};

constexpr bool IsAxisKey(const EKey key)
{
    return key == MouseX || key == MouseY || key == MouseWheelAxis;
}

constexpr bool IsMouseButton(const EKey key)
{
    return key >=LeftMouseButton && key <= MouseWheelAxis;
}

char  ToString    (EKey key);
EKey        ToKey       (std::string_view key);

}