/* ==============================================
*  Copyright ©  2014  Ali M. Al-Bayaty
*  
*  Video-Game-Engine is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  any later version.
*  
*  Video-Game-Engine is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*  
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/* ==============================================
*  
*  Video Game Engine Project
*  ( SDK: The game engine )
*  
*  MSEE student:    Ali M. Al-Bayaty
*  EE659: System-On-Chip
*  Personal website: <http://albayaty.github.io/>
*  Source code link: <https://github.com/albayaty/Video-Game-Engine.git>
*
*  ==============================================
*/
#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "xil_types.h"
#include "malloc.h"
#include "xgpio_l.h"

// MACRO as variables replacement:
#define NOTHING 20  // Indication for the Nothing
#define CLEAR 21     // Indication for the Sky
#define BLOCK 23     // Indication for the Block
#define LADDER1 24   // Indication for the Ladder_Left
#define LADDER2 25   // Indication for the Ladder_Right
#define FIRE 26      // Indication for the fire
#define BERRY 16     // Indication for the strawberry
#define ENEMY1 18    // Indication for the enemy1: skull
#define ENEMY2 19    // Indication for the enemy2: bat
#define ENEMY3 31    // Indication for the enemy3

// MACRO to get the index:
#define index(y,x)  y*40+x

// Enumerations for NES Controller's Buttons:
enum BUTTONS { NONE, RIGHT, LEFT, DOWN=4, UP=8, START=16, SELECT=32, B=64, A=128 };

// Declaring the memory set function, in order to use it in this compiler:
void * memset ( void * ptr, int value, size_t num );

// Global Variables:
// Foreground tile-map (40x30):
char fore_tile[1200];

// Background tile-map (40x30):
char back_tile[1200];

// Sprites map (64x32):
char sprites[2048] = {
			// 0: Character Sprite1 right-head:
			8,8,8,8,8,7,7,7,
			8,8,8,7,7,7,7,7,
			8,8,8,8,7,7,7,7,
			8,8,8,8,8,7,4,4,
			8,8,8,8,8,4,4,4,
			8,8,8,8,4,4,0,4,
			8,8,8,8,4,4,4,4,
			8,8,8,8,8,4,4,4,

			// 1: Character Sprite1 left-head:
			7,7,7,8,8,8,8,8,
			7,7,7,7,8,8,8,8,
			7,7,7,7,8,8,8,8,
			4,4,7,7,8,8,8,8,
			4,4,4,7,8,8,8,8,
			4,0,4,4,8,8,8,8,
			4,4,4,4,8,8,8,8,
			4,4,4,8,8,8,8,8,

			// 2: Character Sprite1 right-shoulder:
			8,8,8,8,8,4,7,4,
			8,8,8,8,8,4,4,7,
			8,8,8,8,8,8,4,4,
			8,8,8,8,8,8,4,4,
			8,8,8,8,8,8,4,4,
			8,8,8,8,4,4,4,4,
			8,8,8,4,4,4,4,4,
			8,8,4,4,4,4,4,4,

			// 3: Character Sprite 1 left-shoulder:
			4,7,4,8,8,8,8,8,
			7,4,4,8,8,8,8,8,
			4,4,8,8,8,8,8,8,
			4,4,8,8,8,8,8,8,
			4,4,8,8,8,8,8,8,
			4,4,4,4,8,8,8,8,
			4,4,4,4,4,8,8,8,
			4,4,4,4,4,4,8,8,

			// 4: Character Sprite 1 right-hand:
			8,8,4,4,8,4,4,4,
			8,4,4,8,8,4,4,4,
			8,4,4,8,8,4,4,4,
			8,4,4,8,8,4,4,4,
			8,4,4,8,8,4,4,4,
			8,4,4,8,8,4,4,4,
			8,4,4,8,8,4,4,0,
			4,4,4,4,8,0,0,0,

			// 5: Character Sprite 1 left-hand:
			4,4,4,8,4,4,8,8,
			4,4,4,8,8,4,4,8,
			4,4,4,8,8,4,4,8,
			4,4,4,8,8,4,4,8,
			4,4,4,8,8,4,4,8,
			4,4,4,8,8,4,4,8,
			0,4,4,8,8,4,4,8,
			0,0,0,8,4,4,4,4,

			// 6: Character Sprite 1 right-leg:
			8,4,4,8,8,0,0,0,
			8,8,4,8,8,7,7,0,
			8,8,8,8,8,7,7,7,
			8,8,8,8,8,7,7,7,
			8,8,8,8,8,7,7,7,
			8,8,8,8,8,7,7,8,
			8,8,8,8,8,7,7,8,
			8,8,8,8,8,7,7,8,

			// 7: Character Sprite 1 left-leg:
			0,0,0,8,8,4,4,8,
			0,7,7,8,8,4,8,8,
			7,7,7,8,8,8,8,8,
			7,7,7,8,8,8,8,8,
			7,7,7,8,8,8,8,8,
			8,7,7,8,8,8,8,8,
			8,7,7,8,8,8,8,8,
			8,7,7,8,8,8,8,8,

			// 8: Character Sprite 1 right-shoe:
			8,8,8,8,8,7,7,8,
			8,8,8,8,8,7,7,8,
			8,8,8,8,8,7,7,8,
			8,8,8,8,8,7,7,8,
			8,8,8,8,8,7,7,8,
			8,8,8,8,8,0,0,8,
			8,8,8,8,0,0,0,8,
			8,8,8,8,0,0,0,8,

			// 9: Character Sprite 1 left-shoe:
			8,7,7,8,8,8,8,8,
			8,7,7,8,8,8,8,8,
			8,7,7,8,8,8,8,8,
			8,7,7,8,8,8,8,8,
			8,7,7,8,8,8,8,8,
			8,0,0,8,8,8,8,8,
			8,0,0,0,8,8,8,8,
			8,0,0,0,8,8,8,8,

			// 10: Character Sprite 1 right-shoe (MOVING LEFT):
			8,8,8,8,8,7,7,8,
			8,8,8,8,8,7,7,7,
			8,8,8,8,8,8,7,7,
			8,8,8,8,8,8,8,7,
			8,8,8,8,8,8,8,7,
			8,8,8,8,8,8,8,0,
			8,8,8,8,8,8,0,0,
			8,8,8,8,8,8,0,0,

			// 11: Character Sprite 1 left-shoe (MOVING LEFT):
			8,7,7,8,8,8,8,8,
			8,7,7,7,8,8,8,8,
			8,8,7,7,8,8,8,8,
			7,8,8,7,7,8,8,8,
			7,8,8,7,7,8,8,8,
			0,8,8,0,0,8,8,8,
			0,8,0,0,0,8,8,8,
			0,8,0,0,0,8,8,8,

			// 12: Character Sprite 1 right-shoe (MOVING RIGHT):
			8,8,8,8,8,7,7,8,
			8,8,8,8,7,7,7,8,
			8,8,8,8,7,7,8,8,
			8,8,8,7,7,8,8,7,
			8,8,8,7,7,8,8,7,
			8,8,8,0,0,8,8,0,
			8,8,8,0,0,0,8,0,
			8,8,8,0,0,0,8,0,

			// 13: Character Sprite 1 left-shoe (MOVING RIGHT):
			8,7,7,8,8,8,8,8,
			7,7,7,8,8,8,8,8,
			7,7,8,8,8,8,8,8,
			7,8,8,8,8,8,8,8,
			7,8,8,8,8,8,8,8,
			0,8,8,8,8,8,8,8,
			0,0,8,8,8,8,8,8,
			0,0,8,8,8,8,8,8,

			// 14: Character Sprite 1 right-shoe (JUMPING):
			8,8,8,8,8,7,7,8,
			8,8,8,8,7,7,8,8,
			8,0,8,8,7,7,8,8,
			8,0,0,7,7,8,8,8,
			8,0,0,7,7,8,8,8,
			8,0,0,7,7,8,8,8,
			8,8,8,8,8,8,8,8,
			8,8,8,8,8,8,8,8,

			// 15: Character Sprite 1 left-shoe (JUMPING):
			8,7,7,8,8,8,8,8,
			8,8,7,7,8,8,8,8,
			8,8,7,7,8,8,0,8,
			8,8,8,7,7,0,0,8,
			8,8,8,7,7,0,0,8,
			8,8,8,7,7,0,0,8,
			8,8,8,8,8,8,8,8,
			8,8,8,8,8,8,8,8,

			// 16: Strawberry: (14=8+6, 8:nothing, 6:sky color), this strawberry used in forground and background
			14,14,14,2,2,14,14,14,
			14,14,1,1,2,1,14,14,
			14,1,7,2,2,2,1,14,
			1,1,1,2,2,7,1,7,
			1,7,1,1,1,1,1,1,
			1,1,1,1,7,1,7,1,
			14,1,7,1,1,1,1,14,
			14,14,1,1,1,1,14,14,

			// 17: Strawberry Down:
			8,8,8,2,2,8,1,8,
			1,7,8,8,2,8,8,8,
			8,8,1,2,2,2,7,1,
			1,8,8,2,2,8,8,8,
			8,1,8,7,8,8,8,7,
			8,7,8,8,1,7,1,8,
			8,8,1,1,7,1,8,8,
			1,8,8,8,1,8,8,1,

			// 18: ENEMY1: Skull
			8,8,7,7,7,7,8,8,
			8,1,7,7,7,7,1,8,
			7,7,1,7,7,1,7,7,
			7,7,7,7,7,7,7,7,
			7,7,7,7,7,7,7,7,
			8,0,7,0,7,0,7,8,
			8,7,0,7,0,7,0,8,
			8,8,8,7,7,8,8,8,

			// 19: ENEMY2: BAT:
			8,8,0,8,8,0,8,8,
			8,0,1,0,0,1,0,8,
			0,0,0,1,1,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,8,0,0,0,0,8,0,
			8,8,8,0,0,8,8,8,

			// 20: Character Sprite 3_Nothing:
			8,8,8,8,8,8,8,8,
			8,8,8,8,8,8,8,8,
			8,8,8,8,8,8,8,8,
			8,8,8,8,8,8,8,8,
			8,8,8,8,8,8,8,8,
			8,8,8,8,8,8,8,8,
			8,8,8,8,8,8,8,8,
			8,8,8,8,8,8,8,8,

			// 21: BKG 1 (Sky):
			6,6,6,6,6,6,6,6,
			6,6,6,6,6,6,6,6,
			6,6,6,6,6,6,6,6,
			6,6,6,6,6,6,6,6,
			6,6,6,6,6,6,6,6,
			6,6,6,6,6,6,6,6,
			6,6,6,6,6,6,6,6,
			6,6,6,6,6,6,6,6,

			// 22: BKG 2 (Smooth Grass):
			2,2,2,2,2,2,2,2,
			2,2,2,2,2,2,2,2,
			2,2,2,2,2,2,2,2,
			2,2,2,2,2,2,2,2,
			2,2,2,2,2,2,2,2,
			2,2,2,2,2,2,2,2,
			2,2,2,2,2,2,2,2,
			2,2,2,2,2,2,2,2,

			// 23: BKG 3 (Block1):
			2,2,2,2,2,2,2,2,
			2,4,4,4,4,4,4,2,
			2,4,4,2,2,4,4,2,
			2,4,2,4,2,2,4,2,
			2,4,2,2,4,2,4,2,
			2,4,4,2,2,4,4,2,
			2,4,4,4,4,4,4,2,
			2,2,2,2,2,2,2,2,

			// 24: BKG 4 (Ladder_left):
			5,5,5,6,6,6,6,6,
			5,5,5,6,6,6,6,6,
			5,5,5,6,6,6,6,6,
			5,5,5,5,5,5,5,5,
			5,5,5,5,5,5,5,5,
			5,5,5,6,6,6,6,6,
			5,5,5,6,6,6,6,6,
			5,5,5,6,6,6,6,6,

			// 25: BKG 5 (Ladder_Right):
			6,6,6,6,6,5,5,5,
			6,6,6,6,6,5,5,5,
			6,6,6,6,6,5,5,5,
			5,5,5,5,5,5,5,5,
			5,5,5,5,5,5,5,5,
			6,6,6,6,6,5,5,5,
			6,6,6,6,6,5,5,5,
			6,6,6,6,6,5,5,5,

			// 26: BKG 5 (Fire):
			6,6,6,3,6,6,6,6,
			6,6,3,1,3,6,6,6,
			6,3,1,3,1,3,6,6,
			6,3,1,1,3,1,3,6,
			6,6,3,1,3,1,1,3,
			3,3,1,3,3,3,1,1,
			3,1,1,1,3,3,1,1,
			3,3,1,1,1,1,1,3,

			// 27: BKG 5 (Ring_Left):
			8,8,8,8,8,8,8,8,
			8,8,8,8,8,3,3,3,
			8,8,8,8,3,8,8,8,
			8,8,8,3,8,8,8,8,
			8,8,8,3,8,8,8,8,
			8,8,8,8,3,8,8,8,
			8,8,8,8,8,3,3,3,
			8,8,8,8,8,8,8,8,

			// 28: BKG 5 (Ring_Right):
			8,8,8,8,8,8,8,8,
			3,3,3,8,8,8,8,8,
			8,8,8,3,8,8,8,8,
			8,8,8,8,3,8,8,8,
			8,8,8,8,3,8,8,8,
			8,8,8,3,8,8,8,8,
			3,3,3,8,8,8,8,8,
			8,8,8,8,8,8,8,8,

			// 29: BKG 5 (Wing_Left):
			8,8,8,8,8,8,8,8,
			7,7,7,7,7,7,7,7,
			8,7,7,7,7,7,7,7,
			8,8,7,7,7,7,7,7,
			8,8,8,7,7,7,7,7,
			8,8,8,8,7,7,7,7,
			8,8,8,8,8,7,7,7,
			8,8,8,8,8,8,7,7,

			// 30: BKG 5 (Wing_Right):
			8,8,8,8,8,8,8,8,
			7,7,7,7,7,7,7,7,
			7,7,7,7,7,7,7,8,
			7,7,7,7,7,7,8,8,
			7,7,7,7,7,8,8,8,
			7,7,7,7,8,8,8,8,
			7,7,7,8,8,8,8,8,
			7,7,8,8,8,8,8,8,

			// 31: Crazy Bomb:
			8,8,8,8,1,1,1,8,
			8,8,8,3,3,1,8,8,
			8,8,8,3,8,8,8,8,
			8,8,0,3,0,8,8,8,
			8,0,0,0,0,0,8,8,
			8,0,0,0,0,0,8,8,
			8,0,0,0,0,0,8,8,
			8,8,0,0,0,8,8,8};

