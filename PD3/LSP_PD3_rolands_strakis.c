#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

typedef enum {true, false} bool;
char *fileName;
bool atrada = false;

void FindFilePath(DIR *d, char *prevPath){
	struct dirent *de;
	DIR *nextD;
	char *dirName = malloc(256);

	while((de = readdir(d)) != NULL){
		printf("Apskata: '%s' '%d'\n", de->d_name, de->d_type);

		if(strcmp(de->d_name, fileName) == 0){
			printf("%s/%s\n", prevPath, de->d_name);
			atrada = true;
			return;
		}

		if((strcmp(de->d_name, ".")) != 0 && (strcmp(de->d_name, "..")) != 0 && de->d_type == 4){
			printf("Atrada direktoriju '%s'\n", de->d_name);
			strcat(dirName, prevPath);
			strcat(dirName, "/");
			strcat(dirName, de->d_name);
			printf("Ver vala '%s'\n", dirName);
			nextD = opendir(dirName);
			if(nextD != NULL){
				FindFilePath(nextD, dirName);
			}
			else {
				perror(dirName);
			}
			if(atrada == true) return;
			printf("\n");
		}

	}

	closedir(d);

	return;
}

int main(int argc, char **argv){
	DIR *d;
	char *path = malloc(256);

	if (argc < 3){
		printf("Kluda: Parak maz argumenti!");
		return -1;
	}

	fileName = argv[1];

	d = opendir(argv[2]);
	if(d != NULL){
		FindFilePath(d, argv[2]);
	}
	else {
		perror("Kluda direktorijas atversana: ");
		return -1;
	}

	if(atrada == false){
		printf("Neatrada failu '%s'\n", fileName);
	}

	free(path);
	path = 0;

	return 0;
}
