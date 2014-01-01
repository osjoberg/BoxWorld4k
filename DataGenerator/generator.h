#include <stdio.h>
#include <windows.h>

void PrintResult(void);
void SetBits(unsigned char data, unsigned char count);
void GFXNewTile(void);
void GFXSetPenColor(unsigned char i);
void GFXSetFillColor(unsigned char i);
void GFXRectangle(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);
void GFXFill(unsigned char x, unsigned char y);
void GFXPutPixel(unsigned char x, unsigned char y);
void GFXLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);
void GFXCopy(unsigned char sx, unsigned char sy, unsigned char dx, unsigned char dy, unsigned char width, unsigned char height, unsigned char flipx, unsigned char flipy);