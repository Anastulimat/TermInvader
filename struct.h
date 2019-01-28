#ifndef STRUCT_H_
#define STRUCT_H_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define DIR_UP 1
#define DIR_DOWN -1


typedef char**	 	t_vaisseau; // Tableau de char à deux dimension qui va contenir les vaisseaux


/*
 * Var : t_pt
 *
 * Desc : Point avec coordonées x et y
 *		
 */
 typedef struct 	s_pt
 {
 	int x; // Colonnes
 	int y; // Lignes
 }t_pt;


/*
 * Var : t_tir
 *
 * Desc : Liste chainées de tirs
 *	  	  Contient la description d'un tir
 *
 */

typedef struct 		s_tir
{
	t_pt			pos;		// pos x/y du tir
	int				vitesse;	// vitesse en case/sec
	int				puissance;	// Degat du tir
	float			freq;		// Frequence de tir
	char			sym;		// Representation visuel Char
	char			dir;		// Direction (^ / v )
	struct s_tir	*next;		// Tir suivant lorsque liste
} t_tir;


 /*
 * Var : t_liste_vaisseaux
 *
 * Desc : Liste chainées de vaisseaux
 *	  	  Contient la description d'un vaisseau
 *
 */
 typedef struct 	s_liste_vaisseaux
 {
 	t_vaisseau 					vaisseau; 	// char **
 	int 						hauteur;	// Taille Y
 	int 						largeur;	// Taille X
 	int 						life;		// Vie restantes
 	int							longueur_cycle_deplacement;	// nombre de tour par liste deplacement
 	int							*deplacement_horizontal;	// liste deplacement X
 	int							*deplacement_vertical;		// liste deplacement Y
 	t_tir 						tir;		// Modele de tir de ce vaisseau.
 	t_pt						pos;		// Pos x/y du vaisseau
 	int 						counter; 	// Nombre de tour depuis le debut
 	int 						offset; 	// Offset de debut
 	struct s_liste_vaisseaux	*suivant;	// Prochain vaisseau
 } t_liste_vaisseaux;


/*
 * Var : struct s_niveau_ship
 *
 * Desc : Type de vaisseau 
 *
 */

struct s_niveau_ship 	// Type de vaisseau 
{
 	int			type;	// type (1, 2, 3, etc.)
 	float		offset; // Nombre de tour avant affichage du vaisseau sur le level du mod
 	t_pt		pos;	// position de démarrage du vaisseau
};


 /*
 * Var : t_liste_niveaux
 *
 * Desc : Liste chainées de niveaux
 *	  	  Contient la description d'un niveau
 *
 */
 typedef struct 	s_liste_niveaux
 {
 	t_liste_vaisseaux			*list_vaisseaux; 	// Liste ordonee en ordre de passage, des objets vaisseau
 	t_tir 						*tirs;				// Liste des tirs du niveau
 	int							nbr_vaisseaux;		// Nombre de vaisseau dans le niveau
 	struct s_niveau_ship		*vaisseaux; 		// Liste ordonée d'ordre de passage des vaisseau
 	struct s_liste_niveaux		*suivant;			// Level Up
 } t_liste_niveaux;








/****************************** t_pt Fonctions ******************************/
t_pt *pt_new(int x, int y);

void pt_copy(t_pt *from, t_pt *to);




/****************************** liste_niveaux Fonctions ******************************/
t_liste_niveaux 	*list_niveaux_next(t_liste_niveaux *list);

t_liste_niveaux		*list_niveaux_last_elem(t_liste_niveaux *list);

void 				list_niveaux_add_elem(t_liste_niveaux *list, t_liste_niveaux *new);

t_liste_niveaux		*list_niveaux_new();

t_liste_niveaux		*list_niveaux_new_elem(t_liste_niveaux *list);





/****************************** liste_vaisseaux Fonctions ******************************/
t_liste_vaisseaux 	*list_vaisseaux_get(t_liste_vaisseaux *list, int n);

t_liste_vaisseaux	*list_vaisseaux_next(t_liste_vaisseaux *list);

t_liste_vaisseaux	*list_vaisseaux_last_elem(t_liste_vaisseaux *list);

void				list_vaisseaux_add_elem(t_liste_vaisseaux *list, t_liste_vaisseaux *new);

t_liste_vaisseaux	*list_vaisseaux_new(t_vaisseau vaisseau);

t_liste_vaisseaux	*list_vaisseaux_new_elem(t_liste_vaisseaux *list, t_vaisseau vaisseau);

t_liste_vaisseaux	*copy_vaisseau(t_liste_vaisseaux *ship);





/****************************** t_tir functions  ******************************/
t_tir 	*tir_get(t_tir *list, int n);

int  tir_exist_by_coord(t_tir *list, int x, int y);


t_tir	*tir_next(t_tir *list);

void	tir_add_elem(t_tir *list, t_tir *new);

t_tir	*tir_new();

t_tir	*shoot(t_liste_vaisseaux *from, t_tir *);

t_tir	*tir_last_elem(t_tir *list);

t_tir	*tir_new_elem(t_tir *list, t_vaisseau vaisseau);


#endif








