#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <dirent.h>

#include "scannerCSVsorter.h"

int child_count;
char *file_name;	//Name of the file being sort_flag
char *file_path;	//Path leading to the file being sort_flag
char *output_name;
char *final_name;
char *dir_output;
int dir_output_flag;
int dir_input_flag;

int main(int argc, char **argv) {

	int originalPID = getpid();
	if (argv[2] == '\0' || argv[2] == NULL){
		printf("Error! Invalid Argument.\n");
		return -1;
	}

	char *dir_input=malloc(2048);
	dir_output = malloc(2048);
	output_name = malloc(2048);
	DIR *dir;
	char path[2048];
	dir_output_flag=0;
	dir_input_flag=0;

	if(argc == 3){

		if (strcmp(argv[1], "-c") != 0){
			printf("Error! Invalid Argument.\n");
			return -1;
		}


		if(getcwd(path, sizeof(path)) != NULL){
		// gets current working directory and copies it into path variable
		} else {
			printf("Error! Invalid Argument.");
			return -1;
		}

		file_path = path;

		if((dir = opendir(path)) == NULL){
			printf("Error! Invalid Argument.\n");
			return -1;
        }

}

	else if(argc == 5)
	{

		// Argument 1 -> "-c"
		if (strcmp(argv[1], "-c") != 0)
		{
			printf("Error! Invalid Argument.\n");
			return -1;
		}

		// Argument 3 -> "-d"
		if (strcmp(argv[3],"-d") == 0)
		{

			dir_input_flag=1;
		}
		//Argument 3 -> "-o"
		else if (strcmp(argv[3],"-o")==0)
		{
			dir_output_flag = 1;

		}
		else
		{
			printf("Error! Invalid Argument.\n");
			return -1;
		}

		if (dir_input_flag)
		{
			strcpy(dir_input, argv[4]);

			file_path = dir_input;
			int p;
			strcpy(path,dir_input);
			if((dir = opendir(path)) == NULL){
				printf("Error! Invalid Argument.\n");
				return -1;
			}
		}

		if(dir_output_flag)
		{

			strcpy(dir_output, argv[4]);

			if(getcwd(path, sizeof(path)) != NULL)
			{
				// No need to do anything here
			}
			else
			{
			printf("Error! Invalid Argument.");
			return -1;
			}

			file_path = path;
			dir = opendir(path);
		}


	}
	else if(argc == 7)
	{
		if (strcmp(argv[3],"-d") == 0)
		{
			strcpy(dir_input,argv[4]);
			dir_input_flag=1;
		}
		if (strcmp(argv[5],"-o")==0)
		{
			dir_output_flag = 1;
			strcpy(dir_output,argv[6]);
		}
		else
		{
			printf("Error! Invalid Argument.\n");
			return -1;
		}


			file_path = dir_input;
			strcpy(path,dir_input);

			if((dir = opendir(path)) == NULL){
				printf("Error! Invalid Argument.\n");
				return -1;
			}
	}
	else {
		printf("Error! Invalid Argument.\n");
		return -1;
	}


	printf("Initial PID: %d\n", getpid());
	printf("PIDS of all child processes: ");
	fflush(stdout);

	child_count = 0; 	// Children count
    struct dirent *str;


    if ( dir != NULL) // check nullity
    {
		while((str = readdir(dir)) != NULL)
		{
			if((strcmp(str->d_name,"..") != 0) && (strcmp(str->d_name,".") != 0))
			{


				if(str->d_type == DT_DIR) // checks if it is a directory
				{

					int pid = fork();
					if(pid == 0) // checks if it is child
					{

						closedir(dir); //close the old directory
						strcat(path,"/");
						strcat(path, str->d_name);
						dir = opendir(path);	//open the new directory
						child_count = 0;	//reset number of children  in children fork
					}
					else
					{
						child_count++; //increase child count in parent
					}

				}
				else
				{
					char* file_type = strrchr(str->d_name,'.'); // gets the file_type of the file
					char* sort_flag = strstr(str->d_name,"-sort_flag-");
					if (sort_flag != NULL)
					{
						//skips if it is already sorted
					}


					if(file_type!=NULL && sort_flag==NULL)
					{
						if(strcmp(file_type,".csv")==0)			// ALL CSV FILES GO HERE
						{

								int pid = fork(); // fork on here
								if(pid == 0) // if its  a child
								{

									child_count = 0; //reset number of children
									file_name = str->d_name; //set the name of the file
									file_path = path; //set the path of the file
									break;
								}
								else
								{
									child_count++; //increment number of children in parent
								}

						}
					}

				}
			}
		}
	}
	closedir(dir); //close whatever directory is open.


	int i;
	int exitnum;
	if(file_path != NULL && file_name == NULL) // goes in here if its a directory fork.
	{
			for(i = child_count;i>0;i--) // waits for children to finish
			{
				int status;
				wait(&status);
				exitnum+=WEXITSTATUS(status);
			}
		if(originalPID == getpid())
		{
				printf("\nTotal number of processes: %i\n",exitnum+child_count+1);
		}

		return child_count+exitnum;

	}

		final_name = malloc(2048);

		output_name = strcpy(output_name,file_name);

		final_name = strcpy(final_name, file_path);
		final_name = strcat(final_name, "/");
		final_name = strcat(final_name, file_name); // Sets the full path name

		char *output_full = strcat(dir_output,output_name);

		struct Record* record = NULL;

	  char *line = NULL;
	  size_t len = 0;
	  ssize_t nread;

	  int keycol = 0;
	  int *ptrkeycol = &keycol;

	  char *field = NULL;
	  int keyType = 0;
	  FILE* stream = fopen(final_name, "r");

		getline(&line, &nread, stream); //READS THE FIRST LINE
	  char *firstline = strdup(line);
		char *finalfirstline = strdup(firstline);

 		char *key = getKey(firstline, argv[2], ptrkeycol); //CHECKS IF COLUMN TO BE sort_flag EXISTS
	  keyType = determineSortType(key);
	  while ((nread = getline(&line, &len, stream)) != -1) { //READS REST OF THE FILE
	        if(nread > 1){
	          char* tmp = strdup(line);

	          field = strdup(getfield(tmp,keycol));
	          trimQuotes(field);
	          push(&record,field,line); //CREATES NEW NODE AND STORES ROW WITH KEY
	          free(tmp);
	  }
	}


	MergeSort(&record,keyType);
	if(dir_output_flag)
	{
	 printList(record,finalfirstline,output_full);
	}
	else
	{
		printList(record,finalfirstline,final_name);
	}

		return 0;

} //TADADA
