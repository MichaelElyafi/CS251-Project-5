hmap.o: hmap.c hmap.h
	gcc -c -DPVAL hmap.c -g
graph.o: graph.c graph.h hmap.o
	gcc -c -DPVAL graph.c -g
fakemake: main.c graph.o hmap.o
	gcc -o fakemake main.c graph.o hmap.o -g
