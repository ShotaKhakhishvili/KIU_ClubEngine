#pragma once
#include "Object.h"
#include "InstancedObject.h"

enum class ObjectType {
    SABook,
    Barrier,
    SABookSequence,
    Empty
};

struct ObjectWrapper {
    ObjectType ObjectType;
    float ObjectDistance;
    unsigned int ObjectCount;
};

class Placeholder {
    Object* player{};

    std::pair<InstancedObject*, std::vector<unsigned int>> LeftObject;
    std::pair<InstancedObject*, std::vector<unsigned int>> MiddleObject;
    std::pair<InstancedObject*, std::vector<unsigned int>> RightObject;

public:
    Placeholder(ObjectWrapper a, ObjectWrapper b, ObjectWrapper  c, glm::vec3 Position);
    ~Placeholder();

    InstancedObject* GetInstObjFromObjType(ObjectType);
};
