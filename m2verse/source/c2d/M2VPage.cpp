#include "M2VPage.h"
#include <cassert>

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

//=====================================================================================================


//================================================ConnectionPage=======================================
Pages ConnectionPage::run(){
  //Treat inputs
  this->update();

  //Render the scene
  this->draw();

  if(connectionValid){
    return PROFILE_PAGE;
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
  C2D_SpriteSetScale(&sprites[0], 0.17f, 0.17f);
}

void ConnectionPage::update(){
    u32 kDown = hidKeysDown();
    if(kDown & KEY_A) connectionValid = true;
}

void ConnectionPage::draw(){
  C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
  C2D_TargetClear(top, clrClear);
  C2D_SceneBegin(top);
  C2D_DrawSprite(&sprites[0]);
  message("Welcome to M2V ! Please enter \nyour username and your password.");

  C2D_SceneBegin(bottom);
  //C2D_DrawRectangle(20, BOTTOM_MAX_Y-100, 0, BOTTOM_MAX_X-40, 80, clrBox, clrBox, clrBox, clrBox);
  //C2D_DrawRectangle(20, BOTTOM_MAX_Y-100, 0, BOTTOM_MAX_X-40, 80, clrBox, clrBox, clrBox, clrBox);
  C2D_TargetClear(bottom, clrClear);
  C2D_DrawRectangle(160, 40, 0, 120, 45, clrBox, clrBox, clrBox, clrBox);
  C2D_DrawRectangle(160, 105, 0, 120, 45, clrBox, clrBox, clrBox, clrBox);
  C2D_DrawRectangle(40, 170, 0, 240, 55, clrBox2, clrBox2, clrBox2, clrBox2);
  textWrite("Username : ", 40, 50, 0.8f, 0.8f, clrText);
  textWrite("Password : ", 40, 115, 0.8f, 0.8f, clrText);
  textWrite("CONNECT TO M2V", 65, 185, 0.85f, 0.85f, clrClear);
  C3D_FrameEnd(0);
}


//=====================================================================================================
