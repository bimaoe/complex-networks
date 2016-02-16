"""This module implements Induced Subgraph Sampling method based on 
		"Statistical Analysis of Network Data" """

import igraph
import math
import matplotlib.pyplot as pyplot
import numpy

class InducedSubgraphSampling(object):
	"""This class implements the Induced Subgraph Sampling method.
			This method consists in sampling N nodes uniformly and creating the subgraph
			that is induced by them."""
	@classmethod
	def sample(cls, graph, parameters):
		"""Samples a graph.

		Parameters:
			graph: An igraph.Graph that is the network that will be sampled.
			parameters: A tuple (sample_size).
				sample_size: An integer indicating the number of nodes of the sampled graph
				or a float between 0 and 1 indicating the proportion of nodes to be sampled.
		
		Returns:
			An igraph.Graph that is the sampled graph.
		"""
		sample_size = (parameters[0] if isinstance(parameters, tuple)
				else parameters)
		if sample_size <= 1:
			sample_size = math.floor(graph.vcount() * sample_size)
		sampled_vertices = numpy.random.choice(graph.vcount(), sample_size, replace=False)
		sampled_graph = graph.induced_subgraph(sampled_vertices)
		original_degrees = graph.degree(sampled_vertices)
		return sampled_graph, original_degrees

if __name__ == '__main__':
	size_of_network = 1000
	graph = igraph.GraphBase.Erdos_Renyi(size_of_network, 0.01)
	sampled_graph, degrees = InducedSubgraphSampling.sample(graph, 100)
	pyplot.hist(degrees)
	pyplot.show()
