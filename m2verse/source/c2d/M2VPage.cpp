#include "M2VPage.h"
#include <cassert>
#include <inttypes.h>

//================================================M2VPage==============================================
void M2VPage::textWrite(const char * str, float x, float y, float scale_x, float scale_y, u32 color){
  C2D_TextBufClear(g_staticBuf);
  C2D_TextParse(&g_staticText, g_staticBuf, str);
  C2D_TextOptimize(&g_staticText);
  C2D_DrawText(&g_staticText, C2D_WithColor, x, y, scale_x, scale_x, scale_y, color);
}

void M2VPage::message(const char * str){
  C2D_DrawRectangle(20, TOP_MAX_Y-100, 0, TOP_MAX_X-40, 80, clrBox, clrBox, clrBox, clrBox);
  textWrite(str, 30.0f, 150.0f, 0.8f, 0.8f, clrText);
}

char * M2VPage::strAdd(const char * str1, const char * str2) const{
  char * str3 = new char[strlen(str1) + strlen(str2) + 1];
  unsigned int i, k=0;
  for(i=0; i < strlen(str1); i++){
    if(str1[i] != '\0') str3[i] = str1[i];
    else str3[i] = ' ';
  }
  for(unsigned int j = i; j < strlen(str1) + strlen(str2); j++){
    if(str2[strlen(str1) - i+k] != '\0') str3[j] = str2[strlen(str1) - i+k];
    else str3[j] = ' ';
    k++;
  }
  str3[strlen(str1) + strlen(str2)] = '\0';

  return str3;
}

void M2VPage::keyboardInput(char * buffer, unsigned int max, const char * hint){
  swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, max);
  swkbdSetButton(&swkbd, SWKBD_BUTTON_LEFT, "Cancel", false);
  swkbdSetButton(&swkbd, SWKBD_BUTTON_RIGHT, "Confirm", true);
  swkbdSetHintText(&swkbd, hint);
  char * result = new char[max];
  button = swkbdInputText(&swkbd, result, max);

  if(button & SWKBD_BUTTON_RIGHT){
    for(unsigned int i=0; i < strlen(result); i++){
      buffer[i] = result[i];
    }
    buffer[strlen(result)] = '\0';
  }
  delete result;
}

