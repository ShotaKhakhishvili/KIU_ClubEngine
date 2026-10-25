#pragma once

#include <type_traits>

namespace CE
{

template<typename T, typename... Args>
    requires std::is_base_of_v<UActorComponent,T>
T* AActor::CreateDefaultSubobject(Args&&... args)
{
    T* newSubobject = level->CreateComponent<T>(std::forward<Args>(args)...);
    ownedComponents.push_back(newSubobject);

    return newSubobject;
}

}