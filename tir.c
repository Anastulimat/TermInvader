#include "struct.h"


/*
 * Func : tir_get(t_tir *list, int n)
 *
 * Desc : Retourne l'élément n dans list
 *		
 */
t_tir 	*tir_get(t_tir *list, int n)
{
	int i = 0;
	while(i < n && list != NULL)
	{
		list = list->next;
		i++;
	}
	return list;
}

int  tir_exist_by_coord(t_tir *list, int x, int y){
	while (list)
	{
		if (list->pos.x == x && list->pos.y == y)
			return 1;
		list = list->next;
	}
	return 0;
}

/*
 * Func : tir_next(t_tir *list)
 *
 * Desc : Retourne l'élément next dans list
 *		
 */
t_tir	*tir_next(t_tir *list)
{
	if(list != NULL)
	{
		return list->next;
	}
	return NULL;
}



/*
 * Func : tir_last_elem(t_tir *list)
 *
 * Desc : Retourne le dernier élément dans list
 *		
 */
t_tir	*tir_last_elem(t_tir *list)
{
	while(list && list->next)
	{
		list = list->next;
	}
	return list;
}



/*
 * Func : tir_add_elem(t_tir *list, t_tir *new)
 *
 * Desc : Ajouter new à la fin de la list
 *		
 */
void				tir_add_elem(t_tir *list, t_tir *new)
{
	if(list && new)
	{
		list = tir_last_elem(list);
		list->next = new;
		new->next = NULL;
	}
}



/*
 * Func : tir_new()
 *
 * Desc : Créer une nouvelle liste et la retourner
 *		
 */
t_tir	*tir_new()
{
	t_tir 	*new;

	if((new = malloc(sizeof(t_tir))) == NULL)
		return NULL;


	new->pos.x = 0;
	new->pos.y = 0;
	new->vitesse = 1;
	new->puissance = 1;
	new->freq = 1;
	new->sym = 1;
	new->dir = 1;
	new->next = NULL;

	return new;
}

/*
 * Func : shoot(t_liste_vaisseaux *from)
 *
 * Desc : Créer l'émément new d'un tir et le retourner
 *		  Faire déplacer un tir à partir de from et mettre 
 *		  le nouveau resultat de déplacement dans new
 *		
 */
t_tir	*shoot(t_liste_vaisseaux *from, t_tir * list)
{
	t_tir 	*new;

	int posx = from->pos.x + from->largeur/2;
	int posy = from->pos.y + from->hauteur * from->tir.dir;
	if (list && tir_exist_by_coord(list, posx, posy))
		return NULL;
	if (!(new = tir_new()))
	{
		return NULL;
	}

	new->pos.x = 		posx;
	new->pos.y = 		posy;
	new->vitesse = 		from->tir.vitesse;
	new->puissance = 	from->tir.puissance;
	new->freq = 		from->tir.freq;
	new->sym = 			from->tir.sym;
	new->dir = 			from->tir.dir;
	new->next = 		NULL;

	return new;
}

/*
 * Func : tir_new_elem(t_vaisseau vaisseau)
 *
 * Desc : Créer un nouvel element de la liste et le rajouter à la fin de la liste
 *		  Retourner le nouvel element new
 *		
 */
t_tir	*tir_new_elem(t_tir *list, t_vaisseau vaisseau)
{
	t_tir *new;
	new = tir_new(vaisseau);
	tir_add_elem(list, new);

	return new;
}























