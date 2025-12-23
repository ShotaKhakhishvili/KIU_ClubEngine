#pragma once

#include "Camera.h"

class Actor
{
public:
    virtual ~Actor() = default;
    virtual void Draw(){}

    virtual void OnActorBegin(){}
    virtual void OnActorEnd(){}
    virtual void Update(){}
};