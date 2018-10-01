#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "simpleCSVsorter.h"


int main(int argc, char **argv)
{
int err = 0;

	if (argc != 3) {
		err = 1;
	} else if (argc == 3) {
		if (strcmp(argv[1], "-c") != 0) {
			err = 1;
		}
	}

	if (err != 0) {
		fprintf(stderr, "syntax: simpleCSVsorter -c <column_name>\n");
		return 1;
	}

    struct Record* record = NULL;

		printf("Argument %s\n", argv[2]);
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    int keycol = 0;
    int *ptrkeycol = &keycol;

    char *field = NULL;



    getline(&line, &nread, stdin); //READS THE FIRST LINE
		char *firstline = strdup(line);


		char *key = getKey(firstline, argv[2], ptrkeycol); //CHECKS IF COLUMN TO BE SORTED EXISTS

		while ((nread = getline(&line, &len, stdin)) != -1) { //READS REST OF THE FILE
        	if(nread > 1){
			char* tmp = strdup(line);

        field = strdup(getfield(tmp,keycol));

        push(&record,field,line); //CREATES NEW NODE AND STORES ROW

        free(tmp);
		}
	 }
	int keyType = determineSortType(record);
	// if(keyType == 1){
 //  MergeSort(&record);
	// }
	// else{
 //   printList(record);
 // }
return 0;
}
