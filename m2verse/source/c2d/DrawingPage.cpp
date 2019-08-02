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
  C2D_SpriteSetScale(&sprites[0], 0.34f, 0.34f);
  C2D_SpriteSetPos(&sprites[1], 0, 0);

  clearDrawing();
  save();
}

void DrawingPage::clearDrawing(){
  for(unsigned int y = 0; y < DRAW_MAX_Y; y++){
    for(unsigned int x = 0; x < DRAW_MAX_X; x++){
      drawing[x][y] = clrWhite;
    }
  }
}

void DrawingPage::save(){
  for(unsigned int y = 0; y < DRAW_MAX_Y; y++){
    for(unsigned int x = 0; x < DRAW_MAX_X; x++){
      drawing2[x][y] = drawing[x][y];
    }
  }
}

void DrawingPage::undo(){
  u32 tampon[DRAW_MAX_X][DRAW_MAX_Y];
  for(unsigned int y = 0; y < DRAW_MAX_Y; y++){
    for(unsigned int x = 0; x < DRAW_MAX_X; x++){
      tampon[x][y] = drawing2[x][y];
      drawing2[x][y] = drawing[x][y];
      drawing[x][y] = tampon[x][y];
    }
  }
}

bool DrawingPage::isEmpty(){
  bool isEmpty = true;
  for(unsigned int y = 0; y < DRAW_MAX_Y; y++){
    for(unsigned int x = 0; x < DRAW_MAX_X; x++){
      if (drawing[x][y] != WHITE) isEmpty = false;
    }
  }
  return isEmpty;
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

  if(kDown & KEY_Y) undo();

  //Read the touch screen coordinates
  hidTouchRead(&touch);
  if(!colorPickerOpen){
    if(touch.px > 0 && touch.py > 0){
      curX = touch.px;
      curY = touch.py;
      if (checkButtonInput() == DRAW_INPUT){
        if(curX2 == 0 && curY2 == 0){
          save();}
          if(curX2 != 0 && curY2 != 0){
            drawLine(curX2, curY2-20, curX, curY-20);
          }
          drawPixel(curX, curY-20);
          curX2 = curX;
          curY2 = curY;
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
          if(tool >= ERASER_1 && tool <= ERASER_3) this->drawColor = BLACK;
          this->tool = PEN_1;
          break;
          case PEN_2_INPUT:
          if(tool >= ERASER_1 && tool <= ERASER_3) this->drawColor = BLACK;
          this->tool = PEN_2;
          break;
          case PEN_3_INPUT:
          if(tool >= ERASER_1 && tool <= ERASER_3) this->drawColor = BLACK;
          this->tool = PEN_3;
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
          if(tool >= ERASER_1 && tool <= ERASER_3) this->tool = PEN_2;
          this->drawColor = BLACK;
          break;
          case RED_INPUT:
          if(tool >= ERASER_1 && tool <= ERASER_3) this->tool = PEN_2;
          this->drawColor = RED;
          break;
          case GREEN_INPUT:
          if(tool >= ERASER_1 && tool <= ERASER_3) this->tool = PEN_2;
          this->drawColor = GREEN;
          break;
          case BLUE_INPUT:
          if(tool >= ERASER_1 && tool <= ERASER_3) this->tool = PEN_2;
          this->drawColor = BLUE;
          break;
          case COLOR_PICKER_INPUT:
          if(tool >= ERASER_1 && tool <= ERASER_3) this->tool = PEN_2;
          if(drawColor != colorPick1 || drawColor != colorPick2) drawColor = colorPick1;
          colorPickerOpen = true;
          break;
          case CLR_PICK_1_INPUT:
          if(tool >= ERASER_1 && tool <= ERASER_3) this->tool = PEN_2;
          this->drawColor = colorPick1;
          select1 = true;
          break;
          case CLR_PICK_2_INPUT:
          if(tool >= ERASER_1 && tool <= ERASER_3) this->tool = PEN_2;
          this->drawColor = colorPick2;
          select1 = false;
          break;
          case CLEAR_INPUT:
          if(!isEmpty()) save();
          clearDrawing();
          break;
          case UNDO_INPUT:
          undo();
          break;
          case DRAW_INPUT:
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
    }else{
      if(touch.px > 0 && touch.py > 0){
        curX = touch.px;
        curY = touch.py;
        switch (checkButtonInput()){
          case RED_PICKER_INPUT:
          //update color
          if(select1) r1 = ((float)(curX-100)/170) * 255;
          else r2 = ((float)(curX-100)/170) * 255;
          colorPick1 = C2D_Color32(r1, g1, b1, 0xFF);
          colorPick2 = C2D_Color32(r2, g2, b2, 0xFF);
          break;
          case GREEN_PICKER_INPUT:
          //update color
          if(select1) g1 = ((float)(curX-100)/170) * 255;
          else g2 = ((float)(curX-100)/170) * 255;
          colorPick1 = C2D_Color32(r1, g1, b1, 0xFF);
          colorPick2 = C2D_Color32(r2, g2, b2, 0xFF);
          break;
          case BLUE_PICKER_INPUT:
          //update color
          if(select1) b1 = ((float)(curX-100)/170) * 255;
          else b2 = ((float)(curX-100)/170) * 255;
          colorPick1 = C2D_Color32(r1, g1, b1, 0xFF);
          colorPick2 = C2D_Color32(r2, g2, b2, 0xFF);
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
          case COLOR_PICKER_INPUT:
          colorPickerOpen = false;
          if(select1) this->drawColor = colorPick1;
          else this->drawColor = colorPick2;
          break;
          case CLR_PICK_1_INPUT:
          if(tool >= ERASER_1 && tool <= ERASER_3) this->tool = PEN_2;
          this->drawColor = colorPick1;
          select1 = true;
          break;
          case CLR_PICK_2_INPUT:
          if(tool >= ERASER_1 && tool <= ERASER_3) this->tool = PEN_2;
          this->drawColor = colorPick2;
          select1 = false;
          break;
          default:break;
        }
        input = false;
        curX = 0;
        curY = 0;
        curX2 = 0;
        curY2 = 0;
      }
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
    if(!colorPickerOpen){
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
      C2D_DrawRectSolid(97, 221, 0.0f, 18.0f, 18.0f, clrBox);
      C2D_DrawRectangle(100, 224, 0.0f, 12.0f, 12.0f, BLUE, RED, GREEN, YELLOW);
      if(select1){
        C2D_DrawRectSolid(123, 221, 0.0f, 18.0f, 18.0f, clrBox2);
        C2D_DrawRectSolid(124, 222, 0.0f, 16.0f, 16.0f, clrBox);
        C2D_DrawRectSolid(126, 224, 0.0f, 12.0f, 12.0f, colorPick1);
      }else {
        C2D_DrawRectSolid(123, 221, 0.0f, 18.0f, 18.0f, clrBox);
        C2D_DrawRectSolid(126, 224, 0.0f, 12.0f, 12.0f, colorPick1);
      }
      if(!select1){
        C2D_DrawRectSolid(144, 221, 0.0f, 18.0f, 18.0f, clrBox2);
        C2D_DrawRectSolid(145, 222, 0.0f, 16.0f, 16.0f, clrBox);
        C2D_DrawRectSolid(147, 224, 0.0f, 12.0f, 12.0f, colorPick2);
      }else {
        C2D_DrawRectSolid(144, 221, 0.0f, 18.0f, 18.0f, clrBox);
        C2D_DrawRectSolid(147, 224, 0.0f, 12.0f, 12.0f, colorPick2);
      }
      textWrite("M2V v0.1", 170,225,0.5,0.5,BLACK);
      C2D_DrawSprite(&sprites[1]);

      //render canvas
      for(unsigned int y = 0; y < DRAW_MAX_Y; y++){
        for(unsigned int x = 0; x < DRAW_MAX_X; x++){
          C2D_DrawRectSolid(x, y + 20, 1.0f, 1.0f, 1.0f, drawing[x][y]);
        }
      }
    }else{
      textWrite("RGB Color Picker", 57, 10, 1, 1, clrText);
      C2D_DrawRectSolid(97, 221, 0.0f, 18.0f, 18.0f, RED);
      textWrite("X",102,221,0.7,0.7,WHITE);
      //C2D_DrawRectangle(100, 224, 0.0f, 12.0f, 12.0f, BLUE, RED, GREEN, YELLOW);
      if(select1){
        C2D_DrawRectSolid(123, 221, 0.0f, 18.0f, 18.0f, clrBox2);
        C2D_DrawRectSolid(124, 222, 0.0f, 16.0f, 16.0f, clrBox);
        C2D_DrawRectSolid(126, 224, 0.0f, 12.0f, 12.0f, colorPick1);
      }else {
        C2D_DrawRectSolid(123, 221, 0.0f, 18.0f, 18.0f, clrBox);
        C2D_DrawRectSolid(126, 224, 0.0f, 12.0f, 12.0f, colorPick1);
      }
      if(!select1){
        C2D_DrawRectSolid(144, 221, 0.0f, 18.0f, 18.0f, clrBox2);
        C2D_DrawRectSolid(145, 222, 0.0f, 16.0f, 16.0f, clrBox);
        C2D_DrawRectSolid(147, 224, 0.0f, 12.0f, 12.0f, colorPick2);
      }else {
        C2D_DrawRectSolid(144, 221, 0.0f, 18.0f, 18.0f, clrBox);
        C2D_DrawRectSolid(147, 224, 0.0f, 12.0f, 12.0f, colorPick2);
      }
      C2D_DrawRectSolid(40, 40, 0.0f, 240.0f, 150.0f, clrBox);
      textWrite("R :", 55, 61, 1, 1, WHITE);
      textWrite("G :", 55, 101, 1, 1, WHITE);
      textWrite("B :", 55, 141, 1, 1, WHITE);
      if(select1){
        C2D_DrawRectangle(100, 65, 1.0f, 170.0f, 20.0f, C2D_Color32(0,g1,b1,255), C2D_Color32(255,g1,b1,255), C2D_Color32(0,g1,b1,255), C2D_Color32(255,g1,b1,255));
        C2D_DrawRectangle(100, 105, 1.0f, 170.0f, 20.0f, C2D_Color32(r1,0,b1,255), C2D_Color32(r1,255,b1,255), C2D_Color32(r1,0,b1,255), C2D_Color32(r1,255,b1,255));
        C2D_DrawRectangle(100, 145, 1.0f, 170.0f, 20.0f, C2D_Color32(r1,g1,0,255), C2D_Color32(r1,g1,255,255), C2D_Color32(r1,g1,0,255), C2D_Color32(r1,g1,255,255));
        C2D_DrawRectSolid((((float)r1/255)*170)+100, 62, 1.0f, 5.0f, 26.0f, BLACK);
        C2D_DrawRectSolid((((float)g1/255)*170)+100, 102, 1.0f, 5.0f, 26.0f, BLACK);
        C2D_DrawRectSolid((((float)b1/255)*170)+100, 142, 1.0f, 5.0f, 26.0f, BLACK);
      }else{
        C2D_DrawRectangle(100, 65, 1.0f, 170.0f, 20.0f, C2D_Color32(0,g2,b2,255), C2D_Color32(255,g2,b2,255), C2D_Color32(0,g2,b2,255), C2D_Color32(255,g2,b2,255));
        C2D_DrawRectangle(100, 105, 1.0f, 170.0f, 20.0f, C2D_Color32(r2,0,b2,255), C2D_Color32(r2,255,b2,255), C2D_Color32(r2,0,b2,255), C2D_Color32(r2,255,b2,255));
        C2D_DrawRectangle(100, 145, 1.0f, 170.0f, 20.0f, C2D_Color32(r2,g2,0,255), C2D_Color32(r2,g2,255,255), C2D_Color32(r2,g2,0,255), C2D_Color32(r2,g2,255,255));
        C2D_DrawRectSolid((((float)r2/255)*170)+100, 62, 1.0f, 5.0f, 26.0f, BLACK);
        C2D_DrawRectSolid((((float)g2/255)*170)+100, 102, 1.0f, 5.0f, 26.0f, BLACK);
        C2D_DrawRectSolid((((float)b2/255)*170)+100, 142, 1.0f, 5.0f, 26.0f, BLACK);
      }

      textWrite("M2V v0.1", 170,225,0.5,0.5,BLACK);
    }
    //C2D_Flush();
    C3D_FrameEnd(0);
  }

  ButtonInput DrawingPage::checkButtonInput() const{
    ButtonInput button = NO_INPUT;
    if(!colorPickerOpen){
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
      if(curX >= 97 && curX <= 115){
        if(curY >= 221 && curY <= 240){
          button = COLOR_PICKER_INPUT;
        }
      }
      if(curX >= 123 && curX <= 141){
        if(curY >= 221 && curY <= 240){
          button = CLR_PICK_1_INPUT;
        }
      }
      if(curX >= 144 && curX <= 162){
        if(curY >= 221 && curY <= 240){
          button = CLR_PICK_2_INPUT;
        }
      }
    }else{
      if(curX >= 97 && curX <= 115){
        if(curY >= 221 && curY <= 240){
          button = COLOR_PICKER_INPUT;
        }
      }
      if(curX >= 123 && curX <= 141){
        if(curY >= 221 && curY <= 240){
          button = CLR_PICK_1_INPUT;
        }
      }
      if(curX >= 144 && curX <= 162){
        if(curY >= 221 && curY <= 240){
          button = CLR_PICK_2_INPUT;
        }
      }
      if(curX >= 100 && curX <= 270){
        if(curY >= 65 && curY <= 85){
          button = RED_PICKER_INPUT;
        }
      }
      if(curX >= 100 && curX <= 270){
        if(curY >= 105 && curY <= 125){
          button = GREEN_PICKER_INPUT;
        }
      }
      if(curX >= 100 && curX <= 270){
        if(curY >= 145 && curY <= 165){
          button = BLUE_PICKER_INPUT;
        }
      }
    }
    return button;
  }
