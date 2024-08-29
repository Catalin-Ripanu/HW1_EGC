#pragma once

#include "utils/glm_utils.h"


namespace transform2D
{
    // Translate matrix
    inline glm::mat3 Translate(float translateX, float translateY)
    {
        
        return glm::transpose(glm::mat3(1.f, 0.f, (float) translateX,
            0.f, 1.f, (float) translateY,
            0.f, 0.f, 1.f));

    }

    // Scale matrix
    inline glm::mat3 Scale(float scaleX, float scaleY)
    {
       
        return glm::transpose(glm::mat3( (float) scaleX, 0.f, 0.f,
            0.f, (float) scaleY, 0.f,
            0.f, 0.f, 1.f));

    }

    // Rotate matrix
    inline glm::mat3 Rotate(float radians)
    {
       
        return glm::transpose(glm::mat3( (float) cos(radians), - (float) sin(radians), 0.f,
            (float) sin(radians), (float) cos(radians), 0.f,
            0.f, 0.f, 1.f));

    }
}   