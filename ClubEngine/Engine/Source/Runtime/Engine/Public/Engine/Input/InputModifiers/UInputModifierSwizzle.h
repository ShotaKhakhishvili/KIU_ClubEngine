#pragma once

#include <Engine/Input/UInputModifier.h>

#include <Core/Types/CoreTypes.h>

namespace CE
{

enum class EInputAxisSwizzle : uint8
{
    YXZ,
    ZYX,
    XZY,
    YZX,
    ZXY
};


class UInputModifierSwizzle : public UInputModifier
{
    GENERATED_BODY(UInputModifierSwizzle, UInputModifier)

public:
    explicit UInputModifierSwizzle(EInputAxisSwizzle order = EInputAxisSwizzle::YXZ);

    FInputActionValue ModifyRaw(FInputActionValue value, float dt) override;

    EInputAxisSwizzle order;
};

}