Result M2VPage::http_post(const char* url, const char* data){
	Result ret=0;
	httpcContext context;
	char *newurl=NULL;
	u32 statuscode=0;
	u32 contentsize=0, readsize=0, size=0;
	u8 *buf, *lastbuf;
	do {
		ret = httpcOpenContext(&context, HTTPC_METHOD_POST, url, 0);

		// This disables SSL cert verification, so https:// will be usable
		ret = httpcSetSSLOpt(&context, SSLCOPT_DisableVerify);

		// Enable Keep-Alive connections
		ret = httpcSetKeepAlive(&context, HTTPC_KEEPALIVE_ENABLED);

		// Set a User-Agent header so websites can identify your application
		ret = httpcAddRequestHeaderField(&context, "User-Agent", "m2verse-3DS/1.0.0");

		// Set a Content-Type header so websites can identify the format of our raw body data.
		// If you want to send form data in your request, use:
		//ret = httpcAddRequestHeaderField(&context, "Content-Type", "multipart/form-data");
		// If you want to send raw JSON data in your request, use:
		ret = httpcAddRequestHeaderField(&context, "Content-Type", "application/json");

		// Post specified data.
		// If you want to add a form field to your request, use:
		//ret = httpcAddPostDataAscii(&context, "data", value);
		// If you want to add a form field containing binary data to your request, use:
		//ret = httpcAddPostDataBinary(&context, "field name", yourBinaryData, length);
		// If you want to add raw data to your request, use:
		ret = httpcAddPostDataRaw(&context, (u32*)data, strlen(data));

		ret = httpcBeginRequest(&context);
		if(ret!=0){
			httpcCloseContext(&context);
			if(newurl!=NULL) free(newurl);
			return ret;
		}

		ret = httpcGetResponseStatusCode(&context, &statuscode);
		if(ret!=0){
			httpcCloseContext(&context);
			if(newurl!=NULL) free(newurl);
			return ret;
		}

		if ((statuscode >= 301 && statuscode <= 303) || (statuscode >= 307 && statuscode <= 308)) {
			if(newurl==NULL) newurl = (char*)malloc(0x1000); // One 4K page for new URL
			if (newurl==NULL){
				httpcCloseContext(&context);
				return -1;
			}
			ret = httpcGetResponseHeader(&context, "Location", newurl, 0x1000);
			url = newurl; // Change pointer to the url that we just learned
			httpcCloseContext(&context); // Close this context before we try the next
		}
	} while ((statuscode >= 301 && statuscode <= 303) || (statuscode >= 307 && statuscode <= 308));

	if(statuscode!=200){
		httpcCloseContext(&context);
		if(newurl!=NULL) free(newurl);
		return -2;
	}

	// This relies on an optional Content-Length header and may be 0
	ret=httpcGetDownloadSizeState(&context, NULL, &contentsize);
	if(ret!=0){
		httpcCloseContext(&context);
		if(newurl!=NULL) free(newurl);
		return ret;
	}

	// Start with a single page buffer
	buf = (u8*)malloc(0x1000);
	if(buf==NULL){
		httpcCloseContext(&context);
		if(newurl!=NULL) free(newurl);
		return -1;
	}

	do {
		// This download loop resizes the buffer as data is read.
		ret = httpcDownloadData(&context, buf+size, 0x1000, &readsize);
		size += readsize;
		if (ret == (s32)HTTPC_RESULTCODE_DOWNLOADPENDING){
				lastbuf = buf; // Save the old pointer, in case realloc() fails.
				buf = (u8*)realloc(buf, size + 0x1000);
				if(buf==NULL){
					httpcCloseContext(&context);
					free(lastbuf);
					if(newurl!=NULL) free(newurl);
					return -1;
				}
			}
	} while (ret == (s32)HTTPC_RESULTCODE_DOWNLOADPENDING);

	if(ret!=0){
		httpcCloseContext(&context);
		if(newurl!=NULL) free(newurl);
		free(buf);
		return -1;
	}

	// Resize the buffer back down to our actual final size
	lastbuf = buf;
	buf = (u8*)realloc(buf, size);
	if(buf==NULL){ // realloc() failed.
		httpcCloseContext(&context);
		free(lastbuf);
		if(newurl!=NULL) free(newurl);
		return -1;
	}

	httpcCloseContext(&context);
	free(buf);
	if (newurl!=NULL) free(newurl);
	return 0;
}

//=====================================================================================================


//================================================ConnectionPage=======================================
Pages ConnectionPage::run(){
  //Treat inputs
  this->update();

  //Render the scene
  this->draw();

  if(connectionValid){
    return DRAWING_PAGE;
  }else{
    return CONNECTION_PAGE;
  }
}

void ConnectionPage::init(){
  spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/spritesMenu.t3x");
  spriteNb = C2D_SpriteSheetCount(spriteSheet);
  assert(spriteNb > 0);
  sprites = new C2D_Sprite[spriteNb];
  for(unsigned int i=0; i<spriteNb; i++){
    C2D_SpriteFromSheet	(&sprites[i], spriteSheet, i);
    C2D_SpriteSetCenter(&sprites[i], 0.5f, 0.5f);
    C2D_SpriteSetPos(&sprites[i], TOP_MAX_X/2, TOP_MAX_Y/2 - 30);
  }
  C2D_SpriteSetScale(&sprites[0], 0.34f, 0.34f);
}

