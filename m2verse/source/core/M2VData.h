#ifndef M2V_DATA_H
#define M2V_DATA_H

#include <citro2d.h>
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
  DRAW_INPUT,
  CLEAR_INPUT,
  UNDO_INPUT,
  PEN_1_INPUT,
  PEN_2_INPUT,
  PEN_3_INPUT,
  ERASER_1_INPUT,
  ERASER_2_INPUT,
  ERASER_3_INPUT,
  BLACK_INPUT,
  RED_INPUT,
  GREEN_INPUT,
  BLUE_INPUT,
};

enum DrawTools : int {
  PEN_1 = 0,
  PEN_2,
  PEN_3,
  ERASER_1,
  ERASER_2,
  ERASER_3
};

enum Color : u32{
  RED = C2D_Color32(0xFF, 0x00, 0x00, 0xFF),
  GREEN = C2D_Color32(0x00, 0xFF, 0x00, 0xFF),
  BLUE = C2D_Color32(0x00, 0x00, 0xFF, 0xFF),
  BLACK = C2D_Color32(0x00, 0x00, 0x00, 0xFF),
  WHITE = C2D_Color32(0XFF, 0XFF, 0XFF, 0xFF)
};

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
