#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>

#include "display.h"
#include "struct.h"
#include "parser.h"

#define BG_BLINK		"\033[41m"
#define BG_TITLE		"\033[46m\033[35m"
#define RESET_COLORS	"\033[0m"
#define PLAYER 			"\033[32m"
#define ENNEMY			"\033[31m"

/*
 * Func : char  *remove_eol(char *s)
 *
 * Desc : Récuperer la taille du terminale en x et y en t_pt
 *		
 */
t_pt *size_terminal()
{
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	t_pt *pt = pt_new(w.ws_col, w.ws_row);

	return pt;
}



/*
 * Func : void put_slow_str_to(int x, int y, char *s, int intervall)
 *
 * Desc : Affichage relanti d'une chaine en x et y
 *		
 */
void put_slow_str_to(int x, int y, char *s, int intervall)
{
	int i =0;
	int len = strlen(s);
	while(i < len)
	{
		int r = rand() %1000 * (rand()%100);
		char c = s[i++];
		printf("\x1b[%d;%dH%c\x1b[H\n", y, x+i, c);
		usleep(intervall+r);
	}
}



/*
 * Func : void put_char_to(char c, int x, int y)
 *
 * Desc : Affichage d'un caractère en x et y
 *		
 */
void put_char_to(char c, int x, int y)
{
	char buffer[32] = "\0";
	sprintf(buffer, "\x1b[%d;%dH%c\x1b[H\n", y, x, c);
	write(1, buffer, strlen(buffer));

}



/*
 * Func : void put_str_to(char* s, int x, int y)
 *
 * Desc : Affichage d'une chaine de caractères en x et y
 *		
 */
void put_str_to(char* s, int x, int y)
{
	char buffer[64] = "\0";
	sprintf(buffer, "\x1b[%d;%dH%s\n", y, x, s);
	write(1, buffer, strlen(buffer));
	put_char_to('\t', 0, 0);
}



/*
 * Func : void put_menuline_to(char* s, int x, int y)
 *
 * Desc : Affichage d'une ligne de menu en x et y
 *		
 */
void put_menuline_to(char* s, int x, int y)
{
	char buffer[64] = "\0";
	sprintf(buffer, "\x1b[%d;%dH     %s     \n", y, x-2, s);
	write(1, buffer, strlen(buffer));
}



/*
 * Func : void put_blink_str_to(char* s, int x, int y)
 *
 * Desc : Affichage de la couleur blink sur une ligne de menu en x et y
 *		
 */
void put_blink_str_to(char* s, int x, int y)
{
	char buffer[64] = "\0";
	sprintf(buffer, "\x1b[%d;%dH%s    %s    %s", y, x-1,BG_BLINK, s, RESET_COLORS);
	write(1, buffer, strlen(buffer));
}




/*
 * Func : void put_title_to(char* s, int x, int y)
 *
 * Desc : Affichage le titre du menu en x et y avec de la couleur
 *		
 */
void put_title_to(char* s, int x, int y)
{
	char buffer[64] = "\0";
	sprintf(buffer, "\x1b[%d;%dH%s %s %s", y, x-1,BG_TITLE, s, RESET_COLORS);
	write(1, buffer, strlen(buffer));
}


/*
 * Func : void clean_screen()
 *
 * Desc : Nettoyer l'écran et cacher le corseur
 *		
 */
void clean_screen()
{
	// char buffer[32] = "\0";
	printf("\x1b[?25l\x1b[2J\x1b[H\n");
	// write(1, buffer, strlen(buffer));
	printf("\x1b[2J\x1b[H");
}



/*
 * Func : void check_pos_vaisseau(t_liste_vaisseaux *v, int min, int max)
 *
 * Desc : 
 *		
 */
void check_pos_vaisseau(t_liste_vaisseaux *v, int min, int max)
{
	t_pt *pos = &v->pos;
	t_pt *s = size_terminal();
	min = (min ? min : 5);
	max = (max ? max : -4);

	if (pos->y > s->y + max){
		move_vaisseau(v, 0, -(pos->y - (s->y + max)) );
	}
	else if (pos->y < min) {
	 	move_vaisseau(v, 0, ( min - pos->y) );	 	
	}
	if (pos->x < 2)
	{
	 	move_vaisseau(v, 2-pos->x, 0 );	 	
	}
	else if (pos->x > s->x - 6)
	{
	 	move_vaisseau(v, (s->x-6)-pos->x, 0 );	 	
	}

}



