#include "boxworld.h"

HMENU hGame, hLevel, hMenu;
HDC hdcTiles;
char can_undo;
char level;
char maxlevel;
#define GET_LEVELS
#include "data.h"
#undef GET_LEVELS


unsigned char GetBits(unsigned char count)
{	
	#define GET_GRAPHICS
	#include "data.h"
	#undef GET_GRAPHICS

	unsigned char i;
	GraphicsData[0]=0;
	for(i=0; i<sizeof(GraphicsData); i++)
		GraphicsData[i]=(GraphicsData[i]<<count)|(GraphicsData[i+1]>>(8-count));

	return GraphicsData[0];
}



// Recursive fill function that replaces the null tiles inside of a level with a floor tile.
void FillLevel(unsigned char *lpPosition)
{
	if(*lpPosition==0)
	{
		*lpPosition=TILE_FLOOR;
		FillLevel(lpPosition-1);
		FillLevel(lpPosition+1);
		FillLevel(lpPosition-16);
		FillLevel(lpPosition+16);
	}
}

// Copies a level matrix to another, may be replaced with lstrcpy
void CopyLevel(unsigned char* destination, unsigned char* source)
{
	unsigned char i;
	for(i=0; i<16*14; i++)
		destination[i]=source[i];
}

void WinMainCRTStartup(void)
{
	WNDCLASS boxclass;
	HDC hdcScreen;
	MSG message;
	char buffer[2];
	unsigned char x,y, offset=0, pc;
	char i;
	HBITMAP hBmp;
	DWORD Palette[]=
	{
		{0x00000000}, // Black   (Countour)
		{0x001F1FFF}, // Red     (Spot)
		{0x0000FFFF}, // Yellow1 (Box)
		{0x0002C4D0}, // Yellow2 (Box)
		{0x00DDDDDD}, // Grey    (Wall lited)
		{0x00F04C48}, // Bluish  (Wall)
		{0x007D2B0B}  // Bluish  (Wall shadow)
	};

	// Start at highest level.
	GetProfileString("!", "!", "!", buffer, 2);
	level=buffer[0]-'!'+1;

	// New graphics loader.
	hdcScreen=GetDC(NULL);
	hdcTiles=CreateCompatibleDC(hdcScreen);
	hBmp=CreateCompatibleBitmap(hdcScreen, 224, 32);
	ReleaseDC(NULL, hdcScreen);
	SelectObject(hdcTiles, hBmp);

	// TODO: Investigate if it would be more efficient to store the graphics as EMF. It is a reasonably compact
	// format and we would probably need verry little code to decode it since there is WIN32 API support.

	// Draw floor on all tiles. (This piece takes about 65 bytes, graphics by code is smaller (GFX by code is approx. 50 bytes)
	for(y=0; y<32; y++)
		for(x=0; x<224; x++)
			SetPixel(hdcTiles, x, y, ((x/8+y/8)%2)?RGB(208, 208, 208):RGB(180, 180, 180));

	// Draw seven tiles.
	for(i=0; i<GRAPHICS_INSTRUCTIONS; i++)
	{
		switch(GetBits(3)) 
		{
			case NEW_TILE:
				offset+=32;
				break;

			case SET_PEN_COLOR:		
				pc=GetBits(3);
				SelectObject(hdcTiles, CreatePen(PS_SOLID, 1, Palette[pc]));
				break;

			case SET_FILL_COLOR:
				SelectObject(hdcTiles, CreateSolidBrush(Palette[GetBits(3)]));
				break;

			case RECTANGLE:
				Rectangle(hdcTiles, offset+GetBits(5), GetBits(5), offset+GetBits(5)+1, GetBits(5)+1);
				break;

			case FILL:
				FloodFill(hdcTiles, offset+GetBits(5), GetBits(5), Palette[pc]);
				break;

			case PUT_PIXEL:
				SetPixel(hdcTiles, offset+GetBits(5), GetBits(5), Palette[pc]);
				break;

			case LINE:
				MoveToEx(hdcTiles, offset+GetBits(5), GetBits(5), NULL);
				LineTo(hdcTiles, offset+GetBits(5), GetBits(5));
				break;

			case COPY:
				x=GetBits(5);
				y=GetBits(5);
				StretchBlt(hdcTiles, GetBits(8), GetBits(5), x, y, hdcTiles, GetBits(8), GetBits(5), GetBits(1)?-x:x, GetBits(1)?-y:y, SRCCOPY);
				break;
		}
	}

	// Build menus.
	hGame=CreateMenu();
	hLevel=CreateMenu();
	hMenu=CreateMenu();
	AppendMenu(hGame, MF_ENABLED|MF_STRING, MENU_RETRY, "&Retry\tENTER");
	AppendMenu(hGame, MF_ENABLED|MF_STRING, MENU_UNDO, "&Undo\tBACKSP");
	AppendMenu(hGame, MF_ENABLED|MF_STRING, MENU_ABOUT, "&About");
	AppendMenu(hGame, MF_ENABLED|MF_STRING, MENU_EXIT, "&Exit\tESC");
	AppendMenu(hMenu, MF_ENABLED|MF_STRING|MF_POPUP, (UINT_PTR)hGame, "&Game");
	AppendMenu(hMenu, MF_ENABLED|MF_STRING|MF_POPUP, (UINT_PTR)hLevel, "&Level");

	// Create and register class.
	boxclass.style=0;
	boxclass.lpfnWndProc=WindowProc;
	boxclass.cbClsExtra=0;
	boxclass.cbWndExtra=0;
	boxclass.hIcon=0;
	boxclass.hCursor=0;
	boxclass.hbrBackground=CreateSolidBrush(RGB(0, 0, 0));
	boxclass.lpszMenuName=NULL;
	boxclass.lpszClassName="Box World 4K!";
	
	// Get application memory offset. (This solution takes about 13 bytes, can be skipped, but application will not work if it is loaded at other adress than 4mb)
	boxclass.hInstance=GetModuleHandle(NULL);
	RegisterClass(&boxclass);

	// Create and show window.
	CreateWindow("Box World 4K!", "Box World 4K!", WS_TILED|WS_VISIBLE|WS_SYSMENU|WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 32*16+12, 32*15+12, NULL, hMenu, boxclass.hInstance, NULL);

	// The message loop.
	while(GetMessage(&message, NULL, 0, 0 ))
	{ 
		TranslateMessage(&message); 
		DispatchMessage(&message); 
	}
}

