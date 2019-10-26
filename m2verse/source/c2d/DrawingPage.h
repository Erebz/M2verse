#ifndef DRAWING_PAGE_H
#define DRAWING_PAGE_H

#include <citro2d.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../core/M2VPost.h"
#include "../core/M2VData.h"
#include "M2VPage.h"
#include <inttypes.h>

class DrawingPage : public M2VPage{
private:
  void draw();
  void update();
  void init();
  char title[TITLE_MAX_LENGTH + 1] = "Untitled";
  u32 drawing[DRAW_MAX_X][DRAW_MAX_Y];
  u32 drawing2[DRAW_MAX_X][DRAW_MAX_Y];
  DrawTools tool = PEN_1;
  u32 drawColor = BLACK;
  int r1 = 0, g1 = 0, b1 = 0;
  int r2 = 200, g2 = 12, b2 = 70;
  u32 colorPick1 = C2D_Color32(r1, g1, b1, 0xFF);
  u32 colorPick2 = C2D_Color32(r2, g2, b2, 0xFF);
  bool select1 = true;
  bool colorPickerOpen = false;
  unsigned int curX2=0, curY2=0;
  Result res;
  ButtonInput checkButtonInput() const;
  void clearDrawing();
  void save();
  void undo();
  bool isEmpty();
  void drawPixel(unsigned int x, unsigned int y);
  void drawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
  void map(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, float ratio);
public:
  DrawingPage();
  DrawingPage(C3D_RenderTarget * top = NULL, C3D_RenderTarget * bottom = NULL) : M2VPage(top, bottom) {
    this->init();
  }
  virtual ~DrawingPage(){}
  Pages run();
};

#endif // DRAWING_PAGE_H
