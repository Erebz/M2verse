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
  spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/spritesDrawing.t3x");
  spriteNb = C2D_SpriteSheetCount(spriteSheet);
  assert(spriteNb > 0);
  sprites = new C2D_Sprite[spriteNb];
  for(unsigned int i=0; i<spriteNb; i++){
    C2D_SpriteFromSheet	(&sprites[i], spriteSheet, i);
  }
  C2D_SpriteSetCenter(&sprites[0], 0.5f, 0.5f);
  C2D_SpriteSetPos(&sprites[0], TOP_MAX_X/2, TOP_MAX_Y/2 - 30);
  C2D_SpriteSetScale(&sprites[0], 0.17f, 0.17f);
  C2D_SpriteSetPos(&sprites[1], 0, 0);

  clearDrawing();
}

void DrawingPage::clearDrawing(){
  for(unsigned int y = 0; y < DRAW_MAX_Y; y++){
    for(unsigned int x = 0; x < DRAW_MAX_X; x++){
      drawing[x][y] = clrWhite;
    }
  }
}

void DrawingPage::drawPixel(unsigned int x, unsigned int y){
  int boldness = 1;
  if(tool == PEN_1 || tool == ERASER_1) boldness = 1;
  if(tool == PEN_2 || tool == ERASER_2) boldness = 2;
  if(tool == PEN_3 || tool == ERASER_3) boldness = 3;
  switch (boldness) {
    case 1:
    drawing[x][y] = drawColor;
    break;
    case 2:
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
    case 3:
    drawing[x][y] = drawColor;
    if(x < DRAW_MAX_X-1) drawing[x+1][y] = drawColor;
    if(x > 0) drawing[x-1][y] = drawColor;
    if(y < DRAW_MAX_Y-1) drawing[x][y+1] = drawColor;
    if(y > 0) drawing[x][y-1] = drawColor;
    if(y > 0 && x > 0) drawing[x-1][y-1] = drawColor;
    if(y > 0 && x < DRAW_MAX_X-1) drawing[x+1][y-1] = drawColor;
    if(y < DRAW_MAX_Y-1 && x > 0) drawing[x-1][y+1] = drawColor;
    if(y < DRAW_MAX_Y-1 && x < DRAW_MAX_X-1) drawing[x+1][y+1] = drawColor;
    if(x-2 >=0) {
      drawing[x-2][y-2] = drawColor;
      drawing[x-2][y-1] = drawColor;
      drawing[x-2][y] = drawColor;
      drawing[x-2][y+1] = drawColor;
      drawing[x-2][y+2] = drawColor;
    }
    if(x+2 < DRAW_MAX_X) {
      drawing[x+2][y-2] = drawColor;
      drawing[x+2][y-1] = drawColor;
      drawing[x+2][y] = drawColor;
      drawing[x+2][y+1] = drawColor;
      drawing[x+2][y+2] = drawColor;
    }
    drawing[x-1][y-2] = drawColor;
    drawing[x][y-2] = drawColor;
    drawing[x+1][y-2] = drawColor;
    drawing[x-1][y+2] = drawColor;
    drawing[x][y+2] = drawColor;
    drawing[x+1][y+2] = drawColor;
    break;
    default: break;
  }
}

