#include "M2V_C2D.h"

int main(int argc, char* argv[]){
  M2V_C2D * app = NULL;
  app = new M2V_C2D();
  app->run();
  delete app;
  return 0;
}
