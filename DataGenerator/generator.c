#include "generator.h"
#include "levels.h"

static unsigned char ds[255]={0};
int co=2;
int ins=0;


int main(void)
{
	// Tile one (spot)
	GFXSetPenColor(1);
	GFXSetFillColor(1);
	GFXLine( 2,  1, 30,  1);
	GFXLine( 2, 30, 30, 30);
	GFXLine( 1,  2,  1, 30);
	GFXLine(30,  2, 30, 30);
	GFXLine( 1, 27, 27,  1);
	GFXLine(30,  4,  4, 30);
	GFXLine( 1, 11, 11,  1);
	GFXLine( 1, 17, 17,  1);
	GFXLine(30, 14, 14, 30);
	GFXLine(30, 20, 20, 30);
	GFXFill( 8,  8);
	GFXFill(16, 16);
	GFXFill(24, 24);

	// Tile two (floor)
	GFXNewTile();

	// Tile three (box on spot)
	GFXNewTile();
	// Rest of tile is copied at the end...

	// Tile four (box)
	GFXNewTile();
	GFXSetPenColor(0);
	GFXSetFillColor(2);
	GFXRectangle( 2,  2, 29, 29);
	GFXSetFillColor(3);
	GFXRectangle( 7,  7, 24, 24);
	GFXRectangle( 7,  4,  8,  5);
	GFXRectangle( 4,  7,  5,  8);
	GFXLine(11,  8, 24, 21);
	GFXLine( 8, 11, 21, 24);
	GFXSetFillColor(2);
	GFXFill(16, 16);
	GFXCopy(111, 2, 112, 2, 14, 14, TRUE, FALSE);
	GFXCopy(98, 15, 98, 16, 28, 14, FALSE, TRUE);
	GFXLine(77, 16, 80, 19);
	GFXLine(80, 13, 83, 16);

	// Tile five (player on spot)
	GFXNewTile();
	GFXCopy(0, 0, 128, 0, 31, 31, FALSE, FALSE);
	GFXSetFillColor(0);
	GFXLine( 4, 0, 28, 0);
	GFXRectangle(6,9,25, 12);
	GFXRectangle(7,8,24, 13);
	GFXRectangle(6,19,25, 22);
	GFXRectangle(7,18,24, 23);
	// Rest of tile is copied at the end...

	// Tile six (player)
	GFXNewTile();
	GFXRectangle(6,9,25, 12);
	GFXRectangle(7,8,24, 13);
	GFXRectangle(6,19,25, 22);
	GFXRectangle(7,18,24, 23);
	GFXSetFillColor(2);
	GFXRectangle(9,7,22,25);
	GFXSetFillColor(0);
	GFXRectangle(11, 8, 12, 9);
	GFXRectangle(19, 8, 20, 9);
	GFXRectangle(12, 10, 19, 15);
	GFXRectangle(11, 15, 12, 16);
	GFXRectangle(19, 15, 20, 16);
	GFXRectangle(13, 20, 14, 21);
	GFXLine( 4, 0, 28, 0);
	GFXLine( 6, 2, 10, 2);
	GFXLine(22, 2, 26, 2);
	GFXLine(10, 3, 10, 7);
	GFXLine(21, 3, 21, 7);
	GFXLine(13, 3, 19, 3);
	GFXLine(13, 5, 19, 5);
	GFXLine(13, 24, 19, 24);
	GFXPutPixel(5, 1);
	GFXPutPixel(26, 1);
	GFXPutPixel(13, 4);
	GFXPutPixel(18, 4);
	GFXPutPixel(13, 9);
	GFXPutPixel(18, 9);
	GFXSetFillColor(2);
	GFXFill(7, 1);
	GFXSetPenColor(2);
	GFXLine(10, 7, 22, 7);

	// Tile seven (wall)
	GFXNewTile();
	GFXSetPenColor(4);
	GFXSetFillColor(5);
	GFXRectangle(0, 0, 31, 15);
	GFXSetPenColor(6);
	GFXLine( 1, 15, 31, 15);
	GFXLine(31,  1, 31, 16);
	GFXCopy(208, 0, 192, 16, 16, 16, FALSE, FALSE);
	GFXCopy(192, 0, 208, 16, 16, 16, FALSE, FALSE);

	// More (box on spot)
	GFXCopy(0, 0, 64, 0, 31, 31, FALSE, FALSE);
	GFXCopy(98, 2, 66, 2, 28, 28, FALSE, FALSE);
	
	// More (player on spot)
	GFXCopy(169, 7, 137, 7, 14, 19, FALSE, FALSE);
	GFXCopy(170, 2, 138, 2, 12, 14, FALSE, FALSE);
	GFXCopy(165, 1, 133, 1, 22, 2, FALSE, FALSE);


	PrintResult();
}



void PrintResult(void)
{
	int i;
	FILE* fp;
	
	fopen_s(&fp, "..\\BoxWorld4K\\data.h", "w");
	fprintf(fp, "#define GRAPHICS_INSTRUCTIONS %d\n\n", ins);
	fprintf(fp, "// Graphics data, %d bytes.\n", co);
	fprintf(fp, "#ifdef GET_GRAPHICS\n");
	fprintf(fp, "static unsigned char GraphicsData[]=\n{");
	for(i=0; i<co; i++)
	{
		if(i%5==0)
			fprintf(fp, "\n\t");
		fprintf(fp, "0x%02X, ", ds[i]);
	}
	fprintf(fp, "\n};\n#endif\n\n");



	fprintf(fp, "// Level data, %d bytes.\n", sizeof(CompressedLevelData));
	fprintf(fp, "#ifdef GET_LEVELS\n");
	fprintf(fp, "DWORD LevelData[]=\n{");
	for(i=0; i<sizeof(CompressedLevelData); i++)
	{
		if(i%20==0)
			fprintf(fp, "\n\t");

		if(i%4==0)
			fprintf(fp, "0x");

		fprintf(fp, "%02X", CompressedLevelData[i+4-(i%4)*2-1]);

		if((i+1)%4==0)
			fprintf(fp, ", ");

	}
	fprintf(fp, "\n};\n#endif");
	fclose(fp);

	
}

void SetBits(unsigned char data, unsigned char count)
{
	unsigned char i=1;
	static unsigned char j=128;
	static unsigned char* dp=NULL;

	if(dp==NULL)
	{
		dp=(unsigned char *)&ds;
		dp++;
	}
	
	i=i<<(count-1);
    
	while(count--)
	{
		*dp=*dp|(i&data?j:0);

		i=i>>1;
		j=j>>1;
		if(j==0)
		{
			j=128;
			dp++;
			co++;
			*dp=0;
			
		}
	}
}

void GFXNewTile(void)
{
	SetBits(0 ,3);
	ins++;
}


void GFXSetPenColor(unsigned char i)
{
	SetBits(1, 3);
	SetBits(i, 3);
	ins++;
}
void GFXSetFillColor(unsigned char i)
{
	SetBits(2, 3);
	SetBits(i, 3);
	ins++;
}

void GFXRectangle(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
	SetBits(3, 3);
	SetBits(y2, 5);
	SetBits(x2, 5);
	SetBits(y1, 5);
	SetBits(x1, 5);
	ins++;
}

void GFXFill(unsigned char x, unsigned char y)
{
	SetBits(4, 3);
	SetBits(y, 5);
	SetBits(x, 5);
	ins++;
}

void GFXPutPixel(unsigned char x, unsigned char y)
{
	SetBits(5, 3);
	SetBits(y, 5);
	SetBits(x, 5);
	ins++;
}

void GFXLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
	SetBits(6, 3);
	SetBits(y1, 5);
	SetBits(x1, 5);
	SetBits(y2, 5);
	SetBits(x2, 5);
	ins++;
}


void GFXCopy(unsigned char sx, unsigned char sy, unsigned char dx, unsigned char dy, unsigned char width, unsigned char height, unsigned char flipx, unsigned char flipy)
{
	SetBits(7, 3);
	SetBits(width,  5);
	SetBits(height, 5);
	SetBits(flipy, 1);
	SetBits(flipx, 1);
	SetBits(sy, 5);
	SetBits(sx, 8);
	SetBits(dy, 5);
	SetBits(dx, 8);
	ins++;
}
