#include "myObject2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* myObject2D::CreateSquareRectangle(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* myObject2D::CreateSquareRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length1,
    float length2,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length1, 0, 0), color),
        VertexFormat(corner + glm::vec3(length1, length2, 0), color),
        VertexFormat(corner + glm::vec3(0, length2, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}


Mesh* myObject2D::CreateTriangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill,
    std::vector<unsigned int> indices)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length / 2, length , 0), color),
    };

    Mesh* square = new Mesh(name);
    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* myObject2D::CreateCircle(
    const std::string& name,
    glm::vec3 color
)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    GLfloat arg;
    Mesh* circle = new Mesh(name);
    vertices.emplace_back(glm::vec3(0, 0, 0), color);
    for (GLushort i = 0; i < NUM_TRIANGLES; i++)
    {
        arg = TWO_PI * i / NUM_TRIANGLES;

        vertices.emplace_back(glm::vec3(cos(arg), sin(arg), 0), color);
        indices.push_back(i);
    }
    indices.push_back(NUM_TRIANGLES);
    indices.push_back(1);

    // Create a new mesh from buffer data
    circle->InitFromData(vertices, indices);
    circle->SetDrawMode(GL_TRIANGLE_FAN);
    return circle;
}
