#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtx\matrix_transform_2d.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <iostream>
#include <vector>

#include "Path.h"
#include "Shader.h"
#include "VAO.h"
#include "EBO.h"
#include "Texture.h"
#include "Camera.h"
#include "FuncLib.h"
#include "Object.h"
#include "InstancedObject.h"
#include "World.h"
#include "Font.h"
#include "TextRenderer.h""

namespace Game
{
    void Init();
}