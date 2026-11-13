#pragma once

namespace CE
{

template <typename T>
    requires std::is_base_of_v<UActorComponent, T>
T* ULevel::CreateComponent(AActor* owner)
{
    static_assert(!std::is_abstract_v<T> && std::is_default_constructible_v<T>,
              "Components must be default constructible");

    auto handle = componentRegistry.Create<T>();
    auto newComponent = componentRegistry.Resolve<T>(handle);

    newComponent->owner = owner;

    return newComponent;
}

template <typename T>
    requires std::is_base_of_v<UActorComponent, T>
T* ULevel::ResolveComponent(TObjectHandle<T> componentHandle)
{
    return componentRegistry.Resolve<T>(componentHandle);
}

template<typename T>
    requires std::is_base_of_v<AActor, T>
T* ULevel::ResolveActor(TObjectHandle<T> componentHandle)
{
    return actorRegistry.Resolve<T>(componentHandle);
}

}