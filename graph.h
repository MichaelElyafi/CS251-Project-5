#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hmap.h"

typedef struct graph GRAPH;

extern void g_free(GRAPH *g);

extern GRAPH * g_from_file(char *fname);

extern void fake_time(GRAPH *g);

extern void fake_touch(GRAPH *g, char *fname);

extern void fake_timestamp(GRAPH *g, char *fname);

extern void fake_timestamps(GRAPH *g);

extern void fake_make(GRAPH *g, char *fname);