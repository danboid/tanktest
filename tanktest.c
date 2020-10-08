/** \file tanktest.c
*   \brief The main source file for tanktest, a basic test of Uzebox sprites
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

uint_least8_t     player_x = 80, ///< player x position on screen. 0 is far left
		          player_y = 40; ///< player y position on screen. 0 is top
		          
int btnPrev = 0;     // Previous button
int btnHeld = 0;     // buttons that are held right now
int btnPressed = 0;  // buttons that were pressed this frame
int btnReleased = 0; // buttons that were released this frame 

void drawIntro(void);
void processIntro(void);
void initIntro(void);
static void initialSetup(void);

/**
 * \brief The main game loop. This just cycles endlessly.
 */
int main()
{
		//some basic prep work
        initialSetup();
        initIntro();
		//Main loop
		while(1)
		{
			//wait until the next frame
			WaitVsync(1);
			drawIntro();
			processIntro();
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
	MapSprite2(0, tankright, 0); //setup tank for drawing
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
    //btnPressed = btnHeld & (btnHeld ^ btnPrev);
    //btnReleased = btnPrev & (btnHeld ^ btnPrev);

    if(btnHeld & BTN_RIGHT){
        MapSprite2(0,tankright,0);
        player_x++;
    }
    if(btnHeld & BTN_LEFT){
        MapSprite2(0,tankright,SPRITE_FLIP_X);
        player_x--;
    }
    if(btnHeld & BTN_UP){
        MapSprite2(0,tankup,0);
        player_y--;
    }
    if(btnHeld & BTN_DOWN){
        MapSprite2(0,tankup,SPRITE_FLIP_Y);
        player_y++;
    }
    btnPrev = btnHeld;
}

