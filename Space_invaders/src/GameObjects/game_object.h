#pragma once

#include "tonic/graphics/vertex.h"
#include "tonic/graphics/material.h"
#include "tonic/graphics/texture.h"

#include "tonic/core/time.h"

#include "external/glm/glm.hpp"

#include <memory>

class Gameobject
{
public:

	enum class State
	{
		Active,
		Dying,
		Deactivated
	};

	Gameobject(
		std::weak_ptr<tonic::graphics::VertexArray> va,
		std::weak_ptr<tonic::graphics::Material> material,
		std::weak_ptr<tonic::graphics::Texture> texture,
		const glm::vec2& size = glm::vec2(0.5f),
		const glm::vec2& pos  = glm::vec2(0.f),
		State state = State::Deactivated)
		: m_VA(va)
		, m_material(material)
		, m_texture(texture)
		, m_pos(pos)
		, m_size(size)
		, m_state(state)
	{
		CalculateModelMat();
	}

	void Move(const glm::vec2& by);
	void Scale(const glm::vec2& by);
	void SetPos(const glm::vec2& pos);
	void SetSize(const glm::vec2& size);

	inline std::weak_ptr<tonic::graphics::Texture>  GetTexture() { return m_texture; }
	inline std::weak_ptr<tonic::graphics::Material> GetMaterial() { return m_material; }

	inline void SetTexture(std::weak_ptr<tonic::graphics::Texture> texture) { m_texture = texture; }
	inline void SetMaterial(std::weak_ptr<tonic::graphics::Material> material) { m_material = material; }


	inline const glm::vec2& GetPos()   { return m_pos; }
	inline const glm::vec2& GetSize()  { return m_size; }
	inline State GetState()			   { return m_state; }
	inline void  SetState(State state) { m_state = state; }

	virtual void Update();
	virtual void Render();

protected:
	void CalculateModelMat();

private:
	std::weak_ptr<tonic::graphics::VertexArray> m_VA;
	std::weak_ptr<tonic::graphics::Material>	m_material;
	std::weak_ptr<tonic::graphics::Texture>	    m_texture;

	glm::vec2 m_pos;
	glm::vec2 m_size;
	glm::mat4 m_model;

	State m_state;
};

class AnimatedObject : public Gameobject
{
public:
	AnimatedObject(
		std::weak_ptr<tonic::graphics::VertexArray> va,
		std::weak_ptr<tonic::graphics::Material> material,
		std::vector<std::weak_ptr<tonic::graphics::Texture>> animation,
		const glm::vec2& size = glm::vec2(0.5f),
		float fps = 0, // default set to animation.size()
		const glm::vec2& pos = glm::vec2(0.f),
		State state = State::Active)
		:
		Gameobject(va, material, animation[0], size, pos, state)
		, m_frame(0)
		, m_animation(animation)
		, m_frame_rate(1 / (fps > 0 ? fps : animation.size()))
		, m_delay     (1 / (fps > 0 ? fps : animation.size()))
	{
	}

	void Update() override;

	inline float GetFrameRate() { return 1 / m_frame_rate; }
	inline void SetFrameRate(float frame_rate) { m_frame_rate = 1 / frame_rate; }
	inline void NextFrame()      { m_frame = (unsigned short)((m_frame + 1)                      % m_animation.size()); }
	inline void PrevFrame()      { m_frame = (unsigned short)((m_frame + m_animation.size() - 1) % m_animation.size()); }
	inline void ResetAnimation() { m_frame = (unsigned short)0; }
	inline void SetAnimation(std::vector<std::weak_ptr<tonic::graphics::Texture>> frames) { m_animation = frames; }

protected:
	unsigned short m_frame;
	float m_frame_rate;
	float m_delay;
	std::vector<std::weak_ptr<tonic::graphics::Texture>> m_animation;
}; 