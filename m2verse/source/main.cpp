#include "M2VPost.h"
#define MAX_KBD_INPUT 144

u32 waitInput(u32 wait){
	u32 input = 0;
	while(input != wait){
		hidScanInput();
		input = hidKeysDown();
	}
	return input;
}

void keyboardInput(char * buffer, unsigned int bufferSize, char hint[64]){
	SwkbdState swkbd;
	SwkbdButton button = SWKBD_BUTTON_NONE;
	swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, MAX_KBD_INPUT);
	swkbdSetButton(&swkbd, SWKBD_BUTTON_LEFT, "Cancel", false);
	swkbdSetButton(&swkbd, SWKBD_BUTTON_RIGHT, "Confirm", true);
	swkbdSetHintText(&swkbd, hint);
	char result[MAX_KBD_INPUT];
	button = swkbdInputText(&swkbd, result, sizeof(result));

	if(button & SWKBD_BUTTON_RIGHT && result[0] != '\0'){
		for(unsigned int i=0; i < bufferSize - 1; i++){
			buffer[i] = result[i];
		}
		buffer[bufferSize - 1] = '\0';
	}
}

int main(int argc, char* argv[])
{
	gfxInitDefault();
	PrintConsole topScreen, bottomScreen, leftWindow, rightWindow;
	consoleInit(GFX_TOP, &topScreen);
	consoleInit(GFX_BOTTOM, &bottomScreen);
	consoleInit(GFX_TOP, &leftWindow);
	consoleInit(GFX_TOP, &rightWindow);

	consoleSetWindow(&leftWindow, 1, 1, 23, 28);
	consoleSetWindow(&rightWindow, 26, 1, 23, 28);
	consoleSelect(&leftWindow);
  printf("This text is in the left window!\n");
  consoleSelect(&rightWindow);
  printf("This text is in the right window!\n");
  consoleSelect(&topScreen);
	consoleSelect(&bottomScreen);

	char username[USERNAME_MAX_LENGTH + 1] = {'\0'};
	char title[TITLE_MAX_LENGTH + 1] = {'\0'};
	char text[TEXT_MAX_LENGTH + 1] = {'\0'};
	bool valide = false;
	M2VTextPost * post = NULL;

	// Main loop
	while (aptMainLoop())
	{
		gspWaitForVBlank();
		gfxSwapBuffers();
		hidScanInput();
		u32 kDown = hidKeysDown();

		if(valide){
			printf("\x1b[1;0HPosted successfully!\n"); //x1b[r;cH
			printf("\x1b[2;0H%s\n", post->getTitle());
			printf("\x1b[4;0H%s\n", post->getText());
			printf("\x1b[7;0HBy %s!\n", post->getUsername());
			printf("\x1b[9;0HRestart (A)");

			if(kDown & KEY_A){
				delete post;
				post = NULL;
				valide = false;
				username[0] = '\0';
				text[0] = '\0';
				title[0] = '\0';
				consoleClear();
			}

		}else{
			printf("\x1b[1;0HPress A to enter a name : %s\n", username); //x1b[r;cH
			printf("\x1b[2;0HPress B to enter a title : %s\n", title);
			printf("\x1b[3;0HPress Y to enter a text : \n");
			printf("\x1b[4;0H%s\n", text);
			printf("\x1b[7;0HPress X to finish the post. \n");
			if(kDown & KEY_A){
				keyboardInput(username, sizeof(username), "Enter a name");
				consoleClear();
			}
			if(kDown & KEY_B){
				keyboardInput(title, sizeof(title), "Enter a title");
				consoleClear();
			}
			if(kDown & KEY_Y){
				keyboardInput(text, sizeof(text), "Enter a text");
				consoleClear();
			}
			if(kDown & KEY_X){
				if(username[0] == '\0' || text[0] == '\0' || title[0] == '\0'){
					printf("\x1b[8;0HError! One of the field is blank.\n");
					printf("\x1b[9;34HOK (A)\n");
					waitInput(KEY_A);
				}else{
					valide = true;
					Date date;
					post = new M2VTextPost(username, date, title, text);
				}
				consoleClear();
			}
		}

		if (kDown & KEY_START)
		break; // break in order to return to hbmenu
	}

	if(post != NULL) delete post;
	gfxExit();
	return 0;
}
