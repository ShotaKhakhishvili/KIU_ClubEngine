#include <Engine/Input/Types/FInputState.h>

namespace CE
{

bool FInputState::IsDown         (const EKey key) const {    return down.test(key);      }
bool FInputState::WasPressed     (const EKey key) const {    return pressed.test(key);   }
bool FInputState::WasReleased    (const EKey key) const {    return released.test(key);  }

float FInputState::GetKeyValue(const EKey key) const
{
    switch (key)
    {
    case EKey::MouseX:          return mouseDelta.x;
    case EKey::MouseY:          return mouseDelta.y;
    case EKey::MouseWheelAxis:  return wheelDelta;
    default:                    return IsDown(key) ? 1.0f : 0.0f;
    }
};



}