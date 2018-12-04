#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>

#define C 1
#define S 2
#define NKEYS (sizeof(lookupTable)/sizeof(paramKeyStruct))
#define MY_BUFFER_SIZE 1024

typedef struct {
    char *key;
    int val;
} paramKeyStruct;

static paramKeyStruct lookupTable[] = {
        {"-c", C},
        {"-s", S}
};

int valueFromString(char *key) {
    int i;
    for (i = 0; i < NKEYS; i++) {
        paramKeyStruct *sym = &lookupTable[i];
        if (strcmp(sym->key, key) == 0)
            return sym->val;
    }
    return 0;
}

typedef struct blockInfoNode {
    bool isFree;
    int blockSize;
    struct blockInfoNode *next;
    unsigned char *p;
    int takenSpace;
} blockInfoNode;

unsigned char myBuffer[MY_BUFFER_SIZE];
blockInfoNode *first;
blockInfoNode *last;
int chunksCount = 0;
int chunksSum = 0;
int sizesCount = 0;
int didntFit = 0;

void readFile(FILE *source, int *array, char fileType) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int i = 0;
    while ((read = getline(&line, &len, source)) != -1) {
        array[i] = (int) strtol(line, NULL, 0);
        i++;
        switch (fileType) {
            case 'S':
                sizesCount++;
                break;
            case 'C':
                chunksSum += (int) strtol(line, NULL, 0);
                chunksCount++;
                break;
            default:
                break;
        }
    }
}

void addNewBlockNode(blockInfoNode *curr){

    blockInfoNode *new;

    new = malloc(sizeof(blockInfoNode)); //Fill new free block data
    new->isFree = true;
    new->blockSize = curr->blockSize - curr->takenSpace;
    new->takenSpace = 0;
    new->p = curr->p + curr->takenSpace;

    if(curr->next == NULL){ //Insert new node into list
        new->next = NULL;
        curr->next = new;
    }
    else{
        new->next = curr->next;
        curr->next = new;
    }
    chunksCount++;
}

bool allocateFreeChunks(char *chunksFile) {

    FILE *source;
    source = fopen(chunksFile, "r");

    if (source == NULL) {
        perror("Error");
        return false;
    } else {
        int chunksSizes[1024] = {0};
        readFile(source, chunksSizes, 'C');
        if(chunksSum > MY_BUFFER_SIZE){
            printf("Can not insert %d bytes of chunks into %d bytes long array\n", chunksSum, MY_BUFFER_SIZE);
            return false;
        }

        first = NULL;
        last = NULL;
        blockInfoNode *curr = NULL;
        int usedBufferSpace = 0;

        int i;
        for (i = 0; chunksSizes[i] != 0; i++) {
            if (chunksSizes[i] <= MY_BUFFER_SIZE - usedBufferSpace) {
                curr = malloc(sizeof(blockInfoNode));
                if (first == NULL) {
                    first = curr;
                    last = curr;
                } else {
                    last->next = curr;
                    last = curr;
                }
                curr->next = NULL;
                curr->p = &myBuffer[usedBufferSpace];
                usedBufferSpace += chunksSizes[i];
                curr->isFree = true;
                curr->takenSpace = 0;
                curr->blockSize = chunksSizes[i];
            } else {
                printf("Not enough space to allocate %d bytes of memory", chunksSizes[i]);
                return false;
            }
        }
        return true;
    }
}

void FirstFit(int *sizes) {

    float start, end; // For getting execution time
    start = clock(); // Start taking time

    int i = 0;
    while (sizes[i] != 0) { // Loop through sizes
        bool insertSuccess = false; // Expect the worst
        blockInfoNode *curr = first; // Set current to first chunk node
        while (curr != NULL) {

            if (curr->blockSize >= sizes[i] && curr->isFree == true) { // Check if size fits and if it hasn't been taken yet
                curr->takenSpace = sizes[i]; // Register the size in chunk
                curr->isFree = false; // Take possession on chunk
                insertSuccess = true; // Take pride

                if(sizes[i] < curr->blockSize){
                    addNewBlockNode(curr);
                }
                printf("%d was inserted in %d\n", curr->takenSpace, curr->blockSize); // Print inserted size and chunk size
                break;
            }

            curr = curr->next; // Go to the next chunk
        }

        if (!insertSuccess) { // If the unfortunate has happened and we couldn't allocate when we just print it
            printf("%d was not inserted!\n", sizes[i]);
		didntFit += sizes[i];
        }

        i++;
    }
    end = clock(); // Stop taking time

    printf("Time required for execution: %f seconds.\n", (end - start) / CLOCKS_PER_SEC);

    float combindedChunkSize = 0;
    float combindedTakenSpace = 0;

    blockInfoNode *curr = first;
    while (curr != NULL) { // Get total size of used chunks and taken space in those chunks
        if (curr->isFree) {
            combindedChunkSize += curr->blockSize;
		if(combindedTakenSpace < curr->blockSize){
	        	combindedTakenSpace = curr->blockSize;
		}
        }

        curr = curr->next;
    }
    float fragmentedSpace = (1 - (combindedTakenSpace / combindedChunkSize)) * 100; // And calculate the fragmented space

    printf("Fragmented Space: %f %%\n", fragmentedSpace);
}

