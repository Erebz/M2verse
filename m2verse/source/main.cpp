#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>

#include "M2VPostData.h"

#define USERNAME_MAX_LENGTH 10

struct Date {
	u32 day = 1;
	u32 month = 1;
	u32 year = 2019;
	u32 hour = 0;
	u32 minute = 0;
};

struct M2VPostInfos {
	char username[USERNAME_MAX_LENGTH];
	u32 type = 0; //0 : text, 1 : drawing
	Date date;
	M2VPostData * data;
};

int main(int argc, char* argv[])
{
	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);

	printf("Hello, world!\n");

	// Main loop
	while (aptMainLoop())
	{
		gspWaitForVBlank();
		gfxSwapBuffers();
		hidScanInput();

		// Your code goes here
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu
	}

	gfxExit();
	return 0;
}
