#ifndef MODS_H_
#define MODS_H_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "struct.h"

/*
 * Var : t_mod
 *
 * Desc : Contient la description d'un mod
 *		
 */
typedef struct 	s_mod{
	char* 					name; 		// Nom du mod
	char* 					path;   	// Emplacement du mode (nom du dossier)
	int *					deroulement;// deroulement des niveaux [1, 2, 3]
	t_liste_niveaux*		levels; 	// Liste des levels
	t_liste_niveaux*		lvl; 		// pointeur vers level actuel
	t_liste_vaisseaux*		ships;  	// liste des vaisseaux du mod
	t_liste_vaisseaux*		player;     // Notre vaisseau
	int 					score;		// Score du jeu
	int 					level;		// Level actuel
} 				t_mod;





/****************************** t_mod Fonctions ******************************/
char* get_mod_name(char* dir_name);

char*** get_mods_list();


/****************************** Infos jeu Fonctions ******************************/
int game_over();

int level_up(t_mod *m);

int you_win();

int get_freq_tir(float freq);

#endif