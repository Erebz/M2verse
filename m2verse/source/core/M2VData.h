#ifndef M2V_DATA_H
#define M2V_DATA_H

#define USERNAME_MAX_LENGTH 15
#define PWD_MAX_LENGTH 15
#define TITLE_MAX_LENGTH 30
#define TEXT_MAX_LENGTH 144
#define DRAW_MAX_X 100
#define DRAW_MAX_Y 50
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
  PROFILE_PAGE
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
