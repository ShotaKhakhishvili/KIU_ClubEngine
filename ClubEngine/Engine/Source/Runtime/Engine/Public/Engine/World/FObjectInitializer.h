#pragma once

namespace CE
{

class ULevel;

class FObjectInitializer
{
public:
    explicit FObjectInitializer(ULevel* level);
    ~FObjectInitializer();

    FObjectInitializer(const FObjectInitializer& other) = delete;
    FObjectInitializer& operator= (const FObjectInitializer& other) = delete;

    static ULevel* GetCurrent();

private:
    ULevel* previous;
};

}