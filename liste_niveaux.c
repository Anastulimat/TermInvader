#include "struct.h"


/*
 * Func : list_niveaux_next(t_liste_niveaux *list)
 *
 * Desc : Retourne l'élément suivant dans list
 *		
 */
t_liste_niveaux 		*list_niveaux_next(t_liste_niveaux *list)
{
	if(list != NULL)
	{
		return list->suivant;
	}
	else
	{
		return NULL;
	}
}




/*
 * Func : list_niveaux_last_elem(t_liste_niveaux *list)
 *
 * Desc : Retourne le dernier élément suivant dans list
 *		
 */
 t_liste_niveaux		*list_niveaux_last_elem(t_liste_niveaux *list)
 {
 	while(list && list->suivant)
 	{
 		list = list->suivant;
 	}
 	return list;
 }



/*
 * Func : list_niveaux_add_elem(t_liste_niveaux *list, t_liste_niveaux *new)
 *
 * Desc : Ajouter l'élément new à la fin de la liste list
 *		
 */
 void 					list_niveaux_add_elem(t_liste_niveaux *list, t_liste_niveaux *new)
 {
 	if(list && new)
 	{
 		list = list_niveaux_last_elem(list);
 		list->suivant = new;
 		new->suivant = NULL;
 	}
 }




/*
 * Func : list_niveaux_new()
 *
 * Desc : Créér une liste, la remplir et la retourner
 *		
 */
t_liste_niveaux			*list_niveaux_new()
{
	t_liste_niveaux		*new;
	if((new = malloc(sizeof(t_liste_niveaux))) == NULL)
	{
		return NULL;
	}
	new->suivant 		= NULL;
	new->nbr_vaisseaux	= 0;
	new->tirs			= NULL;
	new->vaisseaux = NULL;	
	new->list_vaisseaux = NULL;

	return new;
}



/*
 * Func : list_niveaux_new_elem(t_liste_niveaux *list)
 *
 * Desc : Créer un nouvel elem new et le rajouter à la liste
 *		
 */
 t_liste_niveaux		*list_niveaux_new_elem(t_liste_niveaux *list)
 {
 	t_liste_niveaux		*new;
 	new = list_niveaux_new();
 	list_niveaux_add_elem(list, new);

 	return new;
 }

























