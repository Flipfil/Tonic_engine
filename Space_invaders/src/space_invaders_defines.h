#pragma once

// borders
#define BOTTOM_BORDER_HEIGHT     1 / 8.f 
#define TOP_BORDER_HEIGHT        1 / 4.f

// invaders
#define INVADER_SIZE			  glm::vec2(0.1f, 0.1f * 2 / 3.f)
#define INVADER_SPEED			  0.017f
#define INVADER_MOVE_DELAY		  0.5f
#define INVADER_SHOOT_DELAY		  0.5f
#define INVADER_MAX_SHOTS		  6
#define INVADER_TYPE_CNT		  5
#define INVADER_GRID_SIZE         glm::vec2(1.8f, 1.2f)
#define INVADER_GRID_CENTER_POS   glm::vec2(1.8f, 1.2f)
#define INVADERS_IN_ROW			  11
#define INVADERS_IN_COL			  5

// max score
#define SCORE_DIGITS 7

// players
#define PLAYER_SIZE       glm::vec2(0.15f, 0.075f)
#define PLAYER_SPEED      0.5f
#define MAX_LIVES         3
#define MAX_PLAYERS       2
#define PLAYER_1_COLOR    "green"
#define PLAYER_1_CONTROLS { TONIC_KEY_SPACE, TONIC_KEY_LEFT, TONIC_KEY_RIGHT }
#define PLAYER_2_COLOR    "cyan"
#define PLAYER_2_CONTROLS { TONIC_KEY_LCTRL, TONIC_KEY_A, TONIC_KEY_D }

// shots
#define SHOT_SIZE			glm::vec2(0.05f * 3 / 7.f, 0.05f)
#define SHOT_BLAST_SPEED    1.f  
#define SHOT_SQUIGGLY_SPEED 0.7f 
#define SHOT_SPIRAL_SPEED   0.5f 

// mothership
#define MOTHERSHIP_SIZE glm::vec2(0.18f, 0.06f)
#define MOTHERSHIP_SPEED PLAYER_SPEED
#define MOTHERSHIP_SPAWN_RATE 10.f

// menus
#define MENU_LETTER_SIZE glm::vec2(1/8.f, 1/8.f)
#define TITLE_SIZE glm::vec2(1.6f, 0.3f)
#define TITLE_POS  glm::vec2(0.0f, 1 / 2.f)

// hideout
#define HIDEOUT_SIZE glm::vec2(0.4f,0.2f)
#define HIDEOUT_SPEED 0.1f
