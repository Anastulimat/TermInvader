#include <stdlib.h>

#include "struct.h"
#include "parser.h"
#include "filesystem.h"
#include "mods.h"
#include "display.h"

#define BUFFER_SIZE 1024
#define TRUE 	1
#define FALSE	0
#define MAX_LEVELS 10


/*
 * Func : void decale_str(char *s, int i)
 *
 * Desc : Pour le décalage à gauche dans une chaine de caractère
 *    
 */
void decale_str(char *s, int i)
{
  while (s && s[++i])
    s[i-1] = s[i];
  s[i-1] = 0;
}



/*
 * Func : void my_strclean(char* s)
 *
 * Desc : Enlever deux espaces et les remplacer par une seule espace
 *    
 */
void my_strclean(char* s)
{
    if (!s)
    return ;
  int i = 0;

  while(s[0] == ' ')
  {
    decale_str(s, 0);
  }

  while (s[i]) 
  {
    while (s[i] == ' ' && s[i + 1] == ' ')
      decale_str(s, i + 1);
    if (s[i] != 0)
      i += 1;
  }

  while (s[i] == 0)
    i -= 1;

  while (s[i] == ' ' && s[i+1] == 0)
    s[i--] = 0;
}



/*
 * Func : void percent_to_absolut(int *x, int *y)
 *
 * Desc : Récupérer la taille du terminal en x et y dans t_pt *term
 *        et les convertir en valeur absolu
 *    
 */
void percent_to_absolut(int *x, int *y)
{
  t_pt *term = size_terminal();
  if (x)
    *x = *x * term->x/100;
  if (y)
    *y = (100 - *y) * term->y/100;
}



/*
 * Func : void absolut_to_percent(int *x, int *y)
 *
 * Desc : Récupérer la taille du terminal en x et y dans t_pt *term
 *        et les convertir en pourcentage
 *    
 */
void absolut_to_percent(int *x, int *y)
{
  t_pt *term = size_terminal();
  if (x)
    *x = *x * 100/term->x;
  if (y)
    *y = 100-(*y * 100/term->y);
}



/*
 * Func : char *my_splitstr(char* str, char *sep)
 *
 * Desc : Chercher s'il y a une espace dans une chaine de caractère
 *    
 */
char *my_splitstr(char* str, char *sep)
{
  my_strclean(str);
  char *occ = strstr(str, sep);

  if (!occ || strlen(occ) <= 1)
  {
    return NULL;
  }
  occ[0] = 0;
  occ += 1;
  return occ;
}



/*
 * Func : int *get_nbs_in_line(char* line, int nb, char *sep)
 *
 * Desc : 
 *    
 */
int *get_nbs_in_line(char* line, int nb, char *sep)
{
  int *res = malloc(sizeof(int)*(nb+2));
  if (!res)
    return NULL;
  char *occ = line;
  char *next = NULL;
  int i = 0;
  while (occ && i <= nb) 
  {
    next = my_splitstr(occ, sep);
    res[i] = atoi(occ);
    occ = next;
    i += 1;
  }
  return res;
}



/*
 * Func : int *get_deroulement()
 *
 * Desc : Récuperer le fichier déroulement d'un mode et le retourner
 *    
 */
int *get_deroulement()
{
  int *res = malloc(sizeof(int)*(MAX_LEVELS+1));
    if (!res)
      return NULL;

  FILE *f = fopen("deroulement", "r");
  char *l = NULL;
  l = read_line(f);
  //free(l);
  int i = 0;
  while ((l = read_line(f)) && i < MAX_LEVELS){
    res[i] = atoi(l);
    free(l);
    i += 1;
  }
  return res;
}



/*
 * Func : t_liste_niveaux *get_level(char* filename)
 *
 * Desc : Récuperer les informations du niveau d'un mod et le retourner
 *    
 */
