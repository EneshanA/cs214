#define STDIN 0
#define STDOUT 1
#define STDERR 2

struct Record{
  char *key;
  char *fullText;
  struct Record *next;
} record;

int determineSortType(struct Record * tmp){
  int keyType = 0;
  while(tmp!=NULL)
    {
      char *tmpkey = strdup(tmp->key);
      printf("%lu\n", strlen(tmp->key));
      for(int i = 0; i < strlen(tmpkey); i++){
        if(isdigit(tmpkey[i]) || tmpkey[i] == '.'){
          printf("IT is integer: \n");
        }
        else{
          printf("it is string: \n");
          keyType = 1;
        }
      }
      tmp = tmp->next;
    }
    return keyType;
}



struct Record* SortedMerge(struct Record* a, struct Record* b);
void FrontBackSplit(struct Record* source, struct Record** frontRef, struct Record** backRef);
const char* getfield(char* line, int num)
{
    const char* tok;
    char *field;
    char *found;
    while((found = strsep(&line,",")) != NULL)
    {
        if (!--num)
            // key = strdup(tok);
            // printf("length of tok: %d\n", strlen(tok));
            return found;
    }
    return NULL;
}
const char* getKey(char* line, char *key, int *ptrkeycol)
{
  /* FROM FIRST LINE OF THE CSV FILE CHECKS IF COLUMN TO BE SORTED EXIST
  and IF IT EXIST STORES COLUMN NUMBER*/
    const char* tok;
    for (tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",\n"))
    {
        (*ptrkeycol)++;
        if (!strcmp(tok, key)){
           // printf("KEY!!!: %s\n ", key);
           // printf("colnumber: %d\n", *ptrkeycol);
            return key;
          }
    }
    return NULL;
}
  void printList(struct Record *record)
  {
    while(record!=NULL)
      {
       // printf("SORTED: %s\n ", record->key);
	printf("%s\n", record->fullText);
        record = record->next;
      }
  }
  void FrontBackSplit(struct Record* source,
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
void MergeSort(struct Record** headRef)
{

struct Record* head = *headRef;
struct Record* a;
struct Record* b;
/* Base case -- length 0 or 1 */
if ((head == NULL) || (head->next == NULL))
{
    return;
}

/* Split head into 'a' and 'b' sublists */
FrontBackSplit(head, &a, &b);

/* Recursively sort the sublists */
MergeSort(&a);
MergeSort(&b);

/* answer = merge the two sorted lists together */
*headRef = SortedMerge(a, b);
}

/* See https://www.geeksforgeeks.org/?p=3622 for details of this
function */
struct Record* SortedMerge(struct Record* a, struct Record* b)
{
struct Record* result = NULL;

/* Base cases */
if (a == NULL)
    return(b);
else if (b==NULL)
    return(a);

/* Pick either a or b, and recur */
if (strcmp(a->key,b->key) < 0)
{
    result = a;
    result->next = SortedMerge(a->next, b);
}
else
{
    result = b;
    result->next = SortedMerge(a, b->next);
}
return(result);
}

void push(struct Record** head_ref, char *key1, char *fullText)
{
/* allocate node */
struct Record* new_node = (struct Record*) malloc(sizeof(struct Record));
/* put in the data */
new_node->key = strdup(key1);
//printf("KEY: %s\n", new_node->key);
new_node->fullText = strdup(fullText);
/* link the old list off the new node */
new_node->next = (*head_ref);
//printf("FULL LINE: %s\n", new_node->fullText);
/* move the head to point to the new node */
(*head_ref) = new_node;
//printf("CHECKPOINT \n");
}
