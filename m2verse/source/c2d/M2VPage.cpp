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
  u32 clrGreen = C2D_Color32(0x00, 0xFF, 0x00, 0xFF);
  u32 clrBlue  = C2D_Color32(0x00, 0x00, 0xFF, 0xFF);

  u32 clrClear = C2D_Color32(0x00, 0xFF, 0x00, 0xFF);

  C2D_TextBuf g_staticBuf;
  C2D_Text g_staticText;
  g_staticBuf  = C2D_TextBufNew(4096);
  C2D_TextParse(&g_staticText, g_staticBuf, "Bienvenue dans Miiverse 2 !");
  C2D_TextOptimize(&g_staticText);




  // Main loop
  while (aptMainLoop())
  {
    //this->draw();
    hidScanInput();

    // Respond to user input
    u32 kDown = hidKeysDown();
    if (kDown & KEY_START)
    break; // break in order to return to hbmenu

    // Render the scene
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(top, clrClear);
    C2D_SceneBegin(top);
    C2D_DrawRectangle(20, TOP_MAX_Y-100, 0, TOP_MAX_X-40, 80, clrBlue, clrBlue, clrBlue, clrBlue);

    // Draw static text strings
    float text2PosX = 400.0f - 16.0f - g_staticText.width*0.75f; // right-justify
    C2D_DrawText(&g_staticText, 0, 30.0f, 150.0f, 0.0f, 0.8f, 0.8f);

    C3D_FrameEnd(0);
  }
	C2D_TextBufDelete(g_staticBuf);
}
