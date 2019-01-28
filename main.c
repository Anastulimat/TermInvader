#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

#include "mods.h"
#include "menu.h"
#include "display.h"
#include "parser.h"


static struct termios orig_tios;
static struct termios tios;

struct sigaction sa;

/*
 * Func : end_program()
 *
 * Desc :
 *		Restore les propriétés du Terminal
 */
void end_program()
{
	char buffer[32] = "\0";
	tcsetattr(0, TCSANOW, &orig_tios);
	sprintf(buffer, "\x1b[?25h");
	write(1, buffer, strlen(buffer));
}

/*
 * Func : intHandler(int)
 *
 * Desc :
 *		Appelé lors du ^C :
 *       lance end_program() et exit.
 */
void intHandler(int dummy) 
{
	end_program();
	printf("\n!!!!! ^C SIGINT catched !!!!!\n");
	exit(1);
}


/*
 * Func : init_term()
 *
 * Desc :
 *		Initialise le terminal pour le lancement du jeu.
 *        Desactive mode cannonique et echo.
 * 		  & Clear le Terminal
 */
int init_term()
{
	/* Obtenir les paramètres actuels du terminal */
	if (tcgetattr(0, &orig_tios))
	{
		printf("Erreur lors de l'obtention des paramètres de terminal en cours\n");
		return 3;
	}
	/* Copier les paramètres oraginales et les mettre dans "tios" pour effectuer les modifications */
	tios = orig_tios;

	/* Nous voulons désactiver le mode canonique */
	tios.c_lflag &= (~ICANON);
	tios.c_lflag &= (~ECHO);

	/* Appliquer nos paramètres */
	if (tcsetattr(0, TCSANOW, &tios))
	{
		printf("Erreur lors de l'application des paramètres de terminal\n");
		return 3;
	}

	/* Vérifiez si nos paramètres ont été correctement appliqués */
	if (tcgetattr(0, &tios))
	{
		tcsetattr(0, TCSANOW, &orig_tios);
		printf("Erreur lors de l'affirmation des paramètres du terminal\n");
		return 3;
	}

	/* Nettoyer le terminal */
	// char buffer[32] = "\0";
	// sprintf(buffer, "\x1b[?25l\x1b[2J\x1b[H");
	// write(1, buffer, strlen(buffer));
	// printf("\e[2J\e[H");
	 clean_screen();

	// sa.sa_handler = &intHandler;
	// sa.sa_flags = 0;
	// sa.sa_mask = 0;

	// sigaction(SIGINT, &sa, NULL);

	/* Catch le ^C
	 * SIGINT = Signal ^C
	 * intHandler = Pointeur sur fonction
	 */

	signal(SIGINT, intHandler);
	return 0;
}



/*
 * Func : print_infos(t_mod *m)
 *
 * Desc :
 *		Affichage des informations de jeu
 */
void print_infos(t_mod *m)
{
	printf("%s %s | Level %d | %d %s | Score : %d %s  \n","\x1b[0;1H ", m->name, m->level, m->player->life, HEART, m->score, DIAMOND);
}





/*
 * Func : int game_loop(t_mod *m)
 *
 * Desc :
 *		Fonction pricipale du jeu (main du jeu) + catch l'appuie de touche pour déplacer le vaisseu et tirer
 */
