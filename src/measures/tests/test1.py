""" This file tests the search information method of the graph measures module."""
import sys
sys.path.insert(0, "../")
import graph_measures
import igraph

def main():
  ex = 'resources/ex1.edgelist'
  graph = igraph.GraphBase.Read_Edgelist(ex, directed=False)
  print graph_measures.Measures.measure_average_search_information(graph)

# list for ex1
#[1, 1, 0.5, 0.41666666666666663, 0.5, 0.16666666666666666, 0.3333333333333333, 1, 0.3333333333333333, 0.2777777777777778, 0.3333333333333333, 0.1111111111111111, 0.16666666666666666, 0.3333333333333333, 1, 0.3333333333333333, 0.3333333333333333, 0.20833333333333331, 0.41666666666666663, 0.5, 1, 0.5, 0.16666666666666666, 0.125, 0.25, 0.25, 0.25, 1, 0.25, 0.16666666666666666, 0.3333333333333333, 0.3333333333333333, 0.3333333333333333, 1, 1]
if __name__ == '__main__':
  main()