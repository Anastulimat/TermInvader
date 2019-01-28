#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FS_F_NAME 0
#define FS_F_TYPE 1


/****************************** File System Fonctions ******************************/
char*	remove_eol(char *s);

int 	is_directory(const char *path);

int 	count_files_in_dir();

char* 	read_line(FILE* file);

char* 	read_first_line(char* filename);

char**	read_file(char* filename);

char***	read_directory(char* dir_name);

int 	array_len(void **arr);

#endif