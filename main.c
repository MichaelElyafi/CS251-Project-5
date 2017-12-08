#include "graph.h"

int main(int argc, char *argv[]){
	if (argc != 2){
		printf("incorrect command line arguments\n");
		return 0;
	}
	GRAPH *g = g_from_file(argv[1]);
	if (g == NULL){
		//fprintf(stderr, "Error in reading input file\n");
		return -1;
	}
	char buffer[50];
	char command[50];
	char arguement[50];
	fgets(buffer, 50, stdin);
	int nargs = sscanf(buffer, "%s %s", command, arguement);
	while (strcmp(command, "quit") != 0){
		if (strcmp(command, "time") == 0 && nargs == 1){
			fake_time(g);
		}
		else if (strcmp(command, "touch") == 0 && nargs == 2){
			fake_touch(g, arguement);
		}
		else if (strcmp(command, "timestamp") == 0 && nargs == 2){
			fake_timestamp(g, arguement);
		}
		else if (strcmp(command, "timestamps") == 0 && nargs == 1){
			fake_timestamps(g);
		}
		else if (strcmp(command, "make") == 0 && nargs == 2){
			fake_make(g, arguement);
		}
		else{
			printf("invalid command\n");
		}
		fgets(buffer, 50, stdin);
		nargs = sscanf(buffer, "%s %s", command, arguement);
	}
	g_free(g);
	return 0;
}
