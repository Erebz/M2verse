#include "M2VPage.h"
#include <cassert>
#include <fstream>
#include "spritesMenu.h"

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
//=====================================================================================================


//================================================ConnectionPage=======================================
void ConnectionPage::run(){
  this->init();
  this->loop();
}

void ConnectionPage::init(){
  spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/spritesMenu.t3x");
  //if (!spriteSheet) svcBreak(USERBREAK_PANIC);
  spriteNb = C2D_SpriteSheetCount(spriteSheet);
  /*std::ofstream file("log.txt");
  assert(file.is_open());
  file << spriteNb;
  file << 30000;
  file.close();*/
  assert(spriteNb > 0);
  sprites = new C2D_Sprite[spriteNb];
  for(unsigned int i=0; i<spriteNb; i++){
    C2D_SpriteFromSheet	(&sprites[i], spriteSheet, i);
    C2D_SpriteSetCenter(&sprites[i], 0.5f, 0.5f);
    C2D_SpriteSetPos(&sprites[i], TOP_MAX_X/2, TOP_MAX_Y/2 - 30);
  }
  C2D_SpriteSetScale(&sprites[0], 0.17f, 0.17f);
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

void ConnectionPage::loop(){
  bool quit = false;
  while (aptMainLoop() && !quit){
    //Treat inputs
    hidScanInput();
    u32 kDown = hidKeysDown();
    if (kDown & KEY_START) quit = true;

    //Render the scene
    this->draw();
  }
}

//=====================================================================================================
