#include "menu.h"
#include "display.h"

#define MAX_ENTRIES 4



/*
 * Func : int nb_entries_in_array(char **a)
 *
 * Desc : Récuperer la taille du tableau **a et la retourner
 */
int nb_entries_in_array(char **a)
{
	int i = 0;
	while(a[i])
		i += 1;
	return i;
}



/*
 * Func : void display_menu(int choice, char ** tab)
 *
 * Desc : Affichage du menu de lancement du jeu
 */
void display_menu(int choice, char ** tab)
{
	int i = 0;
	int max_entries = nb_entries_in_array(tab);
	put_title_to("                               ", 29, 4);  		
	put_title_to("     ** CHOOSE YOUR MOD **     ", 29, 5);  		
	put_title_to("                               ", 29, 6);  		
	while(i < max_entries)
	{
		int line = 10+i*4;
		int col  = 35;
	  	if (choice == i)
	  	{	
	  		put_blink_str_to(tab[i], col, line);  		
	  	}
	  	else
	  		put_menuline_to(tab[i], col, line);
	i += 1;
	}
}



/*
 * Func : void erase_menu(int max_entries)
 *
 * Desc : Effacer du menu de lancement du jeu
 */
void erase_menu(int max_entries)
{
	int i = 0;
	while(i < max_entries)
	{
		int line = 5+i*4;
		int col  = 30;
  		put_str_to("                                          ", col, line);
	i += 1;
	}
	clean_screen();
}



/*
 * Func : int menu_loop(char **tab)
 *
 * Desc : Fonction de l'affichage principle du menu de lancement de jeu,
 *		  catch l'appuie de touche pour se déplacer entre les choix possibles
 */
int menu_loop(char **tab)
{
	struct pollfd fds;
	int ret = 0;
	int buf[4] = {0};
	int choice = 0;
	int validated = 0;
	// char tab[MAX_ENTRIES][16] = {{"Choix 1"},{"Choix 2"},{"Choix 3"},{"La reponse D"}};


	fds.fd = 0;
	fds.events = POLLIN;
	int max_entries = nb_entries_in_array(tab);
	while(!validated) {
		display_menu(choice, (char**)tab);
		if(poll(&fds, 1, -1))
		{
			if(fds.revents & (POLLOUT | POLLERR | POLLHUP) )
				return 0;
			if((ret = read(0, buf, sizeof(int))) <= 0)
				return 0;
			int k = buf[0];
			if(k == K_UP)
				--choice;
			else if(k == K_DOWN)
				++choice;
			else if(k == K_SPACE1 || k == K_SPACE2 || k == K_ENTER1 || k == K_ENTER2)
			{
				validated = 1;
				erase_menu(max_entries);
				return choice;
			}

			choice %= max_entries;
			if (choice < 0)
				choice = max_entries -1;
		}
	}

	return 1;
}
