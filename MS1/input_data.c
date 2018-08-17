#include <stdlib.h>
#include "input_data.h"
#include "graph.h"
#include <string.h>


struct Input {
  int node_count;
  int edge_count; 
  char** node_names;
  int* edge_source; 
  int* edge_dest; 
};

 
char** get_node_names(Input* i) {
    return i-> node_names; 
}
int get_node_count(Input* i) {
    return i->node_count; 
}

int get_edge_count(Input* i) {
    return i->edge_count; 
}

int* get_edge_source(Input* i) {
    return i->edge_source; 
}

int* get_edge_dest(Input* i) {
    return i->edge_dest; 
}


Input* parse_input_from_file(char* path) {
    FILE* f = fopen(path, "r");
    size_t len = 0; 
    
    Input* i = (Input*) malloc(sizeof(Input)); 
    fscanf(f, "%d\n", &i->node_count); 
    fscanf(f, "%d\n", &i->edge_count); 
    i->node_names = (char**) malloc(sizeof(char*) * i->node_count); 
    int size;
    char buffer[100];  
    for(int j=0; j< (i->node_count -1); j++) {
	size = getline(&i->node_names[j], &len, f);
          
    	(i->node_names[j])[size-1] = '\0';
        (i->node_names[j])[size-2] = '\0';

	len = 0;
    } 
    
    len = 0; 
    size = getline(&i->node_names[i->node_count -1], &len, f); 
    (i->node_names[i->node_count - 1])[size-1] = '\0';
    i->edge_source = (int*) malloc(sizeof(int) * i->edge_count); 
    i->edge_dest = (int*) malloc(sizeof(int) * i->edge_count); 
  
    int a,b; 
    for (int j=0; j<i->edge_count; j++) {
        if (fscanf(f, "%d %d\n", &a, &b) != 2) {
            return NULL;
        }
        i->edge_source[j] = a; 
        i->edge_dest[j] = b; 
    }
    fclose(f); 
    return i; 
}



Input* parse_input_from_arg(int argc, char** argv) {
    Input* i = (Input*) malloc(sizeof(Input)); 
    i->node_count = strtol(argv[0], (char**) NULL, 10); 
    i->edge_count = strtol(argv[1], (char**) NULL, 10); 
 
    i->node_names = (char**) malloc(sizeof(char*) * i->node_count); 

    for(int j=0; j< i->node_count; j++) {
        i->node_names[j] = argv[j+2];       
    } 
  
    i->edge_source = (int*) malloc(sizeof(int) * i->edge_count); 
    i->edge_dest = (int*) malloc(sizeof(int) * i->edge_count); 
  
    int a,b; 
    for (int j=0; j<i->edge_count; j++) {
        if (sscanf(argv[j + i->node_count + 2], "%d %d", &a, &b) != 2) {
            return NULL;
        }
        i->edge_source[j] = a; 
        i->edge_dest[j] = b; 
    }
    return i; 
} 

Graph* get_graph_from_input(Input* i) { 
  //return create_graph(i->node_count, i->edge_count, i->edge_source, i->edge_dest); 
  return NULL; 
} 

