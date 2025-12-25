#pragma once
#include "Object.h"
#include "InstancedObject.h"

enum class ObjectType {
    SABook,
    Barrier,
    Empty
};

class Placeholder {
    Object* player{};

    std::pair<InstancedObject*, unsigned int> LeftObject;
    std::pair<InstancedObject*, unsigned int> MiddleObject;
    std::pair<InstancedObject*, unsigned int> RightObject;

public:
    Placeholder(ObjectType a, ObjectType b, ObjectType  c, glm::vec3 Position);
    ~Placeholder();

    InstancedObject* GetInstObjFromObjType(ObjectType);
};
