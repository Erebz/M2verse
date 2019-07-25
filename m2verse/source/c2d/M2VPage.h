#ifndef M2V_PAGE_H
#define M2V_PAGE_H

#include <citro2d.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../core/M2VPost.h"
#include "../core/M2VData.h"

class M2VPage{
private:
  //
protected:
  //M2V_C2D * app = NULL;
  C3D_RenderTarget * top = NULL;
  C3D_RenderTarget * bottom = NULL;
  virtual void draw() = 0;
  virtual void loop() = 0;
  void message(char * message, unsigned int messageSize);
  void dialog();
  void question();

public:
  M2VPage(C3D_RenderTarget * top = NULL, C3D_RenderTarget * bottom = NULL) : top(top), bottom(bottom) {}
  M2VPage();
  virtual ~M2VPage();
  virtual void run() = 0;
};

class ConnectionPage : public M2VPage{
private:
  //fonctions de requetes http pour la connexion
  void draw();
  void loop();
  //void init();
public:
  ConnectionPage();
  ConnectionPage(C3D_RenderTarget * top = NULL, C3D_RenderTarget * bottom = NULL) : M2VPage(top, bottom) {}
  virtual ~ConnectionPage();

  void run();
};



#endif // M2V_PAGE_H
