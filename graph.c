#include "graph.h"
/* Michael Elyafi project5
*/
typedef struct node_graph{
	char *dependency_label;		// Used for naming dependencies
    struct node_graph *next;	// Used for going to the next node
}NODE;

typedef struct vertex{
	int time;					// Time of each vertex
	int vertex_id;				// Id of each vertex
	int num_of_dependencies;	// How many dependencies a vertex has
	char *color;				// Used to see if there are cycles in the graph
	char *vertex_label;			// Used for naming vertices
	NODE *dependencies;			// Array of dependencies
}VERTEX;

typedef struct graph {
	int size;					// Size of graph
	int global_time;			// Time of all operations
   	VERTEX *vertices;			// Array of vertices
    HMAP * map;					// Hmap of the graph
}GRAPH;

/***** FORWARD DECLARATIONS *****/
void fake_check_cycle(GRAPH *g);
void g_free(GRAPH *g);
/***** END FORWARD DECLARATIONS *****/


/***** BEGIN graph FUNCTIONS ******/
GRAPH * g_from_file(char *fname) {
	FILE *fp = fopen(fname, "r");
	if (fp == NULL) {
		fprintf(stderr, "g_from_file failed, input file doesn't exist\n");
		return NULL;
	}
	int k;
	int j;
	int size_buf = 0;
	FILE *filetemp = fopen(fname, "r");
	char strings;
	while ((strings = fgetc(filetemp)) != EOF){
		if (strings != ' ' && strings != '\n' && strings != ':')
			size_buf++;
	}
	fclose(filetemp);
	size_buf = (size_buf * 2);
	char *current_word = malloc(sizeof(char) * size_buf);
	char *target_word = malloc(sizeof(char) * size_buf);
	char *line = malloc(sizeof(char) * size_buf);
	GRAPH *g;
	g = malloc(sizeof(GRAPH));
	g->size = 0;
	g->global_time = 1;
	g->vertices = malloc(sizeof(VERTEX) * size_buf);
	g->map = hmap_create(0, 0.0);

	while (fgets(line, size_buf, fp)){
	current_word = strtok(line, " \n");
		while (current_word){
			if (strcmp(current_word, ":") == 0){
				int dep_id = 0;
				VERTEX *target = hmap_get(g->map, target_word);
				target->dependencies = malloc(sizeof(NODE) * size_buf);
				current_word = strtok(NULL, " ");
				while (target_word[0] != '\n') {
					if (strcmp(current_word, ":") == 0){
						fprintf(stderr, "Error: input file contains target file has another set of colons\n");
						exit(0);
					}
					if (strcmp(current_word, target->vertex_label) == 0){
						fprintf(stderr, "Error: dependency has the same name as target\n");
						exit(0);
					}
					if (current_word[strlen(current_word) - 1] == '\n'){
						current_word[strlen(current_word) - 1] = '\0';
						target_word[0] = '\n';
					}
					target->dependencies[dep_id].dependency_label = malloc(sizeof(char)* size_buf);
					strcpy(target->dependencies[dep_id].dependency_label, current_word);
					target->num_of_dependencies++;
					hmap_set(g->map, target->vertex_label, target);
					dep_id++;
					if (target_word[0] != '\n')
						current_word = strtok(NULL, " ");
				}
				VERTEX *duplicate_dep;
				for (k = 0; k < target->num_of_dependencies; k++){
					for (j = k + 1; j < target->num_of_dependencies; j++){
						if (strcmp(target->dependencies[k].dependency_label, target->dependencies[j].dependency_label) == 0){
							fprintf(stderr, "Error: dependencies have the same name\n");
							exit(0);
						}
					}
				}
			}
			else if (!hmap_contains(g->map, current_word) && strcmp(current_word, ":") != 0){
				VERTEX *basic = malloc(sizeof(VERTEX));
				basic->color = malloc(sizeof(char) * size_buf);
				basic->vertex_label = malloc(sizeof(char) * size_buf);
				strcpy(basic->vertex_label, current_word);
				basic->color[g->size] = 'W';
				basic->vertex_id = g->size;
				basic->time = 0;
				basic->dependencies = NULL;
				basic->num_of_dependencies = 0;
				g->vertices[g->size] = basic[0];
				hmap_set(g->map, current_word, basic);
				g->size++;
			}
			else if (hmap_contains(g->map, current_word)){
				fprintf(stderr, "Error: duplicate file names detected\n");
				exit(0);
			}
			strcpy(target_word, current_word);
			current_word = strtok(NULL, " \n");
			VERTEX *target_check = hmap_get(g->map, target_word);
			if (current_word != NULL && target_check->num_of_dependencies == 0 ){
				if (strcmp(current_word, ":") != 0){
					fprintf(stderr, "Error: need ':' after target name\n");
					exit(0);
				}
			}
		}
	}
	VERTEX *temp;
	for (k = 0; k < g->size; k++){
		temp = hmap_get(g->map, g->vertices[k].vertex_label);
		if (temp->num_of_dependencies > 0) {
			for (j = 0; j < temp->num_of_dependencies; j++){
				if (!hmap_contains(g->map, temp->dependencies[j].dependency_label)){
					fprintf(stderr, "Error: dependency is not found\n");
					exit(0);
				}
			}	
		}
	}
	fake_check_cycle(g);
	free(current_word);
	free(target_word);
	free(line);
	fclose(fp);
	return g;
}

