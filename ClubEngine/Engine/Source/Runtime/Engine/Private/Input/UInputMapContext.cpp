#include <Engine/Input/UInputMapContext.h>

namespace CE
{

FActionKeyMapping* UInputMapContext::AddMapping(UInputAction* action, const EKey key)
{
    if (!action)
        return nullptr;

    const int32 idx = FindMappingIndex(action, key);
    if (idx != -1)
        return &mappings[idx];

    mappings.emplace_back(key, action);
    return &mappings.back();
}

void UInputMapContext::RemoveMapping(const UInputAction* action, const EKey key)
{
    const int32 idx = FindMappingIndex(action, key);

    if (idx != -1)
        mappings.erase(mappings.begin() + idx);
}

void UInputMapContext::RemoveAllKeysFromMapping(UInputAction* action)
{
    std::erase_if(mappings, [action](const FActionKeyMapping& mapping)
{
    return mapping.GetInputAction() == action;
});
}

int32 UInputMapContext::FindMappingIndex(const UInputAction* action, const EKey key) const
{
    if (!action)
        return -1;

    for (size_t i = 0; i < mappings.size(); i++)
    {
        if (mappings[i].GetInputAction() == action && mappings[i].GetKey() == key)
            return i;
    }

    return -1;
}

void UInputMapContext::UnmapAll()
{
    mappings.clear();
}

const TArray<FActionKeyMapping>& UInputMapContext::GetMappings() const
{
    return mappings;
}

}
