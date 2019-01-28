#include "struct.h"


/*
 * Func : pt_new(int x, int y)
 *
 * Desc : Créer un nouveau t_pt et le retourner
 *		
 */
 t_pt	*pt_new(int x, int y)
 {
 	t_pt *new;

 	if((new = malloc(sizeof(t_pt))) == NULL)
 		return NULL;
 	new->x = x;
 	new->y = y;

 	return new;
 }



 /*
 * Func : pt_copy(t_pt *from, t_pt *to)
 *
 * Desc : Copie le pt_from vers to
 *		
 */
void pt_copy(t_pt *from, t_pt *to)
{
	if(!from || !to)
		return;
	to->x = from->x;
	to->y = from->y;
}