void g_free(GRAPH *g){
	int i = 0;
	for (i = 0; i<g->size; i++) {
		NODE *cur = g->vertices[i].dependencies;
		while (cur != NULL) {
			NODE *temp = cur->next;
			free(cur);
			cur = temp;
		}
	}
	free(g->vertices);
	free(g);
}

void fake_time(GRAPH *g){
	printf("%d\n", g->global_time);
}

void fake_touch(GRAPH *g, char *fname){
	VERTEX *v = hmap_get(g->map, fname);
	if (v == NULL){
		fprintf(stderr, "Error: invalid file name\n");
		return;
	}
	if (v->num_of_dependencies > 0){
		fprintf(stderr, "Error: can not touch target files\n");
		return;
	}
	printf("file '%s' has been modified\n", fname);
	g->vertices[v->vertex_id].time = v->time = g->global_time;
	hmap_set(g->map, fname, v);
	g->global_time++;
}

void fake_timestamp(GRAPH *g, char *fname){
	VERTEX *v = hmap_get(g->map, fname);
	if (v == NULL){
		fprintf(stderr, "Error: invalid file name\n");
		return;
	}
	printf("  %s : %d\n", v->vertex_label, v->time);
}

void fake_timestamps(GRAPH *g) {
	int i;
	for (i = 0; i < g->size; i++) {
		printf("%s : %d\n", g->vertices[i].vertex_label, g->vertices[i].time);
	}
}

void fake_make_recursion(GRAPH *g, VERTEX *target, char *fname){
	if (target->num_of_dependencies == 0){
		return;
	}
	int i;
	int flag = 0;
	VERTEX *dependency;
	for (i = 0; i < target->num_of_dependencies; i++){
		dependency = hmap_get(g->map, target->dependencies[i].dependency_label);
		fake_make_recursion(g, dependency, target->dependencies[i].dependency_label);
		if (dependency->time > target->time){
			flag = 1;
		}
	}
	if (flag){
		g->vertices[target->vertex_id].time = target->time = g->global_time;
		g->global_time++;
		hmap_set(g->map, fname, target);
		printf("making %s...done\n", fname);
	}
	else {
		printf("%s is up to date\n", fname);
	}
	return;
}

void fake_make(GRAPH *g, char *fname){
	VERTEX *target = hmap_get(g->map, fname);
	if (target == NULL){
		fprintf(stderr, "Error: invalid file name\n");
		return;
	}
	fake_make_recursion(g, target, fname);
}

void fake_check_cycle_recursion(GRAPH *g, VERTEX *target, char *fname){
	if (target->num_of_dependencies == 0){
		return;
	}
	target->color[target->vertex_id] = 'G';
	hmap_set(g->map, target->vertex_label, target);
	target = hmap_get(g->map, target->vertex_label);
	int i;
	VERTEX *dependency;
	for (i = 0; i < target->num_of_dependencies; i++){
		dependency = hmap_get(g->map, target->dependencies[i].dependency_label);

		if (dependency->color[dependency->vertex_id] == 'W') {
			fake_check_cycle_recursion(g, dependency, target->dependencies[i].dependency_label);
		}
		else if (dependency->color[dependency->vertex_id] == 'G') {
			fprintf(stderr, "Error: input file contains a cycle\n");
			exit(0);
		}
	}
	target->color[target->vertex_id] = 'B';
	return;
}

void fake_check_cycle(GRAPH *g) {
	VERTEX *target;
	int i;
	int k;
	for (i = 0; i < g->size; i++){
		for (k = 0; k < g->size; k++){
			target = hmap_get(g->map, g->vertices[k].vertex_label);
			target->color[k] = 'W';
			hmap_set(g->map, g->vertices[k].vertex_label, target);
		}
		target = hmap_get(g->map, g->vertices[i].vertex_label);
		fake_check_cycle_recursion(g, target, target->vertex_label);
	}
	return;
}