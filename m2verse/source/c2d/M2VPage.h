#ifndef M2V_PAGE_H
#define M2V_PAGE_H

#include <citro2d.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../core/M2VPost.h"
#include "../core/M2VData.h"
//#include "M2V_C2D.h"

class M2VPage{
private:
  //M2V_C2D * app = NULL;
  SwkbdState swkbd;
  SwkbdButton button = SWKBD_BUTTON_NONE;
protected:
  C3D_RenderTarget * top = NULL;
  C3D_RenderTarget * bottom = NULL;
  u32 clrGreen = C2D_Color32(0x83, 0xEC, 0x8A, 0xFF);
  u32 clrBlue  = C2D_Color32(0x00, 0x00, 0xFF, 0xFF);
  u32 clrClear = C2D_Color32(0x04, 0x2D, 0x30, 0xFF);
  u32 clrWhite = C2D_Color32(0x00, 0x00, 0x00, 0xFF);
  u32 clrBox = C2D_Color32(0x09, 0x5A, 0x60, 0xC8);
  u32 clrBox2 = C2D_Color32(0x84, 0xAC, 0xAF, 0xC8);
  u32 clrText = C2D_Color32(0x8B, 0x8F, 0x52, 0xFF);
  C2D_TextBuf g_staticBuf;
  C2D_Text g_staticText;
  C2D_SpriteSheet spriteSheet;
  C2D_Sprite * sprites = NULL;
  unsigned int spriteNb = 0;
  virtual void draw() = 0;
  virtual void update() = 0;
  void textWrite(const char * str, float x, float y, float scale_x, float scale_y, u32 color);
  void message(const char * str);
  char * strAdd(const char * str1, const char * str2) const;
  void dialog();
  void question();
  virtual ButtonInput checkButtonInput() const = 0;
  void keyboardInput(char * buffer, unsigned int max, const char * hint);
  unsigned int curX = 0, curY = 0;
  bool input = false;

public:
  M2VPage(C3D_RenderTarget * top = NULL, C3D_RenderTarget * bottom = NULL) : top(top), bottom(bottom) {
    g_staticBuf  = C2D_TextBufNew(4096);
  }
  M2VPage();
  virtual ~M2VPage(){
    C2D_TextBufDelete(g_staticBuf);
    delete [] sprites;
    sprites = NULL;
    C2D_SpriteSheetFree(spriteSheet);
  }
  virtual Pages run() = 0;
};

class ConnectionPage : public M2VPage{
private:
  //insérer ici fonctions de requetes http pour la connexion
  void draw();
  void update();
  void init();
  ButtonInput checkButtonInput() const;
  char nameBuf[USERNAME_MAX_LENGTH + 1];
  char pwdBuf[PWD_MAX_LENGTH + 1];
  char nameAff[10];
  char pwdAff[10];
  bool connexionButtonHighlight = false, connectionValid = false;
  User user;
public:
  ConnectionPage();
  ConnectionPage(C3D_RenderTarget * top = NULL, C3D_RenderTarget * bottom = NULL) : M2VPage(top, bottom) {
    this->init();
  }
  virtual ~ConnectionPage(){}
  Pages run();
  User getUser(){
    return this->user;
  }
};



#endif // M2V_PAGE_H
