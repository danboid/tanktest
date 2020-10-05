/** \file tanktest.c
*   \brief The main source file for tanktest
*   \author -
*   \date 2020
*/

#include <stdbool.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <uzebox.h>

#include "data/title_tiles.inc" // menu tiles
#include "data/ghost_sprites.inc" // all the sprites for the game
#include "data/tileset.inc" // tank sprites
#include "data/uzeboxlogo_8x8.pic.inc" // these are used to draw the normal uzebox logo
#include "data/uzeboxlogo_8x8.map.inc" // for the custom intro

uint_least16_t    lastbuttons=0, ///< saves the buttons pressed during the previous frame
                  joy=0; ///< bitfield used for tracking the buttons pressed during the current frame
uint_least8_t     player_x = 80, ///< player x position on screen. 0  is far left
		          player_y = 40; ///< player y position on screen. 0 is top

//const char * player_sprites[9] = {ghost0, ghost1, ghost2, ghost3, ghost4, ghost5, ghost6, ghost7, ghost8}; ///< convenience array for displaying the ghost sprites
const char * current_sprite; ///< used as an index in player_sprites array to display correct ghost sprite

/**
 * \enum state
 * \brief Badly titled, but this is for the actual state of the game as a whole. Used for determining which screen to draw, what to do with player input, and what game logic to use.
 */
typedef enum {INTRO, MAIN_MENU, GAME, LOCAL_SCORES} state;
state game_state = INTRO; ///< Tracks current state of the game.

void drawIntro(void);
void processIntro(void);
void initIntro(void);
static void initialSetup(void);

/**
 * \brief The main game loop. This just cycles endlessly, it uses the game's 'state' to determine which screen to show and what to do.
 */
int main(){
	//looping back and forth forever (cards against humanity reference)
	while(1)
	{
		//some basic prep work performed once before our custom intro
		if(game_state == INTRO)
		{
			initialSetup();
			initIntro();
		}
		//perform custom intro
		while(game_state == INTRO)
		{
			//wait until the next frame
			WaitVsync(1);
			drawIntro();
			processIntro();
		}
		
    }
}

/**
 * \brief Performs some basic initialization functions.
 *
 * Sets up our graphics, initializes a few variables for convenience
 */
static void initialSetup()
{
	SetSpritesTileTable(ghost_sprites); //sets the tiles to be used for our various sprites
	SetFontTilesIndex(TITLE_TILES_SIZE); //tiles for the font were included immediately after the include for background tiles
	                               //therefore this says how many tiles in memory to move forward from the first one
	                               //in order to find the beginning of the font tiles
	SetTileTable(title_tiles); //sets the tiles to be used for the normal background tiles in the game
	ClearVram(); //fill entire screen with first tile in the tileset (blank the screen)
}



/**
 * \brief Setup for custom intro
 */
void initIntro(void)
{
	MapSprite2(1, tileset[1], 0); //setup tank for drawing
	player_x = 0; //set ghost to far left
	player_y = 80; //center ghost vertically
	SetTileTable(logo_tileset); //setup tiles for drawing uzebox logo
}

/**
 * \brief Draws the custom intro
 */
void drawIntro(void)
{
	ClearVram(); //wipe screen each frame
	//	current_sprite = player_sprites[0]; //change our tracking variable to the correct sprite based on new frame
	//	MapSprite2(0, current_sprite, 0); //actually reassign the sprites in memory to the correct images
	MoveSprite(1, player_x, player_y, 3, 3); //position ghost sprite
	DrawMap(13,12,map_uzeboxlogo); //draw uzebox logo and name
}

/**
 * \brief Logic for custom intro and processes controller input for skipping intro.
 */
void processIntro(void)
{
//	player_y = (u8)(100+pgm_read_byte(&(sine32[wave_tick]))); //set ghost height based on sine function
	player_x++; //move ghost left to right 1 pixel

	//read in our player one joypad input
	joy=ReadJoypad(0);

	//if player 1 is currently pressing start
	if((joy&BTN_START) && (joy != lastbuttons))
	{
		SetTileTable(title_tiles);
		lastbuttons=joy;
	}
	lastbuttons=joy;
}

