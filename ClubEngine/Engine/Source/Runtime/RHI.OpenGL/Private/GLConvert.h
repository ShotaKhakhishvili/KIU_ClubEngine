#pragma once

#include <Core/Types/RHITypes.h>

#include <glad/glad.h>

namespace CE::RHI
{

GLenum ToGL(ShaderDataType v) noexcept;
GLenum ToGL(BufferUsage v) noexcept;
GLenum ToGL(TextureType v) noexcept;
GLenum ToGL(TextureSlot v) noexcept;
GLenum ToGL(TextureInternalFormat v) noexcept;
GLenum ToGL(TextureFormat v) noexcept;
GLenum ToGL(TexturePixelType v) noexcept;
GLuint ToGL(ShaderStage v) noexcept;
GLenum ToGL(BufferType type) noexcept;
GLenum ToGL(PrimitiveType primitive )noexcept;
GLenum ToGL(IndexType type) noexcept;

}