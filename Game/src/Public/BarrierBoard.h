#pragma once
#include "Barrier.h"

class BarrierBoard : public Barrier {
public:
    explicit BarrierBoard(Player* player);
    unsigned int AddInstance(glm::vec3 position) override;

    float GetColX() const override { return 0.5f; }
    float GetColY() const override { return 0.5f; }
    float GetColZ() const override { return 0.5f; }

    float GetColCoordX() const override { return 0.0f; }
    float GetColCoordY() const override { return 0.5f; }
    float GetColCoordZ() const override { return 0.0f; }
};