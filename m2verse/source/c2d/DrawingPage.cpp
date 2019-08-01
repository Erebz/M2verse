#include "M2VPage.h"
#include "DrawingPage.h"
#include <cassert>
#include <math.h>

Pages DrawingPage::run(){
  //Treat inputs
  this->update();

  //Render the scene
  this->draw();

  return DRAWING_PAGE;
}

void DrawingPage::init(){
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

  for(unsigned int y = 0; y < DRAW_MAX_Y; y++){
    for(unsigned int x = 0; x < DRAW_MAX_X; x++){
      drawing[x][y] = clrWhite;
    }
  }
}

void DrawingPage::drawPixel(unsigned int x, unsigned int y){
  switch (tool) {
    case PEN_1:
    drawing[x][y] = drawColor;
    if(x < DRAW_MAX_X-1) drawing[x+1][y] = drawColor;
    if(x > 0) drawing[x-1][y] = drawColor;
    if(y < DRAW_MAX_Y-1) drawing[x][y+1] = drawColor;
    if(y > 0) drawing[x][y-1] = drawColor;

    if(y > 0 && x > 0) drawing[x-1][y-1] = drawColor;
    if(y > 0 && x < DRAW_MAX_X-1) drawing[x+1][y-1] = drawColor;
    if(y < DRAW_MAX_Y-1 && x > 0) drawing[x-1][y+1] = drawColor;
    if(y < DRAW_MAX_Y-1 && x < DRAW_MAX_X-1) drawing[x+1][y+1] = drawColor;

    break;
    default: break;
  }
}

void DrawingPage::drawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2){
  unsigned int cell = 0;
  switch (tool) {
    case PEN_1:
    cell = 1;
    break;
    default: break;
  }
  unsigned int distance = sqrt( pow((int)(x2-x1), 2) + pow((int)(y2-y1), 2));
  unsigned int k = distance/cell;
  for (unsigned int ratio = 1; ratio <= k; ratio++) {
    map(x1, y1, x2, y2, (float) ratio/k);
  }
}

void DrawingPage::map(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, float ratio) {
  int vec_x = x2 - x1;
  int vec_y = y2 - y1;

  unsigned int pix_x = x1 + (int)(ratio*vec_x);
  unsigned int pix_y = y1 + (int)(ratio*vec_y);

  drawPixel(pix_x, pix_y);
}

void DrawingPage::update(){
  u32 kDown = hidKeysDown();

  if(kDown & KEY_A) {
    keyboardInput(title, TITLE_MAX_LENGTH + 1, "Enter  the title.");
  }

  //Read the touch screen coordinates
  hidTouchRead(&touch);
  if(touch.px > 0 && touch.py > 0){
    curX = touch.px;
    curY = touch.py;
    switch (checkButtonInput()){
      case DRAW_INPUT:
      if(curX2 != 0 && curY2 != 0){
        drawLine(curX2, curY2-20, curX, curY-20);
      }
      drawPixel(curX, curY-20);
      curX2 = curX;
      curY2 = curY;
      break;
      default: break;
    }

  }
  if(touch.px == 0 && touch.py == 0){
    if(curX != 0 && curY != 0){
      input = true;
    }
  }
  if(input){
    switch (checkButtonInput()){
      case NO_INPUT: break;
      default: break;
    }
    input = false;
    curX = 0;
    curY = 0;
    curX2 = 0;
    curY2 = 0;
  }
}

void DrawingPage::draw(){
  C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
  C2D_TargetClear(top, clrClear);
  C2D_SceneBegin(top);
  C2D_DrawSprite(&sprites[0]);
  message("Press A to add a title.");

  C2D_SceneBegin(bottom);
  C2D_TargetClear(bottom, clrClear);
  for(unsigned int y = 0; y < DRAW_MAX_Y; y++){
    for(unsigned int x = 0; x < DRAW_MAX_X; x++){
      C2D_DrawRectSolid(x, y + 20, 1.0f, 1.0f, 1.0f, drawing[x][y]);
    }
  }
  //C2D_Flush();
  C3D_FrameEnd(0);
}

ButtonInput DrawingPage::checkButtonInput() const{
  ButtonInput button = NO_INPUT;
  if(curX >= 0 && curX <= 320){
    if(curY >= 20 && curY <= 220){
      button = DRAW_INPUT;
    }
  }
  return button;
}
