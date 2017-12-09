#include "graph.h"

int main(int argc, char *argv[]){
	if (argc != 2){
		printf("Error: no input file specified\n");
		exit(0);
	}
	printf("Type in ? to see the commands! \n");
	GRAPH *g = g_from_file(argv[1]);
	char buffer[1000];
	char command[1000];
	char arguement[1000];
	fgets(buffer, 1000, stdin);
	int num_of_args = sscanf(buffer, "%s %s", command, arguement);
	while (strcmp(command, "quit") != 0){
		if (strcmp(command, "?") == 0 && num_of_args == 1){
			printf("Commands include:\n");
			printf("'touch (file name)'\n");
			printf("'timestamp (file name)'\n");
			printf("'timestamps'\n");
			printf("'make (file name)'\n");
			printf("'quit' to close the program\n");
		}
		else if (strcmp(command, "time") == 0 && num_of_args == 1){
			fake_time(g);
		}
		else if (strcmp(command, "touch") == 0 && num_of_args == 2){
			fake_touch(g, arguement);
		}
		else if (strcmp(command, "timestamp") == 0 && num_of_args == 2){
			fake_timestamp(g, arguement);
		}
		else if (strcmp(command, "timestamps") == 0 && num_of_args == 1){
			fake_timestamps(g);
		}
		else if (strcmp(command, "make") == 0 && num_of_args == 2){
			fake_make(g, arguement);
		}
		else{
			printf("invalid command\n");
		}
		fgets(buffer, 1000, stdin);
		num_of_args = sscanf(buffer, "%s %s", command, arguement);
	}
	g_free(g);
	return 0;
}
