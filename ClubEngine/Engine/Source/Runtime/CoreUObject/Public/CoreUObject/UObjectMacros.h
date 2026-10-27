#pragma once

#define GENERATED_BODY(TClass, TParent) \
public: \
    using Super = TParent; \
    static const char* StaticClassName() noexcept { return #TClass; } \
    const char* GetClassName() const noexcept override { return #TClass; } \
private:
