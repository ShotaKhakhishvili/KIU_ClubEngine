#pragma once

#include <Asset/AssetManager.h>

class Texture;
class Shader;
class Material;

AssetManager<Texture>& GetTextures();
AssetManager<Shader>& GetShaders();
AssetManager<Material>& GetMaterials();