void DrawingPage::drawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2){
  unsigned int cell = 0;
  if(tool == PEN_1 || tool == ERASER_1) cell = 1;
  if(tool == PEN_2 || tool == ERASER_2) cell = 1;
  if(tool == PEN_3 || tool == ERASER_3) cell = 2;
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
      case PEN_1_INPUT:
      this->tool = PEN_1;
      this->drawColor = BLACK;
      break;
      case PEN_2_INPUT:
      this->tool = PEN_2;
      this->drawColor = BLACK;
      break;
      case PEN_3_INPUT:
      this->tool = PEN_3;
      this->drawColor = BLACK;
      break;
      case ERASER_1_INPUT:
      this->tool = ERASER_1;
      this->drawColor = WHITE;
      break;
      case ERASER_2_INPUT:
      this->tool = ERASER_2;
      this->drawColor = WHITE;
      break;
      case ERASER_3_INPUT:
      this->tool = ERASER_3;
      this->drawColor = WHITE;
      break;
      case BLACK_INPUT:
      this->drawColor = BLACK;
      break;
      case RED_INPUT:
      this->drawColor = RED;
      break;
      case GREEN_INPUT:
      this->drawColor = GREEN;
      break;
      case BLUE_INPUT:
      this->drawColor = BLUE;
      break;
      case CLEAR_INPUT:
      clearDrawing();
      break;
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
  message("Press A to publish your drawing.");

  C2D_SceneBegin(bottom);

  //render tool bar
  C2D_TargetClear(bottom, clrClear);
  C2D_DrawRectSolid(7, 1, 0.0f, 32.0f, 18.0f, clrBox);
  C2D_DrawRectSolid(47, 1, 0.0f, 30.0f, 18.0f, clrBox);
  if(tool == PEN_1){
    C2D_DrawRectSolid(93, 1, 0.0f, 18.0f, 18.0f, clrBox2);
    C2D_DrawRectSolid(94, 2, 0.0f, 16.0f, 16.0f, clrBox);
  }else C2D_DrawRectSolid(93, 1, 0.0f, 18.0f, 18.0f, clrBox);
  if(tool == PEN_2){
    C2D_DrawRectSolid(115, 1, 0.0f, 18.0f, 18.0f, clrBox2);
    C2D_DrawRectSolid(116, 2, 0.0f, 16.0f, 16.0f, clrBox);
  }else C2D_DrawRectSolid(115, 1, 0.0f, 18.0f, 18.0f, clrBox);
  if(tool == PEN_3){
    C2D_DrawRectSolid(137, 1, 0.0f, 18.0f, 18.0f, clrBox2);
    C2D_DrawRectSolid(138, 2, 0.0f, 16.0f, 16.0f, clrBox);
  }else C2D_DrawRectSolid(137, 1, 0.0f, 18.0f, 18.0f, clrBox);
  if(tool == ERASER_1){
    C2D_DrawRectSolid(164, 1, 0.0f, 18.0f, 18.0f, clrBox2);
    C2D_DrawRectSolid(165, 2, 0.0f, 16.0f, 16.0f, clrBox);
  }else C2D_DrawRectSolid(164, 1, 0.0f, 18.0f, 18.0f, clrBox);
  if(tool == ERASER_2){
    C2D_DrawRectSolid(186, 1, 0.0f, 18.0f, 18.0f, clrBox2);
    C2D_DrawRectSolid(187, 2, 0.0f, 16.0f, 16.0f, clrBox);
  }else C2D_DrawRectSolid(186, 1, 0.0f, 18.0f, 18.0f, clrBox);
  if(tool == ERASER_3){
    C2D_DrawRectSolid(208, 1, 0.0f, 18.0f, 18.0f, clrBox2);
    C2D_DrawRectSolid(209, 2, 0.0f, 16.0f, 16.0f, clrBox);
  }else C2D_DrawRectSolid(208, 1, 0.0f, 18.0f, 18.0f, clrBox);
  if(drawColor == BLACK){
    C2D_DrawRectSolid(235, 1, 0.0f, 18.0f, 18.0f, clrBox2);
    C2D_DrawRectSolid(236, 2, 0.0f, 16.0f, 16.0f, clrBox);
  }else C2D_DrawRectSolid(235, 1, 0.0f, 18.0f, 18.0f, clrBox);
  if(drawColor == RED){
    C2D_DrawRectSolid(257, 1, 0.0f, 18.0f, 18.0f, clrBox2);
    C2D_DrawRectSolid(258, 2, 0.0f, 16.0f, 16.0f, clrBox);
  }else C2D_DrawRectSolid(257, 1, 0.0f, 18.0f, 18.0f, clrBox);
  if(drawColor == GREEN){
    C2D_DrawRectSolid(279, 1, 0.0f, 18.0f, 18.0f, clrBox2);
    C2D_DrawRectSolid(280, 2, 0.0f, 16.0f, 16.0f, clrBox);
  }else C2D_DrawRectSolid(279, 1, 0.0f, 18.0f, 18.0f, clrBox);
  if(drawColor == BLUE){
    C2D_DrawRectSolid(301, 1, 0.0f, 18.0f, 18.0f, clrBox2);
    C2D_DrawRectSolid(302, 2, 0.0f, 16.0f, 16.0f, clrBox);
  }else C2D_DrawRectSolid(301, 1, 0.0f, 18.0f, 18.0f, clrBox);
  C2D_DrawSprite(&sprites[1]);

  //render canvas
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
  if(curX >= 7 && curX <= 39){
    if(curY >= 1 && curY <= 19){
      button = CLEAR_INPUT;
    }
  }
  if(curX >= 47 && curX <= 77){
    if(curY >= 1 && curY <= 19){
      button = UNDO_INPUT;
    }
  }
  if(curX >= 93 && curX <= 111){
    if(curY >= 1 && curY <= 19){
      button = PEN_1_INPUT;
    }
  }
  if(curX >= 115 && curX <= 133){
    if(curY >= 1 && curY <= 19){
      button = PEN_2_INPUT;
    }
  }
  if(curX >= 137 && curX <= 155){
    if(curY >= 1 && curY <= 19){
      button = PEN_3_INPUT;
    }
  }
  if(curX >= 164 && curX <= 182){
    if(curY >= 1 && curY <= 19){
      button = ERASER_1_INPUT;
    }
  }
  if(curX >= 186 && curX <= 204){
    if(curY >= 1 && curY <= 19){
      button = ERASER_2_INPUT;
    }
  }
  if(curX >= 208 && curX <= 226){
    if(curY >= 1 && curY <= 19){
      button = ERASER_3_INPUT;
    }
  }
  if(curX >= 235 && curX <= 253){
    if(curY >= 1 && curY <= 19){
      button = BLACK_INPUT;
    }
  }
  if(curX >= 257 && curX <= 275){
    if(curY >= 1 && curY <= 19){
      button = RED_INPUT;
    }
  }
  if(curX >= 279 && curX <= 297){
    if(curY >= 1 && curY <= 19){
      button = GREEN_INPUT;
    }
  }
  if(curX >= 301 && curX <= 320){
    if(curY >= 1 && curY <= 19){
      button = BLUE_INPUT;
    }
  }
  return button;
}