/*
 * Func : void erase_vaisseau(t_liste_vaisseaux *ship)
 *
 * Desc : Effacer un vaisseau
 *		
 */
void erase_vaisseau(t_liste_vaisseaux *ship)
{
 	int i, y, j = 0;
 	t_pt pos;
 	pos = ship->pos;
 	int h = ship->hauteur;
 	int l = ship->largeur;
 	while(i < h)
 	{
 		j = 0;
 		while (j < l){
	 		y = pos.y - h + i + 1;
	 		put_char_to(' ', pos.x+j, y);
	 		j += 1; 			
 		}
 		i += 1;
 	}
 	printf("\n");
}


/*
* Func : void 	print_vaisseau(t_liste_vaisseaux *vaisseau)
*
* Desc : Permet d'afficher un vaisseau
*		
*/
 void print_vaisseau(t_liste_vaisseaux *vaisseau)
 {
 	int dir = vaisseau->tir.dir;
 	int i, y = 0;
 	t_pt pos;
 	if (!vaisseau)
 		return;
 	pos.x = vaisseau->pos.x;
 	pos.y = vaisseau->pos.y;
 	int h = vaisseau->hauteur;

 	t_vaisseau body_ship = vaisseau->vaisseau;
 	printf("%s\n", (dir>0? ENNEMY: PLAYER ) );
 	while(body_ship != NULL && body_ship[i])
 	{
 		y = pos.y - h + i + 1;
 		put_str_to(body_ship[i], pos.x, y);
 		i += 1;
 	}
 	printf("%s\n",RESET_COLORS);
 }



/*
 * Func : void move_vaisseau(t_liste_vaisseaux *ship, int x, int y)
 *
 * Desc : Faire bouger les vaisseux en x et y
 *		
 */
void move_vaisseau(t_liste_vaisseaux *ship, int x, int y)
{
 	printf("\n");
 	erase_vaisseau(ship);
 	ship->pos.x += x*1;
 	ship->pos.y += y*1;

 	printf("\n");
 	print_vaisseau(ship);
 	printf("\n");
}



/*
 * Func : void move_vaisseau_IA(t_liste_vaisseaux *ship)
 *
 * Desc : Déplacement automatuique d'un vaisseau ennmie
 *		
 */
void move_vaisseau_IA(t_liste_vaisseaux *ship)
{
 	if (ship->offset > 0){
 		ship->offset -= 1;
 		return ; 		
 	}

 	int i = ship->counter % ship->longueur_cycle_deplacement;
 	int x = ship->deplacement_horizontal[i]*2;
 	int y = -ship->deplacement_vertical[i]*2;

 	move_vaisseau(ship, x, y);
 	int max = 60;
 	percent_to_absolut(NULL, &max);
 	check_pos_vaisseau(ship, 1,-max);
 	ship->counter +=1;
 }




/*
 * Func : t_liste_vaisseaux		*move_liste_vaisseaux_IA(t_liste_vaisseaux *ships, t_mod *m)
 *
 * Desc : Déplacement automatuique d'unu liste de vaisseaux ennmies
 *		
 */
t_liste_vaisseaux		*move_liste_vaisseaux_IA(t_liste_vaisseaux *ships, t_mod *m)
{
 	t_liste_vaisseaux *first = ships;
 	t_liste_vaisseaux *prev = NULL;
	while(ships != NULL)
 	{
 		if (ships->life <= -4)
 		{
 			erase_vaisseau(ships);
 			if (prev)
 				prev->suivant = ships->suivant;
 			else
 				first = ships->suivant;
 		}
 		else if (ships->life > 0) 
 		{
			move_vaisseau_IA(ships);
			if (rand() % (get_freq_tir(ships->tir.freq)+100) >= 100)
			{
				if (m->lvl->tirs)
					tir_last_elem(m->lvl->tirs)->next = shoot(ships, m->lvl->tirs);
				else
					m->lvl->tirs = shoot(ships, m->lvl->tirs);				
			}
 		}
 		else 
 		{
 			// erase_vaisseau(ships);
 			explode_ship(ships, 1);
			ships->life -= 1;
 		}
		prev = ships;
 		ships = ships->suivant;
 	}
 	return first;
}



/*
 * Func : void 	print_list_vaisseau(t_liste_vaisseaux *vaisseau)
 *
 * Desc : Permet d'afficher une liste de vaisseux en caractères.
 *		
 */