// Initialized Global variables:
char loadscreen = 1;
char show = 1, enable = 1, shoot = 1;
char enemy = 1;
char shoot_dir=0 ,smurf_dir = 2;   // 0: NONE, 1: LEFT, 2: RIGHT
char enemy_dir = -1;          // 0: NONE, -1: LEFT, +1: RIGHT

// Structure of Screen's Elements and Player:
struct element{
	char x, y;      // For X and Y dimensions
	char direction; // Drawing Direction,0:left-to-right,1:up-to-down
	char repeat;    // How many times to draw the same sprite
	char endType;   // Indication of the end of the same type
	char sprite;    // The location of sprite in the Sprites Map
	struct element* next;  // The linked list to the next element
};

// Structure of Screens:
struct screen{
	struct element *head; // The head that points to the first
                          // element of the same screen
	struct screen *next;  // The linked list to the next screen
};

// Declaring the units:
struct element *back_units[40], *smurf[10], *berry, *skull, *flames[2];
struct element *ring[2], *wing[2];
struct screen *screen1;

// Element Constructor: (Screens and Player)
void constructor(struct element* unit, char x, char y, char direction,
		         char repeat, char endType, char sprite, struct element* next)
{
	unit->x = x;
	unit->y = y;
	unit->direction = direction;
	unit->repeat = repeat;
	unit->endType = endType;
	unit->sprite = sprite;
	unit->next = next;
}

// Clearing the tile maps:
void cleartiles( char once )
{
	int i;

	// Clearing the tile maps:
	memset(&fore_tile[0], NOTHING, 1200);   // Nothing = 20
	memset(&back_tile[0], CLEAR, 1200);       // Sky = 21

	// Writing stage: writing to the three RAMs with WE=1
	*(volatile int*)(XPAR_VGA_IP_CORE_0_BASEADDR) = 1;     // we

	for( i=0 ; i<2048 ; i++ ){
	  	if( i < 1200 ){
	   		*(volatile int*)(XPAR_VGA_IP_CORE_0_BASEADDR+4) = i;        // add_bus1
	   		*(volatile int*)(XPAR_VGA_IP_CORE_0_BASEADDR+8) = i;        // add_bus2

	   		*(volatile int*)(XPAR_VGA_IP_CORE_0_BASEADDR+16) = 20;      // data_bus1
	   		*(volatile int*)(XPAR_VGA_IP_CORE_0_BASEADDR+20) = 21;      // data_bus2
	   	}
	   	// once: used for loading the sprites to hardware
	   	// only once, at the beginning of the program:
	  	if( once == 1 ){
	  		*(volatile int*)(XPAR_VGA_IP_CORE_0_BASEADDR+12) = i;           // add_bus3
	  		*(volatile int*)(XPAR_VGA_IP_CORE_0_BASEADDR+24) = sprites[i];  // data_bus3
	  	}
	}
	// Output stage: Reading from RAMs to display to VGA
	*(volatile int*)(XPAR_VGA_IP_CORE_0_BASEADDR) = 0;     // we
}

// Filling the background map:
void paintbgd( struct screen* x )
{
	int i, posx, posy, index;
	struct element *units;
	units = x->head;

	// Clearing the background tile map first:
	memset(&back_tile[0], CLEAR, 1200);   // Sky=21

	for(;;)
	{
		posx = units->x;
		posy = units->y;

		// Horizontal drawing:
		if( units->direction == 0 ){
			for( i=0 ; i<units->repeat ; i++ ){
				index= (posy*40)+(posx+i);
				back_tile[index] = units->sprite;
			}
		}
		// Vertical drawing:
		else if( units->direction == 1 ){
			for( i=0 ; i<units->repeat ; i++ ){
				index= ((posy+i)*40)+posx;
				back_tile[index] = units->sprite;
			}
		}
		// Load the next linked list unit, if it's not NULL:
		if( units->next == NULL )
			break;
		else
			units = units->next;
	}

	// Writing stage: writing to the background RAM with WE=1
	*(volatile int*)(XPAR_VGA_IP_CORE_0_BASEADDR) = 1;     // we

	for( i=0 ; i<1200 ; i++ ){
		*(volatile int*)(XPAR_VGA_IP_CORE_0_BASEADDR+8)  = i;             // add_bus2
		*(volatile int*)(XPAR_VGA_IP_CORE_0_BASEADDR+20) = back_tile[i];  // data_bus2
	}
	// Output stage: Reading from RAMs to display to VGA
	*(volatile int*)(XPAR_VGA_IP_CORE_0_BASEADDR) = 0;     // we
}

