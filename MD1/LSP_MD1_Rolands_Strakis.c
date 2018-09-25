#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {true, false} bool;
typedef struct Person Person;
typedef struct Tree Tree;

struct Person{
	char *name;
	int generation;
};

struct Tree{
	Tree *next;
	Person **person;
	int size;
};

Tree *tree;
Person *parent;

char CompareType(char *type){
	int i = 0;
	bool match = false;
	char mType = -1;
	char vards[] = "VARDS";
	char mate[] = "MATE";
	char tevs[] = "TEVS";
	for(i = 0; i < sizeof(vards); i++){
		if(type[i] != vards[i]){
			match = false;
			break;
		}
		match = true;
		mType = 1;
	}

	if(match == false){
		for(i = 0; i < sizeof(mate); i++){
			if(type[i] != mate[i]){
				match = false;
				break;
			}
			match = true;
			mType = 2;
		}

	}

	if(match == false){
		for(i = 0; i < sizeof(tevs); i++){
			if (type[i] != tevs[i]){
				match = false;
				break;
			}
			match = true;
			mType = 3;
		}
	}

	if(match == false) return -1;

	return mType;
}

Tree *AddNewTree(char *personName, char *motherName, char *fatherName){
	fprintf(stdout, "INFO: In AddNewTree function\n");
	Tree *newTree = malloc(sizeof(Tree));
	Person *mother = malloc(sizeof(Person));
	Person *person = malloc(sizeof(Person));
	Person *father = malloc(sizeof(Person));

	newTree->size = 0;
	newTree->next = 0;
	newTree->person = malloc(100 * sizeof(Person));

	fprintf(stdout, "Writing '%s %p'\n", personName, personName);
	person->name = personName;
	person->generation = 0;
	newTree->person[0] = person;
	newTree->size++;

	if(fatherName[0] != '\0'){
		fprintf(stdout, "Writing '%s %p'\n", fatherName, fatherName);
		father->name = fatherName;
		father->generation = -1;
		newTree->person[1] = father;
		newTree->size++;
	}

	if(motherName[0] != '\0'){
		fprintf(stdout, "Writing '%s %p'\n", motherName, motherName);
		mother->name = motherName;
		mother->generation = -1;
		newTree->person[2] = mother;
		newTree->size++;
	}

	person = 0;
	father = 0;
	mother = 0;

	return newTree;
}

void CreateNewTree(char *personName, char *motherName, char *fatherName){
	fprintf(stdout, "INFO: In CreateNewTree function\n");
	Tree *p = tree;
	Person *person = malloc(sizeof(Person));
	Person *father = malloc(sizeof(Person));
	Person *mother = malloc(sizeof(Person));

	if(p->size == 0){

		fprintf(stdout, "Writing '%s %p'\n", personName, personName);
		person->name = personName;
		person->generation = 0;
		p->person[0] = person;
		p->size++;

		if(fatherName[0] != '\0'){
			fprintf(stdout, "Writing '%s %p'\n", fatherName, fatherName);
			father->name = fatherName;
			father->generation = -1;
			p->person[1] = father;
			p->size++;
		}

		if(motherName[0] != '\0'){
			fprintf(stdout, "Writing '%s %p'\n", motherName, motherName);
			mother->name = motherName;
			mother->generation = -1;
			p->person[2] = mother;
			p->size++;
		}

		person = 0;
		father = 0;
		mother = 0;

		return;
	}

	while(p->next != 0) p = p->next;

	p->next = AddNewTree(personName, motherName, fatherName);

	p = 0;
}

Tree *FindPerson(char *personName, Tree *curr){
	fprintf(stdout, "In FindPerson function\n");
	int i;
	if(personName[0] == '\0') return NULL;
	for(i = 0; i < curr->size; i++){
		fprintf(stdout, "Comparing '%s' with '%s'\n", curr->person[i]->name, personName);
		if(strcmp(curr->person[i]->name, personName) == 0){
			fprintf(stdout, "Atrada!\n");
			return curr;
		}
	}
	return 0;
}

Tree *FindFather(char *fatherName, Tree *curr){
	fprintf(stdout, "In FindFather function\n");
	int i;
	if(fatherName[0] == '\0') return NULL;
	for(i = 0; i < curr->size; i++){
	        fprintf(stdout, "Comparing '%s' with '%s'\n", curr->person[i]->name, fatherName);
	        if(strcmp(curr->person[i]->name, fatherName) == 0){
	            fprintf(stdout, "Atrada!\n");
	            if(parent == 0) parent = curr->person[i];
	            return curr;
	        }
	}
	return 0;
}

Tree *FindMother(char *motherName, Tree *curr){
	fprintf(stdout, "In FindMother function\n");
	int i;
	if(motherName[0] == '\0') return NULL;
	for(i = 0; i < curr->size; i++){
	        fprintf(stdout, "Comparing '%s' with '%s'\n", curr->person[i]->name, motherName);
	        if(strcmp(curr->person[i]->name, motherName) == 0){
	            fprintf(stdout, "Atrada!\n");
	            if(parent == 0) parent = curr->person[i];
	            return curr;
	        }
	}
	return 0;
}

void PrintFamilies(){
	Tree *p = tree;
	int i;
	while(p != NULL){
		for(i = 0; i < p->size; i++){
			fprintf(stdout, "%s\t%d\n", p->person[i]->name, p->person[i]->generation);
		}
		p = p->next;
		fprintf(stdout, "\n");
	}

}

