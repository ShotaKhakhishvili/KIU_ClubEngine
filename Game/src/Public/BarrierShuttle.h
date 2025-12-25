#pragma once
#include "Barrier.h"

class BarrierShuttle : public Barrier {
public:
    explicit BarrierShuttle(Player* player);

    float GetColX() const override { return 5.0f; }
    float GetColY() const override { return 1.5f; }
    float GetColZ() const override { return 1.8f; }

    float GetColCoordX() const override { return 0.0f; }
    float GetColCoordY() const override { return 0.0f; }
    float GetColCoordZ() const override { return 0.0f; }
};