#ifdef _DEBUG
int main(VOID)
{
	return WinMainCRTStartup();
}
#endif

#ifdef _RELEASE
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	return WinMainCRTStartup();
}
#endif

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static unsigned char undo[16*14];
	static unsigned char orginal[16*14];
	static unsigned char state[16*14];
	unsigned char *stateplayer, *orginalplayer, *statenear, *statefar, *orginalnear, *orginalfar;
	PAINTSTRUCT ps;
	char buffer[9];
	unsigned char* lpTemp, *lpData;
	unsigned char i, j;
	char k, l, o, x1, x2, y2;

	switch(uMsg) 
	{
		// Window creation...
		case WM_CREATE:
			NEW_LEVEL:

			// There is no more than 43 levels.
			if(level>43)
				level=43;

			// If new level, save level.
			if(level>maxlevel)
			{
				maxlevel=level;
				buffer[0]=level+'!'-1;
				buffer[1]='\0';
				WriteProfileString("!", "!", buffer);
			}

			// Decompress level.
            
			// Get pointer to resource.
			lpData=(unsigned char*)LevelData;

			// Reset destination.
			// RtlSecureZeroMemory is called instead of ZeroMemory macro which does not work
			// since it is using memset which is a part of stdlib which is not included.
			RtlSecureZeroMemory(state, 16*14);

			// Find level.
			for(i=0; i<level-1; i++)
				while(*lpData++!=0);

			// Calculate level dimensions.
			lpTemp=lpData;
			k=0, l=0, o=0, x1=100, x2=-100, y2=-100;
			do
			{
				k+=DEC_LEVEL_X(*lpTemp);
				l+=DEC_LEVEL_Y(*lpTemp);

				// Calculate left offset
				o=k<o?k:o;

				// Calculate width and height.
				x1=(k<x1?k:x1);
				x2=(k>x2?k:x2);
				y2=(l>y2?l:y2);

				*lpTemp++;
			}
			while(k!=0 || l!=0);

			// Center level
			lpTemp=state+-o+8-(x2-x1+2)/2+16*7-((y2+2)/2)*16;

			// Draw walls
			while(*lpTemp!=TILE_WALL)
			{
				for(i=0; i<2; i++)
					for(k=i?DEC_LEVEL_Y(*lpData):DEC_LEVEL_X(*lpData); k; k>0?k--:k++)
					{
						*lpTemp=TILE_WALL;
						lpTemp+=(k>0?1:-1)*(i?16:1);
					}
					lpData++;	
			} 

			// Fill room with floor.
			FillLevel(state+120);

			// Draw player start.
			state[*lpData]=TILE_PLAYER;

			lpTemp=state;
			// Draw boxes, spots and walls
			while(*++lpData)
			{
				// Get tile offset and length
				i=DEC_TILE_LENGTH(*lpData);
				j=1+(DEC_TILE_TYPE(*lpData)!=1?DEC_TILE_OFFSET(*lpData):DEC_TILE_OFFSET_WALL(*lpData));

				if(DEC_TILE_TYPE(*lpData)==0)
				{
					j=DEC_TILE_SPACE(*lpData);
					i=1;
				}
				
				// For each length...
				while(i--)	
				{
					// Find offset or next tile...
					while(j--)
						// Move to next floor tile
						while(*++lpTemp!=TILE_FLOOR);

					// Write tile.
					switch(DEC_TILE_TYPE(*lpData)) 
					{
					case 4:
					case 5:
						*lpTemp=TILE_BOX;
						break;
					case 3:
					case 2:
						*lpTemp=(*lpTemp==TILE_PLAYER)?TILE_PLAYER_SPOT:TILE_SPOT;
						break;
					case 6:
						*lpTemp=TILE_BOX_SPOT;
						break;
					case 1:
						*lpTemp=TILE_WALL;
						break;
					case 0:
						break;

					}
					j=1;
				}
			}

			CopyLevel(orginal, state);

			// Substitute player with floor and remove boxes
			for(i=0; i<16*14; i++)
			{
				switch(orginal[i])
				{
					case TILE_PLAYER:
					case TILE_BOX:
						orginal[i]=TILE_FLOOR;
						break;
					case TILE_BOX_SPOT:
					case TILE_PLAYER_SPOT:
						orginal[i]=TILE_SPOT;
				}
			}
			
			// Add level to menu
			for(i=(unsigned char)GetMenuItemCount(hLevel)+1; i<=level; i++)
			{
				wsprintf(buffer, "%02d", i);
				AppendMenu(hLevel, MF_ENABLED|MF_STRING, MENU_LEVEL+i, buffer);
			}

			// Mark active level
			for(j=1; j<=i; j++)
			{
				wsprintf(buffer, "%02d", j);
				ModifyMenu(hMenu, MENU_LEVEL+j, (j==level?MF_CHECKED:MF_UNCHECKED)|MF_ENABLED|MF_STRING, MENU_LEVEL+j, buffer);
			}
			
			InvalidateRect(hWnd, NULL, TRUE);
			can_undo=0;
			goto ENABLE_UNDO;

		// Screen updates...
		case WM_PAINT:
			BeginPaint(hWnd, &ps);
			for(i=0; i<16*14; i++)
				if(state[i])
					StretchBlt(ps.hdc, (i%16)*32, (i/16)*32, 32, 32, hdcTiles, 32*((state[i])-1), 0, 32, 32, SRCCOPY);
			EndPaint(hWnd,&ps);
			break;

		// Keyboard commands...
		case WM_KEYDOWN:
			switch(wParam) 
			{
				default:
					InvalidateRect(hWnd, NULL, FALSE);

					// Set pointer offset depending on direction.
					k=0;
					switch(wParam) 
					{
						case VK_LEFT:
							k+=-2;
						case VK_RIGHT:
							k+=17;
						case VK_UP:
							k+=-32;
						case VK_DOWN:
							k+=16;
					}
					if(!k)
						break;

					// Locate player
					for(i=0; i<16*14; i++)
						if(state[i]==TILE_PLAYER || state[i]==TILE_PLAYER_SPOT)
							break;

					// Set pointers for tiles that are about to be touched.
					stateplayer=state+i;
					orginalplayer=orginal+i;
					statenear=state+i+k;
					orginalnear=orginal+i+k;
					statefar=state+i+k*2;
					orginalfar=orginal+i+k*2;

					// Is there space near?
					if(*statenear==TILE_FLOOR || *statenear==TILE_SPOT)
					{
						*statenear=(*orginalnear==TILE_SPOT?TILE_PLAYER_SPOT:TILE_PLAYER);
						*stateplayer=*orginalplayer;
						break;
					}

					// Is there a box to the left, and is there space behind it?
					else if((*statenear==TILE_BOX || *statenear==TILE_BOX_SPOT) && (*statefar==TILE_FLOOR || *statefar==TILE_SPOT))
					{
						CopyLevel(undo, state);
						*statenear=(*orginalnear==TILE_SPOT?TILE_PLAYER_SPOT:TILE_PLAYER);
						*stateplayer=*orginalplayer;
						*statefar=(*orginalfar==TILE_SPOT?TILE_BOX_SPOT:TILE_BOX);	

						// Check if level is completed.
						for(i=0; i<16*14; i++)
							if(state[i]==TILE_SPOT || state[i]==TILE_PLAYER_SPOT)
							{
								can_undo=1;
								goto ENABLE_UNDO;
							}

						MessageBox(hWnd, "Level complete!", "Box World 4K!", MB_OK|MB_ICONINFORMATION);
						level++;
						goto NEW_LEVEL;
					}
					break;

				case VK_RETURN:
					goto NEW_LEVEL;
				
				case VK_BACK:
					UNDO:
					if(can_undo)
					{
						CopyLevel(state, undo);
						can_undo=0;
						UpdateWindow(hWnd);
						InvalidateRect(hWnd, NULL, FALSE);
					}
					break;

				case VK_ESCAPE:
					goto CLOSE;
			}
			break;

		// Menu commands...
		case WM_COMMAND:
			switch(wParam)
			{
				case MENU_EXIT:
					goto CLOSE;
				case MENU_UNDO:
					goto UNDO;
				case MENU_RETRY:
					goto NEW_LEVEL;
				case MENU_ABOUT:
					MessageBox(hWnd, "By Oskar Sjöberg (oskar@oskarsjoberg.se), after an original game by Jeng-Long Jiang.", "Box World 4K!", MB_OK|MB_ICONINFORMATION);
			}

			if(wParam>MENU_LEVEL)
			{
				level=(char)wParam-MENU_LEVEL;
				goto NEW_LEVEL;
			}
			break;

		// Exit...
		case WM_CLOSE:
			CLOSE:
			PostQuitMessage(0);
			break;

		// All other messages...
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	ENABLE_UNDO:
	ModifyMenu(hGame, MENU_UNDO, (can_undo?MF_ENABLED:MF_DISABLED|MF_GRAYED)|MF_STRING, MENU_UNDO, "&Undo\tBACKSP");
	return 0;	
}

int __stdcall WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR cmlLIne, int nShowCmn)
{
	WinMainCRTStartup();
	return 0;
}