t_liste_niveaux *get_level(char* filename)
{
  t_liste_niveaux *lvl = list_niveaux_new();

  char** file = read_file(filename);
  if(!file)
    return NULL;

  lvl->nbr_vaisseaux = atoi(file[0]);
  lvl->vaisseaux = malloc(sizeof(struct s_niveau_ship) * (lvl->nbr_vaisseaux + 1));
  int i = 0;
  while(i < lvl->nbr_vaisseaux)
  {
    int* data = get_nbs_in_line(file[i+1], 4, " ");
    lvl->vaisseaux[i].type = data[0];
    lvl->vaisseaux[i].offset = data[1];
    lvl->vaisseaux[i].pos.x = data[2];
    lvl->vaisseaux[i].pos.y = data[3];
    i += 1;
  }
  return lvl;
}




/*
 * Func : t_liste_vaisseaux* build_vaisseaux_list(t_mod *m, t_liste_niveaux * level)
 *
 * Desc : 
 *    
 */
t_liste_vaisseaux* build_vaisseaux_list(t_mod *m, t_liste_niveaux * level)
{
  struct s_niveau_ship *ship=level->vaisseaux;
  t_liste_vaisseaux * list = m->ships;
  t_liste_vaisseaux * result = NULL;
  t_liste_vaisseaux * last = NULL;
  t_liste_vaisseaux * prev = NULL;
  int i = 0;
  int len = s_niveau_array_size(ship);

  while ((ship != NULL && i < len)) 
  {
    prev = last;
    if ((last =  copy_vaisseau(list_vaisseaux_get( list, ship[i].type -1))))
    {
      if (prev)
        prev->suivant = last;

      last->pos.x = ship[i].pos.x;
      last->pos.y = ship[i].pos.y;
      last->offset = ship[i].offset;
      percent_to_absolut( &(last->pos.x), &(last->pos.y) );

      if (!result)
        result = last;
    }
    else
      last = prev;
    i += 1;
  }

  if (last)
    last->suivant = NULL;

  level->list_vaisseaux = result;

  return result; // return result ou NULL.
}



/*
 * Func : t_liste_niveaux *get_levels(t_mod *m)
 *
 * Desc : Construire une liste de niveaux qui exsite dans un mod et la retourner
 *    
 */
t_liste_niveaux *get_levels(t_mod *m)
{
  t_liste_niveaux *begin = NULL;
  t_liste_niveaux *prev = NULL;
  t_liste_niveaux *new = NULL;

  int i = 1;
  char buff[8] = "";
  
  while(i < 999 && sprintf(buff, "%d", i) && (new = get_level(buff)))
  {
    if(!begin)
      begin = new;
    build_vaisseaux_list(m, new);

    if(prev)
      prev->suivant = new;

    prev = new;
    i += 1;
  }
  m->lvl=begin;
  return begin;
}




/*
 * Func : t_liste_vaisseaux *get_vaisseau(char* filename)
 *
 * Desc : Récuperer les informations du vaisseux d'un mod et le retourner
 *    
 */
t_liste_vaisseaux *get_vaisseau(char* filename)
{
  char** file = read_file(filename);
  if(!file)
    return NULL;

  t_liste_vaisseaux *ship = list_vaisseaux_new(file + SHIP_SHIP-1);
  int i = 0;
  int val;
  int *v;

  while(i < SHIP_SHIP)
  {
    char *line = file[i];
    i += 1;

    switch(i)
    {
      case SHIP_OCCR :
      case SHIP_LIFE :
      case SHIP_SPED :
      case SHIP_POWR :
        val = atoi(line);
        break;

      case SHIP_MOVX :
      case SHIP_MOVY :
        v = get_nbs_in_line(line, atoi(file[SHIP_OCCR-1]), " ");
        break ;


      case SHIP_FREQ : ship->tir.freq = atof(line);     break;
      case SHIP_SYMB : ship->tir.sym = line[0];         break ;

      case SHIP_DIMS :

        v = get_nbs_in_line(line, 2, " ");
        ship->largeur = v[0];
        ship->hauteur = v[1];
        break;
      
    }
    switch(i) {
      case SHIP_OCCR :  ship->longueur_cycle_deplacement = val;   break ;
      case SHIP_LIFE :  ship->life = val;                         break ;
      case SHIP_SPED :  ship->tir.vitesse = val;                  break ;
      case SHIP_POWR :  ship->tir.puissance = val;                break ;

      case SHIP_MOVX :  ship->deplacement_horizontal = v;        break ;
      case SHIP_MOVY :  ship->deplacement_vertical = v;          break ;
    }
  }
  ship->tir.dir = 1;
  return ship;
}



