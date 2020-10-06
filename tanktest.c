/** \file tanktest.c
*   \brief The main source file for tanktest, a basic test/example of using sprites on the Uzebox console
*   \author Dan MacDonald
*   \date 2020
*/

#include <stdbool.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <uzebox.h>

#include "data/tileset.inc"
#include "data/uzeboxlogo_8x8.pic.inc" // these are used to draw the normal uzebox logo
#include "data/uzeboxlogo_8x8.map.inc" // for the custom intro

uint_least8_t     player_x = 80, ///< player x position on screen. 0  is far left
		          player_y = 40; ///< player y position on screen. 0 is top
		          
int btnPrev = 0;     // Previous button
int btnHeld = 0;     // buttons that are held right now
int btnPressed = 0;  // buttons that were pressed this frame
int btnReleased = 0; // buttons that were released this frame 

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
	SetSpritesTileTable(tileset); //sets the tiles to be used for our various sprites
	ClearVram(); //fill entire screen with first tile in the tileset (blank the screen)
}



/**
 * \brief Setup for custom intro
 */
void initIntro(void)
{
	MapSprite2(0, mysprites, 0); //setup tank for drawing
	player_x = 0; //set tank to far left
	player_y = 100; //center tank vertically
	SetTileTable(logo_tileset); //setup tiles for drawing uzebox logo
}

/**
 * \brief Draws the custom intro
 */
void drawIntro(void)
{
	ClearVram(); //wipe screen each frame
	MoveSprite(0, player_x, player_y, 1, 1); //position tank sprite
	DrawMap(13,12,map_uzeboxlogo); //draw uzebox logo and name
}

/**
 * \processes controller input
 */
void processIntro(void)
{    
    btnHeld = ReadJoypad(0); //read in our player one joypad input
    btnPressed = btnHeld & (btnHeld ^ btnPrev);
    btnReleased = btnPrev & (btnHeld ^ btnPrev);

	if(btnHeld & BTN_RIGHT){
        player_x++;
    }
    if(btnHeld & BTN_LEFT){
        player_x--;
    }
    if(btnHeld & BTN_UP){
        player_y--;
    }
    if(btnHeld & BTN_DOWN){
        player_y++;
    }
    btnPrev = btnHeld;
}

