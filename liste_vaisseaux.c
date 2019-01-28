#include "struct.h"


/*
 * Func : list_vaisseaux_get(t_liste_vaisseaux *list, int n)
 *
 * Desc : Retourne l'élément n dans list
 *		
 */
t_liste_vaisseaux 	*list_vaisseaux_get(t_liste_vaisseaux *list, int n)
{
	int i = 0;
	while(i < n && list != NULL)
	{
		list = list->suivant;
		i++;
	}
	return list;
}



/*
 * Func : list_vaisseaux_next(t_liste_vaisseaux *list)
 *
 * Desc : Retourne l'élément suivant dans list
 *		
 */
t_liste_vaisseaux	*list_vaisseaux_next(t_liste_vaisseaux *list)
{
	if(list != NULL)
	{
		return list->suivant;
	}
	return NULL;
}



/*
 * Func : list_vaisseaux_last_elem(t_liste_vaisseaux *list)
 *
 * Desc : Retourne le dernier élément dans list
 *		
 */
t_liste_vaisseaux	*list_vaisseaux_last_elem(t_liste_vaisseaux *list)
{
	while(list && list->suivant)
	{
		list = list->suivant;
	}
	return list;
}



/*
 * Func : list_vaisseaux_add_elem(t_liste_vaisseaux *list)
 *
 * Desc : Ajouter new à la fin de la list
 *		
 */
void				list_vaisseaux_add_elem(t_liste_vaisseaux *list, t_liste_vaisseaux *new)
{
	if(list && new)
	{
		list = list_vaisseaux_last_elem(list);
		list->suivant = new;
		new->suivant = NULL;
	}
}



/*
 * Func : list_vaisseaux_new(t_vaisseau vaisseau)
 *
 * Desc : Créer une nouvelle liste et la retourner
 *		  Paramètre vaisseau peut être NULL
 *		
 */
t_liste_vaisseaux	*list_vaisseaux_new(t_vaisseau vaisseau)
{
	t_liste_vaisseaux 	*new;

	if((new = malloc(sizeof(t_liste_vaisseaux))) == NULL)
		return NULL;

	new->vaisseau = vaisseau;
	new->hauteur = 0;
	new->largeur = 0;
	new->longueur_cycle_deplacement = 0;
	new->deplacement_horizontal = NULL;
	new->deplacement_vertical = NULL;
	new->tir.freq = 0;
	new->tir.vitesse = 0;
	new->tir.puissance = 0;
	new->tir.sym = '|';
	new->pos.x = 0;
	new->pos.y = 0;
	new->counter = 0;
	new->offset = 0;
	new->suivant = NULL;

	return new;
}



/*
 * Func : list_vaisseaux_new_elem(t_vaisseau vaisseau)
 *
 * Desc : Créer un nouvel element de la liste et le rajouter à la fin de la liste
 *		  Retourner le nouvel element new
 *		
 */
t_liste_vaisseaux	*list_vaisseaux_new_elem(t_liste_vaisseaux *list, t_vaisseau vaisseau)
{
	t_liste_vaisseaux *new;
	new = list_vaisseaux_new(vaisseau);
	list_vaisseaux_add_elem(list, new);

	return new;
}

/*
 * Func : copy_vaisseau(t_liste_vaisseaux *ship)
 *
 * Desc : Faire une copie de ship dans l'élément new
 *		  en utilisan memcpy et retourner new		
 *
 */


t_liste_vaisseaux	*copy_vaisseau(t_liste_vaisseaux *ship) 
{
	t_liste_vaisseaux *new;

	if (!ship)
		return NULL;

	if (!(new = malloc(sizeof(t_liste_vaisseaux))))
		return NULL;

	memcpy(new, ship, sizeof(t_liste_vaisseaux));
	new->suivant = NULL;

	return new;
}
























