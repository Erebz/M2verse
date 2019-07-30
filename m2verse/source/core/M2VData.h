#ifndef M2V_DATA_H
#define M2V_DATA_H

#define USERNAME_MAX_LENGTH 15
#define PWD_MAX_LENGTH 15
#define TITLE_MAX_LENGTH 30
#define TEXT_MAX_LENGTH 144
#define DRAW_MAX_X 320
#define DRAW_MAX_Y 200
#define MAX_KBD_INPUT 144
#define TOP_MAX_Y 240
#define TOP_MAX_X 400
#define BOTTOM_MAX_Y 240
#define BOTTOM_MAX_X 320

enum Region : int{
  NONE = 0,
  JPN,
  EUR,
  NA,
  OTHER
};

enum Pages : int{
  CONNECTION_PAGE = 0,
  CONNECTION_VALID,
  ERROR_PAGE,
  PROFILE_PAGE,
  DRAWING_PAGE
};

enum ButtonInput : int{
  NO_INPUT = 0,
  PASSWORD_INPUT,
  USERNAME_INPUT,
  CONNECTION_INPUT,
  DRAW_INPUT
};

enum DrawTools : int {
  PEN_1 = 0,
  PEN_2,
  PEN_3,
  ERASER_1,
  ERASER_2,
  ERASER_3
};

/*enum Color : u32{
  GREEN = C2D_Color32(0x83, 0xEC, 0x8A, 0xFF);
  BLUE = C2D_Color32(0x00, 0x00, 0xFF, 0xFF);
  u32 clrClear = C2D_Color32(0x04, 0x2D, 0x30, 0xFF);
  u32 clrWhite = C2D_Color32(0x00, 0x00, 0x00, 0xFF);
  u32 clrBox = C2D_Color32(0x09, 0x5A, 0x60, 0xC8);
  u32 clrBox2 = C2D_Color32(0x84, 0xAC, 0xAF, 0xC8);
  u32 clrText = C2D_Color32(0x8B, 0x8F, 0x52, 0xFF);
};*/

struct User{
  //char username[USERNAME_MAX_LENGTH];
  char * username;
  int id = 0;
  char pwd[PWD_MAX_LENGTH];
  bool connected = false;
  Region region = NONE;
  unsigned int followers = 0;
};

struct Date {
	int day = 1;
	int month = 1;
	int year = 2019;
	int hour = 0;
	int minute = 0;
};

#endif //M2V_DATA_H
