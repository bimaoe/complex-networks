"""This module gathers sampling methods."""

import breadth_first_search_sampling
import igraph
import incident_subgraph_sampling
import induced_subgraph_sampling
import numpy
import math
import sys

class SamplingMethods(object):
  """Sampling methods caller."""

  @classmethod
  def sample(cls, sampling_method, graph, parameter_list):
    """Generates a network of the given type and with the given parameters.
    Parameters:
      sampling_method: A string that represents the desired sampling method.
      graph: The graph that will be sampled.
      parameter_list: A list of parameters for the given method.

    Returns:
      The return values depend on the chosen method.
    """
    sampling_method_dict = {
        "BFS": breadth_first_search_sampling.BreadthFirstSearchSampling.sample,
        "incident": incident_subgraph_sampling.IncidentSubgraphSampling.sample,
        "induced": induced_subgraph_sampling.InducedSubgraphSampling.sample
    }
    return sampling_method_dict[sampling_method](graph, parameter_list)

def test():
  size_of_network = 1000
  graph = igraph.GraphBase.Erdos_Renyi(size_of_network, 0.01)
  print SamplingMethods.sample("incident", graph, [100])

if __name__ == '__main__':
  test()