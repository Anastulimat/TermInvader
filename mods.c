#include <math.h>

#include "mods.h"
#include "filesystem.h"


/*
 * Func : get_mod_name(char* dir_name)
 *
 * Desc : Récuperer le nome d'un mod et le retourner
 *		
 */
char* get_mod_name(char* dir_name)
{
	char* mode_name;
	chdir(dir_name);

	read_directory(".");
	mode_name = remove_eol(read_first_line("deroulement"));
	if (!mode_name)
		printf("Erreur : le mode %s ne contient pas de fichier 'déroulement'\n", dir_name);

	chdir("..");

	return mode_name;
}



/*
 * Func : char*** get_mods_list() 
 *
 * Desc : Un tableau de tableau de chaine de caractères
 *		  Retourner une liste contenant les mods lus
 *		
 */
char*** get_mods_list()
{
	char** 	mods;
	char** 	dirs;
	char***	list_dir;

	if(!is_directory("mods")){
		printf("Erreur : Dossier mods inexistant.\n");
		return NULL;		
	}

	chdir("mods");

	if(!(list_dir = read_directory(".")))  
	{
		printf("Erreur : impossible de lire le dossier.\n");
		chdir("..");
		return NULL;
	}

	int nb_dir = array_len((void**)list_dir);

	if (   !(mods = malloc(sizeof(char **) * (nb_dir+2)))
		|| !(dirs = malloc(sizeof(char **) * (nb_dir+2)))
	   	) 
	{
		printf("Error : Unable to alloc memory.\n");
		chdir("..");
		return NULL;
	}

	int i = 0;
	int j = 0;
	char**	dir;

	while((dir = list_dir[i]) != NULL)
	{
		if(dir[FS_F_TYPE][0] == 'd')
		{
			if ((mods[j] = get_mod_name(dir[FS_F_NAME])) != NULL) 
			{
				dirs[j] = dir[FS_F_NAME];
				j += 1;
			}
		}
		i += 1;
	}
	mods[j] = NULL;

	char ***tab = malloc(sizeof(char**) * 3);
	tab[0] = mods;
	tab[1] = dirs;
	tab[2] = NULL;

	chdir("..");

	return tab;
}


/*
 * Func : t_liste_niveaux 	*get_mod_niveau(int nb, t_mod *mod){
 *
 * Desc : Retourne le niveu nb dans mod
 *		
 */
t_liste_niveaux 	*get_mod_niveau(int nb, t_mod *mod){
	t_liste_niveaux	*lvl = mod->levels;
	int i = 0;

	while (i < nb && lvl)
		lvl = lvl->suivant;
	return lvl;
}


/*
 * Func : t_liste_vaisseaux 	*get_mod_ship(int nb, t_mod *mod)
 *
 * Desc : Retourne le vaisseau nb dans mod
 *		
 */
t_liste_vaisseaux 	*get_mod_ship(int nb, t_mod *mod)
{
	t_liste_vaisseaux	*ship = mod->ships;
	int i = 0;

	while (i < nb && ship)
		ship = ship->suivant;

	return ship;
}


/*
 * Func : int get_freq_tir(float freq)
 *
 * Desc : Retourne la fréquence du tir,
 *		  convertir freq du float à int à l'aide de la fonction lroundf dans <math.h>
 *		
 */
int get_freq_tir(float freq)
{
	return (int)lroundf(freq*10);
}