// Filling the foreground map:
void paintfgd( char clear)
{
	int i, indx;

	struct element *units;
	units = (struct element*)malloc(sizeof(struct element));
	units = smurf[0];

	// Clearing the foreground tile map, when there is a movement:
	if( clear == 1 )
		memset(&fore_tile[0], NOTHING, 1200);   // Nothing

	// Filling the foreground tile map.
	// And, the character current position:
	for(;;)
	{
		indx = index(units->y, units->x);
		fore_tile[indx] = units->sprite;   // Foreground tile map

		// Load the next linked list unit, if it's not NULL:
		if( units->next == NULL )
			break;
		else
			units = units->next;
	}

	// Writing stage: writing to the foreground RAM with WE=1
	*(volatile int*)(XPAR_VGA_IP_CORE_0_BASEADDR) = 1;     // we

	for( i=0 ; i<1200 ; i++ ){
		*(volatile int*)(XPAR_VGA_IP_CORE_0_BASEADDR+4)  = i;             // add_bus1
		*(volatile int*)(XPAR_VGA_IP_CORE_0_BASEADDR+16) = fore_tile[i];  // data_bus1
	}
	// Output stage: Reading from RAMs to display to VGA
	*(volatile int*)(XPAR_VGA_IP_CORE_0_BASEADDR) = 0;     // we
}

// To move the unit and draw it in the foreground tile map:
void move( struct element *units, char shiftx, char shifty ){
	for(;;)
	{
		units->x = units->x+shiftx;
		units->y = units->y+shifty;

		// Load the next linked list unit of the same type:
		if( units->endType == 1 )	break;
		else 						units = units->next;
	}
	// Draw what moved:
	paintfgd(1);
}

// This function responsible for the movements:
void takeActions( int buttons )
{
	int jmp_steps=0;   // counter for the steps of jump
	// Right Movement:
	if( buttons == RIGHT && back_tile[index(smurf[1]->y,smurf[1]->x)+1] != BLOCK
			&& back_tile[index(smurf[3]->y,smurf[3]->x)+1] != BLOCK
			&& back_tile[index(smurf[5]->y,smurf[5]->x)+1] != BLOCK
			&& back_tile[index(smurf[7]->y,smurf[7]->x)+1] != BLOCK
			&& back_tile[index(smurf[9]->y,smurf[9]->x)+1] != BLOCK )
	{
		// Update the location & Legs:
		smurf[8]->sprite = 12; smurf[9]->sprite = 13;
		move(smurf[0], 1, 0);
		smurf_dir = 2;
	}

	// Left Movement:
	if( buttons == LEFT && back_tile[index(smurf[0]->y,smurf[0]->x)-1] != BLOCK
			&& back_tile[index(smurf[2]->y,smurf[2]->x)-1] != BLOCK
			&& back_tile[index(smurf[4]->y,smurf[4]->x)-1] != BLOCK
			&& back_tile[index(smurf[6]->y,smurf[6]->x)-1] != BLOCK
			&& back_tile[index(smurf[8]->y,smurf[8]->x)-1] != BLOCK  )
	{
		// Update the location & Legs:
		smurf[8]->sprite = 10; smurf[9]->sprite = 11;
		move(smurf[0], -1, 0);
		smurf_dir = 1;
	}

	// Down Movement:
	if( buttons == DOWN ) 	{
		// On the top of the ladder:
		if ( back_tile[index(smurf[8]->y,smurf[8]->x)+40] == LADDER1 &&
				back_tile[index(smurf[9]->y,smurf[9]->x)+40] == LADDER2 )
	    {
			// Update the location & Legs:
			smurf[8]->sprite = 8; smurf[9]->sprite = 9;
			move(smurf[0], 0, 1);
	    }
		// On the middle/stop of the ladder:
		else if ( back_tile[index(smurf[0]->y,smurf[0]->x)] == LADDER1 &&
				back_tile[index(smurf[1]->y,smurf[1]->x)] == LADDER2 &&
				back_tile[index(smurf[8]->y,smurf[8]->x)+40] != BLOCK &&
				back_tile[index(smurf[9]->y,smurf[9]->x)+40] != BLOCK )
	   {
			// Update the location & Legs:
			smurf[8]->sprite = 8; smurf[9]->sprite = 9;
			move(smurf[0], 0, 1);
	   }
	}

	// Up Movement:
	if( buttons == UP  )  {
		// On the Bottom of the ladder:
		if( back_tile[index(smurf[8]->y,smurf[8]->x)] == LADDER1 &&
				back_tile[index(smurf[9]->y,smurf[9]->x)] == LADDER2 &&
				back_tile[index(smurf[0]->y,smurf[0]->x)-40] != BLOCK &&
				back_tile[index(smurf[1]->y,smurf[1]->x)-40] != BLOCK)
        {
			// Update the location & Legs:
			smurf[8]->sprite = 8; smurf[9]->sprite = 9;
			move(smurf[0], 0, -1);
        }
		// On the Middle/stop of the ladder:
		else if( back_tile[index(smurf[0]->y,smurf[0]->x)] == LADDER1 &&
				back_tile[index(smurf[1]->y,smurf[1]->x)] == LADDER2 )
	    {
			// Update the location & Legs:
			smurf[8]->sprite = 8; smurf[9]->sprite = 9;
			move(smurf[0], 0, -1);
	    }
	}

	// Up-Jumping Movement: (B Only)
	if( buttons == B )  {
		if( back_tile[index(smurf[0]->y,smurf[0]->x)-40] == CLEAR &&
				back_tile[index(smurf[1]->y,smurf[1]->x)-40] == CLEAR &&
				(back_tile[index(smurf[8]->y,smurf[8]->x)+40] == BLOCK ||
				back_tile[index(smurf[9]->y,smurf[9]->x)+40] == BLOCK) )
		{
			jmp_steps = 1;
			if( back_tile[index(smurf[0]->y,smurf[0]->x)-80] != BLOCK &&
					back_tile[index(smurf[1]->y,smurf[1]->x)-80] != BLOCK )
			{
				jmp_steps++;
				if( back_tile[index(smurf[0]->y,smurf[0]->x)-120] != BLOCK &&
						back_tile[index(smurf[1]->y,smurf[1]->x)-120] != BLOCK )
				{
					jmp_steps++;
					if( back_tile[index(smurf[0]->y,smurf[0]->x)-160] != BLOCK &&
							back_tile[index(smurf[1]->y,smurf[1]->x)-160] != BLOCK )
						jmp_steps++;
				}
			}
			// Update the location & Legs:
			smurf[8]->sprite = 14; smurf[9]->sprite = 15;
			move(smurf[0], 0, -jmp_steps); //for(i=0;i<7000;i++);
		}
	}

	// Right-Jumping Movement: (B+Right)
	if( buttons == B+RIGHT )  {
		if( back_tile[index(smurf[0]->y,smurf[0]->x)-39] == CLEAR &&
				back_tile[index(smurf[1]->y,smurf[1]->x)-39] == CLEAR &&
				(back_tile[index(smurf[8]->y,smurf[8]->x)+40] == BLOCK ||
				back_tile[index(smurf[9]->y,smurf[9]->x)+40] == BLOCK) )
		{
			jmp_steps = 1;
			if( back_tile[index(smurf[0]->y,smurf[0]->x)-78] == CLEAR &&
					back_tile[index(smurf[1]->y,smurf[1]->x)-78] == CLEAR )
			{
				jmp_steps++;
				if( back_tile[index(smurf[0]->y,smurf[0]->x)-117] == CLEAR &&
						back_tile[index(smurf[1]->y,smurf[1]->x)-117] == CLEAR )
				{
					jmp_steps++;
					if( back_tile[index(smurf[0]->y,smurf[0]->x)-156] == CLEAR &&
							back_tile[index(smurf[1]->y,smurf[1]->x)-156] == CLEAR ){
						jmp_steps++;
					}
				}
			}
			// Update the location:
			move(smurf[0], jmp_steps, -jmp_steps);
		}
		smurf_dir = 2;
	}

	// Left-Jumping Movement: (B+Left)
	if( buttons == B+LEFT )
	{
		if( back_tile[index(smurf[0]->y,smurf[0]->x)-41] == CLEAR &&
				back_tile[index(smurf[1]->y,smurf[1]->x)-41] == CLEAR &&
				(back_tile[index(smurf[8]->y,smurf[8]->x)+40] == BLOCK ||
				back_tile[index(smurf[9]->y,smurf[9]->x)+40] == BLOCK) )
		{
			jmp_steps = 1;
			if( back_tile[index(smurf[0]->y,smurf[0]->x)-82] == CLEAR &&
					back_tile[index(smurf[1]->y,smurf[1]->x)-82] == CLEAR )
			{
				jmp_steps++;
				if( back_tile[index(smurf[0]->y,smurf[0]->x)-123] == CLEAR &&
						back_tile[index(smurf[1]->y,smurf[1]->x)-123] == CLEAR )
				{
					jmp_steps++;
					if( back_tile[index(smurf[0]->y,smurf[0]->x)-164] == CLEAR &&
							back_tile[index(smurf[1]->y,smurf[1]->x)-164] == CLEAR )
						jmp_steps++;
				}
			}
			// Update the location:
			move(smurf[0], -jmp_steps, -jmp_steps);
		}
		smurf_dir = 1;
	}
}

// To zerofied the contents and link the units together:
void killList(void)
{
	constructor(berry, 0, 0, 0, 1, 1, 20, skull);         move(berry, 0, 0);
	constructor(skull, 0, 0, 0, 1, 1, 20, flames[0]);     move(skull, 0, 0);
	constructor(flames[0], 0, 0, 0, 1, 1, 20, flames[1]); move(flames[0], 0, 0);
	constructor(flames[1], 0, 0, 0, 1, 1, 20, NULL);      move(flames[1], 0, 0);
}

// To initialize and load different screens/levels:
void loadScreens(char screenNumber);

