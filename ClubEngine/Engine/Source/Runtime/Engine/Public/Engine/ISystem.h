#pragma once

namespace CE
{

class ISystem
{
public:

    virtual ~ISystem() = default;

    virtual const char* GetName() const noexcept = 0;
    virtual bool Init();
    virtual void Shutdown();
    virtual int Update(float dt) = 0;
};

}