void SortFamilies(){
	Tree *p = tree;
	Person *temp;
	int i, j;
	while(p != NULL){
		for(i = 0; i < p->size - 1; i++){
			for(j = 0; j < p->size - i - 1; j++){
				if(p->person[i]->generation > p->person[i+1]->generation){
					temp = p->person[i];
					p->person[i] = p->person[i+1];
					p->person[i+1] = temp;
				}
			}
		}
		p = p->next;
	}
}
/*
int GetGenerationDifference(Tree *fTree, Tree *mTree, char *fatherName, char *motherName){
	fprintf(stdout, "In function GetGenerationDifference\n");
	Person *father;
	Person *mother;
	int i;

	for(i = 0; i < fTree->size; i++)
		if(strcmp(fTree->person[i]->name, fatherName) == 0){
			father = fTree->person[i];
			parent = father;
			break;
		}

	for(i = 0; i < mTree->size; i++)
		if(strcmp(mTree->person[i]->name, motherName) == 0){
			mother = mTree->person[i];
			break;
		}

	return father->generation - mother->generation;
}

void MergeTrees(Tree *fTree, Tree *mTree, int genDiff){
	int i;

	if(fTree->size + mTree->size > sizeof(fTree->person)){
		fprintf(stdout, "Realloc no '%ld' uz '%d'\n", sizeof(fTree->person), fTree->size + mTree->size);
		realloc(fTree->person, sizeof(fTree->person) + mTree->size);
	}
	for(i = 0; i < mTree->size; i++){
		mTree->person[i]->generation = mTree->person[i]->generation + genDiff;
		fTree->person[fTree->size] = mTree->person[i];
		fTree->size++;
	}
}
*/
void AddMember(char *personName, char *motherName, char *fatherName){
	fprintf(stdout, "INFO: In AddMember function\n");
	Tree *pTree = 0;
	Tree *fTree = 0;
	Tree *mTree = 0;
	Tree *p = tree;
	parent = 0;
	Person *person = malloc(sizeof(Person));
	int genDiff;

	if(personName[0] == '\0' && motherName[0] == '\0' && fatherName[0] == '\0'){
		fprintf(stdout, "Tuksi dati\n");
		return;
	}

	while(p != 0){
		if(pTree == 0) pTree = FindPerson(personName, p);
		if(fTree == 0) fTree = FindFather(fatherName, p);
		if(mTree == 0) mTree = FindMother(motherName, p);
		p = p->next;
	}

	if(pTree == 0 && mTree == 0 && fTree == 0){
		fprintf(stdout, "Needs to create new tree\n");
		CreateNewTree(personName, motherName, fatherName);
	}
/*
	if(pTree == 0 && mTree != 0 && fTree != 0 && mTree != fTree){
		fprintf(stdout, "Merge tree\n");
		genDiff = GetGenerationDifference(fTree, mTree, fatherName, motherName);
		fprintf(stdout, "genDiff: '%d'\n", genDiff);
		MergeTrees(fTree, mTree, genDiff);
		person->name = personName;
		person->generation = parent->generation + 1;
		fTree->person[fTree->size] = person;
		fTree->size++;
	}
*/
	if(pTree == 0 && fTree == mTree && fTree != 0){
		fprintf(stdout, "Adding new person '%s'\n", personName);
		person->name = personName;
		person->generation = parent->generation + 1;
		fTree->person[fTree->size] = person;
		fTree->size++;
	}

	parent = 0;
	pTree = 0;
	fTree = 0;
	mTree = 0;
	p = 0;
	person = 0;
}


int main(){

	char buffer[64];
	char *type = (char *) malloc(6);
	char *name = (char *) malloc(60);
	char *vards = (char *) malloc(60);
	char *mate = (char *) malloc(60);
	char *tevs = (char *) malloc(60);
	char mType;
	tree = malloc(sizeof(Tree));
	tree->next = 0;
	tree->size = 0;
	tree->person = malloc(100 * sizeof(Person));

	fgets(buffer, sizeof(buffer), stdin);
	sscanf(buffer,"%s %s\n", type, name);

	while(!feof(stdin))
	{
		fprintf(stdout, "Read line: %s %s\n", type, name);
		if (buffer[0] == '\n'){
			fprintf(stdout, "Padod datus: VARDS '%s'\tMATE '%s'\tTEVS '%s'\n", vards, mate, tevs);
			AddMember(vards, mate, tevs);
			vards = malloc(60);
			mate = malloc(60);
			tevs = malloc(60);
			vards[0] = '\0';
			mate[0] = '\0';
			tevs[0] = '\0';
		}

		mType = CompareType(type);
		switch(mType){
			case 1:
				vards = name;
				fprintf(stdout, "Got VARDS as '%s'\n", vards);
				break;

			case 2:
				mate = name;
				fprintf(stdout, "Got MATE as '%s'\n", mate);
				break;

			case 3:
				tevs = name;
				fprintf(stdout, "Got TEVS as '%s'\n", tevs);
				break;
		}

		type = malloc(6);
		name = malloc(60);
		fgets(buffer, sizeof(buffer), stdin);
		sscanf(buffer,"%s %s\n", type, name);
	}

	if(vards[0] != '\0'){
        	fprintf(stdout, "Padod datus: VARDS '%s'\tMATE '%s'\tTEVS '%s'\n", vards, mate, tevs);
        	AddMember(vards, mate, tevs);
	}
	free(type);
	free(name);

	type = 0;
	name = 0;
	vards = 0;
	mate = 0;
	tevs = 0;

	SortFamilies();

	fprintf(stdout, "\n");
	fprintf(stdout, "Reals outputs\n");
	fprintf(stdout, "\n");

	PrintFamilies();

	Tree *p = tree;
	while(p != 0){
		p = tree->next;
		free(tree);
		tree = p;
	}
	p = 0;
	tree = 0;

	return 0;

}
