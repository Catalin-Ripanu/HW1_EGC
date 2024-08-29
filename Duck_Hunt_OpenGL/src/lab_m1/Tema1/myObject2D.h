#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

#define TWO_PI			(6.28318530718f)
#define NUM_TRIANGLES	(50)

namespace myObject2D
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquareRectangle(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateSquareRectangle(const std::string& name, glm::vec3 leftBottomCorner, float length1, float length2, glm::vec3 color, bool fill = false);
    Mesh* CreateTriangle(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false, std::vector<unsigned int> indices = { 0, 1, 2 });
    Mesh* CreateCircle(const std::string& name, glm::vec3 color);
    
}
