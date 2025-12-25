#pragma once
#include "Barrier.h"

class BarrierA : public Barrier {
    public:
        explicit BarrierA(Player* player);
        unsigned int AddInstance(glm::vec3 position) override;

    float GetColX() const override { return 0.5f; }
    float GetColY() const override { return 0.5f; }
    float GetColZ() const override { return 0.5f; }

    float GetColCoordX() const override { return 0.25f; }
    float GetColCoordY() const override { return 0.0f; }
    float GetColCoordZ() const override { return 0.0f; }
};