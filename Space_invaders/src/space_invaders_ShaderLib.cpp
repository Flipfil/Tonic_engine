#include "space_invaders.h"

//----------------------------------------------------
// Shaders

void SpaceInvaders::InitializeShaderLib()
{
    const char* VERTEX_SHADER = R"(
                #version 410 core

                layout (location = 0) in vec3 position;
                layout (location = 1) in vec2 texture_coords;

                out vec2 uvs;

                uniform mat4 model = mat4(1.0);
                void main()
                {
                    uvs = texture_coords;
                    gl_Position = model * vec4(position, 1.0);
                }
            )"; // translates vertices and passes uvs

    const char* FRAGMENT_SHADER_TEXTURE_SILHOULETTE = R"(
                #version 410 core

                in vec2 uvs;

                out vec4 out_color; 

                uniform vec3 color;
                uniform sampler2D tex;
                void main()
                {
                    out_color = vec4(color, texture(tex, uvs).a);
                }
            )"; // passes only alpha from texture and adds custom color i.e. create a silhoulette from texture and slap a new color on top

    const char* FRAGMENT_SHADER_TEXTURE = R"(
                #version 410 core

                in vec2 uvs;

                out vec4 out_color; 

                uniform sampler2D tex;
                void main()
                {
                    out_color = texture(tex, uvs);
                }
            )"; // draws texture as is

    m_shader_lib.Load("texture_silhoulette", std::make_shared<tonic::graphics::Shader>(VERTEX_SHADER, FRAGMENT_SHADER_TEXTURE_SILHOULETTE));
    m_shader_lib.Load("texture", std::make_shared<tonic::graphics::Shader>(VERTEX_SHADER, FRAGMENT_SHADER_TEXTURE));
}