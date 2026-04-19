#include <Render/RenderAssets.h>
#include <Render/Texture.h>
#include <Render/Shader.h>
#include <Render/Material.h>

static AssetManager<Texture> gTextures;
static AssetManager<Shader> gShaders;
static AssetManager<Material> gMaterials;

AssetManager<Texture>& GetTextures()   { return gTextures; }
AssetManager<Shader>& GetShaders()     { return gShaders; }
AssetManager<Material>& GetMaterials() { return gMaterials; }