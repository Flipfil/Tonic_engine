#include "rect_creator.h"

#include "tonic/engine.h"

#include "external/imgui/imgui.h"

//----------------------------------------------------------
void RectCreator::LoadFromFile(const std::string& file_name)
{
}

//----------------------------------------------------------
void RectCreator::SaveToFile(const std::string& file_name)
{
}

//----------------------------------------------------------
void RectCreator::Render()
{
	auto& rm = Engine::GetInstance().GetRenderManager();
	rm.Submit(TONIC_SUBMIT_RENDER_CMD(PushFrameBuffer, m_frame_buffer));

    rm.Submit(std::move(
        m_texture
        ? std::make_unique<graphics::RENDER_COMMANDS::RenderVertexArrayTextured>(m_va, m_texture, m_shader)
        : std::make_unique<graphics::RENDER_COMMANDS::RenderVertexArray>(m_va, m_shader)
    ));


	rm.Submit(TONIC_SUBMIT_RENDER_CMD(PopFrameBuffer));
}

//----------------------------------------------------------
void RectCreator::ImguiRender()
{
    if (ImGui::Begin("Rect View"))
    {
        if (ImGui::IsWindowHovered())
        {
            ImGui::CaptureMouseFromApp(false);
        }

        ImVec2 size = { 100,100 };
        ImVec2 uv0 = { 0,1 };
        ImVec2 uv1 = { 1,0 };
        ImGui::Image((void*)(intptr_t)m_frame_buffer->GetTextureID(), size, uv0, uv1);
    }
    ImGui::End();
}