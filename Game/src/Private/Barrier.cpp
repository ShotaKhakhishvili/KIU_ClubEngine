#include "Barrier.h"

Barrier::Barrier(const char* modelPath, const char* texturePath, Player* player, PlayerInteraction interaction)
    : PlayerDetector(modelPath, texturePath, "default.frag", player, interaction)
{
}

unsigned int Barrier::AddInstance(glm::vec3 position) {
    unsigned int newIndex = PlayerDetector::AddInstance(position);

    SetScale(newIndex, {0.08, 0.08,0.08});
    SetRotation(newIndex, {0, -90, 0});
    return newIndex;
}

bool Barrier::DetectPlayer(glm::vec3 instPos)
{
    if (!player || player->state == PlayerState::GameOver)
        return false;

    const glm::vec3 playerPos = player->GetPosition();

    // ================= Barrier collider =================
    const float bSizeX = GetColX();
    const float bSizeY = GetColY();
    const float bSizeZ = GetColZ();

    const float bOffX = GetColCoordX();
    const float bOffY = GetColCoordY();
    const float bOffZ = GetColCoordZ();

    const glm::vec3 bColPos = {
        instPos.x + bOffX,
        instPos.y + bOffY,
        instPos.z + bOffZ
    };

    const float bHalfX = bSizeX * 0.5f;
    const float bHalfZ = bSizeZ * 0.5f;

    const float bMinX = bColPos.x - bHalfX;
    const float bMaxX = bColPos.x + bHalfX;

    const float bMinZ = bColPos.z - bHalfZ;
    const float bMaxZ = bColPos.z + bHalfZ;

    const float bMinY = bColPos.y;
    const float bMaxY = bColPos.y + bSizeY;

    // ================= Player collider =================
    const float pSizeX = player->colX;
    const float pSizeY = player->colY;
    const float pSizeZ = player->colZ;

    const float pOffX = player->colCoordX;
    const float pOffY = player->colCoordY;
    const float pOffZ = player->colCoordZ;

    const glm::vec3 pColPos = {
        playerPos.x + pOffX,
        playerPos.y + pOffY,
        playerPos.z + pOffZ
    };

    const float pHalfX = pSizeX * 0.5f;
    const float pHalfZ = pSizeZ * 0.5f;

    const float pMinX = pColPos.x - pHalfX;
    const float pMaxX = pColPos.x + pHalfX;

    const float pMinZ = pColPos.z - pHalfZ;
    const float pMaxZ = pColPos.z + pHalfZ;

    const float pMinY = pColPos.y;
    const float pMaxY = pColPos.y + pSizeY;

    // ================= AABB overlap =================
    const bool overlapX = (bMinX <= pMaxX) && (bMaxX >= pMinX);
    const bool overlapY = (bMinY <= pMaxY) && (bMaxY >= pMinY);
    const bool overlapZ = (bMinZ <= pMaxZ) && (bMaxZ >= pMinZ);

    return overlapX && overlapY && overlapZ;
}






