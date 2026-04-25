#pragma once

#include <RenderCore/RenderTypes.h>
#include <RenderCore/RHI/RHITypes.h>

#include <glad/glad.h>

GLenum ToGL(ShaderDataType v) noexcept;
GLenum ToGL(BufferUsage v) noexcept;
GLenum ToGL(TextureType v) noexcept;
GLenum ToGL(TextureSlot v) noexcept;
GLenum ToGL(TextureInternalFormat v) noexcept;
GLenum ToGL(TextureFormat v) noexcept;
GLenum ToGL(TexturePixelType v) noexcept;
GLenum ToGL(RHIBufferType type) noexcept;
GLenum ToGL(RHIPrimitiveType primitive )noexcept;
GLenum ToGL(RHIIndexType type) noexcept;