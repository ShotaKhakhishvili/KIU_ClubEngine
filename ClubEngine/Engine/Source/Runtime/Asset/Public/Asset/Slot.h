#pragma once

template<typename T>
struct Slot
{
    std::unique_ptr<T> asset;
    uint32_t generation = 1;
    bool alive = false;
};  