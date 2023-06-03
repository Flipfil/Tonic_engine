#include "tonic/input/controller.h"

#include "tonic/log.h"

#include "SDL_events.h"
#include "SDL_gamecontroller.h"

#include <algorithm>

namespace tonic::input
{
	std::unordered_map<int, std::unique_ptr<Controller::ControllerStruct>> Controller::available_controllers;
	float Controller::deadzone = 0.1f;


	void Controller::OnControllerConnected(SDL_ControllerDeviceEvent& event)
	{
		int device_id = event.which;
		if (SDL_IsGameController(device_id))
		{
			auto c = std::make_unique<ControllerStruct>();
			c->gc = SDL_GameControllerOpen(device_id);
			if (c->gc)
			{
				c->controller_id = device_id;
				c->buttons_down.fill(false);
				c->buttons_down_last.fill(false);
				c->axes.fill(0.f);
				c->axes_last.fill(0.f);

				int map_id = GetFreeMapID();
				TONIC_TRACE("Controller connected: map_id({}), device_id({})", map_id, device_id);
				available_controllers[map_id] = std::move(c);
			}
			else
			{
				TONIC_TRACE("Error opening game controller with Device ID {}: {}", device_id, SDL_GetError());
			}
		}
	}

	void Controller::OnControllerDisconnected(SDL_ControllerDeviceEvent& event)
	{
		int device_id = event.which;
		for (auto it = available_controllers.begin(); it != available_controllers.end(); it++)
		{
			ControllerStruct* c = it->second.get();
			if (c->controller_id == device_id)
			{
				TONIC_TRACE("Controller disconnected: {}", device_id);
				SDL_GameControllerClose(c->gc);
				available_controllers.erase(it);
				break;
			}
		}
	}

	void Controller::Update()
	{
		for (auto it = available_controllers.begin(); it != available_controllers.end(); it++)
		{
			ControllerStruct* c = it->second.get();
			TONIC_ASSERT(c && c->gc, "Invalid game controller in available_controllers map");
			if (c && c->gc)
			{
				for (unsigned int i = 0; i < static_cast<int>(Button::COUNT); i++)
				{
					c->buttons_down_last[i] = c->buttons_down[i];
					c->buttons_down[i] = SDL_GameControllerGetButton(c->gc,static_cast<SDL_GameControllerButton>(i));
				}
				for (unsigned int i = 0; i < static_cast<int>(Axis::COUNT); i++)
				{
					c->axes_last[i] = c->axes[i];
					c->axes[i] = std::clamp(SDL_GameControllerGetAxis(c->gc, static_cast<SDL_GameControllerAxis>(i)) / 32767.f, -1.f, 1.f); // SDL Axis ranges from -32767 to 32767
				}
			}
		}
	}

	void Controller::Shutdown()
	{
		for (auto it = available_controllers.begin(); it != available_controllers.end();)
		{
			ControllerStruct* c = it->second.get();
			SDL_GameControllerClose(c->gc);
			available_controllers.erase(it);
		}
	}

	bool Controller::IsControllerAvailable(int controller_id)
	{
		return available_controllers.count(controller_id) > 0;
	}

	bool Controller::HoldingButton(int controller_id, Button button)
	{
		auto it = available_controllers.find(controller_id);
		if (it == available_controllers.end())
		{
			TONIC_ERROR("Controller with id '{}' is not available!", controller_id);
			return false;
		}

		return it->second->buttons_down[static_cast<int>(button)];
	}

	bool Controller::PressedButton(int controller_id, Button button)
	{
		auto it = available_controllers.find(controller_id);
		if (it == available_controllers.end())
		{
			TONIC_ERROR("Controller with id '{}' is not available!", controller_id);
			return false;
		}

		return it->second->buttons_down[static_cast<int>(button)] && !it->second->buttons_down_last[static_cast<int>(button)];
	}

	bool Controller::ReleasedButton(int controller_id, Button button)
	{
		auto it = available_controllers.find(controller_id);
		if (it == available_controllers.end())
		{
			TONIC_ERROR("Controller with id '{}' is not available!", controller_id);
			return false;
		}

		return !it->second->buttons_down[static_cast<int>(button)] && it->second->buttons_down_last[static_cast<int>(button)];
	}

	float Controller::GetRawAxis(int controller_id, Axis axis)
	{
		auto it = available_controllers.find(controller_id);
		if (it == available_controllers.end())
		{
			TONIC_ERROR("Controller with id '{}' is not available!", controller_id);
			return 0.f;
		}

		return it->second->axes[static_cast<int>(axis)];
	}

	float Controller::GetAxis(int controller_id, Axis axis)
	{
		float raw = GetRawAxis(controller_id, axis);
		return abs(raw) > deadzone ? raw : 0.f;
	}

	float Controller::GetAxisDelta(int controller_id, Axis axis)
	{
		auto it = available_controllers.find(controller_id);
		if (it == available_controllers.end())
		{
			TONIC_ERROR("Controller with id '{}' is not available!", controller_id);
			return 0.f;
		}

		return it->second->axes[static_cast<int>(axis)] - it->second->axes_last[static_cast<int>(axis)];
	}

	void Controller::SetDeadzone(float deadzone)
	{
		Controller::deadzone = deadzone;
	}

	int Controller::GetFreeMapID()
	{
		int ret = -1;

		for (int i = 0; i < SDL_NumJoysticks(); i++)
		{
			if (available_controllers.count(i) == 0)
			{
				ret = i;
				break;
			}
		}

		return ret;
	}
}
