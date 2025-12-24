#pragma once
#include "Object.h"

class SABook : public Object {
    Object* player;

    public:
        SABook();
        void Update(double dTime) override;
};
