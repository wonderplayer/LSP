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
	Person *person[100];
	int size;
};

Tree tree ;

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
	Tree *newTree = &(Tree){.size = 0, .next = 0};
	fprintf(stdout, "Writing '%s %p'\n", personName, personName);
	Person *person;
	fprintf(stdout, "Person memory '%s %p'", person->name, person->name);
	person->name = personName;
	person->generation = 0;
	fprintf(stdout, "Person memory '%s %p'", person->name, person->name);
	fprintf(stdout, "Writing '%s %p'\n", fatherName, fatherName);
	Person *father = &(Person) {fatherName, -1};
	fprintf(stdout, "Writing '%s %p'\n", motherName, motherName);
	Person *mother = &(Person) {motherName, -1};

	newTree->person[0] = person;
	fprintf(stdout, "INFO: Ierakstija: '%s'\n", newTree->person[0]->name);
	newTree->person[1] = father;
	newTree->person[2] = mother;
	newTree->size = 3;

	return newTree;
}

void CreateNewTree(char *personName, char *motherName, char *fatherName){
	fprintf(stdout, "INFO: In CreateNewTree function\n");
	Tree *p = &tree;
	if(p->size == 0){
		fprintf(stdout, "Writing '%s %p'\n", personName, personName);
		Person person;
		Person *personP = &person;
		fprintf(stdout, "Person memory '%p'\n", personP->name);
		personP->name = (char *) malloc(60);
		strcpy(personP->name, personName);
		personP->generation = 0;
		fprintf(stdout, "Person memory '%s %p'\n", personP->name, personP->name);
		fprintf(stdout, "Writing '%s %p'\n", fatherName, fatherName);
		fprintf(stdout, "Writing '%s %p'\n", fatherName, fatherName);
		Person *father = &(Person) {fatherName, -1};
		fprintf(stdout, "Writing '%s %p'\n", motherName, motherName);
		Person *mother = &(Person) {motherName, -1};

		p->person[0] = personP;
		fprintf(stdout, "INFO: Ierakstija: '%s %p'\n", p->person[0]->name, p->person[0]->name);
		p->person[1] = father;
		p->person[2] = mother;
		p->size = 3;
		return;
	}

	while(p->next != 0) p = p->next;

	p->next = AddNewTree(personName, motherName, fatherName);
}

int FindMembers(char *personName, char *fatherName, char *motherName, Tree *pTree, Tree *fTree, Tree *mTree,  Tree *curr){
	fprintf(stdout, "In FindMembers function\n");
	int i;
	int generation = 0;
	for(i = 0; i < curr->size; i++){
		fprintf(stdout, "Comparing '%s' with '%s'\n", curr->person[i]->name, personName);
		if(curr->person[i]->name == personName){
			pTree = curr;
			generation = curr->person[i]->generation;
		}
		fprintf(stdout, "Comparing '%s' with '%s'\n", curr->person[i]->name, fatherName);
		if(curr->person[i]->name == fatherName){
			fTree = curr;
			generation = curr->person[i]->generation + 1;
		}
		fprintf(stdout, "Comparing '%s' with '%s'\n", curr->person[i]->name, motherName);
		if(curr->person[i]->name == motherName){
			mTree = curr;
			generation = curr->person[i]->generation + 1;
		}
	}

	return generation;
}

void AddMember(char *personName, char *motherName, char *fatherName){
	fprintf(stdout, "INFO: In AddMember function\n");
	Tree *pTree = 0;
	Tree *fTree = 0;
	Tree *mTree = 0;
	Tree *p = &tree;
	int generation;

	if(personName[0] == '\0' && motherName[0] == '\0' && fatherName[0] == '\0'){
		fprintf(stdout, "Tuksi dati\n");
		return;
	}

	while(p != 0){
		generation = FindMembers(personName, fatherName, motherName, pTree, fTree, mTree, p);
		p = p->next;
	}

	if(pTree == 0 && mTree == 0 && fTree == 0){
		fprintf(stdout, "Needs to create new tree\n");
		CreateNewTree(personName, motherName, fatherName);
		fprintf(stdout, "INFO: Ierakstija: '%s %p'\n", tree.person[0]->name, tree.person[0]->name);
		return;
	}

	if(pTree == 0 && fTree == mTree){
		Person person = {personName, generation};
		fTree->person[fTree->size] = &person;
		return;
	}
}


int main(){

	char buffer[64];
	char *type = (char *) malloc(6);
	char *name = (char *) malloc(60);
	char *vards = (char *) malloc(60);
	char *mate = (char *) malloc(60);
	char *tevs = (char *) malloc(60);
	char mType;
	tree.next = 0;
	tree.size = 0;

	fgets(buffer, sizeof(buffer), stdin);
	sscanf(buffer,"%s %s\n", type, name);

	while(!feof(stdin))
	{
		fprintf(stdout, "Read line: %s %s\n", type, name);
		if (buffer[0] == '\n'){
			fprintf(stdout, "Padod datus: VARDS '%s'\tMATE '%s'\tTEVS '%s'\n", vards, mate, tevs);
			AddMember(vards, mate, tevs);
			fprintf(stdout, "INFO: Ierakstija: '%s %p'\n", tree.person[0]->name, tree.person[0]->name);
			fprintf(stdout, "INFO: Ierakstija: '%s %p'\n", tree.person[0]->name, tree.person[0]->name);
			fprintf(stdout, "INFO: Ierakstija: '%s %p'\n", tree.person[0]->name, tree.person[0]->name);
			/*mate = "";
			vards = "";
			tevs = "";*/
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

		type[0] = '\0';
		name = malloc(60);
		fgets(buffer, sizeof(buffer), stdin);
		sscanf(buffer,"%s %s\n", type, name);
	}

/*
	char buffer[100] = "Some string\n";
	char *str = (char *) malloc(60);
	char *str2;
	str2 = str;
	fprintf(stdout, "'%s  %p'\t'%s  %p'\n", str, str, str2, str2);
	str = "Some string";
	str2 = str;
	fprintf(stdout, "'%s  %p'\t'%s  %p'\n", str, str, str2, str2);
	str = "Some string2";
	fprintf(stdout, "'%s  %p'\t'%s  %p'\n", str, str, str2, str2);
	Person *p = &(Person) {str, 0};
	fprintf(stdout, "Name: '%s'  Generation: '%d'\n", p->name, p->generation);
*/
	return 0;

}
