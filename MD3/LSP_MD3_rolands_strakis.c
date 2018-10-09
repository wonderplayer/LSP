#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
bool dParam;

void PrintTreePreOrder(Node *p){
	if(p == NULL) return;
	PrintTreePreOrder(p->left);
	PrintTreePreOrder(p->right);
}

void FreeTree(Node *p){
	if(p == NULL) return;
	FreeTree(p->left);
	FreeTree(p->right);
	free(p->name);
	free(p->path);
	free(p);
}

void PrintDuplicates(){
	Duplicate *d = duplicates;
	DuplicateFile *df;
	struct stat buf;
	int error;
	char timeBuffer[26];
	struct tm *tm_info;
	while(d != NULL){
		df = d->df;

		if(df == NULL) return;

		error = stat(df->path, &buf);
		if(error < 0){
			perror("PrintDuplicates: ");
		}
		tm_info = localtime(&buf.st_mtime);
		strftime(timeBuffer, 26, "%Y-%m-%d %H:%M", tm_info);
		printf("=== %s %ld %s\n", timeBuffer, buf.st_size, df->name);
		while(df != NULL){
			printf("%s\n", df->path + 2);
			df = df->next;
		}
		printf("\n");
		d = d->next;

	}
}

void FreeDuplicateFile(DuplicateFile *df){
	free(df->path);
	free(df->name);
	df->path = NULL;
	df->name = NULL;
	free(df);
}

void FreeDuplicates(){
	Duplicate *d = duplicates;
	DuplicateFile *df = d->df;
	while(d != NULL){
		while(df != NULL){
			df = d->df->next;
			FreeDuplicateFile(d->df);
			d->df = df;
		}
		d = duplicates->next;
		free(duplicates);
		duplicates = d;
	}
}

void AddDuplicateRecord(char *path, char *name, Duplicate *p){
	DuplicateFile *df = p->df;
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

void AddDuplicate(char *path, char *name, Node *n){
	Duplicate *p = duplicates;
	Duplicate *prev = duplicates;
	if(p->df == NULL){
		p->next = NULL;
		p->df = malloc(sizeof(DuplicateFile));
		p->df->next = NULL;
		p->df->path = malloc(strlen(n->path) + strlen(n->name));
		p->df->name = malloc(strlen(n->name));
		strcpy(p->df->path, n->path);
		strcat(p->df->path,"/");
		strcat(p->df->path, n->name);
		strcpy(p->df->name, n->name);
		p->df->next = malloc(sizeof(DuplicateFile));
		p->df->next->next = NULL;
		p->df->next->path = malloc(strlen(path) + strlen(name));
		p->df->next->name = malloc(strlen(name));
		strcpy(p->df->next->path, path);
		strcat(p->df->next->path,"/");
		strcat(p->df->next->path, name);
		strcpy(p->df->next->name, name);
	}
	else {
		while(p != NULL){
			if(strcmp(p->df->name, name) == 0){
				AddDuplicateRecord(path, name, p);
				return;
			}
			prev = p;
			p = p->next;
		}
		p = malloc(sizeof(Duplicate));
		p->next = NULL;
		p->df = malloc(sizeof(DuplicateFile));
		p->df->next = NULL;
		p->df->path = malloc(strlen(n->path) + strlen(n->name));
		p->df->name = malloc(strlen(n->name));
		strcpy(p->df->path, n->path);
		strcat(p->df->path,"/");
		strcat(p->df->path, n->name);
		strcpy(p->df->name, n->name);
		p->df->next = malloc(sizeof(DuplicateFile));
		p->df->next->next = NULL;
		p->df->next->path = malloc(strlen(path) + strlen(name));
		p->df->next->name = malloc(strlen(name));
		strcpy(p->df->next->path, path);
		strcat(p->df->next->path,"/");
		strcat(p->df->next->path, name);
		strcpy(p->df->next->name, name);
		prev->next = p;
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

	if(tree->path == NULL){
		p->path = malloc(strlen(path));
		p->name = malloc(strlen(name));
		strcpy(p->path, path);
		strcpy(p->name, name);
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
				existsCheckFile = stat(fullCheckFilePath, &checkFile);
				if(existsCheckFile < 0 || existsCurrentFile < 0){
					perror("Problema ar faila atversanu: ");
					return;
				}
				else {
					if(currentFile.st_size == checkFile.st_size){
						if(dParam == true){
							if(currentFile.st_mtime == checkFile.st_mtime){
								AddDuplicate(path, name, p);
								return;
							}
							p = p->right;
							continue;
						}

						AddDuplicate(path, name, p);
						return;
					}
					else{
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

	}
}


void FindFilePath(DIR *d, char *prevPath, unsigned char tabCount){
	struct dirent *de;
	DIR *nextD;
	char *dirName = malloc(256);

	while((de = readdir(d)) != NULL){
		if(de->d_type == 8){
			AddFileToRecord(prevPath, de->d_name, tabCount);
		}
		else if((strcmp(de->d_name, ".")) != 0 && (strcmp(de->d_name, "..")) != 0 && de->d_type == 4){
			strcpy(dirName, prevPath);
			strcat(dirName, "/");
			strcat(dirName, de->d_name);
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
	int i;
	dParam = false;
	for(i = 1; i < argc; i++){
		if(strcmp(argv[i], "-h") == 0){
			printf("\tPamaciba par argumentiem\n");
			printf("\t-d\tSalidzinat failus ari pec datuma\n");
			printf("\t-m\tAprekina failu MD5 vertibas (Nav implementets)\n");
			printf("\t-h\tPalidziba\n");
			return 0;
		}
		if(strcmp(argv[i], "-d") == 0) dParam = true;
	}

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
	FreeDuplicates();
	tree = NULL;

	return 0;
}
