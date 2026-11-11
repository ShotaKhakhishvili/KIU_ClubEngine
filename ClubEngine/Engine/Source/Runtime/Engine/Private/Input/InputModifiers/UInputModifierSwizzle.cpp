#include <Engine/Input/InputModifiers/UInputModifierSwizzle.h>

namespace CE
{

UInputModifierSwizzle::UInputModifierSwizzle(const EInputAxisSwizzle order)
    : order(order)
{
}

FInputActionValue UInputModifierSwizzle::ModifyRaw(const FInputActionValue value, const float dt)
{
    (void)dt;

    const FVectorF& v = value.value;
    FVectorF result = v;

    switch (order)
    {
    case EInputAxisSwizzle::YXZ: result = FVectorF(v.y, v.x, v.z); break;
    case EInputAxisSwizzle::ZYX: result = FVectorF(v.z, v.y, v.x); break;
    case EInputAxisSwizzle::XZY: result = FVectorF(v.x, v.z, v.y); break;
    case EInputAxisSwizzle::YZX: result = FVectorF(v.y, v.z, v.x); break;
    case EInputAxisSwizzle::ZXY: result = FVectorF(v.z, v.x, v.y); break;
    }

    return FInputActionValue{result, value.type};
}

}