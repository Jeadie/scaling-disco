import matplotlib.pyplot as plt
import networkx as nx
from cosc3500_ass1 import Graph


def plot_graph(graph, solution=None): 
    G=nx.DiGraph()
    G.add_nodes_from(list(range(graph.n)))
    G.add_edges_from([ x for x in graph.edges], color = 'black', width=1)
    print([ x for x in graph.edges])
    width = ["red"] * graph.n

    if solution:
        G.add_edges_from(solution, color='red', width=1)
        width[solution[0][0]] = 'green'
    edges = G.edges()

    colors = [G[u][v]['color'] for u,v in edges]

    plt.plot()
    nx.draw_networkx(G,pos=nx.spring_layout(G),  edges=edges, edge_color=colors, node_color = width)
   
    plt.show()

def plot_path(graph_path, solution_path): 
    g=Graph.create_from_file(graph_path)
    if solution_path: 
        f=open(solution_path, "r")
        nodes = [int(f.readline()) for i in range(g.n)]
        path= [(nodes[i], nodes[i+1]) for i in range(g.n-1)]
        plot_graph(g, solution=None)
    plot_graph(g)
        
def main(): 
    print("jjj")
    plot_path("9_false.txt", None)
    print("hhh")

if __name__ == '__main__': 
    main()
