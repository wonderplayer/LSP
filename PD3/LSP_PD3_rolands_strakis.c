#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

char *fileName;

char *FindFilePath(DIR *d, char *prevPath){
	struct dirent *de;
	DIR *nextD;
	char *path = malloc(256);
	char dirName[256];

	while((de = readdir(d)) != NULL){
		printf("Atvera!\n");
		printf("Apskata: '%s' '%d'\n", de->d_name, de->d_type);
		if((strcmp(de->d_name, ".")) != 0 && (strcmp(de->d_name, "..")) != 0 && de->d_type == 4){
			printf("Atrada direktoriju '%s'\n", de->d_name);
			strcat(dirName, prevPath);
			strcat(dirName, "/");
			strcat(dirName, de->d_name);
			printf("Ver vala '%s'\n", dirName);
			nextD = opendir(dirName);
			if(nextD != NULL){
				path = FindFilePath(nextD, dirName);
			}
			else {
				perror(dirName);
			}
			printf("\n");
		}

	}

	closedir(d);
	
	free(de);
	de = 0;
	free(path);
	path = 0;
	free(nextD);
	nextD = 0;

	return "Kaut kas";
}

int main(int argc, char **argv){
	fileName = malloc(256);
	struct stat buf;
	int exists;
	DIR *d;
	char *path = malloc(256);

	if (argc < 3){
		printf("Kluda: Parak maz argumenti!");
		return -1;
	}

	exists = stat(argv[1], &buf);
	if (exists < 0)
	{
		perror("Faila atversanas kluda: ");
		return -1;
	}
	else{
		printf("Atvera failu '%s' ar izmeru '%ld'\n", argv[1], buf.st_size);
	}
	fileName = argv[1];

	d = opendir(argv[2]);
	if(d != NULL){
		fileName = FindFilePath(d, argv[2]);
	} 
	else {
		perror("Kluda direktorijas atversana: ");
		return -1;
	}

	free(path);
	path = 0;

	return 0;
}