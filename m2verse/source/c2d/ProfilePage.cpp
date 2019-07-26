#include "ProfilePage.h"
#include <cassert>
#include <string>

Pages ProfilePage::run(){
  //Treat inputs
  this->update();

  //Render the scene
  this->draw();

  return PROFILE_PAGE;
}

void ProfilePage::init(){
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

void ProfilePage::update(){
    u32 kDown = hidKeysDown();
}

void ProfilePage::draw(){
  C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
  C2D_TargetClear(top, clrBlue);
  C2D_SceneBegin(top);
  C2D_DrawSprite(&sprites[0]);
  char * str = strAdd("Bienvenue, ", this->user.username);
  message(str);
  if(str!=NULL) delete str;
  str = NULL;

  C2D_SceneBegin(bottom);
  //C2D_DrawRectangle(20, BOTTOM_MAX_Y-100, 0, BOTTOM_MAX_X-40, 80, clrBox, clrBox, clrBox, clrBox);
  //C2D_DrawRectangle(20, BOTTOM_MAX_Y-100, 0, BOTTOM_MAX_X-40, 80, clrBox, clrBox, clrBox, clrBox);
  C2D_TargetClear(bottom, clrBlue);
  C2D_DrawRectangle(160, 40, 0, 120, 45, clrBox, clrBox, clrBox, clrBox);
  C2D_DrawRectangle(160, 105, 0, 120, 45, clrBox, clrBox, clrBox, clrBox);
  C2D_DrawRectangle(40, 170, 0, 240, 55, clrBox2, clrBox2, clrBox2, clrBox2);
  textWrite("Username : ", 40, 50, 0.8f, 0.8f, clrText);
  textWrite("Password : ", 40, 115, 0.8f, 0.8f, clrText);
  textWrite("CONNECT TO M2V", 65, 185, 0.85f, 0.85f, clrBlue);
  C3D_FrameEnd(0);
}


//=====================================================================================================
