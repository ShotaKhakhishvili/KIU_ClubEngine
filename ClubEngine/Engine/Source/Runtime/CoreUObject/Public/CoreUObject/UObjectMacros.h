#pragma once

#include <CoreUObject/UClass.h>

#define GENERATED_BODY(TClass, TParent) \
public: \
using Super = TParent; \
using ThisClass = TClass; \
static ::CE::UClass* StaticClass(); \
::CE::UClass* GetClass() const override { return StaticClass(); } \
private:

#define IMPLEMENT_CLASS(TClass) \
::CE::UClass* TClass::StaticClass() \
{ \
static ::CE::UClass classObject(#TClass, TClass::Super::StaticClass(), ::CE::GetClassConstructor<TClass>());\
return &classObject;\
}
