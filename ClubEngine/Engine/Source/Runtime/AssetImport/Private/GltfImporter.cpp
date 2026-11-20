// ─── AI-Assisted Code ────────────────────────────────────────────────────────
//
//  This file was generated with the assistance of Claude (Anthropic).
//  Reviewed and approved by: Shota Khakhishvili
//  Date: 2026-11-20
//
//  If you modify this file, remove or update this notice accordingly.
//
// ─────────────────────────────────────────────────────────────────────────────

#include <Core/ClubCore.h>

#include <AssetImport/GltfImporter.h>

#include "ImportConversion.h"

#define CGLTF_IMPLEMENTATION
#include <cgltf/cgltf.h>

#include <algorithm>
#include <cmath>
#include <memory>
#include <numeric>
#include <string_view>

namespace CE
{

namespace
{

using FCgltfDataPtr = std::unique_ptr<cgltf_data, decltype(&cgltf_free)>;

constexpr std::string_view SupportedRequiredExtensions[] = {
    "KHR_mesh_quantization",
    "KHR_materials_emissive_strength",
    "KHR_materials_unlit",
};

const char* ToString(const cgltf_result result)
{
    switch (result)
    {
    case cgltf_result_success:
        return "success";
    case cgltf_result_data_too_short:
        return "data too short";
    case cgltf_result_unknown_format:
        return "unknown format";
    case cgltf_result_invalid_json:
        return "invalid JSON";
    case cgltf_result_invalid_gltf:
        return "invalid glTF";
    case cgltf_result_invalid_options:
        return "invalid options";
    case cgltf_result_file_not_found:
        return "file not found";
    case cgltf_result_io_error:
        return "I/O error";
    case cgltf_result_out_of_memory:
        return "out of memory";
    case cgltf_result_legacy_gltf:
        return "glTF 1.0 is not supported";
    default:
        return "unknown error";
    }
}

std::string MakeName(const char* name, const char* fallbackPrefix, const size_t index)
{
    if (name != nullptr && name[0] != '\0')
        return name;

    return std::string(fallbackPrefix) + "_" + std::to_string(index);
}

RHI::TextureWrap ToWrap(const cgltf_wrap_mode mode)
{
    switch (mode)
    {
    case cgltf_wrap_mode_clamp_to_edge:
        return RHI::TextureWrap::ClampToEdge;
    case cgltf_wrap_mode_mirrored_repeat:
        return RHI::TextureWrap::MirroredRepeat;
    default:
        return RHI::TextureWrap::Repeat;
    }
}

// glTF filters are GL enum values, undefined means the engine decides (we pick linear)
RHI::TextureFilter ToFilter(const cgltf_filter_type filter)
{
    switch (filter)
    {
    case cgltf_filter_type_nearest:
    case cgltf_filter_type_nearest_mipmap_nearest:
    case cgltf_filter_type_nearest_mipmap_linear:
        return RHI::TextureFilter::Nearest;
    default:
        return RHI::TextureFilter::Linear;
    }
}

// Only the plain nearest/linear min filters mean "no mipmaps"
bool UsesMipmaps(const cgltf_filter_type minFilter)
{
    return minFilter != cgltf_filter_type_nearest && minFilter != cgltf_filter_type_linear;
}

const cgltf_attribute* FindAttribute(const cgltf_primitive& primitive, const cgltf_attribute_type type, const cgltf_int set = 0)
{
    for (cgltf_size i = 0; i < primitive.attributes_count; ++i)
    {
        const cgltf_attribute& attribute = primitive.attributes[i];

        if (attribute.type == type && attribute.index == set)
            return &attribute;
    }

    return nullptr;
}

TArray<float> UnpackFloats(const cgltf_accessor& accessor, size_t& outComponentCount)
{
    outComponentCount = cgltf_num_components(accessor.type);

    TArray<float> values(accessor.count * outComponentCount);
    cgltf_accessor_unpack_floats(&accessor, values.data(), values.size());

    return values;
}

void BuildTriangleList(const cgltf_primitive_type type, const TArray<uint32>& source, TArray<uint32>& outTriangles)
{
    outTriangles.clear();

    if (type == cgltf_primitive_type_triangles)
    {
        outTriangles.assign(source.begin(), source.end() - static_cast<std::ptrdiff_t>(source.size() % 3));
        return;
    }

    if (source.size() < 3)
        return;

    outTriangles.reserve((source.size() - 2) * 3);

    for (size_t i = 2; i < source.size(); ++i)
    {
        if (type == cgltf_primitive_type_triangle_strip)
        {
            // Odd triangles in a strip are reversed to keep the winding
            const bool even = (i % 2) == 0;

            outTriangles.push_back(source[i - 2]);
            outTriangles.push_back(even ? source[i - 1] : source[i]);
            outTriangles.push_back(even ? source[i] : source[i - 1]);
        }
        else
        {
            outTriangles.push_back(source[0]);
            outTriangles.push_back(source[i - 1]);
            outTriangles.push_back(source[i]);
        }
    }
}

// glTF matrices are column-major and always decomposable into TRS
FTransform DecomposeMatrix(const cgltf_float* m)
{
    FTransform transform;
    transform.Translation = {m[12], m[13], m[14]};

    FVector column0{m[0], m[1], m[2]};
    FVector column1{m[4], m[5], m[6]};
    FVector column2{m[8], m[9], m[10]};

    FVector scale{Length(column0), Length(column1), Length(column2)};

    if (Dot(Cross(column0, column1), column2) < 0.0)
        scale.x = -scale.x;

    if (scale.x != 0.0)
        column0 = column0 / scale.x;

    if (scale.y != 0.0)
        column1 = column1 / scale.y;

    if (scale.z != 0.0)
        column2 = column2 / scale.z;

    const double r00 = column0.x, r01 = column1.x, r02 = column2.x;
    const double r10 = column0.y, r11 = column1.y, r12 = column2.y;
    const double r20 = column0.z, r21 = column1.z, r22 = column2.z;

    const double trace = r00 + r11 + r22;
    FQuat rotation;

    if (trace > 0.0)
    {
        const double s = std::sqrt(trace + 1.0) * 2.0;
        rotation = {(r21 - r12) / s, (r02 - r20) / s, (r10 - r01) / s, 0.25 * s};
    }
    else if (r00 > r11 && r00 > r22)
    {
        const double s = std::sqrt(1.0 + r00 - r11 - r22) * 2.0;
        rotation = {0.25 * s, (r01 + r10) / s, (r02 + r20) / s, (r21 - r12) / s};
    }
    else if (r11 > r22)
    {
        const double s = std::sqrt(1.0 + r11 - r00 - r22) * 2.0;
        rotation = {(r01 + r10) / s, 0.25 * s, (r12 + r21) / s, (r02 - r20) / s};
    }
    else
    {
        const double s = std::sqrt(1.0 + r22 - r00 - r11) * 2.0;
        rotation = {(r02 + r20) / s, (r12 + r21) / s, 0.25 * s, (r10 - r01) / s};
    }

    transform.Rotation = Normalize(rotation);
    transform.Scale = scale;

    return transform;
}

FTransform ReadLocalTransform(const cgltf_node& node, const FGltfImportSettings& settings)
{
    FTransform transform;

    if (node.has_matrix)
    {
        transform = DecomposeMatrix(node.matrix);
    }
    else
    {
        if (node.has_translation)
            transform.Translation = {node.translation[0], node.translation[1], node.translation[2]};

        if (node.has_rotation)
            transform.Rotation = Normalize(FQuat{node.rotation[0], node.rotation[1], node.rotation[2], node.rotation[3]});

        if (node.has_scale)
            transform.Scale = {node.scale[0], node.scale[1], node.scale[2]};
    }

    if (settings.convertAxes)
    {
        transform.Translation = ImportConversion::GltfToEngine(transform.Translation);
        transform.Rotation = ImportConversion::GltfToEngine(transform.Rotation);
        transform.Scale = ImportConversion::GltfScaleToEngine(transform.Scale);
    }

    // Unit scale only goes into translation, otherwise children get it twice
    transform.Translation *= static_cast<double>(settings.scale);

    return transform;
}

}

GltfImporter::GltfImporter(FGltfImportSettings inSettings)
    : settings(inSettings)
{
}

std::optional<FSceneImportResult> GltfImporter::Import(const std::filesystem::path& path)
{
    ClearMessages();

    const std::string pathString = path.string();

    cgltf_options options{};
    cgltf_data* rawData = nullptr;

    const cgltf_result parseResult = cgltf_parse_file(&options, pathString.c_str(), &rawData);

    if (parseResult != cgltf_result_success)
    {
        AddError("Failed to parse glTF file (" + std::string(ToString(parseResult)) + "): " + pathString);
        return std::nullopt;
    }

    FCgltfDataPtr data(rawData, &cgltf_free);

    const cgltf_result loadResult = cgltf_load_buffers(&options, data.get(), pathString.c_str());

    if (loadResult != cgltf_result_success)
    {
        AddError("Failed to load glTF buffers (" + std::string(ToString(loadResult)) + "): " + pathString);
        return std::nullopt;
    }

    const cgltf_result validateResult = cgltf_validate(data.get());

    if (validateResult != cgltf_result_success)
    {
        AddError("glTF validation failed (" + std::string(ToString(validateResult)) + "): " + pathString);
        return std::nullopt;
    }

    if (!CheckRequiredExtensions(*data))
        return std::nullopt;

    FSceneImportResult result;

    ReadTextures(*data, path.parent_path(), result);
    ReadMaterials(*data, result);
    ReadMeshes(*data, result);
    ReadNodes(*data, result);

    if (!result.IsValid())
    {
        AddError("glTF file did not contain valid mesh data: " + pathString);
        return std::nullopt;
    }

    return result;
}

bool GltfImporter::CheckRequiredExtensions(const cgltf_data& data)
{
    bool supported = true;

    for (cgltf_size i = 0; i < data.extensions_required_count; ++i)
    {
        const std::string_view extension = data.extensions_required[i];

        const auto it = std::find(std::begin(SupportedRequiredExtensions), std::end(SupportedRequiredExtensions), extension);

        if (it == std::end(SupportedRequiredExtensions))
        {
            AddError("glTF file requires unsupported extension: " + std::string(extension));
            supported = false;
        }
    }

    return supported;
}

void GltfImporter::ReadTextures(const cgltf_data& data, const std::filesystem::path& baseDir, FSceneImportResult& result)
{
    result.textures.reserve(data.textures_count);

    for (cgltf_size i = 0; i < data.textures_count; ++i)
    {
        const cgltf_texture& texture = data.textures[i];

        const char* sourceName = texture.name;

        if (sourceName == nullptr && texture.image != nullptr)
            sourceName = texture.image->name;

        FTextureImportDesc textureDesc;
        textureDesc.name = MakeName(sourceName, "Texture", i);

        // Defaults for textures without a sampler. Size and format are filled when the image loads.
        textureDesc.desc.wrapU = RHI::TextureWrap::Repeat;
        textureDesc.desc.wrapV = RHI::TextureWrap::Repeat;
        textureDesc.desc.minFilter = RHI::TextureFilter::Linear;
        textureDesc.desc.magFilter = RHI::TextureFilter::Linear;
        textureDesc.desc.generateMipmaps = true;

        if (texture.sampler != nullptr)
        {
            textureDesc.desc.wrapU = ToWrap(texture.sampler->wrap_s);
            textureDesc.desc.wrapV = ToWrap(texture.sampler->wrap_t);
            textureDesc.desc.minFilter = ToFilter(texture.sampler->min_filter);
            textureDesc.desc.magFilter = ToFilter(texture.sampler->mag_filter);
            textureDesc.desc.generateMipmaps = UsesMipmaps(texture.sampler->min_filter);
        }

        if (texture.image == nullptr)
            AddWarning("Texture '" + textureDesc.name + "' has no PNG/JPEG image (WebP/KTX2 are not supported)");
        else if (!ReadImageSource(data, *texture.image, baseDir, textureDesc))
            AddWarning("Failed to read image data for texture '" + textureDesc.name + "'");

        // Pushed even when empty so material texture indices stay valid
        result.textures.push_back(std::move(textureDesc));
    }
}

bool GltfImporter::ReadImageSource(const cgltf_data& data, const cgltf_image& image, const std::filesystem::path& baseDir, FTextureImportDesc& texture)
{
    texture.mimeType = image.mime_type ? image.mime_type : "";

    if (image.buffer_view != nullptr)
    {
        const uint8* bytes = cgltf_buffer_view_data(image.buffer_view);

        if (bytes == nullptr)
            return false;

        texture.encodedBytes.assign(bytes, bytes + image.buffer_view->size);
        return true;
    }

    if (image.uri == nullptr)
        return false;

    const std::string_view uri = image.uri;

    // "data:image/png;base64,..."
    if (uri.starts_with("data:"))
    {
        const size_t comma = uri.find(',');

        if (comma == std::string_view::npos || uri.substr(0, comma).find(";base64") == std::string_view::npos)
            return false;

        if (texture.mimeType.empty())
            texture.mimeType = std::string(uri.substr(5, uri.find(';') - 5));

        const std::string_view payload = uri.substr(comma + 1);

        size_t padding = 0;

        while (padding < payload.size() && payload[payload.size() - 1 - padding] == '=')
            ++padding;

        const size_t decodedSize = payload.size() * 3 / 4 - padding;

        cgltf_options options{};
        void* decoded = nullptr;

        if (cgltf_load_buffer_base64(&options, decodedSize, payload.data(), &decoded) != cgltf_result_success)
            return false;

        const uint8* bytes = static_cast<const uint8*>(decoded);
        texture.encodedBytes.assign(bytes, bytes + decodedSize);

        data.memory.free_func(data.memory.user_data, decoded);
        return true;
    }

    // External file, URI is percent-encoded UTF-8
    std::string decodedUri(uri);
    decodedUri.resize(cgltf_decode_uri(decodedUri.data()));

    texture.filePath = baseDir / std::filesystem::path(std::u8string(decodedUri.begin(), decodedUri.end()));
    return true;
}

void GltfImporter::ReadMaterials(const cgltf_data& data, FSceneImportResult& result)
{
    result.materials.reserve(data.materials_count);

    // Color textures are sRGB, data textures (normal, roughness, occlusion) are linear
    TArray<bool> usedAsColor(data.textures_count, false);
    TArray<bool> usedAsData(data.textures_count, false);

    for (cgltf_size i = 0; i < data.materials_count; ++i)
    {
        const cgltf_material& material = data.materials[i];

        FMaterialImportDesc materialDesc;
        materialDesc.name = MakeName(material.name, "Material", i);

        RHI::FMaterialSettings& settings = materialDesc.settings;

        auto ReadTextureRef = [&](const cgltf_texture_view& view, const char* slotName, const bool isColor) -> FTextureRef
        {
            FTextureRef ref;

            if (view.texture == nullptr)
                return ref;

            ref.textureIndex = static_cast<int32>(cgltf_texture_index(&data, view.texture));
            ref.texCoord = static_cast<uint32>(std::max<cgltf_int>(view.texcoord, 0));

            if (isColor)
                usedAsColor[static_cast<size_t>(ref.textureIndex)] = true;
            else
                usedAsData[static_cast<size_t>(ref.textureIndex)] = true;

            if (ref.texCoord != 0)
                AddWarning("Material '" + materialDesc.name + "': " + slotName + " uses TEXCOORD_" + std::to_string(ref.texCoord) + ", only TEXCOORD_0 is imported");

            if (view.has_transform)
                AddWarning("Material '" + materialDesc.name + "': " + slotName + " uses KHR_texture_transform, which is ignored");

            return ref;
        };

        if (material.has_pbr_metallic_roughness)
        {
            const cgltf_pbr_metallic_roughness& pbr = material.pbr_metallic_roughness;

            settings.baseColor = {pbr.base_color_factor[0], pbr.base_color_factor[1], pbr.base_color_factor[2], pbr.base_color_factor[3]};
            settings.metallic = pbr.metallic_factor;
            settings.roughness = pbr.roughness_factor;

            materialDesc.baseColorTexture = ReadTextureRef(pbr.base_color_texture, "baseColorTexture", true);
            materialDesc.metallicRoughnessTexture = ReadTextureRef(pbr.metallic_roughness_texture, "metallicRoughnessTexture", false);
        }
        else if (material.has_pbr_specular_glossiness)
        {
            AddWarning("Material '" + materialDesc.name + "' uses specular-glossiness, imported with default PBR values");
        }

        materialDesc.normalTexture = ReadTextureRef(material.normal_texture, "normalTexture", false);
        materialDesc.occlusionTexture = ReadTextureRef(material.occlusion_texture, "occlusionTexture", false);
        materialDesc.emissiveTexture = ReadTextureRef(material.emissive_texture, "emissiveTexture", true);

        // cgltf keeps both normal scale and occlusion strength in texture_view::scale
        if (material.normal_texture.texture != nullptr)
            settings.normalScale = material.normal_texture.scale;

        if (material.occlusion_texture.texture != nullptr)
            settings.occlusionStrength = material.occlusion_texture.scale;

        float emissiveStrength = 1.0f;

        if (material.has_emissive_strength)
            emissiveStrength = material.emissive_strength.emissive_strength;

        settings.emissive = FVectorF{material.emissive_factor[0], material.emissive_factor[1], material.emissive_factor[2]} * emissiveStrength;

        settings.alphaCutoff = material.alpha_cutoff;
        settings.unlit = material.unlit;

        switch (material.alpha_mode)
        {
        case cgltf_alpha_mode_mask:
            settings.renderState.blendMode = RHI::BlendMode::Masked;
            break;

        case cgltf_alpha_mode_blend:
            settings.renderState.blendMode = RHI::BlendMode::Translucent;
            settings.renderState.depthWrite = false;
            break;

        default:
            settings.renderState.blendMode = RHI::BlendMode::Opaque;
            break;
        }

        settings.renderState.cullMode = material.double_sided ? RHI::CullMode::None : RHI::CullMode::Back;

        result.materials.push_back(std::move(materialDesc));
    }

    for (size_t i = 0; i < result.textures.size(); ++i)
    {
        result.textures[i].desc.sRGB = usedAsColor[i];

        if (usedAsColor[i] && usedAsData[i])
            AddWarning("Texture '" + result.textures[i].name + "' is used as both color and data, imported as sRGB");
    }
}

void GltfImporter::ReadMeshes(const cgltf_data& data, FSceneImportResult& result)
{
    result.meshes.reserve(data.meshes_count);

    for (cgltf_size i = 0; i < data.meshes_count; ++i)
    {
        const cgltf_mesh& mesh = data.meshes[i];

        FMeshImportResult meshResult;
        meshResult.name = MakeName(mesh.name, "Mesh", i);

        for (cgltf_size p = 0; p < mesh.primitives_count; ++p)
            ReadPrimitive(data, mesh.primitives[p], static_cast<uint32>(p), meshResult);

        if (!meshResult.IsValid())
            AddWarning("Mesh '" + meshResult.name + "' has no triangle data");

        // Pushed even when empty so node mesh indices stay valid
        result.meshes.push_back(std::move(meshResult));
    }
}

void GltfImporter::ReadPrimitive(const cgltf_data& data, const cgltf_primitive& primitive, const uint32 primitiveIndex, FMeshImportResult& mesh)
{
    const std::string sectionName = mesh.name + "_" + std::to_string(primitiveIndex);

    const bool isTriangles = primitive.type == cgltf_primitive_type_triangles ||
        primitive.type == cgltf_primitive_type_triangle_strip ||
        primitive.type == cgltf_primitive_type_triangle_fan;

    if (!isTriangles)
    {
        AddWarning("Primitive '" + sectionName + "' is points/lines, skipped");
        return;
    }

    const cgltf_attribute* positionAttribute = FindAttribute(primitive, cgltf_attribute_type_position);

    if (positionAttribute == nullptr || positionAttribute->data->count == 0)
    {
        AddWarning("Primitive '" + sectionName + "' has no positions, skipped");
        return;
    }

    const size_t vertexCount = positionAttribute->data->count;

    auto ReadAttribute = [&](const cgltf_attribute_type type, const char* attributeName, size_t& outComponents) -> TArray<float>
    {
        outComponents = 0;

        const cgltf_attribute* attribute = FindAttribute(primitive, type);

        if (attribute == nullptr)
            return {};

        if (attribute->data->count != vertexCount)
        {
            AddWarning("Primitive '" + sectionName + "': " + attributeName + " count does not match positions, ignored");
            return {};
        }

        return UnpackFloats(*attribute->data, outComponents);
    };

    size_t positionComponents = 0;
    size_t normalComponents = 0;
    size_t tangentComponents = 0;
    size_t uvComponents = 0;
    size_t colorComponents = 0;

    const TArray<float> positions = UnpackFloats(*positionAttribute->data, positionComponents);
    const TArray<float> normals = ReadAttribute(cgltf_attribute_type_normal, "NORMAL", normalComponents);
    TArray<float> tangents = ReadAttribute(cgltf_attribute_type_tangent, "TANGENT", tangentComponents);
    const TArray<float> uvs = ReadAttribute(cgltf_attribute_type_texcoord, "TEXCOORD_0", uvComponents);
    const TArray<float> colors = ReadAttribute(cgltf_attribute_type_color, "COLOR_0", colorComponents);

    if (FindAttribute(primitive, cgltf_attribute_type_joints) != nullptr)
        AddWarning("Primitive '" + sectionName + "' is skinned, skin data ignored");

    if (primitive.targets_count > 0)
        AddWarning("Primitive '" + sectionName + "' has morph targets, ignored");

    // Spec: without normals use flat normals and ignore tangents
    const bool flatNormals = normals.empty();

    if (flatNormals)
        tangents.clear();

    const bool hasNormalMap = primitive.material != nullptr && primitive.material->normal_texture.texture != nullptr;

    if (tangents.empty() && hasNormalMap)
        AddWarning("Primitive '" + sectionName + "' has a normal map but no tangents, normal mapping will be wrong until tangent generation is added");

    TArray<uint32> sourceIndices;

    if (primitive.indices != nullptr)
    {
        sourceIndices.resize(primitive.indices->count);
        cgltf_accessor_unpack_indices(primitive.indices, sourceIndices.data(), sizeof(uint32), sourceIndices.size());
    }
    else
    {
        sourceIndices.resize(vertexCount);
        std::iota(sourceIndices.begin(), sourceIndices.end(), 0u);
    }

    TArray<uint32> triangles;
    BuildTriangleList(primitive.type, sourceIndices, triangles);

    bool indicesInRange = true;

    for (const uint32 index : triangles)
    {
        if (index >= vertexCount)
        {
            indicesInRange = false;
            break;
        }
    }

    if (triangles.empty() || !indicesInRange)
    {
        AddWarning("Primitive '" + sectionName + "' has no valid triangles, skipped");
        return;
    }

    auto ConvertPoint = [&](FVectorF point)
    {
        if (settings.convertAxes)
            point = ImportConversion::GltfToEngine(point);

        return point * settings.scale;
    };

    auto ConvertDirection = [&](const FVectorF direction)
    {
        if (settings.convertAxes)
            return ImportConversion::GltfToEngine(direction);

        return direction;
    };

    // Bitangent = cross(N, T) * w. The axis mirror and the V flip each negate it.
    float handedness = 1.0f;

    if (settings.convertAxes)
        handedness = -handedness;

    if (settings.flipV)
        handedness = -handedness;

    auto ReadPosition = [&](const size_t i) -> FVectorF
    {
        return {positions[i * 3 + 0], positions[i * 3 + 1], positions[i * 3 + 2]};
    };

    auto MakeVertex = [&](const size_t i) -> FVertex
    {
        FVertex vertex;

        vertex.coord = ConvertPoint(ReadPosition(i));

        if (!normals.empty())
            vertex.normal = Normalize(ConvertDirection({normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2]}));

        if (!tangents.empty())
        {
            const FVectorF tangent = ConvertDirection({tangents[i * 4 + 0], tangents[i * 4 + 1], tangents[i * 4 + 2]});
            vertex.tangent = {tangent, tangents[i * 4 + 3] * handedness};
        }

        if (!uvs.empty())
        {
            const float v = uvs[i * 2 + 1];
            vertex.texUV = {uvs[i * 2 + 0], settings.flipV ? 1.0f - v : v};
        }

        if (!colors.empty())
        {
            const float* color = &colors[i * colorComponents];
            vertex.color = {color[0], color[1], color[2], colorComponents == 4 ? color[3] : 1.0f};
        }

        return vertex;
    };

