#pragma once

#include "GameObjects/projectile.h"

// an encapsulation of player - left as open struct. SpaceInvaders and its functions give this struct meaning
struct Player 
{
	struct Controls
	{
		int   shoot;
		int   left;
		int   right;
	} controls;

	float speed;
	int score;
	int lives;

	std::string color;

	std::shared_ptr<Gameobject>	ship;
	std::shared_ptr<Projectile> shot;
	std::vector<std::shared_ptr<Gameobject>> lives_indicator;
	std::vector<std::shared_ptr<Gameobject>> score_indicator;
};