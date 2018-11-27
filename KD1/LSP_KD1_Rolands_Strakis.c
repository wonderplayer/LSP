#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

#define LINE_BUFFER 1024*64
typedef enum {true, false} bool;
bool tArg = false;
bool sArg = false;
bool oArg = false;
FILE *fout;

/*Vajadzetu vel parbaudit uz to, vai ir tikai divas linijas*/
bool CompareLines(FILE *file){
	char c;
	int garums = 1;
	int counter = 1;

	while(!feof(file)){
		c = fgetc(file);
		if(c == '\n'){
			counter = -1;
			continue;
		}
		garums += counter;
	}
	if(garums == 0){
		return true;
	}
	else{
		return false;
	}
}

void Translate(FILE *file){
	char *line = malloc(1024*64);
	if(CompareLines(file) == true){
		fseek(file, 0, 0);
		fgets(line, LINE_BUFFER, file);
		fgets(line, LINE_BUFFER, file);
		fprintf(fout, "%s", line);
		return;
	}
	fprintf(fout,"Nesakrit grums!");
	return;
}

/*Vajadzetu ielasit binaro failu un tad katru simbolu ielasot no ievada vaila, tad ar fseek atrast poziciju faila attiecigi
ASCII ievada simbola vertibai un parrakstit ievada simbolu */
void Cypher(FILE *file){

	return;
}

int main(int argc, char **argv){
	int i;
    FILE *tFile;
    fout = stdout;

	for(i = 1; i < argc; i++){
		if(strcmp(argv[i], "-t") == 0){
			tArg = true;	
			tFile = fopen(argv[i+1], "r");
			if(tFile == NULL) {
		      fprintf(fout, "Nevar atvērt failu: '%s'", argv[i+1]);
		      return(-1);
		   	}
		}
		if(strcmp(argv[i], "-s") == 0) sArg = true;
		if(strcmp(argv[i], "-o") == 0){
			oArg = true;
			fout = fopen(argv[i + 1], "w+");
		}
	}
	if(tArg == true && sArg == true){
		fprintf(fout, "-t un -s neiet kopa\n");
		return -1;
	}

	if(tArg == true){
		Translate(tFile);
		fclose(tFile);
	}

	fclose(fout);

	return 0;
}