/*
 * Func : t_liste_vaisseaux *get_vaisseaux()
 *
 * Desc : Construire une liste de vaisseaux qui exsite dans un mod et la retourner
 *    
 */
t_liste_vaisseaux *get_vaisseaux()
{
  t_liste_vaisseaux *begin = NULL;
  t_liste_vaisseaux *prev = NULL;
  t_liste_vaisseaux *new = NULL;

  int i = 1;
  char buff[8] = "";
  
  while(i < 999 && sprintf(buff, "%d", i) && (new = get_vaisseau(buff)))
  {
    if(!begin)
      begin = new;

    if(prev)
      prev->suivant = new;

    prev = new;
    i += 1;
  }
  return begin;
}




/*
 * Func : void get_player_ship(t_mod *m)
 *
 * Desc : Construire le vaisseau du joueur
 *    
 */
void get_player_ship(t_mod *m)
{
  char ship1[4][6] = { { ' ',' ','^',' ',  ' '},
                       { ' ','/','H','\\', ' '},
                       { '|','-','^','-',  '|'} };

  t_vaisseau ship = malloc(sizeof(char*)*3);
  ship[0] = malloc(sizeof(char)*6);
  ship[1] = malloc(sizeof(char)*6);
  ship[2] = malloc(sizeof(char)*6);

  strcpy(ship[0], ship1[0]);
  strcpy(ship[1], ship1[1]);
  strcpy(ship[2], ship1[2]);
  ship[3] = NULL;

  m->player = list_vaisseaux_new(ship);
  m->player->hauteur =  3;
  m->player->largeur =  5;

  m->player->pos.x   = 45;
  m->player->pos.y   = 5;
  percent_to_absolut( &(m->player->pos.x), &(m->player->pos.y) );
  m->player->life = 5;
  m->player->tir.dir = -1;
  m->player->tir.sym = '|';
}


/*
 * Func : void fill_mod(t_mod *m)
 *
 * Desc : Fonction principale pour construire le mod,
 *        Récupérer le ficheir déroulement, construire les niveaux et les vaisseux
 *        et le vaisseau du joueur
 *    
 */
void fill_mod(t_mod *m)
{
  if (!m)
    return ;
  m->score = 0;
  m->level = 1;

  m->deroulement = get_deroulement();
  chdir("vaisseaux");
  m->ships = get_vaisseaux();
  chdir("..");
  chdir("niveaux");
  m->levels = get_levels(m);
  chdir("..");

  get_player_ship(m);
}




/*
 * Func : int s_niveau_array_size(struct s_niveau_ship *arr)
 *
 * Desc : Retourner la taille (struct s_niveau_ship *arr)
 *    
 */
int s_niveau_array_size(struct s_niveau_ship *arr)
{
  int i = 0;
  while (arr[i].type > 0)
    i += 1;
  return i;
}



/*
 * Func : int   get_nbr_vaisseaux(char** file, t_liste_niveaux *vaisseau)
 *
 * Desc : Récuperer le nombre des vaisseaux dans le ficheir de vaisseau
 *    
 */
int   get_nbr_vaisseaux(char** file, t_liste_niveaux *vaisseau)
{
  if(!file)
    return -1;

  return vaisseau->nbr_vaisseaux = atoi(file[0]);
}









