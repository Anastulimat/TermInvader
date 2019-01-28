#ifndef MENU_H_
#define MENU_H_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <poll.h>

#define K_UP		0x415B1B
#define K_DOWN		0x425B1B

#define CK_UP		0x41
#define CK_DOWN		0x42
#define CK_RIGHT	0x43
#define CK_LEFT		0x44

#define K_SPACE1	0x415B20 //0xA20
#define K_SPACE2	0x425B20 //0xA20
#define K_ENTER1	0x415B0A //0xAA
#define K_ENTER2	0x415B0A //0xAA



/****************************** Menu Fonctions ******************************/
int menu_loop();

void put_char_to(char c, int x, int y);

void put_str_to(char* s, int x, int y);

void put_blink_str_to(char* s, int x, int y);




#endif