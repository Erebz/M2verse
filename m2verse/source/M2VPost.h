#ifndef M2V_POST
#define M2V_POST

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>
#define USERNAME_MAX_LENGTH 15
#define TITLE_MAX_LENGTH 30
#define TEXT_MAX_LENGTH 144
#define DRAW_MAX_X 100
#define DRAW_MAX_Y 50

struct Date {
	int day = 1;
	int month = 1;
	int year = 2019;
	int hour = 0;
	int minute = 0;
};

class M2VTextPost{
private:
  //char username[USERNAME_MAX_LENGTH];
	char * username;
	//int usernameSize;
	//char title[TITLE_MAX_LENGTH];
  //char text[TEXT_MAX_LENGTH];
  char * title;
  //int titleSize;
  char * text;
  //int textSize;
	Date date;

public:
  M2VTextPost();
	M2VTextPost(char * author, Date postDate, char * title, char * text);
  ~M2VTextPost();

	char * getUsername();
	char * getTitle();
	char * getText();
};

class M2VDrawingPost{
private:
  char username[USERNAME_MAX_LENGTH];
  char title[TITLE_MAX_LENGTH];
  bool pixel[DRAW_MAX_X][DRAW_MAX_Y];
  //char title;
  //int titleSize
	Date date;

public:
  M2VDrawingPost();
  ~M2VDrawingPost();
};


#endif //M2V_POST
