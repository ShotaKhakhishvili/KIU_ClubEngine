#pragma once

#include <Core/Types/RenderTypes.h>
#include <Core/Types/RHITypes.h>

#include <glad/glad.h>

GLenum ToGL(ShaderDataType v) noexcept;
GLenum ToGL(BufferUsage v) noexcept;
GLenum ToGL(TextureType v) noexcept;
GLenum ToGL(TextureSlot v) noexcept;
GLenum ToGL(TextureInternalFormat v) noexcept;
GLenum ToGL(TextureFormat v) noexcept;
GLenum ToGL(TexturePixelType v) noexcept;
GLenum ToGL(RHI::BufferType type) noexcept;
GLenum ToGL(RHI::PrimitiveType primitive )noexcept;
GLenum ToGL(RHI::IndexType type) noexcept;