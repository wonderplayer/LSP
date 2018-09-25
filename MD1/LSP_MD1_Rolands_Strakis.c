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
/*
void FindMembers(char *personName, char *fatherName, char *motherName, Tree *pTree, Tree *fTree, Tree *mTree,  Tree *curr, Person *parent){
	fprintf(stdout, "In FindMembers function\n");
	int i;
	for(i = 0; i < curr->size; i++){
		fprintf(stdout, "Comparing '%s' with '%s'\n", curr->person[i]->name, personName);
		if(strcmp(curr->person[i]->name, personName) == 0){
			pTree = curr;
		}

		fprintf(stdout, "Comparing '%s' with '%s'\n", curr->person[i]->name, fatherName);
		if(strcmp(curr->person[i]->name, fatherName) == 0){
			fprintf(stdout, "Atrada!\n");
			fTree = curr;
			if(parent == 0) parent = curr->person[i];
		}

		fprintf(stdout, "Comparing '%s' with '%s'\n", curr->person[i]->name, motherName);
 		if(strcmp(curr->person[i]->name, motherName) == 0){
			fprintf(stdout, "Atrada!\n");
			mTree = curr;
			if(parent == 0) parent = curr->person[i];
		}
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

	if(personName[0] == '\0' && motherName[0] == '\0' && fatherName[0] == '\0'){
		fprintf(stdout, "Tuksi dati\n");
		return;
	}

	while(p != 0){
		pTree = FindPerson(personName, p);
		fTree = FindFather(fatherName, p);
		mTree = FindMother(motherName, p);
		p = p->next;
	}

	if(pTree == 0 && mTree == 0 && fTree == 0){
		fprintf(stdout, "Needs to create new tree\n");
		CreateNewTree(personName, motherName, fatherName);
	}

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
	free(vards);
	free(mate);
	free(tevs);

	type = 0;
	name = 0;
	vards = 0;
	mate = 0;
	tevs = 0;

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
