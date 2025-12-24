#pragma once

#include "Camera.h"

class Actor
{
public:
    bool IsWidget = false;
    bool pendingKill = false;

    virtual ~Actor() = default;
    virtual void Draw(){}

    virtual void OnActorBegin(){}
    virtual void OnActorEnd(){}
    virtual void Update(double dTime){}

    virtual Shader* GetShader(){return nullptr;}
};