#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "scannerCSVsorter.h"
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>


int determineSortType(char *key){
  int keyType = 0;
          if (strcmp(key, "color") == 0)
						keyType = 0;
					else if (strcmp(key, "director_name") == 0)
						keyType = 0;
					else if (strcmp(key, "num_critic_for_reviews") == 0)
						keyType = 1;
					else if (strcmp(key, "duration") == 0)
						keyType = 1;
					else if (strcmp(key, "director_facebook_likes") == 0)
						keyType = 1;
					else if (strcmp(key, "actor_3_facebook_likes") == 0)
						keyType = 1;
					else if (strcmp(key, "actor_2_name") == 0)
						keyType = 0;
					else if (strcmp(key, "actor_1_facebook_likes") == 0)
						keyType = 1;
					else if (strcmp(key, "gross") == 0)
						keyType = 1;
					else if (strcmp(key, "genres") == 0)
						keyType = 0;
					else if (strcmp(key, "actor_1_name") == 0)
						keyType = 0;
					else if (strcmp(key, "movie_title") == 0)
						keyType = 0;
					else if (strcmp(key, "num_voted_users") == 0)
						keyType = 1;
					else if (strcmp(key, "cast_total_facebook_likes") == 0)
						keyType = 1;
					else if (strcmp(key, "actor_3_name") == 0)
						keyType = 0;
					else if (strcmp(key, "facenumber_in_poster") == 0)
						keyType = 1;
					else if (strcmp(key, "plot_keywords") == 0)
						keyType = 0;
					else if (strcmp(key, "movie_imdb_link") == 0)
						keyType = 0;
					else if (strcmp(key, "num_user_for_reviews") == 0)
						keyType = 1;
					else if (strcmp(key, "language") == 0)
						keyType = 0;
					else if (strcmp(key, "country") == 0)
						keyType = 0;
					else if (strcmp(key, "content_rating") == 0)
						keyType = 0;
					else if (strcmp(key, "budget") == 0)
						keyType = 1;
					else if (strcmp(key, "title_year") == 0)
						keyType = 1;
					else if (strcmp(key, "actor_2_facebook_likes") == 0)
						keyType = 1;
					else if (strcmp(key, "imdb_score") == 0)
						keyType = 1;
					else if (strcmp(key, "aspect_ratio") == 0)
						keyType = 1;
					else if (strcmp(key, "movie_facebook_likes") == 0)
						keyType = 1;

            return keyType;
}



struct Record* SortLL(struct Record* a, struct Record* b, int);


void SplitLL(struct Record* source, struct Record** frontRef, struct Record** backRef);

//GETS KEYS AND LINES
const char* getfield(char* line, int num)
{
    const char* tok;
    char *field;
    char *found;
    while((found = strsep(&line,",")) != NULL)
    {
        if (!--num)
            return found;
    }
    return NULL;
}
void trimQuotes(char* s) {
  char *dst = s;
char *src = s;
char c;

while ((c = *src++) != '\0')
{
    if (c == '\\')
    {
        *dst++ = c;
        if ((c = *src++) == '\0')
            break;
        *dst++ = c;
    }
    else if (c != '"')
        *dst++ = c;
}
*dst = '\0';
}
//GETS COLUMN TO BE SORTED ON
const char* getKey(char* line, char *key, int *ptrkeycol)
{
  /* FROM FIRST LINE OF THE CSV FILE CHECKS IF COLUMN TO BE SORTED EXIST
  and IF IT EXIST STORES COLUMN NUMBER*/
    const char* tok;
    for (tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",\n"))
    {
        (*ptrkeycol)++;
        if (strcmp(tok, key) == 0){
            return key;
          }
    }
    return NULL;
}
 //PRINTS LIST
  void printList(struct Record *record, char *firstline, char *fullname)
  {
    FILE *fp2 = fopen(fullname, "w+");



    fprintf(fp2,firstline, "%s");
    while(record!=NULL)
      {
	      fprintf(fp2,"%s", record->fullText);
        record = record->next;
      }
      fclose(fp2);
  }

  //SPLITS LINKEDLIST
  void SplitLL(struct Record* source,
        struct Record** frontRef, struct Record** backRef)
{
        struct Record* fast;
        struct Record* slow;
    slow = source;
    fast = source->next;

    /* Advance 'fast' two nodes, and advance 'slow' one node */
    while (fast != NULL)
    {
    fast = fast->next;
    if (fast != NULL)
    {
        slow = slow->next;
        fast = fast->next;
    }
    }

    /* 'slow' is before the midpoint in the list, so split it in two
    at that point. */
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}

//MERGESORT
void MergeSort(struct Record** headRef, int keyType)
{
  struct Record* head = *headRef;
  struct Record* a;
  struct Record* b;

  /* Base case -- length 0 or 1 */
  if ((head == NULL) || (head->next == NULL))
  {
      return;
  }


  SplitLL(head, &a, &b);

  MergeSort(&a, keyType);
  MergeSort(&b, keyType);

  *headRef = SortLL(a, b, keyType);
}


struct Record* SortLL(struct Record* a, struct Record* b, int keyType)
  {
  struct Record* result = NULL;

  /* Base cases */
  if (a == NULL)
      return(b);
  else if (b==NULL)
      return(a);

  if(keyType == 1){
  if (atof(a->key) <= atof(b->key))
  {
      result = a;
      result->next = SortLL(a->next, b, keyType);
  }
  else
  {
      result = b;
      result->next = SortLL(a, b->next, keyType);
  }
  return(result);}
  else{
    if (strcmp(a->key,b->key) < 0)
    {
        result = a;
        result->next = SortLL(a->next, b, keyType);
    }
    else
    {
        result = b;
        result->next = SortLL(a, b->next,keyType);
    }
    return(result);
  }
}

void push(struct Record** head_ref, char *key1, char *fullText)
{

struct Record* new_node = (struct Record*) malloc(sizeof(struct Record));

  new_node->key = strdup(key1);

  new_node->fullText = strdup(fullText);

  new_node->next = (*head_ref);

  (*head_ref) = new_node;
}
