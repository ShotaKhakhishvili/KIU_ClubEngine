#pragma once
#include "Actor.h"
#include "AnimObject.h"
#include "Object.h"


class MyObject : public AnimObject
{
    public:
    MyObject();

    void Update(double dTime) override;
};