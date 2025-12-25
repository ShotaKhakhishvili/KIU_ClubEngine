#include "Game.h"
#include "SABook.h"
#include "Barrier.h"
#include "Placeholder.h"

#include "GameLogic.h"

Placeholder::Placeholder(ObjectWrapper a, ObjectWrapper b, ObjectWrapper c, glm::vec3 position){
    InstancedObject* aObj = GetInstObjFromObjType(a.ObjectType);
    InstancedObject* bObj = GetInstObjFromObjType(b.ObjectType);
    InstancedObject* cObj = GetInstObjFromObjType(c.ObjectType);

    if (aObj != nullptr) {
        LeftObject.first = aObj;
        for (int i = 0; i < a.ObjectCount; i++) {
            LeftObject.second.push_back(aObj->AddInstance(position + glm::vec3(a.ObjectDistance*i, 0, -2)));
        }
        MiddleObject.first = bObj;
    }
    if (bObj != nullptr) {
        for (int i = 0; i < b.ObjectCount; i++) {
            MiddleObject.second.push_back(bObj->AddInstance(position + glm::vec3(b.ObjectDistance*i, 0, 0)));
        }
    }
    if (cObj != nullptr) {
        RightObject.first = cObj;
        for (int i = 0; i < c.ObjectCount; i++) {
            RightObject.second.push_back(cObj->AddInstance(position + glm::vec3(c.ObjectDistance*i, 0, 2)));
        }
    }
}

Placeholder::~Placeholder() {
    if (LeftObject.first != nullptr)
        for (int i = 0; i < LeftObject.second.size(); i++)
            LeftObject.first->RemoveInstance(LeftObject.second[i]);
    if (MiddleObject.first != nullptr)
        for (int i = 0; i < MiddleObject.second.size(); i++)
            MiddleObject.first->RemoveInstance(MiddleObject.second[i]);
    if (RightObject.first != nullptr)
        for (int i = 0; i < RightObject.second.size(); i++)
            RightObject.first->RemoveInstance(RightObject.second[i]);
}

InstancedObject* Placeholder::GetInstObjFromObjType(ObjectType objType)
{
    if (objType == ObjectType::SABook || objType == ObjectType::SABookSequence)
        return GameLogic::book;
    if (objType == ObjectType::Barrier)
        return GameLogic::barrier;
    return nullptr;
}

