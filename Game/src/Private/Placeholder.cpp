#include "Game.h"
#include "SABook.h"
#include "Barrier.h"
#include "Placeholder.h"

#include "GameLogic.h"

Placeholder::Placeholder(ObjectType a, ObjectType b, ObjectType c, glm::vec3 position){
    InstancedObject* aObj = GetInstObjFromObjType(a);
    InstancedObject* bObj = GetInstObjFromObjType(b);
    InstancedObject* cObj = GetInstObjFromObjType(c);

    if (aObj != nullptr)
        LeftObject = {aObj, aObj->AddInstance(position - glm::vec3(0, 0, 2))};
    if (bObj != nullptr)
        MiddleObject = {bObj, bObj->AddInstance(position)};
    if (cObj != nullptr)
        RightObject = {cObj, cObj->AddInstance(position + glm::vec3(0, 0, 2))};
}

Placeholder::~Placeholder() {
    if (LeftObject.first != nullptr)
        LeftObject.first->RemoveInstance(LeftObject.second);
    if (MiddleObject.first != nullptr)
        MiddleObject.first->RemoveInstance(MiddleObject.second);
    if (RightObject.first != nullptr)
        RightObject.first->RemoveInstance(RightObject.second);
}

InstancedObject* Placeholder::GetInstObjFromObjType(ObjectType objType)
{
    if (objType == ObjectType::SABook)
        return GameLogic::book;
    if (objType == ObjectType::Barrier)
        return GameLogic::barrier;
    return nullptr;
}

