#include <stdio.h>

#define MY_BUFFER_SIZE 4096
#define ALLOCATED_FLAG_SIZE 1

unsigned char mybuffer[MY_BUFFER_SIZE];

void *myalloc(size_t size);
int myfree(void * ptr);
int last = 0;

/*
	Tā kā laika palika maz, uzrakstīšu algoritma ideju:
	Masīvs galbā tādu kā struktūru atmiņai - atmiņas daudzums, brīva vai nebrīva (0 vai 1), atmiņa, nulles simbols.
	Teorētiski sākumā meklējam sākotnējā brīvaja blokā atmiņu. Kad vairs nevaram izdalīt no brīvā bloka, tad ar cikla palīdzību
	aizejam uz masīva sākumu un pārbaudām blokus, kuriem slots, kurš norāda brīva vai nebrīva atmiņa, norada, ka brīva, tad uzzinam, vai
	pietiek atmiņas. Ja pietiek, tad izdalām atmiņu un uzliekam cik aizņem, nebrīva un beigu simbolu. Tad pēc beigu simbola uzliekam cik nākamajā
	brīvaja blokā ir brīvās atmiņas un flagu brīva. Tā darīt, kamēr neizejam visu masīvu. Ja neielikām, tad atgriezt NULL.
	Ar atbrīvošanu ļoti vienkārši - paņemam pointeri, tad uzliekam, ka tā atmiņa ir brīva un pointeri uzliekam, lai tas norāda uz NULL.
	Netika iedomāta brīvās atmiņas sapludināšana. To varētu viegli realizēt saplūdinot blakus blokus - izdzēšot pa vidu nulles simbolu un tad parskaitļot
	izmēru un ielikt to sākumā.
*/
void *myalloc(size_t size){
	void *mem;
	int seekStart = last;
	unsigned long freeSize;
	/*while(seekStart != MY_BUFFER_SIZE){*/
		freeSize = *(unsigned long *)(mybuffer + last);
		if(freeSize > size){
			mem = &mybuffer[sizeof(unsigned long) + ALLOCATED_FLAG_SIZE + last];
			*(unsigned long *)(mybuffer + last) = (unsigned long)size;
			mybuffer[last + sizeof(unsigned long)] = 1;
			mybuffer[last + sizeof(unsigned long) + ALLOCATED_FLAG_SIZE + size] = '\0';
			last += sizeof(unsigned long) + ALLOCATED_FLAG_SIZE + size + 1;
			*(unsigned long *)(mybuffer + last) = MY_BUFFER_SIZE - last;
			mybuffer[last + sizeof(unsigned long)] = 0;
			return mem;
		}
	/*}*/

	return NULL;
}

int myfree(void *ptr){

	return -1;
}

int main(){
	unsigned long *temp = (unsigned long *)mybuffer;
	*temp = MY_BUFFER_SIZE;
	mybuffer[sizeof(unsigned long)] = 0;
	/*
	printf("%ld\n", *(unsigned long *)mybuffer);
	printf("%ld\n", *(unsigned long *)(mybuffer + 1));
	*/
	void *a = myalloc(4000);
	printf("%ld\n", *(unsigned long *)(a - 1 - sizeof(unsigned long)));
	printf("%d\n", *(int *)(a - 1));
	printf("%ld\n", *(unsigned long *)(mybuffer + last));

	void *b = myalloc(90);
	if(b == NULL){
		printf("%ld\n", *(unsigned long *)(mybuffer + last));
	}

	return 0;
}