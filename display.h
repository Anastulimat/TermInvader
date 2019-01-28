#ifndef DISPLAY_H_
#define DISPLAY_H_
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "struct.h"
#ifndef MODS_H_
# include "mods.h"
#endif


#if defined(_WIN32) || defined(__MSDOS__)
   #define SPADE   "\x06"
   #define CLUB    "\x05"
   #define HEART   "\x03"
   #define DIAMOND "\x04"
#else
   #define SPADE   "\xE2\x99\xA0"
   #define CLUB    "\xE2\x99\xA3"
   #define HEART   "\xE2\x99\xA5"
   #define DIAMOND "\xE2\x99\xA6"
#endif



/****************************** Display Fonctions ******************************/
t_pt 				*size_terminal();

void 				put_slow_str_to(int x, int y, char *s, int intervall);

void 				put_char_to(char c, int x, int y);

void 				put_str_to(char* s, int x, int y);

void 				put_blink_str_to(char* s, int x, int y);

void 				put_title_to(char* s, int x, int y);

void 				put_menuline_to(char* s, int x, int y);

void 				clean_screen();

void 				erase_vaisseau(t_liste_vaisseaux *ship);

void 				erase_tir(t_tir *t);

void 				explode_ship(t_liste_vaisseaux * ship, int);

void 				print_vaisseau_line(char* l);

void 				print_vaisseau(t_liste_vaisseaux* vaisseau);

void 				move_vaisseau(t_liste_vaisseaux *ship, int x, int y);

void 				move_vaisseau_IA(t_liste_vaisseaux *ship);

t_liste_vaisseaux	*move_liste_vaisseaux_IA(t_liste_vaisseaux *ships, t_mod *);

void 				move_list_tir(t_mod *);

void 				check_player_shooted(t_mod * m);

void 				print_list_vaisseau(t_liste_vaisseaux *vaisseau);

void 				check_pos_vaisseau(t_liste_vaisseaux *v, int min, int max);


#endif