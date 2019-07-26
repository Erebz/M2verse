#ifndef PROFILE_PAGE_H
#define PROFILE_PAGE_H

#include <citro2d.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../core/M2VPost.h"
#include "../core/M2VData.h"
#include "M2VPage.h"

class ProfilePage : public M2VPage{
private:
  void draw();
  void update();
  void init();
  User user;
  virtual ButtonInput checkButtonInput() const{
    return NO_INPUT;
  }
public:
  ProfilePage();
  ProfilePage(const User& user, C3D_RenderTarget * top = NULL, C3D_RenderTarget * bottom = NULL) : M2VPage(top, bottom) {
    this->user = user;
    this->init();
  }
  virtual ~ProfilePage(){}
  Pages run();
};

#endif // PROFILE_PAGE_H