int game_loop(t_mod *m)
{
	struct pollfd fds;
	int ret = 0;
	char buf[4] = {0};


	fds.fd = 0;
	fds.events = POLLIN;

	short end = 0;
	int max_y_player = 30; // %
	percent_to_absolut(NULL, &max_y_player);

	int min_y_player = -3; // Cases

	clock_t last_move_IA = (clock_t)NULL;
	clock_t last_move_tir = (clock_t)NULL;
	clock_t now = clock();

	print_vaisseau(m->player);

	while(!end) {
		memset(buf, 0, 4);

		if(poll(&fds, 1, 200))
		{
			clock_t clock_input = clock();

			if(fds.revents & (POLLOUT | POLLERR | POLLHUP) )
				return 0;
			if (fds.revents & (POLLIN)) {
				if((ret = read(0, buf, 3)) <= 0)
					return 0;
				char k = buf[2];
				if(k == CK_RIGHT){
					move_vaisseau(m->player, 1, 0);
				} else if (k == CK_LEFT){	
					move_vaisseau(m->player, -1, 0);
				} else if(k == CK_UP){
					move_vaisseau(m->player, 0, -1);
				} else if (k == CK_DOWN){
					move_vaisseau(m->player, 0, 1);
				} else if(buf[0] == 0x20 )
				{
					if (!m->lvl->tirs)
						m->lvl->tirs = shoot(m->player, m->lvl->tirs);
					else
						tir_last_elem(m->lvl->tirs)->next = shoot(m->player, m->lvl->tirs);
				}
				else if(buf[0] == 0x1b )
				{
					 clean_screen();
					 return 0;
				}
				else
					m->player->life -= 1;

				check_pos_vaisseau(m->player, max_y_player, min_y_player );
			}
			clock_t clock_input_end = clock();
			now -= (clock_t)((double)clock_input_end - (double)clock_input);
		}
		print_infos(m);

		// repere actuelle
		now = clock();
		// Difference entre le dernier mouvement IA et le repere actuel
		if (!last_move_IA || (double)(now - last_move_IA) > 500){
			// On bouge nos tirs
			// move_list_tir(m->lvl->tirs);
			// On bouge les vaisseaux IA
			m->lvl->list_vaisseaux = move_liste_vaisseaux_IA( m->lvl->list_vaisseaux, m);
			last_move_IA = clock();
		}
		if (!last_move_tir || (double)(now - last_move_tir) > 250){
			last_move_tir = clock();
			move_list_tir(m);
		}

		if (m->player->life <= 0){
			return game_over();	
		}
		if (m->lvl->list_vaisseaux == NULL)
			if (!level_up(m))
				return 1;
	}

	return 0;
}



/*
 * Func : int game_over()
 *
 * Desc :
 *		Fonction en cas de perte
 */
int game_over()
{
	int mx = 42;
	int my = 50;
	percent_to_absolut(&mx, &my);
	put_slow_str_to(mx, my, "  You loooooooose...  ", 100000);
	sleep(4);
	clean_screen();
	return 0;
}



/*
 * Func : int level_up(t_mod *m)()
 *
 * Desc :
 *		Fonction pour passer au niveau suivant
 */
int level_up(t_mod *m)
{
	int mx = 42;
	int my = 50;
	percent_to_absolut(&mx, &my);
	m->score += 100 + rand() %30;
	m->level += 1;
	if (m->lvl->suivant)
		m->lvl = m->lvl->suivant;
	else
		return you_win();
	put_slow_str_to(mx, my, "  Level UP  ", 100000);
	sleep(4);
	clean_screen();
	return 1;	
}



/*
 * Func : int you_win()
 *
 * Desc :
 *		Fonction en cas de gain
 */
int you_win()
{
	int mx = 42;
	int my = 50;
	percent_to_absolut(&mx, &my);
	put_slow_str_to(mx, my, "  You WIN !!!  ", 100000);
	sleep(4);
	clean_screen();
	return 0;		
}




int main(int argc, char *argv[])
{

	if(init_term() != 0)
	{
		return 3;
	}

	char ***tab = (char ***)get_mods_list();
	if (!tab)
		return -1;

	char **names=tab[0];
	char **dirs=tab[1];


	int choice = menu_loop(names);

	// printf("\n\n => Mode '%s' in 'mods/%s'\n", names[choice], dirs[choice]);
	if (!dirs[choice] || !chdir(dirs[choice])) 
	{
		printf("An error occured.\n");
		end_program();
		return 2;
	}

	//// On est dans le mod choisi ////
	chdir("mods");
	if (chdir(dirs[choice]) < 0)
		perror("\nAn error occured :");
	
	t_mod mod;
	mod.name = names[choice];
	mod.path = dirs[choice];
	int x = 30;
	int y = 60;
	percent_to_absolut(&x, &y);
	put_str_to("   Chargement en cours... ", x-4, y);
	sleep(1);
	fill_mod(&mod);
	put_str_to("                             ", x-4, y);
	printf("\n");

	game_loop(&mod);


	/* Restore les propriétés du Terminal */
	end_program();
	printf("See you soon...\n");

	return 0;
}