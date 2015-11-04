"""This file writes the value of some measures for a network in csv format."""

import sys
import igraph
import csv
import numpy
import scipy.stats as stats

class Measures(object):
  """Class that calculates network measures."""

  @classmethod
  def write (cls, csv_row):
    for item in csv_row:
      print str(item) + ',',

  @classmethod
  def measure_average_degree(cls, graph):
    # Calculate the average degree.
    degrees = graph.degree(range(0, graph.vcount()))
    return numpy.mean(degrees)

  @classmethod
  def measure_shannon_entropy(cls, graph):
    # Calculate the Shannon Entropy of the degree distribution.
    degrees = graph.degree(range(0, graph.vcount()))
    count_dict = {}
    for deg in degrees:
      if not deg in count_dict:
        count_dict[deg] = 0
      count_dict[deg] += 1
    count_list = []
    for _, cnt in count_dict.items():
      count_list.append(cnt)
    return stats.entropy(count_list)

  @classmethod
  def measure_average_betweenness(cls, graph):
    # Calculate the average and variance of betweenness.
    betweenness = graph.betweenness()
    return numpy.mean(betweenness)

  @classmethod
  def measure_variance_betweenness(cls, graph):
    # Calculate the average and variance of betweenness.
    betweenness = graph.betweenness()
    return numpy.var(betweenness)

  @classmethod
  def measure_diameter(cls, graph):
    # Calculate the diameter.
    return graph.diameter(False)

  @classmethod
  def measure_assortativity(cls, graph):
    # Calculate the assortativity.
    return graph.assortativity_degree(False)

  @classmethod
  def measure_transitivity(cls, graph):
    # Calculate the clustering coefficient.
    return graph.transitivity_undirected("zero")

  @classmethod
  def measure_efficiency(cls, graph):
    # Calculate the efficiency.
    distance_matrix = graph.shortest_paths(mode=igraph.ALL)
    efficiency = 0
    for row in distance_matrix:
      for dist in row:
        efficiency += (1.0 / dist if dist > 0 else 0)
    efficiency /= graph.vcount() * (graph.vcount() - 1)
    return efficiency

  @classmethod
  def measure_average_coreness(cls, graph):
    # Calculate the average coreness.
    return numpy.mean(graph.coreness())

  @classmethod
  def measure_complexity(cls, graph):
    # Calculate the second order moment of the degree.
    degrees = graph.degree(range(0, graph.vcount()))
    return stats.moment(degrees, 2)

  @classmethod
  def measure_average_closeness(cls, graph):
    return numpy.mean(graph.closeness())

  @classmethod
  def measure_average_neighbor_degree(cls, graph):
    # Calculate the average of the average of the degree of the neighbors
    # of the nodes.
    ans = 0.0
    degrees = graph.degree()
    for i in xrange(graph.vcount()):
      neighbors = graph.neighbors(i)
      mean = 0.0
      for j in neighbors:
        mean += degrees[j]
      mean /= len(neighbors)
      ans += mean
    ans /= graph.vcount()
    return ans

  @classmethod
  def get_methods(cls, method_names):
    return [(method_name, getattr(Measures, method_name)) 
        for method_name in method_names 
            if callable(getattr(Measures, method_name))]

  @classmethod
  def all_methods(cls):
    return [(method_name, getattr(Measures, method_name)) 
        for method_name in dir(Measures) 
        if method_name.startswith('measure')
            and callable(getattr(Measures, method_name))]

  @classmethod
  def calculate_all(cls, graph):
    methods = cls.all_methods()
    csv_row = [method[1](graph) for method in methods]
    return csv_row

  @classmethod
  def calculate_measures(cls, graph, measures):
    # Calculates given measures for the graph.
    # Parameters:
    #   graph: igraph graph
    #   measures: list of strings
    methods = cls.all_methods()
    csv_row = [method[1](graph) for method in methods if method[0] in measures]
    return csv_row

if __name__ == '__main__':
  if len(sys.argv):
    filename = sys.argv[1]
      
    # Create graph.
    graph = igraph.GraphBase.Read_Edgelist(filename, directed=False)

    print Measures.write(Measures.calculate_all(graph)),