// The main program:
int main()
{
	init_platform();

	int i, buttons;

	// Memory allocations for the pointers:
	for( i=0 ; i<40 ; ++i){
		back_units[i] = (struct element*)malloc(sizeof(struct element));
		back_units[i]->next = NULL;
	}

	for( i=0 ; i<10 ; ++i){
		smurf[i] = (struct element*)malloc(sizeof(struct element));
		smurf[i]->endType = 0;
		smurf[i]->next = NULL;
	}

	for( i=0 ; i<2 ; ++i){
			ring[i]   = (struct element*)malloc(sizeof(struct element));  smurf[i]->next  = NULL;
			wing[i]   = (struct element*)malloc(sizeof(struct element));  wing[i]->next   = NULL;
			flames[i] = (struct element*)malloc(sizeof(struct element));  flames[i]->next = NULL;
	}

    berry = (struct element*)malloc(sizeof(struct element));   berry->next = NULL;
	skull = (struct element*)malloc(sizeof(struct element));	   skull->next = NULL;
	screen1  = (struct screen*) malloc(sizeof(struct screen));     screen1->next = NULL;

	// Protection Circuits:
	// Filling the sprites in the beginning:
	cleartiles(1);
	// Identifying the stop linked list:
	killList();
	// The starting screen:
	loadscreen = 1;

    for(;;){
    	// Delay Timer for the overall game:
    	for( i=0 ; i<15500 ; i++ );

    	// Checking the pressed buttons of NES Controller:
    	buttons = *(volatile int*)XPAR_NES_IP_CORE_0_BASEADDR;     // NES_buttons
    	*(volatile int*)(XPAR_LEDS_6BIT_BASEADDR) = buttons;

    	// ---------------------------------- <<< SCREENS >>> ------------------------------------------------ //
    	// Screen 1:
    	if( loadscreen == 1 && show == 1 )
    		loadScreens(loadscreen);
    	// Screen 2:
    	if( loadscreen == 2 && show == 1 )
    		loadScreens(loadscreen);
    	// Screen 3:
    	if( loadscreen == 3 && show == 1 )
    		loadScreens(loadscreen);
    	// Screen 4:
    	if( loadscreen == 4 && show == 1 )
    		loadScreens(loadscreen);
    	// Screen 5:
    	if( loadscreen == 5 && show == 1 )
    		loadScreens(loadscreen);
    	// Screen 6:
    	if( loadscreen == 6 && show == 1 )
    		loadScreens(loadscreen);
    	// Screen 7:
    	if( loadscreen == 7 && show == 1 )
    		loadScreens(loadscreen);

    	// -------------------------------- <<< MOVEMENTS >>> ---------------------------------------------- //
		// if key pressed:
    	if( buttons > NONE && enable == 1 )
    		takeActions(buttons);

    	// No Movement ( No key pressed):
    	if( (back_tile[index(smurf[8]->y,smurf[8]->x)+40] == BLOCK ||
    		 back_tile[index(smurf[9]->y,smurf[9]->x)+40] == BLOCK) && buttons == NONE )
    	{
    		// Update the location & Legs:
    		smurf[8]->sprite = 8; smurf[9]->sprite = 9;
    		move(smurf[0], 0, 0);
    	}

    	// Free-falling Movement:
    	if( back_tile[index(smurf[0]->y,smurf[0]->x)]    == CLEAR &&
    		back_tile[index(smurf[1]->y,smurf[1]->x)]    == CLEAR &&
    		back_tile[index(smurf[8]->y,smurf[8]->x)+40] == CLEAR &&
    		back_tile[index(smurf[9]->y,smurf[9]->x)+40] == CLEAR && enable == 1 )
    	{
    		// Update the location & Legs:
    		smurf[8]->sprite = 14; smurf[9]->sprite = 15;
    		move(smurf[0], 0, 1);
    	}

		// ------------------------------- <<< Strawberry >>> -------------------------------------------- //
    	// Throw Strawberries:
    	if( buttons >= A && enable == 1 && shoot == 1 )
    	{
    		// Get the smurf direction:
    		if( smurf_dir == 1 ){  // LEFT
    			berry->x=smurf[2]->x-1; berry->y=smurf[2]->y;
    		}
    		else{ // RIGHT: default
    			berry->x=smurf[3]->x+1; berry->y=smurf[3]->y;
    		}
    		berry->repeat=1; berry->sprite=BERRY;
    		move(berry, 0, 0);
    		shoot_dir = smurf_dir;

    		shoot = 0;
    	}

    	// Display the Shooted strawberry:
    	if( shoot == 0 && enable == 1 )
    	{
    		//constructor(unit, x, y, direction, repeat, endType, sprite, next)
    		// Destroy the Strawberry by the wall, fire or other berry:
    		if( (back_tile[index(berry->y,berry->x)+1] == BLOCK) || (back_tile[index(berry->y,berry->x)-1] == BLOCK) ||
    			(fore_tile[index(berry->y,berry->x)+1] == FIRE)  || (fore_tile[index(berry->y,berry->x)-1] == FIRE)  ||
    			(back_tile[index(berry->y,berry->x)+1] == BERRY) || (back_tile[index(berry->y,berry->x)-1] == BERRY) ) {
    			berry->sprite = 17;	move(berry, 0, 0);	for(i=0;i<7000;i++);    // Delay for explosion
    			berry->sprite = 20;	berry->x=0;berry->y=0;  move(berry, 0, 0);  shoot = 1;
    		}
    		// Continue displaying the Strawberries:
    		//else
    		{   
    			if( shoot_dir == 1 ) // LEFT
    				move(berry, -1, 0);
    			else if( shoot_dir == 2 ) // RIGHT: default
    				move(berry, 1, 0);
    		}
    	}

    	// ------------------------------- <<< Enemies >>> ---------------------------------------------- //
    	// Create the enemies:
    	if( enemy == 1 && enable == 1 ){
    		//constructor(unit, x, y, direction, repeat, endType, sprite, next)
    		if( loadscreen == 1 ) 	skull->sprite = ENEMY3;
    		if( loadscreen == 2 ) 	skull->sprite = ENEMY1;
    		if( loadscreen == 3 ) 	skull->sprite = ENEMY3;
    		if( loadscreen == 4 ) 	skull->sprite = ENEMY2;
    		if( loadscreen == 5 ) 	skull->sprite = ENEMY1;
    		if( loadscreen == 6 ) 	skull->sprite = ENEMY2;
    		move(skull, 0, 0);
    		enemy = 0;
    	}

    	// Continuously move/display the enemies:
    	if( enemy == 0 && enable == 1 ){
    		//constructor(unit, x, y, direction, repeat, endType, sprite, next)
    		// Change to right movement when facing a block on left side:
    		if( back_tile[index(skull->y, skull->x)-1] == BLOCK || skull->x == 0 )		   enemy_dir = 1;
    		// Change to left movement when facing a block on right side:
    		else if( back_tile[index(skull->y, skull->x)+1] == BLOCK || skull->x == 39 )   enemy_dir = -1;
    		// Change to down movement when facing a block on up side:
    		else if( back_tile[index(skull->y, skull->x)-40] == BLOCK || skull->y == 0 )   enemy_dir = 1;
    		// Change to up movement when facing a block on down side:
    		else if( back_tile[index(skull->y, skull->x)+40] == BLOCK || skull->y == 39)   enemy_dir = -1;
    		// type of Movement:
    		if( loadscreen == 1 ) 	move(skull, enemy_dir, 0);   		// Horizontal Movement
    		if( loadscreen == 2 ) 	move(skull, 0, enemy_dir);   		// Vertical Movement
    		if( loadscreen == 3 )	move(skull, 0, enemy_dir);   		// Vertical Movement
    		if( loadscreen == 4 ) 	move(skull, enemy_dir, 0);   		// Horizontal Movement
    		if( loadscreen == 5 ) 	move(skull, 0, enemy_dir);   		// Vertical Movement
    		if( loadscreen == 6 ) 	move(skull, enemy_dir, 0);   		// Horizontal Movement
    	}

    	// Destroy the enemies by smurf's strawberries:
    	//if( (berry->x == skull->x || berry->x+1 == skull->x) && (berry->y == skull->y) )       // on Geo info
    	//if( fore_tile[berry->y*40+berry->x] == 18 || fore_tile[(berry->y*40+berry->x)+1] == 18 ) // on Sprite info
    	if( (berry->x==skull->x || berry->x+1==skull->x || berry->x-1==skull->x) && berry->y==skull->y)
    	{
    		skull->sprite = 20; skull->x=0;skull->y=0; move(skull, 0, 0); enemy = 2;
    		berry->sprite = 17;	move(berry, 0, 0);	for(i=0;i<7000;i++);    // Delay for berry explosion
    		berry->sprite = 20;berry->x=2;berry->y=1;
			// Destroy the link between the Berry and the Skull. 
		    // And, assign new one between Berry and Flame:
			berry->next = flames[0];
    		move(berry, 0, 0);
    		shoot = 1;
    	}

    	// -------------------------------- <<< Flames >>> ----------------------------------------------- //
    	if( loadscreen == 3 ){       // Flames for Screen 3:
    		if( back_tile[index(flames[0]->y, flames[0]->x)-40] == BLOCK ){
    			flames[0]->x=11;flames[0]->y=15;  flames[1]->x=22;flames[1]->y=15;
    		}
    		move(flames[0], 0,-1);
    		move(flames[1], 0,-1);
    	}
    	else if( loadscreen == 4 ){       // Flames for Screen 4:
    		if( back_tile[index(flames[0]->y, flames[0]->x)-40] == BLOCK ){
    			flames[0]->x=13;flames[0]->y=28;  flames[1]->x=28;flames[1]->y=28;
    		}
    		move(flames[0], 0,-1);
    		move(flames[1], 0,-1);
    	}
    	else if( loadscreen == 5 ){       // Flames for Screen 5:
    		if( back_tile[index(flames[0]->y, flames[0]->x)-40] == BLOCK ) {flames[0]->x=24;flames[0]->y=16;}
    		if( back_tile[index(flames[1]->y, flames[1]->x)-40] == BLOCK ) {flames[1]->x=11;flames[1]->y=28;}
    		move(flames[0], 0,-1);
    		move(flames[1], 0,-1);
    	}
    	else if( loadscreen == 6 ){       // Flames for Screen 6:
    		if( back_tile[index(flames[0]->y, flames[0]->x)-40] == BLOCK ){
    			flames[0]->x=13;flames[0]->y=21;  flames[1]->x=28;flames[1]->y=21;
    		}
    		move(flames[0], 0,-1);
    		move(flames[1], 0,-1);
    	}

    	// --------------------------------- <<< MISC >>> ----------------------------------------------- //
    	// Die smurf die ( HAHAHAHAHAHA ):   //checking of sprites for flames:
    	if(( ( fore_tile[index(smurf[0]->y,smurf[0]->x)]    ==  FIRE || fore_tile[index(smurf[1]->y,smurf[1]->x)]    ==  FIRE ) ||
    		 ( fore_tile[index(smurf[0]->y,smurf[0]->x)+40] ==  FIRE || fore_tile[index(smurf[1]->y,smurf[1]->x)+40] ==  FIRE ) ||
    		 ( fore_tile[index(smurf[2]->y,smurf[2]->x)+40] ==  FIRE || fore_tile[index(smurf[3]->y,smurf[3]->x)+40] ==  FIRE ) ||
    		 ( fore_tile[index(smurf[4]->y,smurf[4]->x)+40] ==  FIRE || fore_tile[index(smurf[5]->y,smurf[5]->x)+40] ==  FIRE ) ||
    		 ( fore_tile[index(smurf[6]->y,smurf[6]->x)+40] ==  FIRE || fore_tile[index(smurf[7]->y,smurf[7]->x)+40] ==  FIRE ) ||
    		 ( fore_tile[index(smurf[8]->y,smurf[8]->x)+40] ==  FIRE || fore_tile[index(smurf[9]->y,smurf[9]->x)+40] ==  FIRE ) ||
    		 //checking of sprites for fire:
    		 ( back_tile[index(smurf[8]->y,smurf[8]->x)+40] ==  FIRE || back_tile[index(smurf[9]->y,smurf[9]->x)+40] ==  FIRE ) ||
    		 //checking of sprites for enemies:
    		 ( index(smurf[0]->y, smurf[0]->x)==index(skull->y,skull->x) || index(smurf[0]->y, smurf[0]->x)+1==index(skull->y,skull->x) ||
    		   index(smurf[2]->y, smurf[2]->x)==index(skull->y,skull->x) || index(smurf[2]->y, smurf[2]->x)+1==index(skull->y,skull->x) ||
    		   index(smurf[4]->y, smurf[4]->x)==index(skull->y,skull->x) || index(smurf[4]->y, smurf[4]->x)+1==index(skull->y,skull->x) ||
    		   index(smurf[6]->y, smurf[6]->x)==index(skull->y,skull->x) || index(smurf[6]->y, smurf[6]->x)+1==index(skull->y,skull->x) ||
    		   index(smurf[8]->y, smurf[8]->x)==index(skull->y,skull->x) || index(smurf[8]->y, smurf[8]->x)+1==index(skull->y,skull->x) ||
    		   index(smurf[1]->y, smurf[1]->x)==index(skull->y,skull->x) || index(smurf[1]->y, smurf[1]->x)-1==index(skull->y,skull->x) ||
    		   index(smurf[3]->y, smurf[3]->x)==index(skull->y,skull->x) || index(smurf[3]->y, smurf[3]->x)-1==index(skull->y,skull->x) ||
    		   index(smurf[5]->y, smurf[5]->x)==index(skull->y,skull->x) || index(smurf[5]->y, smurf[5]->x)-1==index(skull->y,skull->x) ||
    		   index(smurf[7]->y, smurf[7]->x)==index(skull->y,skull->x) || index(smurf[7]->y, smurf[7]->x)-1==index(skull->y,skull->x) ||
    		   index(smurf[9]->y, smurf[9]->x)==index(skull->y,skull->x) || index(smurf[9]->y, smurf[9]->x)-1==index(skull->y,skull->x) ))
    		   && enable == 1 )
    	{
    		// Update the smurf with the ring and wings:
    		smurf[8]->sprite = 8; smurf[9]->sprite = 9; move(smurf[0],0,0);
    		// The ring:
    		constructor(ring[0], smurf[0]->x, smurf[0]->y, 1, 1, 0, 27, ring[1]);
    		constructor(ring[1], smurf[1]->x, smurf[1]->y, 1, 1, 1, 28, wing[0]);
    		move(ring[0], 0, -1);
    		// The wings:
    		constructor(wing[0], smurf[2]->x, smurf[2]->y, 1, 1, 1, 29, wing[1]);
    		move(wing[0], -1, 0);
    		constructor(wing[1], smurf[3]->x, smurf[3]->y, 1, 1, 1, 30, berry);
    		move(wing[1], 1, 0);

    		killList();
    		shoot = 1;
    		enemy = 1;
    		enable = 0;
    	}

    	// START Button (Go to the first level):
    	if( buttons == START ){
    		show = 1;
    		loadscreen = 1;
    	}

    	// Next Screen:
    	if( smurf[0]->y == 0 || smurf[1]->x == 39 ){
    		show = 1;
    		loadscreen++;
    	}

    	// Select Button (Repeat the same level):
    	if( buttons == SELECT ){
    		show = 1;
    	}

    	// Win Indication, End of Game: (Go to the final screen):
    	if( loadscreen == 6 && enable == 1 && smurf[9]->x == 5 && smurf[9]->y == 6 ){
    		smurf[9]->x=0; smurf[9]->y=0;
    		show = 1;
    		loadscreen++;
    	}
    }   // <<== for(;;)

    cleanup_platform();

    return 0;
}


