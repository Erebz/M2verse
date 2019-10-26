/*#ifndef TEXT_PAGE_H
#define TEXT_PAGE_H

#include <citro2d.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../core/M2VPost.h"
#include "../core/M2VData.h"
#include "M2VPage.h"

class TextPage : public M2VPage{
private:
  void draw();
  void update();
  void init();
  char title[TITLE_MAX_LENGTH + 1] = "Untitled";
  char message[TEXT_MAX_LENGTH + 1];
public:
  TextPage();
  TextPage(C3D_RenderTarget * top = NULL, C3D_RenderTarget * bottom = NULL) : M2VPage(top, bottom) {
    this->init();
  }
  virtual ~TextPage(){}
  Pages run();
};

#endif // TEXT_PAGE_H
*/
