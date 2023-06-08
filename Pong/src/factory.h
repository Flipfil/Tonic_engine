#pragma once

#include "tonic/graphics/vertex.h"
#include "tonic/graphics/shader.h"

#include "Gameobjects/game_object.h"
#include "Gameobjects/ball.h"


#include "external/glm/glm.hpp"

#include <memory>

namespace factory
{
    static std::vector<float> VERTEX_0 = {  0.5f,  0.5f,  0.f }; // up-right
    static std::vector<float> VERTEX_1 = { -0.5f,  0.5f,  0.f };  // down-right
    static std::vector<float> VERTEX_2 = { -0.5f, -0.5f,  0.f };  // down-left
    static std::vector<float> VERTEX_3 = {  0.5f, -0.5f,  0.f };   // up-left
    

    static std::vector<uint32_t> ELEMENTS
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
                in vec3 pos;
                void main()
                {
                    if (length(pos) > 0.5) discard;
                    out_color = vec4(1.0);
                }
            )";
    
    std::shared_ptr<Gameobject> CreatePaddle(const glm::vec2& pos)
    {
        TONIC_CREATE_VERTEX_BUFFER(vb, float);
        vb->PushVertex(VERTEX_0);
        vb->PushVertex(VERTEX_1);
        vb->PushVertex(VERTEX_2);
        vb->PushVertex(VERTEX_3);
        vb->SetLayout({ 3 });

        std::shared_ptr<tonic::graphics::VertexArray> va = std::make_shared<tonic::graphics::VertexArray>();
        va->PushBuffer(std::move(vb));
        va->SetElements(ELEMENTS);
        va->Upload();

        std::shared_ptr<tonic::graphics::Shader> shader = std::make_shared<tonic::graphics::Shader>(VERTEX_SHADER, BALL_FRAGMENT_SHADER);
        
        glm::vec2 size{ 0.1, 0.3 };
        return std::make_shared<Gameobject>(va, shader, pos, size);
    }

    std::shared_ptr<Ball> CreateBall(const glm::vec2& pos)
    {
        TONIC_CREATE_VERTEX_BUFFER(vb, float);
        vb->PushVertex(VERTEX_0);
        vb->PushVertex(VERTEX_1);
        vb->PushVertex(VERTEX_2);
        vb->PushVertex(VERTEX_3);
        vb->SetLayout({ 3 });

        std::shared_ptr<tonic::graphics::VertexArray> va = std::make_shared<tonic::graphics::VertexArray>();
        va->PushBuffer(std::move(vb));
        va->SetElements(ELEMENTS);
        va->Upload();

        std::shared_ptr<tonic::graphics::Shader> shader = std::make_shared<tonic::graphics::Shader>(VERTEX_SHADER, BALL_FRAGMENT_SHADER);

        glm::vec2 size{ 0.1, 0.1 };

        return std::make_shared<Ball>(va, shader, pos, size);
    }
}