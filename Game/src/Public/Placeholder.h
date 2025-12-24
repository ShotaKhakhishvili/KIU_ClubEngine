#pragma once

enum class ObjectType {
    SABook,
    Barrier,
    Empty
};

class Placeholder {
    Object* player{};

    Object* LeftObject;
    Object* MiddleObject;
    Object* RightObject;

public:
    Placeholder(ObjectType a, ObjectType b, ObjectType  c, glm::vec3 Position, Player* player);

    ~Placeholder();
};
