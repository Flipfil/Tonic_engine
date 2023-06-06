#pragma once

#include "tonic/graphics/mesh.h"
#include "tonic/graphics/shader.h"

#include "Gameobjects/game_object.h"
#include "Gameobjects/ball.h"


#include "external/glm/glm.hpp"

#include <memory>

namespace factory
{
    static float VERTICES[]
    {
          0.5f,  0.5f,  0.f,  // up-right
         -0.5f,  0.5f,  0.f,  // down-right
         -0.5f, -0.5f,  0.f,  // down-left
          0.5f, -0.5f,  0.f   // up-left
    };

    static uint32_t ELEMENTS[]
    {
        0,3,1,
        1,3,2
    };

    static const char* VERTEX_SHADER = R"(
                #version 410 core
                layout (location = 0) in vec3 position;
                out vec3 pos;
                uniform mat4 model = mat4(1.0);
                void main()
                {
                    pos = position;
                    gl_Position = model * vec4(position, 1.0);
                }
            )";

    static const char* FRAGMENT_SHADER = R"(
                #version 410 core
                out vec4 out_color; 
                in vec3 pos;
                void main()
                {
                    out_color = vec4(1.0);
                }
            )";

    static const char* BALL_FRAGMENT_SHADER = R"(
                #version 410 core
                out vec4 out_color; 
                in vec3 center_pos;
                void main()
                {
                    if (length(center_pos) > 0.5) discard;
                    out_color = vec4(1.0);
                }
            )";
    
    std::shared_ptr<Gameobject> CreatePaddle(const glm::vec2& pos)
    {
        std::shared_ptr<tonic::graphics::Mesh> mesh = std::make_shared<tonic::graphics::Mesh>(VERTICES, 4, 3, ELEMENTS, 6);
        std::shared_ptr<tonic::graphics::Shader> shader = std::make_shared<tonic::graphics::Shader>(VERTEX_SHADER, BALL_FRAGMENT_SHADER);
        
        glm::vec2 size{ 0.1, 0.3 };

        return std::make_shared<Gameobject>(mesh, shader, pos, size);
    }

    std::shared_ptr<Ball> CreateBall(const glm::vec2& pos)
    {
        std::shared_ptr<tonic::graphics::Mesh> mesh = std::make_shared<tonic::graphics::Mesh>(VERTICES, 4, 3, ELEMENTS, 6);
        std::shared_ptr<tonic::graphics::Shader> shader = std::make_shared<tonic::graphics::Shader>(VERTEX_SHADER, BALL_FRAGMENT_SHADER);

        glm::vec2 size{ 0.1, 0.1 };

        return std::make_shared<Ball>(mesh, shader, pos, size);
    }
}