    // Winding is kept as-is, the axis mapping mirrors the same way the OBJ path does
    FMeshImportSection section;
    section.name = sectionName;
    section.indexStart = static_cast<uint32>(mesh.indices.size());

    if (primitive.material != nullptr)
        section.materialIndex = static_cast<int32>(cgltf_material_index(&data, primitive.material));

    const uint32 baseVertex = static_cast<uint32>(mesh.vertices.size());

    if (!flatNormals)
    {
        mesh.vertices.reserve(mesh.vertices.size() + vertexCount);
        mesh.indices.reserve(mesh.indices.size() + triangles.size());

        for (size_t i = 0; i < vertexCount; ++i)
            mesh.vertices.push_back(MakeVertex(i));

        for (const uint32 index : triangles)
            mesh.indices.push_back(baseVertex + index);
    }
    else
    {
        // Flat shading needs a vertex per triangle corner
        mesh.vertices.reserve(mesh.vertices.size() + triangles.size());
        mesh.indices.reserve(mesh.indices.size() + triangles.size());

        for (size_t t = 0; t + 2 < triangles.size(); t += 3)
        {
            const FVectorF a = ReadPosition(triangles[t + 0]);
            const FVectorF b = ReadPosition(triangles[t + 1]);
            const FVectorF c = ReadPosition(triangles[t + 2]);

            // Computed in glTF space, then converted like any other direction
            const FVectorF faceNormal = Normalize(ConvertDirection(Cross(b - a, c - a)));

            for (size_t corner = 0; corner < 3; ++corner)
            {
                FVertex vertex = MakeVertex(triangles[t + corner]);
                vertex.normal = faceNormal;

                mesh.indices.push_back(static_cast<uint32>(mesh.vertices.size()));
                mesh.vertices.push_back(vertex);
            }
        }
    }

