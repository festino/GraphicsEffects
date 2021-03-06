#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <sstream>
#include "TexturedModel.h"
#include "Plane.h"

using namespace std;

string to_string(glm::vec3 v)
{
    std::ostringstream ss;
    ss << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return ss.str();
}

string to_string(glm::vec4 v)
{
    std::ostringstream ss;
    ss << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    return ss.str();
}

string to_string(glm::mat4x4 m)
{
    std::ostringstream ss;
    ss << "|" << m[0][0] << ", " << m[1][0] << ", " << m[2][0] << ", " << m[3][0] << "|\n";
    ss << "|" << m[0][1] << ", " << m[1][1] << ", " << m[2][1] << ", " << m[3][1] << "|\n";
    ss << "|" << m[0][2] << ", " << m[1][2] << ", " << m[2][2] << ", " << m[3][2] << "|\n";
    ss << "|" << m[0][3] << ", " << m[1][3] << ", " << m[2][3] << ", " << m[3][3] << "|";
    return ss.str();
}

TexturedModel* makeCube(float length, glm::vec3 pos, glm::mat4x4 rotation, Texture* texture)
{
    glm::vec3 cubeVertices[] = {



        // -x
        {-1.0f, -1.0f, -1.0f},
        {-1.0f, -1.0f,  1.0f},
        {-1.0f,  1.0f,  1.0f},
        {-1.0f,  1.0f,  1.0f},
        {-1.0f,  1.0f, -1.0f},
        {-1.0f, -1.0f, -1.0f},
        // +x
        {1.0f, -1.0f,  1.0f},
        {1.0f, -1.0f, -1.0f},
        {1.0f,  1.0f, -1.0f},
        {1.0f,  1.0f, -1.0f},
        {1.0f,  1.0f,  1.0f},
        {1.0f, -1.0f,  1.0f},
         // -y
        {1.0f, -1.0f, -1.0f},
        {1.0f, -1.0f,  1.0f},
        {-1.0f, -1.0f,  1.0f},
        {-1.0f, -1.0f,  1.0f},
        {-1.0f, -1.0f, -1.0f},
        {1.0f, -1.0f, -1.0f},
         // +y
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f,  -1.0f},
        {-1.0f, 1.0f,  -1.0f},
        {-1.0f, 1.0f,  -1.0f},
        {-1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},
         // -z
        {1.0f, -1.0f, -1.0f},
        {-1.0f, -1.0f, -1.0f},
        {-1.0f,  1.0f, -1.0f},
        {-1.0f,  1.0f, -1.0f},
        {1.0f,  1.0f, -1.0f},
        {1.0f, -1.0f, -1.0f},
         // +z
        {-1.0f, -1.0f,  1.0f},
        {1.0f, -1.0f,  1.0f},
        {1.0f,  1.0f,  1.0f},
        {1.0f,  1.0f,  1.0f},
        {-1.0f,  1.0f,  1.0f},
        {-1.0f, -1.0f,  1.0f},
    };

    float half_length = length / 2;
    constexpr int count = sizeof(cubeVertices) / sizeof(glm::vec3);

    /*glm::vec2 texCoords[] = {
        { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 },
        { 1.0, 1.0 }, { 0.0, 1.0 }, { 0.0, 0.0 }
    };
    glm::vec3 normals[] = {
        { -1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 0.0, -1.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 0.0, -1.0 }, { 0.0, 0.0, 1.0 }
    };
    glm::vec3 tangents[] = {
        { 0.0, 0.0, -1.0 }, { 0.0, 0.0, 1.0 }, { -1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 0.0, -1.0, 0.0 }, { 0.0, 1.0, 0.0 }
    };
    glm::vec3 bitangents[] = {
        { 0.0, -1.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 0.0, -1.0 }, { 0.0, 0.0, 1.0 }, { -1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }
    };
    Vertex* vertices = new Vertex[count];
    for (int i = 0; i < count; i++)
    {
        int k = 3 * i;
        vertices[i] = { { cubeVertices[k], cubeVertices[k + 1], cubeVertices[k + 2] }, texCoords[i % 6], normals[i / 6], tangents[i / 6], bitangents[i / 6] };
    }*/
    glm::vec2 texCoords[] = {
        { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 },
        { 1.0, 1.0 }, { 0.0, 1.0 }, { 0.0, 0.0 },
        { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 },
        { 1.0, 1.0 }, { 0.0, 1.0 }, { 0.0, 0.0 },
        { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 },
        { 1.0, 1.0 }, { 0.0, 1.0 }, { 0.0, 0.0 },
        { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 },
        { 1.0, 1.0 }, { 0.0, 1.0 }, { 0.0, 0.0 },
        { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 },
        { 1.0, 1.0 }, { 0.0, 1.0 }, { 0.0, 0.0 },
        { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 },
        { 1.0, 1.0 }, { 0.0, 1.0 }, { 0.0, 0.0 },
    };
    Vertex* vertices = TexturedModel::createVertices(count, cubeVertices, texCoords);
    Model* model = new Model(count, vertices);
    model->setTransform(pos, rotation, glm::vec3(half_length));
    return new TexturedModel(model, texture);
}