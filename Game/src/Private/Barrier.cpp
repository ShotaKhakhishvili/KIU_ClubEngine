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

    // ---------------- Barrier collision data ----------------
    const float bSizeX = GetColX();
    const float bSizeY = GetColY();
    const float bSizeZ = GetColZ();

    const float bOffX = GetColCoordX();
    const float bOffY = GetColCoordY();
    const float bOffZ = GetColCoordZ();

    // Barrier collider center on X/Z, bottom-based on Y
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

    // ---------------- Player collision data ----------------
    const float pSizeX = Player::colX;
    const float pSizeY = Player::colY;
    const float pSizeZ = Player::colZ;

    const float pHalfX = pSizeX * 0.5f;
    const float pHalfZ = pSizeZ * 0.5f;

    const float pMinX = playerPos.x - pHalfX;
    const float pMaxX = playerPos.x + pHalfX;

    const float pMinZ = playerPos.z - pHalfZ;
    const float pMaxZ = playerPos.z + pHalfZ;

    const float pMinY = playerPos.y;
    const float pMaxY = playerPos.y + pSizeY;

    // ---------------- AABB overlap test ----------------
    const bool overlapX = (bMinX <= pMaxX) && (bMaxX >= pMinX);
    const bool overlapY = (bMinY <= pMaxY) && (bMaxY >= pMinY);
    const bool overlapZ = (bMinZ <= pMaxZ) && (bMaxZ >= pMinZ);

    return overlapX && overlapY && overlapZ;
}





