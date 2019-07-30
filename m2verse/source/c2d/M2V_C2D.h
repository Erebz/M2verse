#ifndef M2V_C2D_H
#define M2V_C2D_H

#include "../core/M2VPost.h"
#include "../core/M2VData.h"
#include "M2VPage.h"
#include "ProfilePage.h"
#include "DrawingPage.h"
#include <citro2d.h>

class M2V_C2D{
private:
  User user;
  Pages page;
  M2VPage * currentPage = NULL;
  C3D_RenderTarget * top = NULL;
  C3D_RenderTarget * bottom = NULL;
  void init();
  void quit();
  void changePage();

public:
  M2V_C2D();
  ~M2V_C2D();
  void run();
  const User& getUser() const{
    return this->user;
  }
};

#endif // M2V_C2D_H
