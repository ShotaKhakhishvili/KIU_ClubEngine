#pragma once

namespace CE
{

template <typename T>
bool UObject::IsA() const noexcept
{
    return GetClass()->IsChildOf(T::StaticClass ());
}

}