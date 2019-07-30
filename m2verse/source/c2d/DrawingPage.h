#ifndef DRAWING_PAGE_H
#define DRAWING_PAGE_H

#include <citro2d.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../core/M2VPost.h"
#include "../core/M2VData.h"
#include "M2VPage.h"

class DrawingPage : public M2VPage{
private:
  void draw();
  void update();
  void init();
  char title[TITLE_MAX_LENGTH + 1];
  u32 drawing[DRAW_MAX_X][DRAW_MAX_Y];
  DrawTools tool = PEN_1;
  u32 drawColor = clrBlue;
  ButtonInput checkButtonInput() const;
  void drawPixel();
public:
  DrawingPage();
  DrawingPage(C3D_RenderTarget * top = NULL, C3D_RenderTarget * bottom = NULL) : M2VPage(top, bottom) {
    this->init();
  }
  virtual ~DrawingPage(){}
  Pages run();
};

#endif // DRAWING_PAGE_H
