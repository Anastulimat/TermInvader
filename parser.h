#ifndef PARSER_H_
#define PARSER_H_

#define SHIP_DIMS 1
#define SHIP_OCCR 2
#define SHIP_MOVX 3
#define SHIP_MOVY 4
#define SHIP_LIFE 5
#define SHIP_FREQ 6
#define SHIP_SPED 7
#define SHIP_POWR 8
#define SHIP_SYMB 9
#define SHIP_SHIP 10

#include "struct.h"
#include "mods.h"

/****************************** Parser & Construction Fonctions ******************************/
void 	percent_to_absolut(int *x, int *y);

void 	absolut_to_percent(int *x, int *y);

void 	my_strclean(char *);

char 	*my_splitstr(char* str, char *sep);

int 	*get_nbs_in_line(char*line, int nb, char *sep);

int 	get_nbr_vaisseaux(char** file, t_liste_niveaux *vaisseau);

void 	fill_mod(t_mod *m);

int s_niveau_array_size(struct s_niveau_ship *arr);

t_liste_vaisseaux* build_vaisseaux_list(t_mod *m, t_liste_niveaux * level);

#endif