#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#define MENU_RETRY 0
#define MENU_UNDO  1
#define MENU_EXIT  2
#define MENU_ABOUT 3
#define MENU_LEVEL 4

#define TILE_NULL   0
#define TILE_SPOT   1
#define TILE_FLOOR  2
#define TILE_BOX_SPOT 3
#define TILE_BOX    4
#define TILE_PLAYER_SPOT   5
#define TILE_PLAYER 6
#define TILE_WALL   7

#define UPDATE_NO_CHANGE				0
#define UPDATE_BOX_AND_PLAYER_MOVEMENT  1
#define UPDATE_PLAYER_MOVEMENT          2
#define UPDATE_LEVEL_COMPLETE			3

#define BIT0   1
#define BIT1   2
#define BIT2   4
#define BIT3   8
#define BIT4  16
#define BIT5  32
#define BIT6  64
#define BIT7 128



#define ENC_TILE_PLAYERSTART(startposition) startposition
#define ENC_TILE_SPOT(offset, length) (64|(offset<<2)|(length-1))
#define ENC_TILE_BOX(offset, length) (128|(offset<<2)|(length-1))
#define ENC_TILE_BOX_SPOT(offset, length) (128|64|(offset<<2)|(length-1))
#define ENC_TILE_WALL(offset, length) (32|(offset<<2)|(length-1))
#define ENC_TILE_SPACE(length) (length-1)
#define ENC_LEVEL(x, y) (x>0?x<<4:x*-1<<4|128)|(y>0?y&(BIT2|BIT1|BIT0):y*-1&(BIT2|BIT1|BIT0)|BIT3)
#define ENC_LEVEL_END 0

#define DEC_TILE_LENGTH(data) ((data&(2|1))+1)
#define DEC_TILE_OFFSET(data) ((data>>2)&(8|4|2|1))
#define DEC_TILE_OFFSET_WALL(data) ((data>>2)&(4|2|1))
#define DEC_TILE_TYPE(data) ((data>>5)&(4|2|1))
#define DEC_LEVEL_X(data) ((data&(BIT6|BIT5|BIT4))>>4)*(data&BIT7?-1:1)
#define DEC_LEVEL_Y(data) (data&(BIT2|BIT1|BIT0))*(data&BIT3?-1:1)
#define DEC_TILE_SPACE(data) ((data&(16|8|4|2|1))+1)



#define NEW_TILE	   0
#define SET_PEN_COLOR  1
#define SET_FILL_COLOR 2 
#define RECTANGLE      3
#define FILL           4
#define PUT_PIXEL      5
#define LINE           6
#define COPY           7
