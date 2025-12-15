#include "Path.h"

// roots
std::string Path::ProjectRoot(){ return PROJECT_DIR;}
std::string Path::Resources(){ return ProjectRoot() + "/Resources";}

// typed dirs
std::string Path::Shaders(){ return Resources() + "/Shaders";}
std::string Path::Textures(){ return Resources() + "/Textures";}

// helpers
std::string Path::Asset(const std::string& relative){ return Resources() + "/" + relative;}
std::string Path::Shader(const std::string& relative){ return Shaders() + "/" + relative;}
std::string Path::Texture(const std::string& relative){ return Textures() + "/" + relative;}