void ConnectionPage::update(){
  u32 kDown = hidKeysDown();
  if(kDown & KEY_A) connectionValid = true;

  //Read the touch screen coordinates
  hidTouchRead(&touch);
  if(touch.px > 0 && touch.py > 0){
    curX = touch.px;
    curY = touch.py;
    if(checkButtonInput() == CONNECTION_INPUT){
      connexionButtonHighlight = true;
    }else{
      connexionButtonHighlight = false;
    }
  }
  if(touch.px == 0 && touch.py == 0){
    if(curX != 0 && curY != 0){
      input = true;
    }
  }
  if(input){
    switch (checkButtonInput()){
      case USERNAME_INPUT:
      for(int i=0; i<10; i++){nameAff[i] = ' ';}
      nameAff[9] = '\0';
      keyboardInput(nameBuf, USERNAME_MAX_LENGTH + 1, "Enter your username.");
      for(unsigned int i=0; (i<10 && i<strlen(nameBuf)); i++){
        nameAff[i] = nameBuf[i];
      }
      if(strlen(nameBuf) >= 10){
        nameAff[6] = '.';
        nameAff[7] = '.';
        nameAff[8] = '.';
        nameAff[9] = '\0';
      }
      break;
      case PASSWORD_INPUT:
      for(int i=0; i<10; i++){pwdAff[i] = ' ';}
      pwdAff[9] = '\0';
      keyboardInput(pwdBuf, PWD_MAX_LENGTH + 1, "Enter your password.");
      if(pwdBuf[0] != '\0') {
        for(int i=0; i<9; i++){pwdAff[i] = '.';}
        pwdAff[9] = '\0';
      }
      break;
      case CONNECTION_INPUT:
      connexionButtonHighlight = false;
      //test if entries are correct
      //connection request
      break;
      case NO_INPUT: break;
      default: break;
    }
    input = false;
    curX = 0;
    curY = 0;
  }
}

void ConnectionPage::draw(){
  C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
  C2D_TargetClear(top, clrClear);
  C2D_SceneBegin(top);
  C2D_DrawSprite(&sprites[0]);
  message("Welcome to M2V ! Please enter \nyour username and your password.");

  C2D_SceneBegin(bottom);
  C2D_TargetClear(bottom, clrClear);
  C2D_DrawRectangle(160, 40, 0, 120, 45, clrBox, clrBox, clrBox, clrBox);
  C2D_DrawRectangle(160, 105, 0, 120, 45, clrBox, clrBox, clrBox, clrBox);
  textWrite("Username : ", 40, 50, 0.8f, 0.8f, clrText);
  textWrite("Password : ", 40, 115, 0.8f, 0.8f, clrText);
  textWrite(nameAff, 165, 53, 0.75f, 0.75f, clrClear);
  textWrite(pwdAff, 180, 107, 1.0f, 1.0f, clrClear);
  if(connexionButtonHighlight){
    C2D_DrawRectangle(38, 168, 0, 244, 59, clrText, clrText, clrText, clrText);
    C2D_DrawRectangle(40, 170, 0, 240, 55, clrBox2, clrBox2, clrBox, clrBox);
    textWrite("CONNECT TO M2V", 65, 185, 0.85f, 0.85f, clrText);
  }else{
    C2D_DrawRectangle(40, 170, 0, 240, 55, clrBox2, clrBox2, clrBox2, clrBox2);
    textWrite("CONNECT TO M2V", 65, 185, 0.85f, 0.85f, clrClear);
  }

  C3D_FrameEnd(0);
}

ButtonInput ConnectionPage::checkButtonInput() const{
  ButtonInput button = NO_INPUT;
  if(curX >= 160 && curX <= 280){
    if(curY >= 40 && curY <= 85){
      button = USERNAME_INPUT;
    }
  }
  if(curX >= 160 && curX <= 280){
    if(curY >= 105 && curY <= 150){
      button = PASSWORD_INPUT;
    }
  }
  if(curX >= 40 && curX <= 280){
    if(curY >= 170 && curY <= 225){
      button = CONNECTION_INPUT;
    }
  }
  return button;
}

//=====================================================================================================
