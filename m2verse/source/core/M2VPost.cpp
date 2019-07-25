#include "M2VPost.h"

M2VTextPost::M2VTextPost(){

}

M2VTextPost::M2VTextPost(char * author, Date postDate, char * title, char * text){
  this->username = author;
  this->date = postDate;
  this->title = title;
  this->text = text;
}

M2VTextPost::~M2VTextPost(){
  this->username = NULL;
  this->title = NULL;
  this->text = NULL;
}

char * M2VTextPost::getUsername(){
  return this->username;
}
char * M2VTextPost::getTitle(){
  return this->title;
}
char * M2VTextPost::getText(){
  return this->text;
}
