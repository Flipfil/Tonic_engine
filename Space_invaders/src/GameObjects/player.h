#pragma once

#include "game_object.h"

struct Player
{
	int   shoot;
	int   left;
	int   right;
	int   lives;
	std::shared_ptr<Gameobject> shot_blast;
	std::shared_ptr<Gameobject>	ship;
	std::string color;
	std::vector<std::shared_ptr<Gameobject>> lives_indicator;
	std::vector<std::shared_ptr<Gameobject>> score_indicator;
};
