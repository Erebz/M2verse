#ifndef M2V_POST_H
#define M2V_POST_H

#include "M2VData.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>

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


#endif //M2V_POST_H