void NextFit(int *sizes){
    float start, end;
    start = clock();

    blockInfoNode *lastUsed = first;
    blockInfoNode *curr;

    bool insertSuccess;
    bool jumpToStart;
    int checkedCunks;

    int i = 0;
    while(sizes[i] != 0){
        insertSuccess = false;
        jumpToStart = false;
        checkedCunks = 0;
        for(curr = lastUsed; checkedCunks < chunksCount; curr = curr->next){

            if(jumpToStart == true){
                curr = first;
            }

            if(curr->blockSize >= sizes[i] && curr->isFree == true){
                lastUsed = curr;
                curr->takenSpace = sizes[i];
                curr->isFree = false;
                insertSuccess = true;

                if(sizes[i] < curr->blockSize){
                    addNewBlockNode(curr);
                }

                printf("%d was inserted in %d\n", curr->takenSpace, curr->blockSize);
                break;
            }
            checkedCunks++;

            if(curr->next == NULL){ //Jump to start
                curr = first;
                jumpToStart = true;
            }
        }
        if (!insertSuccess) { // If the unfortunate has happened and we couldn't allocate when we just print it
            printf("%d was not inserted!\n", sizes[i]);
		didntFit += sizes[i];
        }
        i++;
    }

    end = clock(); // Stop taking time

    printf("Time required for execution: %f seconds.\n", (end - start) / CLOCKS_PER_SEC);

    float combindedChunkSize = 0;
    float combindedTakenSpace = 0;

    curr = first;
    while (curr != NULL) { // Get total size of used chunks and taken space in those chunks
        if (curr->isFree) {
            combindedChunkSize += curr->blockSize;
		if(combindedTakenSpace < curr->blockSize){
	        	combindedTakenSpace = curr->blockSize;
		}
        }

        curr = curr->next;
    }

    float fragmentedSpace = (1 - (combindedTakenSpace / combindedChunkSize)) * 100; // And calculate the fragmented space

    printf("Fragmented Space: %f %%\n", fragmentedSpace);
}

void BestFit(int* sizes) {
    float startTime, endTime;
    startTime = clock();
    int i = 0;
    while (sizes[i] != 0) {
        blockInfoNode* current = first;
        blockInfoNode* bestBlock = NULL;
        int bestFit = INT_MAX;

        while (current != NULL) {
            //loop trough the available blocks and compare the unused space left in each block
            if(current->isFree && current->blockSize - current->takenSpace >= sizes[i]) {
                if(current->blockSize - sizes[i] - current->takenSpace < bestFit && current->blockSize - current->takenSpace - sizes[i] >= 0) {
                    bestFit = current->blockSize - sizes[i] - current->takenSpace;
                    bestBlock = current;
                }
            }
            current = current->next;
        }

        if(bestBlock != NULL) {
            bestBlock->takenSpace += sizes[i];
            if(bestBlock->blockSize == bestBlock->takenSpace) {
                bestBlock->isFree = false;
            }
            printf("%d was inserted in %d\n",sizes[i], bestBlock->blockSize);

        } else {
            printf("%d was not inserted!\n", sizes[i]);
            didntFit += sizes[i];
        }

        i++;
    }

    endTime = clock();
    printf("Time required for execution: %f seconds.\n", (endTime - startTime) / CLOCKS_PER_SEC);

    float combindedChunkSize = 0;
    float combindedTakenSpace = 0;

    blockInfoNode* curr = first;
    while (curr != NULL) { // Get total size of used chunks and taken space in those chunks
        if (curr->isFree) {
            combindedChunkSize += curr->blockSize - curr->takenSpace;
		if(combindedTakenSpace < curr->blockSize - curr->takenSpace){
	        	combindedTakenSpace = curr->blockSize - curr->takenSpace;
		}
        }

        curr = curr->next;
    }
    float fragmentedSpace = (1 - (combindedTakenSpace / combindedChunkSize)) * 100; // And calculate the fragmented space

    printf("Fragmented Space: %f %%\n", fragmentedSpace);
}

