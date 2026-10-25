#pragma once

namespace CE
{

template<typename T, typename... Args>
    requires std::is_base_of_v<UActorComponent, T>
T* ULevel::CreateComponent(AActor* owner, Args&&... args)
{
    auto handle = componentRegistry.Create<T>(std::forward<Args>(args)...);
    auto newComponent = componentRegistry.Resolve<T>(handle);
    newComponent->owner = owner;

    return newComponent;
}

template<typename T>
    requires std::is_base_of_v<UActorComponent, T>
T* ULevel::ResolveComponent(T* componentHandle)
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