#pragma once

#include <CoreUObject/UObject.h>
#include <Engine/Input/FActionKeyMapping.h>

namespace CE
{

class UInputMapContext : public UObject
{
    GENERATED_BODY(UInputMapContext, UObject)

public:

    FActionKeyMapping* AddMapping(UInputAction* action, EKey key);
    void RemoveMapping(const UInputAction* action, EKey key);

    void RemoveAllKeysFromMapping(UInputAction* action);
    void UnmapAll();

    const TArray<FActionKeyMapping>& GetMappings() const;
private:
    int32 FindMappingIndex(const UInputAction* action, EKey key) const;

    TArray<FActionKeyMapping> mappings;
};

}
