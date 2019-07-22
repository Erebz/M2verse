#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>

#define USERNAME_MAX_LENGTH 10

struct M2VPost {
	char[USERNAME_MAX_LENGTH] username;
	u32 type = 0;
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
