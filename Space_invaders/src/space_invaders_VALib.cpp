#include "space_invaders.h"

//----------------------------------------------------
// VertexArrays

void SpaceInvaders::InitializeVALib()
{
	const std::vector<std::vector<float>> SQUARE_VERTICES =
	{
		{ -0.5f,  0.5f,  0.f }, // left-up
		{ -0.5f, -0.5f,  0.f }, // left-down
		{  0.5f, -0.5f,  0.f }, // right-down
		{  0.5f,  0.5f,  0.f }  // right-up
	};

	const std::vector<std::vector<short>> SQUARE_TEXTURE_COORDS =
	{
		{0,1}, // left-up
		{0,0}, // left-down
		{1,0}, // right-down
		{1,1}  // right-up
	};

	const std::vector<uint32_t> ELEMENTS =
	{
		0,3,1,
		1,3,2
	};

	std::shared_ptr<graphics::VertexArray> textured_rect_va = std::make_shared<tonic::graphics::VertexArray>();

	TONIC_CREATE_VERTEX_BUFFER(vb, float);
	TONIC_CREATE_VERTEX_BUFFER(vb_t, short);

	for (auto vertex : SQUARE_VERTICES)
		vb->PushVertex(vertex);
	vb->SetLayout({ 3 });

	for (auto texture_vertex : SQUARE_TEXTURE_COORDS)
		vb_t->PushVertex(texture_vertex);
	vb_t->SetLayout({ 2 });

	textured_rect_va->PushBuffer(std::move(vb));
	textured_rect_va->PushBuffer(std::move(vb_t));
	textured_rect_va->SetElements(ELEMENTS);

	textured_rect_va->Upload();

	m_va_lib.Load("textured_rect", textured_rect_va);
}