#ifndef _SCANNERCSVSORTER_H
#define _SCANNERCSVSORTER_H


#define STDIN 0
#define STDOUT 1
#define STDERR 2

struct Record{
  char *key;
  char *fullText;
  struct Record *next;
} record;

int determineSortType(char *);

struct Record* SortLL(struct Record* , struct Record* , int);

void SplitLL(struct Record* source, struct Record**, struct Record**);

const char* getfield(char* line, int num);

void trimQuotes(char* );

const char* getKey(char* , char *, int *);

void printList(struct Record *, char *, char *);

void SplitLL(struct Record* ,struct Record** , struct Record** );

void MergeSort(struct Record**, int);

struct Record* SortLL(struct Record* , struct Record* , int);

void push(struct Record** , char *, char *);




#endif
