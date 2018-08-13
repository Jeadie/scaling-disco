import random
from random import shuffle
import string
import math

class Graph(object): 

    def __init__(self, n, is_hamiltonian): 
        self.n = n
        self.is_hamiltonian = is_hamiltonian
        self.edges = []
        
    def add_edge(self, a,b): 
        self.edges.append((a, b))
   
    def has_edge(self, a,b): 
        return (a,b) in self.edges


    @staticmethod
    def create_hamiltonian_path(n, noise): 
        path = list(range(n))
        shuffle(path)
        graph = Graph(n, True)
        for i in range(1,n):
            graph.add_edge(path[i-1], path[i])

        for i in range(noise):
            a,b = (random.randint(0, n-1), random.randint(0, n-1))
            if graph.has_edge(a,b):
                noise+=1
            graph.add_edge(a,b)

        return graph


    @staticmethod
    def create_non_hamiltonian_path(n, edges): 
        g = Graph(n-3, False)
        for i in range(edges-3): 
            a,b = (random.randint(0, g.n-1), random.randint(0, g.n-1))
            if g.has_edge(a,b):
                edges+=1               
            g.add_edge(a,b)

        
        for i in range(g.n, n):
            g.add_edge(random.randint(0, g.n-1), i)
        g.n = n
        return g

    def output_format(self): 
        output = []
        output.extend([str(self.n), str(len(self.edges))])
        output.extend([random.choice(string.ascii_lowercase) for x in range(self.n)] )
        output.extend(["{0} {1}".format(edge[0], edge[1]) for edge in self.edges])
        return "\n".join(output)
    
def save_graph(n, hamiltonian, path): 
    g = Graph.create_hamiltonian_path(n, int((n**(3/2)))) if hamiltonian else Graph.create_non_hamiltonian_path(n, int((n**(3/2))))
    f = open(path, "w")
    f.write(g.output_format())
    f.close()


def main():
    for n in range(5,20): #[5, 10, 15, 50, 200, 1000, 5000, 25000, 50000]: 
        save_graph(n, True, "{0}_True.txt".format(n))
        print("{0}_True.txt".format(n))
        save_graph(n, False, "{0}_False.txt".format(n))
        print("{0}_False.txt".format(n))

if __name__ == '__main__': 
    main()
        