    section.indexCount = static_cast<uint32>(mesh.indices.size()) - section.indexStart;
    mesh.sections.push_back(section);
}

void GltfImporter::ReadNodes(const cgltf_data& data, FSceneImportResult& result)
{
    result.nodes.resize(data.nodes_count);

    bool hasSkins = false;
    bool hasCamerasOrLights = false;

    for (cgltf_size i = 0; i < data.nodes_count; ++i)
    {
        const cgltf_node& node = data.nodes[i];
        FNodeImportDesc& nodeDesc = result.nodes[i];

        nodeDesc.name = MakeName(node.name, "Node", i);
        nodeDesc.localTransform = ReadLocalTransform(node, settings);

        if (node.parent != nullptr)
            nodeDesc.parent = static_cast<int32>(cgltf_node_index(&data, node.parent));

        if (node.mesh != nullptr)
            nodeDesc.meshIndex = static_cast<int32>(cgltf_mesh_index(&data, node.mesh));

        nodeDesc.children.reserve(node.children_count);

        for (cgltf_size c = 0; c < node.children_count; ++c)
            nodeDesc.children.push_back(static_cast<int32>(cgltf_node_index(&data, node.children[c])));

        hasSkins = hasSkins || node.skin != nullptr;
        hasCamerasOrLights = hasCamerasOrLights || node.camera != nullptr || node.light != nullptr;
    }

    if (hasSkins)
        AddWarning("Skins are not supported yet, skinned meshes are imported in bind pose");

    if (hasCamerasOrLights)
        AddWarning("Cameras and lights are not imported yet");

    if (data.scenes_count > 1)
        AddWarning("File has " + std::to_string(data.scenes_count) + " scenes, only the default one is imported");

    const cgltf_scene* scene = data.scene;

    if (scene == nullptr && data.scenes_count > 0)
        scene = &data.scenes[0];

    if (scene != nullptr)
    {
        result.rootNodes.reserve(scene->nodes_count);

        for (cgltf_size i = 0; i < scene->nodes_count; ++i)
            result.rootNodes.push_back(static_cast<int32>(cgltf_node_index(&data, scene->nodes[i])));

        return;
    }

    for (size_t i = 0; i < result.nodes.size(); ++i)
    {
        if (result.nodes[i].parent < 0)
            result.rootNodes.push_back(static_cast<int32>(i));
    }
}

const TArray<std::string>& GltfImporter::GetErrors() const
{
    return errors;
}

const TArray<std::string>& GltfImporter::GetWarnings() const
{
    return warnings;
}

void GltfImporter::ClearMessages()
{
    errors.clear();
    warnings.clear();
}

void GltfImporter::AddError(std::string message)
{
    errors.push_back(std::move(message));
}

void GltfImporter::AddWarning(std::string message)
{
    warnings.push_back(std::move(message));
}

}