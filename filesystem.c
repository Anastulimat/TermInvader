#include "filesystem.h"

#define BUF_SIZE 128


/*
 * Func : char  *remove_eol(char *s)
 *
 * Desc : Vérifier si c'est la fin d'une ligne '\n' et le remplacer par '\0'
 *		
 */
char  *remove_eol(char *s)
{
	if (s && s[strlen(s)-1] == '\n') 
	{
		s[strlen(s)-1] = '\0';		
	}
	return s;
}

/*
 * Func : is_directory(const char *path)
 *
 * Desc : Vérifier si c'est un repertoire et le retourner
 *		
 */
int is_directory(const char *path)
{
	struct stat path_stat;
    stat(path, &path_stat);

    return S_ISDIR(path_stat.st_mode);
}


/*
 * Func : count_files_in_dir()
 *
 * Desc : Compter le nombre des fihciers qui existe dans le repertoire courant
 *		
 */
int count_files_in_dir()
{
	int fileCount = 0;
	DIR* dirp;
	struct dirent* entry;

	dirp = opendir(".");
	while((entry = readdir(dirp)) != NULL)
	{
		fileCount += 1;
	}
	closedir(dirp);

	return fileCount;
}



/*
 * Func : read_line(FILE* file)
 *
 * Desc : Lire une ligne dans un fichier et la retourner
 *		
 */
char* read_line(FILE* file)
{
	if(!file)
		return NULL;

	char* buf = malloc(sizeof(char*) * BUF_SIZE);
	if(!buf)
		return NULL;

	if(!(fgets(buf, BUF_SIZE, file)))
	{
		free(buf);
		return NULL;
	}
	return buf;
}


/*
 * Func : char* read_first_line(char* filename)
 *
 * Desc : Lire la première ligne dans un fichier et la retourner
 *		
 */
char* read_first_line(char* filename)
{
	FILE* file = fopen(filename, "r");
	if(!file)
		return NULL;

	char* line = read_line(file);

	fclose(file);

	return line;
}


/*
 * Func : read_file(char* filename)
 *
 * Desc : Retourne un tableau de char * 
 *		  correspondant au contenu du file
 */
char**    read_file(char* filename)
{
  	FILE  *fd;
  	char  *line;
  	char  **file;
  	int   i = 0;

	if (!(filename) ||
	  	!(fd = fopen(filename, "r")) ||
	  	!(file = malloc(sizeof(char *) * 2)))
	return NULL;

	file[0] = NULL;
	file[1] = NULL;

	while ( (line = malloc(sizeof(char) * (BUF_SIZE + 1))) &&
	      (line = fgets(line, BUF_SIZE, fd)))
	{
	  if ((file = realloc(file, sizeof(char*) *( i + 2))) == NULL)
	  	return NULL;

	  file[i + 1] = NULL;
	  file[i] = line;

	  if (line[strlen(line)-1] == '\n')
	  	line[strlen(line)-1] = 0;
	  i += 1;
	}
	return file;
}



/*
 * Func : char *** read_directory(char* dir_name)
 *
 * Desc : Lire tout ce qui existe dans le repertoire courant
 * 		  et le mettre dans un tableau de tableau
 * 		  [['dir1', 'd'], ['dir2','d'], ['file3', 'f']]
 * 		  [['dir1', 'd'], 'dir2', ['file3', 'f']]
 */
char *** read_directory(char* dir_name)
{
	int dirsize = count_files_in_dir();
	char***			tab;
	struct dirent* entry;

	if (!dir_name)
		dir_name=".";

	tab = malloc(sizeof(char**) * dirsize);
	if(!tab)
	{
		printf("Erreur malloc!!!\n");
	return NULL;
	}

	DIR* dirp = opendir(dir_name);
	if(!dirp)
	{
		printf("Erreur dirp !!!\n");
		return NULL;
	}
	int i = 0;
	while((entry = readdir(dirp)) != NULL)
	{
		if(entry->d_type == DT_REG || (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..") ))
		{
			tab[i] = malloc(sizeof(char*) * 2);
			if(!tab[i])
			{
				printf("Erreur malloc tab[i]\n");
				exit(0);
			}
			tab[i][FS_F_NAME] = malloc( sizeof(char) * (strlen(entry->d_name)+1) );
			strcpy(tab[i][FS_F_NAME], entry->d_name);
			tab[i][FS_F_NAME][strlen(entry->d_name)] = '\0';

			tab[i][FS_F_TYPE] = malloc(sizeof(char) * 2);
			if (entry->d_type == DT_REG)
				tab[i][FS_F_TYPE] = "f";
			else if(entry->d_type == DT_DIR && strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
				tab[i][FS_F_TYPE] = "d";

			i +=1;
		}
	}
	tab[i] = NULL;
	return tab;
}

/*
 * Func : int array_len(void **arr)
 *
 * Desc : Retourner la taille d'un tableau
 */
int array_len(void **arr){
	int i = 0;
	while(arr[i])
		i += 1;
	return i;
}