// To initialize and load different screens/levels:
void loadScreens( char screenNumber ){
	// Screen 1:
	if( screenNumber == 1 ){
		cleartiles(0);
		killList();
		//constructor(unit, x, y, direction, repeat, endType, sprite, next)
		// Drawing the background:
		// Upper Ladder:
		constructor(back_units[0], 35, 0, 1, 8, 0, 24, back_units[1]);     constructor(back_units[1], 36, 0, 1, 8, 0, 25, back_units[2]);
		// Upper Blocks:
		constructor(back_units[2], 32, 8, 0, 8, 0, 23, back_units[3]);     constructor(back_units[3], 32, 9, 0, 8, 0, 23, back_units[4]);
		constructor(back_units[4], 29, 10, 0, 11, 0, 23, back_units[5]);
		// Middle Blocks:
		constructor(back_units[5], 0, 15, 0, 24, 0, 23, back_units[6]);    constructor(back_units[6], 0, 16, 0, 24, 0, 23, back_units[7]);
		constructor(back_units[7], 0, 17, 0, 24, 0, 23, back_units[8]);
		// Lower Ladder:
		constructor(back_units[8], 10, 15, 1, 11, 0, 24, back_units[9]);   constructor(back_units[9], 11, 15, 1, 11, 0, 25, back_units[10]);
		// Lower Blocks_Left:
		constructor(back_units[10], 0, 26, 0, 19, 0, 23, back_units[11]);  constructor(back_units[11], 0, 27, 0, 19, 0, 23, back_units[12]);
		constructor(back_units[12], 0, 28, 0, 19, 0, 23, back_units[13]);  constructor(back_units[13], 0, 29, 0, 19, 0, 23, back_units[14]);
		// Lower Blocks_Right:
		constructor(back_units[14], 22, 26, 0, 18, 0, 23, back_units[15]); constructor(back_units[15], 22, 27, 0, 18, 0, 23, back_units[16]);
		constructor(back_units[16], 22, 28, 0, 18, 0, 23, back_units[17]); constructor(back_units[17], 22, 29, 0, 18, 0, 23, back_units[18]);
		// Fire:
		constructor(back_units[18], 19, 29, 0, 3, 0, 26, back_units[19]);
		// Walls:
		constructor(back_units[19], 0, 0, 1, 26, 0, 23, back_units[20]);   constructor(back_units[20], 39, 0, 1, 26, 0, 23, back_units[21]);
		// Extra Units:
		constructor(back_units[21], 29, 11, 0, 11, 0, 23, back_units[22]); constructor(back_units[22], 26, 12, 0, 14, 0, 23, back_units[23]);
		constructor(back_units[23], 26, 13, 0, 14, 0, 23, NULL);
		// DUMMY BLOCKS:
		constructor(back_units[24], 0, 0, 0, 0, 0, 20, NULL);
		constructor(back_units[25], 0, 0, 0, 0, 0, 20, NULL);

		screen1->head = back_units[0];
		paintbgd(screen1);

		// Drawing the foreground:
		constructor(smurf[0], 2, 21, 1, 1, 0, 0, smurf[1]);    constructor(smurf[1], 3, 21, 1, 1, 0, 1, smurf[2]);
		constructor(smurf[2], 2, 22, 1, 1, 0, 2, smurf[3]);    constructor(smurf[3], 3, 22, 1, 1, 0, 3, smurf[4]);
		constructor(smurf[4], 2, 23, 1, 1, 0, 4, smurf[5]);    constructor(smurf[5], 3, 23, 1, 1, 0, 5, smurf[6]);
		constructor(smurf[6], 2, 24, 1, 1, 0, 6, smurf[7]);    constructor(smurf[7], 3, 24, 1, 1, 0, 7, smurf[8]);
		constructor(smurf[8], 2, 25, 1, 1, 0, 8, smurf[9]);    constructor(smurf[9], 3, 25, 1, 1, 1, 9, ring[0]);

		constructor(ring[0], 0, 0, 0, 0, 0, 20, ring[1]);	    constructor(ring[1], 0, 0, 0, 0, 1, 20, wing[0]);
		constructor(wing[0], 0, 0, 0, 0, 0, 20, wing[1]);	    constructor(wing[1], 0, 0, 0, 0, 1, 20, berry);

		constructor(berry, 0, 0, 0, 0, 1, 20, skull);     	    constructor(skull, 20, 11, 0, 1, 1, 20, flames[0]);
		constructor(flames[0], 0, 0, 0, 1, 1, 20, flames[1]); constructor(flames[1], 0, 0, 0, 1, 1, 20, NULL);

		paintfgd(1);
		move(smurf[0], 0, 0);
		show = 0;
		shoot_dir = 0;
		enable = 1;
		shoot = 1;
		enemy = 1;
	}
	//Screen 2:
	if( screenNumber == 2 ){
		cleartiles(0);
		killList();
		//constructor(unit, x, y, direction, repeat, sprite, next)
		// Drawing the background:
		// The walls:
		constructor(back_units[0], 0, 0, 0, 40, 0, 23, back_units[1]);     constructor(back_units[1], 0, 0, 1, 40, 0, 23, back_units[2]);
		constructor(back_units[2], 39, 0, 1, 40, 0, 23, back_units[3]);    constructor(back_units[3], 0, 29, 0, 40, 0, 23, back_units[4]);
		// The Upper ladders:
		constructor(back_units[4], 2, 0, 1, 7, 0, 24, back_units[5]);      constructor(back_units[5], 3, 0, 1, 7, 0, 25, back_units[6]);
		constructor(back_units[6], 4, 5, 1, 7, 0, 24, back_units[7]);      constructor(back_units[7], 5, 5, 1, 7, 0, 25, back_units[8]);
		// The Middle Ladder:
		constructor(back_units[8], 37, 6, 1, 11, 0, 24, back_units[9]);    constructor(back_units[9], 38, 6, 1, 11, 0, 25, back_units[10]);
		// The flying blocks:
		constructor(back_units[10], 18, 6, 0, 19, 0, 23, back_units[11]);  constructor(back_units[11], 8, 9, 0, 9, 0, 23, back_units[12]);
		constructor(back_units[12], 27, 15, 0, 8, 0, 23, back_units[13]);  constructor(back_units[13], 16, 17, 0, 9, 0, 23, back_units[14]);
		constructor(back_units[14], 3, 19, 0, 11, 0, 23, back_units[15]);
		// The Lower ladder
		constructor(back_units[15], 1, 19, 1, 10, 0, 24, back_units[16]);	constructor(back_units[16], 2, 19, 1, 10, 0, 25, back_units[17]);
		// The other blocks:
		constructor(back_units[17], 15, 25, 1, 4, 0, 23, back_units[18]);	constructor(back_units[18], 16, 25, 1, 4, 0, 23, back_units[19]);
		constructor(back_units[19], 20, 25, 0, 3, 0, 21, back_units[20]);	constructor(back_units[20], 20, 26, 0, 5, 0, 23, back_units[21]);
		constructor(back_units[21], 20, 27, 0, 7, 0, 23, back_units[22]);   constructor(back_units[22], 20, 28, 0, 9, 0, 23, back_units[23]);
		// The income ladder
		constructor(back_units[23], 35, 29, 0, 1, 0, 24, back_units[24]);   constructor(back_units[24], 36, 29, 0, 1, 0, 25, back_units[25]);
		// Fire:
		constructor(back_units[25], 17, 29, 0, 3, 0, 26, NULL);

		screen1->head = back_units[0];
		paintbgd(screen1);

		// Drawing the foreground:
		constructor(smurf[0], 35, 24, 1, 1, 0, 0, smurf[1]);    constructor(smurf[1], 36, 24, 1, 1, 0, 1, smurf[2]);
		constructor(smurf[2], 35, 25, 1, 1, 0, 2, smurf[3]);    constructor(smurf[3], 36, 25, 1, 1, 0, 3, smurf[4]);
		constructor(smurf[4], 35, 26, 1, 1, 0, 4, smurf[5]);    constructor(smurf[5], 36, 26, 1, 1, 0, 5, smurf[6]);
		constructor(smurf[6], 35, 27, 1, 1, 0, 6, smurf[7]);    constructor(smurf[7], 36, 27, 1, 1, 0, 7, smurf[8]);
		constructor(smurf[8], 35, 28, 1, 1, 0, 8, smurf[9]);    constructor(smurf[9], 36, 28, 1, 1, 1, 9, ring[0]);

		constructor(ring[0], 0, 0, 0, 0, 0, 20, ring[1]);	    constructor(ring[1], 0, 0, 0, 0, 1, 20, wing[0]);
		constructor(wing[0], 0, 0, 0, 0, 0, 20, wing[1]);	    constructor(wing[1], 0, 0, 0, 0, 1, 20, berry);

		constructor(berry, 0, 0, 0, 0, 1, 20, skull);     	    constructor(skull, 20, 10, 0, 1, 1, 20, flames[0]);
		constructor(flames[0], 0, 0, 0, 1, 1, 20, flames[1]); constructor(flames[1], 0, 0, 0, 1, 1, 20, NULL);

		paintfgd(1);
		move(smurf[0], 0, 0);
		show = 0;
		shoot_dir = 0;
		enable = 1;
		shoot = 1;
		enemy = 1;
	}
	//Screen 3:
	if( screenNumber == 3 ){
		cleartiles(0);
		killList();
		//constructor(unit, x, y, direction, repeat, sprite, next)
		// Drawing the background:
		// The walls:
		constructor(back_units[0], 0, 0, 0, 40, 0, 23, back_units[1]);      constructor(back_units[1], 0, 1, 1, 39, 0, 23, back_units[2]);
		constructor(back_units[2], 1, 26, 0, 39, 0, 23, back_units[3]);     constructor(back_units[3], 1, 27, 0, 39, 0, 23, back_units[4]);
		constructor(back_units[4], 1, 28, 0, 39, 0, 23, back_units[5]);     constructor(back_units[5], 1, 29, 0, 39, 0, 23, back_units[6]);
		// The Ladders:
		constructor(back_units[6], 3, 26, 1, 4, 0, 24, back_units[7]);      constructor(back_units[7], 4, 26, 1, 4, 0, 25, back_units[8]);
		constructor(back_units[8], 37, 16, 1, 10, 0, 24, back_units[9]);    constructor(back_units[9], 38, 16, 1, 10, 0, 25, back_units[10]);
		// The Middle blocks:
		constructor(back_units[10], 24, 16, 0, 13, 0, 23, back_units[11]);  constructor(back_units[11], 13, 15, 0, 8, 0, 23, back_units[12]);
		constructor(back_units[12], 10, 17, 0, 15, 0, 23, back_units[13]);  constructor(back_units[13], 9, 15, 1, 3, 0, 23, back_units[14]);
		constructor(back_units[14], 1, 15, 0, 9, 0, 23, back_units[15]);
		// The Upper ladder
		constructor(back_units[15], 1, 6, 1, 9, 0, 24, back_units[16]);	    constructor(back_units[16], 2,6, 1, 9, 0, 25, back_units[17]);
		// The other blocks:
		constructor(back_units[17], 3, 6, 0, 8, 0, 23, back_units[18]);	    constructor(back_units[18], 12, 6, 0, 10, 0, 23, back_units[19]);
		constructor(back_units[19], 23, 6, 0, 17, 0, 23, back_units[20]);	constructor(back_units[20], 39, 7, 1, 21, 0, 23, back_units[21]);
		// Fire:
		constructor(back_units[21], 10, 16, 0, 14, 0, 26, NULL);
		// DUMMY BLOCKS:
		constructor(back_units[22], 0, 0, 0, 0, 0, 20, NULL);
		constructor(back_units[23], 0, 0, 0, 0, 0, 20, NULL);
		constructor(back_units[24], 0, 0, 0, 0, 0, 20, NULL);
		constructor(back_units[25], 0, 0, 0, 0, 0, 20, NULL);

		screen1->head = back_units[0];
		paintbgd(screen1);

		// Drawing the foreground:
		constructor(smurf[0], 3, 21, 1, 1, 0, 0, smurf[1]);    constructor(smurf[1], 4, 21, 1, 1, 0, 1, smurf[2]);
		constructor(smurf[2], 3, 22, 1, 1, 0, 2, smurf[3]);    constructor(smurf[3], 4, 22, 1, 1, 0, 3, smurf[4]);
		constructor(smurf[4], 3, 23, 1, 1, 0, 4, smurf[5]);    constructor(smurf[5], 4, 23, 1, 1, 0, 5, smurf[6]);
		constructor(smurf[6], 3, 24, 1, 1, 0, 6, smurf[7]);    constructor(smurf[7], 4, 24, 1, 1, 0, 7, smurf[8]);
		constructor(smurf[8], 3, 25, 1, 1, 0, 8, smurf[9]);    constructor(smurf[9], 4, 25, 1, 1, 1, 9, ring[0]);

		constructor(ring[0], 0, 0, 0, 0, 0, 20, ring[1]);	   constructor(ring[1], 0, 0, 0, 0, 1, 20, wing[0]);
		constructor(wing[0], 0, 0, 0, 0, 0, 20, wing[1]);	   constructor(wing[1], 0, 0, 0, 0, 1, 20, berry);

		constructor(berry, 0, 0, 0, 0, 1, 20, skull);     	   constructor(skull, 8, 8, 0, 1, 1, 20, flames[0]);
		constructor(flames[0], 11, 15, 0, 1, 1, 26, flames[1]);  constructor(flames[1], 22, 15, 0, 1, 1, 26, NULL);

		paintfgd(1);
		move(smurf[0], 0, 0);
		show = 0;
		shoot_dir = 0;
		enable = 1;
		shoot = 1;
		enemy = 1;
	}
	//Screen 4:
	if( screenNumber == 4 ){
		cleartiles(0);
		killList();
		//constructor(unit, x, y, direction, repeat, sprite, next)
		// Drawing the background:
		// The walls:
		constructor(back_units[0], 0, 0, 0, 37, 0, 23, back_units[1]);      constructor(back_units[1], 0, 1, 1, 15, 0, 23, back_units[2]);
		constructor(back_units[2], 0, 15, 0, 5, 0, 23, back_units[3]);      constructor(back_units[3], 0, 16, 0, 9, 0, 23, back_units[4]);
		constructor(back_units[4], 0, 17, 0, 13, 0, 23, back_units[5]);     constructor(back_units[5], 0, 18, 0, 13, 0, 23, back_units[6]);
		// Fire:
		constructor(back_units[6], 12, 29, 0, 3, 0, 26, back_units[7]);     constructor(back_units[7], 27, 29, 0, 3, 0, 26, back_units[8]);

		constructor(back_units[8], 0, 28, 0, 12, 0, 23, back_units[9]);     constructor(back_units[9], 0, 29, 0, 12, 0, 23, back_units[10]);
		constructor(back_units[10], 15, 28, 0, 12, 0, 23, back_units[11]);  constructor(back_units[11], 15, 29, 0, 12, 0, 23, back_units[12]);
		constructor(back_units[12], 30, 28, 0, 10, 0, 23, back_units[13]);  constructor(back_units[13], 30, 29, 0, 10, 0, 23, back_units[14]);
		// The Lower ladder:
		constructor(back_units[14], 37, 18, 1, 10, 0, 24, back_units[15]);  constructor(back_units[15], 38, 18, 1, 10, 0, 25, back_units[16]);
		// The other blocks:
		constructor(back_units[16], 29,18, 0, 8, 0, 23, back_units[17]);	constructor(back_units[17], 14, 18, 0, 14, 0, 23, back_units[18]);
		// The Floating Ladder:
		constructor(back_units[18], 1, 3, 1, 5, 0, 24, back_units[19]); 	constructor(back_units[19], 2, 3, 1, 5, 0, 25, back_units[20]);
		// The other blocks:
		constructor(back_units[20], 3, 6, 0, 10, 0, 23, back_units[21]);    constructor(back_units[21], 14, 6, 0, 14, 0, 23, back_units[22]);
		constructor(back_units[22], 29, 6, 0, 10, 0, 23, back_units[23]);
		// The final ladder:
		constructor(back_units[23], 37, 0, 1, 6, 0, 24, back_units[24]);    constructor(back_units[24], 38, 0, 1, 6, 0, 25, back_units[25]);
		// The other blocks:
		constructor(back_units[25], 39, 0, 1, 28, 0, 23, NULL);

		screen1->head = back_units[0];
		paintbgd(screen1);

		// Drawing the foreground:
		constructor(smurf[0], 3, 23, 1, 1, 0, 0, smurf[1]);      constructor(smurf[1], 4, 23, 1, 1, 0, 1, smurf[2]);
		constructor(smurf[2], 3, 24, 1, 1, 0, 2, smurf[3]);      constructor(smurf[3], 4, 24, 1, 1, 0, 3, smurf[4]);
		constructor(smurf[4], 3, 25, 1, 1, 0, 4, smurf[5]);      constructor(smurf[5], 4, 25, 1, 1, 0, 5, smurf[6]);
		constructor(smurf[6], 3, 26, 1, 1, 0, 6, smurf[7]);      constructor(smurf[7], 4, 26, 1, 1, 0, 7, smurf[8]);
		constructor(smurf[8], 3, 27, 1, 1, 0, 8, smurf[9]);      constructor(smurf[9], 4, 27, 1, 1, 1, 9, ring[0]);

		constructor(ring[0], 0, 0, 0, 0, 0, 20, ring[1]);	     constructor(ring[1], 0, 0, 0, 0, 1, 20, wing[0]);
		constructor(wing[0], 0, 0, 0, 0, 0, 20, wing[1]);	     constructor(wing[1], 0, 0, 0, 0, 1, 20, berry);

		constructor(berry, 0, 0, 0, 0, 1, 20, skull);     	     constructor(skull, 26, 12, 0, 1, 1, 20, flames[0]);
		constructor(flames[0], 13, 28, 0, 1, 1, 26, flames[1]);  constructor(flames[1], 28, 28, 0, 1, 1, 26, NULL);

		paintfgd(1);
		move(smurf[0], 0, 0);
		show = 0;
		shoot_dir = 0;
		enable = 1;
		shoot = 1;
		enemy = 1;
	}
	//Screen 5:
	if( screenNumber == 5 ){
		cleartiles(0);
		killList();
		//constructor(unit, x, y, direction, repeat, sprite, next)
		// Drawing the background:
		// The walls:
		constructor(back_units[0], 0, 0, 0, 40, 0, 23, back_units[1]);      constructor(back_units[1], 0, 1, 1, 39, 0, 23, back_units[2]);
		constructor(back_units[2], 1, 28, 0, 9, 0, 23, back_units[3]);      constructor(back_units[3], 1, 29, 0, 9, 0, 23, back_units[4]);
		constructor(back_units[4], 13, 28, 0, 24, 0, 23, back_units[5]);    constructor(back_units[5], 13, 29, 0, 24, 0, 23, back_units[6]);
		// The Ladders:
		constructor(back_units[6], 37, 28, 1, 2, 0, 24, back_units[7]);     constructor(back_units[7], 38, 28, 1, 2, 0, 25, back_units[8]);
		constructor(back_units[8], 1, 6, 1, 22, 0, 24, back_units[9]);      constructor(back_units[9], 2, 6, 1, 22, 0, 25, back_units[10]);
		// The Middle blocks:
		constructor(back_units[10], 3, 17, 0, 8, 0, 23, back_units[11]);    constructor(back_units[11], 3, 18, 0, 8, 0, 23, back_units[12]);
		constructor(back_units[12], 12, 17, 0, 27, 0, 23, back_units[13]);  constructor(back_units[13], 12, 18, 0, 27, 0, 23, back_units[14]);
		constructor(back_units[14], 3, 6, 0, 8, 0, 23, back_units[15]);
		// The Middle ladder:
		constructor(back_units[15], 30, 6, 1, 11, 0, 24, back_units[16]);	constructor(back_units[16], 31,6, 1, 11, 0, 25, back_units[17]);
		// The other blocks:
		constructor(back_units[17], 10, 7, 1, 10, 0, 23, back_units[18]);	constructor(back_units[18], 16, 1, 1, 6, 0, 23, back_units[19]);
		constructor(back_units[19], 16, 6, 0, 8, 0, 23, back_units[20]);	constructor(back_units[20], 25, 6, 0, 5, 0, 23, back_units[21]);
		// Fire:
		constructor(back_units[21], 23, 17, 0, 3, 0, 26, back_units[22]);   constructor(back_units[22], 10, 29, 0, 3, 0, 26, back_units[23]);
		// The income ladder
		constructor(back_units[23], 32, 6, 0, 7, 0, 23, back_units[24]);    constructor(back_units[24], 39, 6, 1, 24, 0, 23, NULL);
		// DUMMY BLOCKS:
		constructor(back_units[25], 0, 0, 0, 0, 0, 20, NULL);

		screen1->head = back_units[0];
		paintbgd(screen1);

		// Drawing the foreground:
		constructor(smurf[0], 37, 23, 1, 1, 0, 0, smurf[1]);    constructor(smurf[1], 38, 23, 1, 1, 0, 1, smurf[2]);
		constructor(smurf[2], 37, 24, 1, 1, 0, 2, smurf[3]);    constructor(smurf[3], 38, 24, 1, 1, 0, 3, smurf[4]);
		constructor(smurf[4], 37, 25, 1, 1, 0, 4, smurf[5]);    constructor(smurf[5], 38, 25, 1, 1, 0, 5, smurf[6]);
		constructor(smurf[6], 37, 26, 1, 1, 0, 6, smurf[7]);    constructor(smurf[7], 38, 26, 1, 1, 0, 7, smurf[8]);
		constructor(smurf[8], 37, 27, 1, 1, 0, 8, smurf[9]);    constructor(smurf[9], 38, 27, 1, 1, 1, 9, ring[0]);

		constructor(ring[0], 0, 0, 0, 0, 0, 20, ring[1]);	    constructor(ring[1], 0, 0, 0, 0, 1, 20, wing[0]);
		constructor(wing[0], 0, 0, 0, 0, 0, 20, wing[1]);	    constructor(wing[1], 0, 0, 0, 0, 1, 20, berry);

		constructor(berry, 0, 0, 0, 0, 1, 20, skull);     	    constructor(skull, 13, 5, 0, 1, 1, 20, flames[0]);
		constructor(flames[0], 24, 16, 0, 1, 1, 26, flames[1]); constructor(flames[1], 11, 28, 0, 1, 1, 26, NULL);

		paintfgd(1);
		move(smurf[0], 0, 0);
		show = 0;
		shoot_dir = 0;
		enable = 1;
		shoot = 1;
		enemy = 1;
	}
	//Screen 6:
	if( screenNumber == 6 ){
		cleartiles(0);
		killList();
		//constructor(unit, x, y, direction, repeat, sprite, next)
		// Drawing the background:
		// The walls:
		constructor(back_units[0], 0, 0, 0, 40, 0, 23, back_units[1]);      constructor(back_units[1], 0, 1, 1, 23, 0, 23, back_units[2]);
		constructor(back_units[2], 1, 7, 0, 12, 0, 23, back_units[3]);      constructor(back_units[3], 14, 7, 0, 14, 0, 23, back_units[4]);
		constructor(back_units[4], 29, 7, 0, 8, 0, 23, back_units[5]);      constructor(back_units[5], 3, 13, 0, 10, 0, 23, back_units[6]);
		// The Ladders:
		constructor(back_units[6], 1, 13, 1, 9, 0, 24, back_units[7]);      constructor(back_units[7], 2, 13, 1, 9, 0, 25, back_units[8]);
		constructor(back_units[8], 37, 7, 1, 6, 0, 24, back_units[9]);      constructor(back_units[9], 38, 7, 1, 6, 0, 25, back_units[10]);
		// The Middle blocks:
		constructor(back_units[10], 14, 13, 0, 14, 0, 23, back_units[11]);  constructor(back_units[11], 29, 13, 0, 10, 0, 23, back_units[12]);
		constructor(back_units[12], 1, 22, 0, 36, 0, 23, back_units[13]);   constructor(back_units[13], 1, 23, 0, 36, 0, 23, back_units[14]);
		constructor(back_units[14], 0, 29, 0, 40, 0, 23, back_units[15]);
		// The Lower ladder:
		constructor(back_units[15], 37, 22, 1, 7, 0, 24, back_units[16]);	    constructor(back_units[16], 38,22, 1, 7, 0, 25, back_units[17]);
		// The other blocks:
		constructor(back_units[17], 5, 2, 0, 1, 0, 16, back_units[18]);	    constructor(back_units[18], 4, 3, 0, 3, 0, 16, back_units[19]);
		constructor(back_units[19], 3, 4, 0, 5, 0, 16, back_units[20]);	    constructor(back_units[20], 2, 5, 0, 7, 0, 16, back_units[21]);
		// Fire:
		constructor(back_units[21], 12, 22, 0, 3, 0, 26, back_units[22]); 	constructor(back_units[22], 27, 22, 0, 3, 0, 26, back_units[23]);
		//
		constructor(back_units[23], 1, 6, 0, 9, 0, 16, back_units[24]);
		constructor(back_units[24], 39, 1, 1, 40, 0, 23, NULL);
		// DUMMY BLOCKS:
		constructor(back_units[25], 0, 0, 0, 0, 0, 20, NULL);

		screen1->head = back_units[0];
		paintbgd(screen1);

		// Drawing the foreground:
		constructor(smurf[0], 1, 24, 1, 1, 0, 0, smurf[1]);    constructor(smurf[1], 2, 24, 1, 1, 0, 1, smurf[2]);
		constructor(smurf[2], 1, 25, 1, 1, 0, 2, smurf[3]);    constructor(smurf[3], 2, 25, 1, 1, 0, 3, smurf[4]);
		constructor(smurf[4], 1, 26, 1, 1, 0, 4, smurf[5]);    constructor(smurf[5], 2, 26, 1, 1, 0, 5, smurf[6]);
		constructor(smurf[6], 1, 27, 1, 1, 0, 6, smurf[7]);    constructor(smurf[7], 2, 27, 1, 1, 0, 7, smurf[8]);
		constructor(smurf[8], 1, 28, 1, 1, 0, 8, smurf[9]);    constructor(smurf[9], 2, 28, 1, 1, 1, 9, ring[0]);

		constructor(ring[0], 0, 0, 0, 0, 0, 20, ring[1]);	   constructor(ring[1], 0, 0, 0, 0, 1, 20, wing[0]);
		constructor(wing[0], 0, 0, 0, 0, 0, 20, wing[1]);	   constructor(wing[1], 0, 0, 0, 0, 1, 20, berry);

		constructor(berry, 0, 0, 0, 0, 1, 20, skull);     	   constructor(skull, 20, 9, 0, 1, 1, 20, flames[0]);
		constructor(flames[0], 13, 21, 0, 1, 1, 26, flames[1]);  constructor(flames[1], 28, 21, 0, 1, 1, 26, NULL);

		paintfgd(1);
		move(smurf[0], 0, 0);
		show = 0;
		shoot_dir = 0;
		enable = 1;
		shoot = 1;
		enemy = 1;
	}

	//Screen 7: (WINNING SCREEN)
	if( screenNumber == 7 ){
		cleartiles(0);
		killList();
		//constructor(unit, x, y, direction, repeat, sprite, next)
		// Drawing NOTHING to the foreground:
		constructor(smurf[0], 1, 24, 1, 1, 0, 20, smurf[1]);    constructor(smurf[1], 2, 24, 1, 1, 0, 20, smurf[2]);
		constructor(smurf[2], 1, 25, 1, 1, 0, 20, smurf[3]);    constructor(smurf[3], 2, 25, 1, 1, 0, 20, smurf[4]);
		constructor(smurf[4], 1, 26, 1, 1, 0, 20, smurf[5]);    constructor(smurf[5], 2, 26, 1, 1, 0, 20, smurf[6]);
		constructor(smurf[6], 1, 27, 1, 1, 0, 20, smurf[7]);    constructor(smurf[7], 2, 27, 1, 1, 0, 20, smurf[8]);
		constructor(smurf[8], 1, 28, 1, 1, 0, 20, smurf[9]);    constructor(smurf[9], 2, 28, 1, 1, 1, 20, ring[0]);

		constructor(ring[0], 0, 0, 0, 0, 0, 20, ring[1]);	   constructor(ring[1], 0, 0, 0, 0, 1, 20, wing[0]);
		constructor(wing[0], 0, 0, 0, 0, 0, 20, wing[1]);	   constructor(wing[1], 0, 0, 0, 0, 1, 20, berry);

		constructor(berry, 2, 2, 0, 0, 1, 20, skull);     	   constructor(skull, 20, 9, 0, 1, 1, 20, flames[0]);
		constructor(flames[0], 13, 21, 0, 1, 1, 20, flames[1]);  constructor(flames[1], 28, 21, 0, 1, 1, 20, NULL);
		paintfgd(1);
		move(smurf[0], 0, 0);

		// Drawing the background:
		// The walls:
		constructor(back_units[0], 0, 0, 0, 40, 0, 26, back_units[1]);      constructor(back_units[1], 0, 1, 1, 39, 0, 26, back_units[2]);
		constructor(back_units[2], 39, 1, 1, 39, 0, 26, back_units[3]);     constructor(back_units[3], 1, 29, 0, 38, 0, 26, back_units[4]);
		// W:
		constructor(back_units[4], 3, 6, 1, 8, 0, 23, back_units[5]);      constructor(back_units[5], 4, 12, 0, 1, 0, 23, back_units[6]);
		constructor(back_units[6], 5, 11, 1, 1, 0, 23, back_units[7]);      constructor(back_units[7], 6, 12, 1, 1, 0, 23, back_units[8]);
		constructor(back_units[8], 7, 6, 1, 8, 0, 23, back_units[9]);
		// i:
		constructor(back_units[9], 9, 7, 1, 1, 0, 23, back_units[10]);    constructor(back_units[10], 9, 9, 1, 5, 0, 23, back_units[11]);
		// n:
		constructor(back_units[11], 11, 9, 1, 5, 0, 23, back_units[12]);   constructor(back_units[12], 12, 9, 0, 2, 0, 23, back_units[13]);
		constructor(back_units[13], 14, 10, 1, 4, 0, 23, back_units[14]);
		// n:
		constructor(back_units[14], 16, 9, 1, 5, 0, 23, back_units[15]);  constructor(back_units[15], 17, 9, 0, 2, 0, 23, back_units[16]);
		constructor(back_units[16], 19, 10, 1, 4, 0, 23, back_units[17]);
		// e:
		constructor(back_units[17], 21, 9, 1, 5, 0, 23, back_units[18]);	constructor(back_units[18], 22, 9, 0, 1, 0, 23, back_units[19]);
		constructor(back_units[19], 23, 9, 1, 3, 0, 23, back_units[20]);	constructor(back_units[20], 22, 11, 0, 1, 0, 23, back_units[21]);
		constructor(back_units[21], 21, 13, 0, 4, 0, 23, back_units[22]);
		// r:
		constructor(back_units[22], 26, 9, 1, 5, 0, 23, back_units[23]);  constructor(back_units[23], 27, 10, 0, 1, 0, 23, back_units[24]);
		constructor(back_units[24], 28, 9, 0, 2, 0, 23, back_units[25]);
		// !:
		constructor(back_units[25], 33, 6, 1, 6, 0, 23, smurf[0]);

		// Continue:
		constructor(smurf[0], 16, 16, 1, 1, 0, 0, smurf[1]);    constructor(smurf[1], 17, 16, 1, 1, 0, 1, smurf[2]);
		constructor(smurf[2], 16, 17, 1, 1, 0, 2, smurf[3]);    constructor(smurf[3], 17, 17, 1, 1, 0, 3, smurf[4]);
		constructor(smurf[4], 16, 18, 1, 1, 0, 4, smurf[5]);    constructor(smurf[5], 17, 18, 1, 1, 0, 5, smurf[6]);
		constructor(smurf[6], 16, 19, 1, 1, 0, 6, smurf[7]);    constructor(smurf[7], 17, 19, 1, 1, 0, 7, smurf[8]);
		constructor(smurf[8], 16, 20, 1, 1, 0, 8, smurf[9]);    constructor(smurf[9], 17, 20, 1, 1, 1, 9, ring[0]);

		constructor(ring[0], 33, 13, 0, 1, 0, 23, ring[1]);	   constructor(ring[1], 9, 18, 0, 1, 1, 16, wing[0]);
		constructor(wing[0], 11, 18, 0, 1, 0, 16, wing[1]);	   constructor(wing[1], 22, 18, 0, 1, 1, 16, berry);

		constructor(berry, 24, 18, 0, 1, 1, 16, skull);     	   constructor(skull, 13, 23, 0, 1, 1, 16, flames[0]);
		constructor(flames[0], 16, 23, 0, 1, 1, 16, flames[1]);  constructor(flames[1], 19, 23, 0, 1, 1, 16, NULL);

		screen1->head = back_units[0];
		paintbgd(screen1);
		show = 0;
		enable = 0;
		shoot = 0;
		enemy = 0;
	}
}