void 	print_list_vaisseau(t_liste_vaisseaux *vaisseau)
 {
 	int i = 0;
 	while(vaisseau != NULL)
 	{
 		i+= 1;
 		print_vaisseau(vaisseau);
 		vaisseau = vaisseau->suivant;
 	}
 }




/*
 * Func : void 	check_player_shooted(t_mod * m)
 *
 * Desc : Vérifier si le joueur a été touché ou pas
 *		
 */
void 	check_player_shooted(t_mod * m)
{
	t_tir 				*tirs;
	t_tir 				*first;
	t_tir 				*prev = NULL;
	t_tir 				*next = NULL;
	t_liste_vaisseaux 	*ship;

	tirs 		= m->lvl->tirs;
	first 		= tirs;
	while (tirs) 
	{
		ship  = m->lvl->list_vaisseaux;
		next = tirs->next;
		while (ship && tirs) 
		{
			if (	tirs->pos.x >= ship->pos.x 
				&& 	tirs->pos.x < (ship->pos.x + ship->largeur)
				&&  tirs->pos.y > (ship->pos.y - ship->hauteur)
				&& 	tirs->pos.y <=  ship->pos.y 
				&&  ship->life >= 0)
			{
				m->score += 5 + rand() %3;
				ship->life -= 1;
				explode_ship(ship, 0);
				if (prev)
					prev->next = next;
				else
					first = next;
				free(tirs);
				tirs = NULL;
			}
			ship = ship->suivant;
		}
		ship = m->player;

		if (	tirs
				&& tirs->pos.x >= ship->pos.x 
				&& 	tirs->pos.x < (ship->pos.x + ship->largeur)
				&&  tirs->pos.y > (ship->pos.y - ship->hauteur)
				&& 	tirs->pos.y <=  ship->pos.y )
			{
				if (ship->life>0)
					ship->life -= 1;
				m->score -= 5;
				erase_tir(tirs);
				explode_ship(ship, 0);
				if (prev)
					prev->next = tirs->next;
				else
					first = tirs->next;
			}

		if (tirs)
			prev = tirs;
		tirs = next;
	}

	m->lvl->tirs = first;
}



/*
 * Func : void explode_ship(t_liste_vaisseaux * ship, int erase)
 *
 * Desc : Vérifier si un vaisseau a été touché, et afficher une explosion
 */
void explode_ship(t_liste_vaisseaux * ship, int erase)
{
 	int i, y, j = 0;
 	t_pt pos;
 	pos = ship->pos;
 	int h = ship->hauteur;
 	int l = ship->largeur;
 	while(i < h)
 	{
 		j = 0;
 		while (j < l)
 		{
	 		y = pos.y - h + i + 1;
	 		if (rand() % 12 > 3){
	 			if (erase)
		 			put_char_to(' ', pos.x+j, y);	 			
	 		}
	 		else
	 			put_char_to('*', pos.x+j, y);

	 		j += 1; 			
 		}
 		i += 1;
 	}
 	printf("\n");
}




/*
 * Func : void erase_tir(t_tir *t)
 *
 * Desc : Supprimer le tir en pos.x et pos.y
 */
void erase_tir(t_tir *t)
{
	put_char_to(' ', t->pos.x, t->pos.y);
}




/*
 * Func : void 	move_list_tir(t_mod *m)
 *
 * Desc : Faire bouger une liste de tirs
 */
void move_list_tir(t_mod *m)
 {
 	t_tir * tir = m->lvl->tirs;
 	t_tir * first = m->lvl->tirs;
 	t_tir * prev = NULL;
 	t_tir * next = NULL;
 	t_pt * s = size_terminal();

 	while(tir != NULL)
 	{
 		next = tir->next;
 		if (tir->pos.y <= 1 || tir->pos.y >= s->y)
 		{
 			if (prev)
 				prev->next = tir->next;
 			else
 				first = tir->next;
 			erase_tir(tir);
 			free(tir);
 			tir = NULL;
 		}
 		else 
 		{
	 		put_char_to(' ', tir->pos.x, tir->pos.y);
	 		tir->pos.y += tir->dir;
	 		put_char_to(tir->sym, tir->pos.x, tir->pos.y);
		}
		if (tir)
			prev = tir;
 		tir = next;
 	}
 	free(s);
 	m->lvl->tirs = first;
	check_player_shooted(m);
 }