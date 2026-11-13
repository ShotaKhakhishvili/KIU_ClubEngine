#pragma once

#include <type_traits>

namespace CE
{

template<typename T>
    requires std::is_base_of_v<UActorComponent,T>
T* AActor::CreateDefaultSubobject(const char* name)
{
    T* newSubobject = level->CreateComponent<T>(this);

    if (name != nullptr)
        newSubobject->SetName(name);

    ownedComponents.push_back(newSubobject);
    return newSubobject;
}

}