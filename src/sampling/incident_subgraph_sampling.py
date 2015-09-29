"""This module implements Incident Subgraph Sampling method based on 
		"Statistical Analysis of Network Data" """

import igraph
import numpy
import matplotlib.pyplot as pyplot

class IncidentSubgraphSampling(object):
	"""This class implements the Incident Subgraph Sampling method.
			This method consists in sampling N edges uniformly and creating the subgraph
			that is formed by them."""
	@classmethod
	def sample(cls, graph, parameters):
		"""Samples a graph.

		Parameters:
			graph: An igraph.Graph that is the network that will be sampled.
			parameters: A tuple (sample_edge_count).
				sample_edge_count: An integer indicating the number of edges of the sampled graph.
		
		Returns:
			An igraph.Graph that is the sampled graph.
		"""
		sample_edge_count = (parameters[0] if isinstance(parameters, tuple)
				else parameters)
		sampled_edges = numpy.random.choice(graph.ecount(), sample_edge_count, replace=False)
		sampled_vertices = set()
		edge_list = graph.get_edgelist()
		for edge in sampled_edges:
			sampled_vertices.add(edge_list[edge][0])
			sampled_vertices.add(edge_list[edge][0])

		sampled_graph = graph.subgraph_edges(sampled_edges)
		#print sampled_graph.vcount(), sampled_graph.ecount()
		degrees = sampled_graph.degree()

		return sampled_graph, degrees

if __name__ == '__main__':
	size_of_network = 1000
	graph = igraph.GraphBase.Erdos_Renyi(size_of_network, 0.01)
	sampled_graph, degrees = IncidentSubgraphSampling.sample(graph, 55)
	pyplot.hist(degrees)
	pyplot.show()
