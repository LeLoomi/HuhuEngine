#include "huhu_model.hpp"

#include "huhu_utils.hpp"

// libs
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

// std
#include <cassert>
#include <cstring>
#include <unordered_map>

namespace std
{
    template <>
    struct hash<huhu::HuhuModel::Vertex>
    {
        size_t operator()(huhu::HuhuModel::Vertex const &vertex) const
        {
            size_t seed = 0;
            huhu::hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
            return seed;
        }
    };
}

namespace huhu
{
    HuhuModel::HuhuModel(HuhuDevice &device, const HuhuModel::Builder &builder) : huhuDevice{device}
    {
        createVertexBuffers(builder.vertices);
        createIndexBuffers(builder.indices);
    }

    HuhuModel::~HuhuModel()
    {
        vkDestroyBuffer(huhuDevice.device(), vertexBuffer, nullptr);
        vkFreeMemory(huhuDevice.device(), vertexBufferMemory, nullptr);

        if (hasIndexBuffer)
        {
            vkDestroyBuffer(huhuDevice.device(), indexBuffer, nullptr);
            vkFreeMemory(huhuDevice.device(), indexBufferMemory, nullptr);
        }
    }

    std::unique_ptr<HuhuModel> HuhuModel::createModelFromFile(HuhuDevice &device, const std::string &filepath)
    {
        Builder builder{};
        builder.loadModel(filepath);
        return std::make_unique<HuhuModel>(device, builder);
    }

    void HuhuModel::createVertexBuffers(const std::vector<Vertex> &vertices)
    {
        vertexCount = static_cast<uint32_t>(vertices.size());
        assert(vertexCount >= 3 && "vertex count must be at least 3");
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

        // create staging buffer to stage to from host mem, and copy from into gpu mem
        VkBuffer stagingBuffer{};
        VkDeviceMemory stagingBufferMemory{};
        huhuDevice.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory);

        void *data;
        vkMapMemory(huhuDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(huhuDevice.device(), stagingBufferMemory);

        // create destination vertex buffer in gpu mem and transfer the data to it
        huhuDevice.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            vertexBuffer,
            vertexBufferMemory);
        huhuDevice.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

        // kill off the staging buffer and free the host mem
        vkDestroyBuffer(huhuDevice.device(), stagingBuffer, nullptr);
        vkFreeMemory(huhuDevice.device(), stagingBufferMemory, nullptr);
    }

    void HuhuModel::createIndexBuffers(const std::vector<uint32_t> &indices)
    {
        indexCount = static_cast<uint32_t>(indices.size());
        hasIndexBuffer = indexCount > 0;

        if (!hasIndexBuffer)
            return; // stop the work if we don't have an index buffer anyways

        VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;

        // create staging buffer to stage to from host mem, and copy from into gpu mem
        VkBuffer stagingBuffer{};
        VkDeviceMemory stagingBufferMemory{};
        huhuDevice.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory);

        void *data;
        vkMapMemory(huhuDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(huhuDevice.device(), stagingBufferMemory);

        // create destination vertex buffer in gpu mem and transfer the data to it
        huhuDevice.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            indexBuffer,
            indexBufferMemory);
        huhuDevice.copyBuffer(stagingBuffer, indexBuffer, bufferSize);

        // kill off the staging buffer and free the host mem
        vkDestroyBuffer(huhuDevice.device(), stagingBuffer, nullptr);
        vkFreeMemory(huhuDevice.device(), stagingBufferMemory, nullptr);
    }

    void HuhuModel::bind(VkCommandBuffer commandBuffer)
    {
        VkBuffer buffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

        if (hasIndexBuffer)
        {
            vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
        }
    }

    void HuhuModel::draw(VkCommandBuffer commandBuffer)
    {
        if (hasIndexBuffer)
        {
            vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
        }
        else
        {
            vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
        }
    }

    std::vector<VkVertexInputBindingDescription> HuhuModel::Vertex::getBindingDescriptions()
    {
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescriptions;
    }

    std::vector<VkVertexInputAttributeDescription> HuhuModel::Vertex::getAttributeDescriptions()
    {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
        // position
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, position);

        // color
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        return attributeDescriptions;
    }

    void HuhuModel::Builder::loadModel(const std::string &filepath)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str()))
        {
            throw std::runtime_error(warn + err);
        }

        vertices.clear();
        indices.clear();

        std::unordered_map<Vertex, uint32_t> uniqueVertices{};

        for (const auto &shape : shapes) // loop the shapes
        {
            for (const auto &index : shape.mesh.indices) // loop through all faces in the shape
            {
                Vertex vertex{};

                if (index.vertex_index >= 0) // negative value would mean no index was provided
                {
                    vertex.position = {
                        attrib.vertices[3 * index.vertex_index + 0],
                        attrib.vertices[3 * index.vertex_index + 1],
                        attrib.vertices[3 * index.vertex_index + 2]};

                    // handle colors after checking if they're in the obj file
                    auto colorIndex = 3 * index.vertex_index + 2;
                    if (colorIndex < attrib.colors.size())
                    {
                        vertex.color = {
                            attrib.colors[colorIndex - 2],
                            attrib.colors[colorIndex - 1],
                            attrib.colors[colorIndex - 0]};
                    }
                    else
                    {
                        vertex.color = {1.f, 1.f, 1.f}; // fallback default color if none provided
                    }
                }

                if (index.normal_index >= 0)
                {
                    vertex.normal = {
                        attrib.normals[3 * index.normal_index + 0],
                        attrib.normals[3 * index.normal_index + 1],
                        attrib.normals[3 * index.normal_index + 2]};
                }

                if (index.texcoord_index >= 0)
                {
                    vertex.uv = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        attrib.texcoords[2 * index.texcoord_index + 1]};
                }

                if(uniqueVertices.count(vertex) == 0)   // if we don't know this vertex yet
                {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());    // so next time we remember we've already saved it
                    vertices.push_back(vertex);
                }
                indices.push_back(uniqueVertices[vertex]);
            }
        }
    }
}