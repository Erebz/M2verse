#include "M2V_C2D.h"

M2V_C2D::M2V_C2D(){
  this->top = NULL;
  this->bottom = NULL;
  this->user.username = "Thibaut";
}

M2V_C2D::~M2V_C2D(){
  if(this->currentPage != NULL) delete this->currentPage;
  this->currentPage = NULL;
  if(this->top != NULL) delete this->top;
  this->top = NULL;
  if(this->bottom != NULL) delete this->bottom;
  this->bottom = NULL;
}

void M2V_C2D::init(){
  romfsInit();
  gfxInitDefault();
  C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
  C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
  C2D_Prepare();

  this->top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
  this->bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
  this->currentPage = new ConnectionPage(this->top, this->bottom);
  this->page = CONNECTION_PAGE;
}

void M2V_C2D::quit(){
  if(currentPage != NULL) delete currentPage;
  currentPage = NULL;
  C2D_Fini();
  C3D_Fini();
  gfxExit();
  romfsExit();
}

void M2V_C2D::run(){
  this->init();
  while(aptMainLoop() && this->currentPage != NULL){
    //Treat inputs
    hidScanInput();
    u32 kDown = hidKeysDown();
    if (kDown & KEY_START) break;

    Pages page = this->currentPage->run();
    gspWaitForVBlank();
    if(this->page != page){
      this->page = page;
      this->changePage();
    }
  }
  this->quit();
}

void M2V_C2D::changePage(){
  if(currentPage != NULL) delete currentPage;
  currentPage = NULL;
  switch (this->page) {
    case CONNECTION_PAGE:
      this->currentPage = new ConnectionPage(this->top, this->bottom);
    break;
    case CONNECTION_VALID:
      user = ((ConnectionPage*) this->currentPage)->getUser();
      this->page = PROFILE_PAGE;
      this->currentPage = new ProfilePage(this->user, this->top, this->bottom);
    break;
    case PROFILE_PAGE:
      this->currentPage = new ProfilePage(this->user, this->top, this->bottom);
    break;
    default:
    break;
  }
}
