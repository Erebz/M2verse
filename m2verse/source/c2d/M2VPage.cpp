#include "M2VPage.h"

M2VPage::~M2VPage(){

}

ConnectionPage::~ConnectionPage(){

}


void ConnectionPage::run(){
  //this->init();
  this->loop();
}

void ConnectionPage::draw(){

}

void ConnectionPage::loop(){
  u32 clrWhite = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);
  u32 clrGreen = C2D_Color32(0x00, 0xFF, 0x00, 0xFF);
  u32 clrRed   = C2D_Color32(0xFF, 0x00, 0x00, 0xFF);
  u32 clrBlue  = C2D_Color32(0x00, 0x00, 0xFF, 0xFF);

  u32 clrCircle1 = C2D_Color32(0xFF, 0x00, 0xFF, 0xFF);
  u32 clrCircle2 = C2D_Color32(0xFF, 0xFF, 0x00, 0xFF);
  u32 clrCircle3 = C2D_Color32(0x00, 0xFF, 0xFF, 0xFF);

  u32 clrSolidCircle = C2D_Color32(0x68, 0xB0, 0xD8, 0xFF);

  u32 clrTri1 = C2D_Color32(0xFF, 0x15, 0x00, 0xFF);
  u32 clrTri2 = C2D_Color32(0x27, 0x69, 0xE5, 0xFF);

  u32 clrRec1 = C2D_Color32(0x9A, 0x6C, 0xB9, 0xFF);
  u32 clrRec2 = C2D_Color32(0xFF, 0xFF, 0x2C, 0xFF);
  u32 clrRec3 = C2D_Color32(0xD8, 0xF6, 0x0F, 0xFF);
  u32 clrRec4 = C2D_Color32(0x40, 0xEA, 0x87, 0xFF);

  u32 clrClear = C2D_Color32(0xFF, 0xD8, 0xB0, 0x68);

  // Main loop
  while (aptMainLoop())
  {
    hidScanInput();

    // Respond to user input
    u32 kDown = hidKeysDown();
    if (kDown & KEY_START)
    break; // break in order to return to hbmenu

    // Render the scene
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(top, clrClear);
    C2D_SceneBegin(top);

    C2D_DrawTriangle(50 / 2, TOP_MAX_Y - 50, clrWhite,0,  TOP_MAX_Y, clrTri1, 50, TOP_MAX_Y, clrTri2, 0);
    C2D_DrawRectangle(TOP_MAX_X - 50, 0, 0, 50, 50, clrRec1, clrRec2, clrRec3, clrRec4);
    C2D_DrawEllipse(0, 0, 0, TOP_MAX_X, TOP_MAX_Y, clrCircle1, clrCircle2, clrCircle3, clrWhite);
    C2D_DrawCircle(TOP_MAX_X / 2, TOP_MAX_Y / 2, 0, 50, clrCircle3, clrWhite, clrCircle1, clrCircle2);
    C2D_DrawCircle(25, 25, 0, 25,clrRed, clrBlue, clrGreen, clrWhite);
    C2D_DrawCircleSolid(TOP_MAX_X - 25, TOP_MAX_Y - 25, 0, 25, clrSolidCircle);
    C3D_FrameEnd(0);
  }
}
