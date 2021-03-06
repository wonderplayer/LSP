#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

typedef enum {true, false} bool;
typedef struct Node Node;
typedef struct Duplicate Duplicate;
typedef struct DuplicateFile DuplicateFile;

struct Node{
	Node *left;
	char *path;
	char *name;
	Node *right;
};

struct DuplicateFile{
	char *path;
	char *name;
	DuplicateFile *next;
};

struct Duplicate{
	DuplicateFile *df;
	Duplicate *next;
};

Node *tree;
Duplicate *duplicates;

void PrintTabs(unsigned char tabCount){
	int i;
	for(i = 0; i < tabCount; i++){
		printf("\t");
	}
}

void PrintTreePreOrder(Node *p){
	if(p == NULL) return;
	printf("Atrada -> '%s/%s'\n", p->path, p->name);
	PrintTreePreOrder(p->left);
	PrintTreePreOrder(p->right);
}

void FreeTree(Node *p){
	if(p == NULL) return;
	FreeTree(p->left);
	FreeTree(p->right);
	printf("Free '%s/%s'\n", p->path, p->name);
	free(p->name);
	free(p->path);
	free(p);
}

void AddDuplicateRecord(char *path, char *name, Duplicate *p){
	DuplicateFile *df = NULL;
	printf("Ieksa AddDuplicateRecord\n");
	printf("Liek ieksa '%s/%s'\n", path, name);
	df = p->df;
	while(df->next != NULL) df = df->next;
	df->next = malloc(sizeof(DuplicateFile));
	df = df->next;
	df->next = NULL;
	df->path = malloc(strlen(path) + strlen(name));
	df->name = malloc(strlen(name));
	strcpy(df->path, path);
	strcat(df->path,"/");
	strcat(df->path, name);
	strcpy(df->name, name);
}

void AddDuplicate(char *path, char *name){
	printf("Ieksa AddDuplicate\n");
	Duplicate *p = duplicates;
	if(p->df == NULL){
		printf("Veido pirmo failu duplikatu ierakstu failam '%s'\n", name);
		p->df = malloc(sizeof(DuplicateFile));
		p->df->next = NULL;
		p->df->path = malloc(strlen(path) + strlen(name));
		p->df->name = malloc(strlen(path));
		strcpy(p->df->path, path);
		strcat(p->df->path,"/");
		strcat(p->df->path, name);
		strcpy(p->df->name, name);
	}
	else {
		while(p->next != NULL){
			if(p->df->name == name){
				AddDuplicateRecord(path, name, p);
				return;
			}
			p = p->next;
		}
		printf("Pievieno duplikatus failam '%s'\n", name);
		p->next = malloc(sizeof(Duplicate));
		p = p->next;
		p->next = NULL;
		p->df = malloc(sizeof(DuplicateFile));
		p->df->next = NULL;
		p->df->path = malloc(strlen(path) + strlen(name));
		p->df->name = malloc(strlen(name));
		strcpy(p->df->path, path);
		strcat(p->df->path,"/");
		strcat(p->df->path, name);
		strcpy(p->df->name, name);
	}
}

void AddFileToRecord(char *path, char *name, unsigned char tabCount){
	Node *p = tree;
	Node *prev = NULL;
	int res, existsCurrentFile, existsCheckFile;
	struct stat currentFile;
	struct stat checkFile;
	char *fullCurrentFilePath = malloc(256);
	char *fullCheckFilePath = malloc(256);
	PrintTabs(tabCount);

	if(tree->path == NULL){
		p->path = malloc(strlen(path));
		p->name = malloc(strlen(name));
		strcpy(p->path, path);
		strcpy(p->name, name);
		printf("Pievienoja '%s/%s'\n", p->path, p->name);
	}
	else {
		while(p != NULL){
			res = strcmp(name, p->name);
			prev = p;

			if(res < 0)
				p = p->left;
			else if(res > 0)
				p = p->right;
			else{
				strcpy(fullCurrentFilePath, path);
				strcat(fullCurrentFilePath, "/");
				strcat(fullCurrentFilePath, name);
				existsCurrentFile = stat(fullCurrentFilePath, &currentFile);
				strcpy(fullCheckFilePath, p->path);
				strcat(fullCheckFilePath, "/");
				strcat(fullCheckFilePath, p->name);
				printf("Path: '%s'\n", fullCheckFilePath);
				existsCheckFile = stat(fullCheckFilePath, &checkFile);
				if(existsCheckFile < 0 || existsCurrentFile < 0){
					perror("Problema ar faila atversanu: ");
					return;
				}
				else {
					if(currentFile.st_size == checkFile.st_size){
						AddDuplicate(path, name);
						return;
					}
					else{
						printf("Dazadz izmers!\n");
						p = p->right;
					}
				}

			}
		}
		p = malloc(sizeof(Node));
		p->left = NULL;
		p->path = malloc(strlen(path));
		strcpy(p->path, path);
		p->name = malloc(strlen(name));
		strcpy(p->name, name);
		p->right = NULL;

		if(res < 0)
			prev->left = p;
		else
			prev->right = p;

		printf("Pievienoja '%s/%s'\n", p->path, p->name);
	}
}


void FindFilePath(DIR *d, char *prevPath, unsigned char tabCount){
	struct dirent *de;
	DIR *nextD;
	char *dirName = malloc(256);

	while((de = readdir(d)) != NULL){
		PrintTabs(tabCount);
		printf("Apskata: '%s' '%d'\n", de->d_name, de->d_type);
		if(de->d_type == 8){
			AddFileToRecord(prevPath, de->d_name, tabCount);
		}
		else if((strcmp(de->d_name, ".")) != 0 && (strcmp(de->d_name, "..")) != 0 && de->d_type == 4){
			PrintTabs(tabCount);
			printf("Atrada direktoriju '%s'\n", de->d_name);
			strcat(dirName, prevPath);
			strcat(dirName, "/");
			strcat(dirName, de->d_name);
			PrintTabs(tabCount);
			printf("Ver vala '%s'\n", dirName);
			nextD = opendir(dirName);
			if(nextD != NULL){
				FindFilePath(nextD, dirName, tabCount + 1);
			}
			else {
				perror(dirName);
			}
		}

	}

	closedir(d);
	free(dirName);
	dirName = NULL;
	de = NULL;
	nextD = NULL;

	return;
}

int main(int argc, char **argv){
	DIR *d;
	tree = malloc(sizeof(Node));
	tree->left = NULL;
	tree->right = NULL;
	tree->path = NULL;
	tree->name = NULL;
	duplicates = malloc(sizeof(Duplicate));
	duplicates->df = NULL;
	duplicates->next = NULL;

	d = opendir(".");
	if(d != NULL){
		FindFilePath(d, ".", 0);
	}
	else {
		perror("Kluda direktorijas atversana: ");
		return -1;
	}
	d = NULL;

	PrintTreePreOrder(tree);
	FreeTree(tree);
	PrintDuplicates();
	tree = NULL;

	return 0;
}
