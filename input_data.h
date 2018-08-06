#ifndef input_struct
#define input_struct

#include "graph.h"

typedef struct Input Input; 

char** get_node_names(Input* i); 

int get_node_count(Input* i);

int get_edge_count(Input* i);

int* get_edge_source(Input* i);

int* get_edge_dest(Input* i); 


Input* parse_input_from_file(char* path);

Input* parse_input_from_arg(int argc, char** argv); 

Graph* get_graph_from_input(Input* i); 

char** get_node_path_from_graph(Graph* g); 

Graph* get_graph_from_input(Input* i); 
#endif