void WorstFit(int* sizes) {
    float startTime, endTime;
    startTime = clock();
    int i = 0;
    while (sizes[i] != 0) {
        blockInfoNode* current = first;
        blockInfoNode* bestWorstBlock = NULL;
        int worstFit = 0;

        while (current != NULL) {
            //loop trough the available blocks and compare the unused space in each block
            if(current->isFree && current->blockSize - current->takenSpace >= sizes[i]) {
                if(current->blockSize - sizes[i] - current->takenSpace > worstFit && current->blockSize - current->takenSpace - sizes[i] >= 0) {
                    worstFit = current->blockSize - sizes[i] - current->takenSpace;
                    bestWorstBlock = current;
                }
            }
            current = current->next;
        }

        if(bestWorstBlock != NULL) {
            bestWorstBlock->takenSpace += sizes[i];
            if(bestWorstBlock->blockSize == bestWorstBlock->takenSpace) {
                bestWorstBlock->isFree = false;
            }
            printf("%d was inserted in %d\n",sizes[i], bestWorstBlock->blockSize);

        } else {
            printf("%d was not inserted!\n", sizes[i]);
            didntFit += sizes[i];
        }

        i++;
    }

    endTime = clock();
    printf("Time required for execution: %f seconds.\n", (endTime - startTime) / CLOCKS_PER_SEC);

    float combindedChunkSize = 0;
    float combindedTakenSpace = 0;

    blockInfoNode* current = first;
    while (current != NULL) { // Get total size of used chunks and taken space in those chunks
        if (current->isFree) {
            combindedChunkSize += current->blockSize - current->takenSpace;
		if(combindedTakenSpace < current->blockSize - current->takenSpace){
	        	combindedTakenSpace = current->blockSize - current->takenSpace;
		}
        }

        current = current->next;
    }
    float fragmentedSpace = (1 - (combindedTakenSpace / combindedChunkSize)) * 100; // And calculate the fragmented space

    printf("Fragmented Space: %f %%\n", fragmentedSpace);
}

int main(int argc, char **argv) {

    if (argc <= 1) {
        printf("No parameters were specified!\n");
        return 1;
    }

    printf("Choose algorithm:\n");
    printf("1. First fit\n");
    printf("2. Next fit\n");
    printf("3. Best fit\n");
    printf("4. Worst fit\n");

    int chosenAlgorithm = 4;
//    scanf("%d", &chosenAlgorithm);


    char chunksFile[50];
    char sizesFile[50];

    bool chunksProvided = false;
    bool sizesProvided = false;

    int i;
    for (i = 1; i < argc; i++) {
        switch (valueFromString(argv[i])) {
            case C:
                strcpy(chunksFile, argv[i + 1]);
                chunksProvided = true;
                i++;
                break;
            case S:
                strcpy(sizesFile, argv[i + 1]);
                sizesProvided = true;
                i++;
                break;
            default:
                printf("%s\n", "Provide correct parameters!");
                return 1;
        }
    }

    if (chunksProvided && sizesProvided) {
        bool allocatedChunks = allocateFreeChunks(chunksFile);
        if (allocatedChunks) {
            /*blockInfoNode *curr;
            for(curr = first;curr!=NULL;curr=curr->next){
                printf("Chunk => size: %d, status: %d, address: %p\n", curr->blockSize, curr->isFree, curr->p);
            }*/
            FILE *source;
            source = fopen(sizesFile, "r");
            if (source == NULL) {
                perror("Error");
                return 1;
            } else {
                int sizes[1024] = {0};
                readFile(source, sizes, 'S');
                switch (chosenAlgorithm) {
                    case 1:
                        FirstFit(sizes);
                        break;
                    case 2:
                        NextFit(sizes);
                        break;
                    case 3:
                        BestFit(sizes);
                        break;
                    case 4:
                        WorstFit(sizes);
                        break;
                }
                printf("Total block size that didn't fit: '%d'\n", didntFit);
                return 0;
            }
        } else {
            return 1;
        }
    } else {
        printf("%s\n", "Provide files with chunks and sizes!");
        return 1;
    }
}
