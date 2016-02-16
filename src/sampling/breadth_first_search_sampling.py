"""This module implements Breadth First Search Sampling method."""

import igraph
import numpy
import matplotlib.pyplot as pyplot
import math

class BreadthFirstSearchSampling(object):
	"""This class implements the BFS Sampling method.
			This method consists in ."""
	@classmethod
	def sample(cls, graph, parameters):
		"""Samples a graph.

		Parameters:
			graph: An igraph.Graph that is the network that will be sampled.
			parameters: A tuple (sample_size, seed).
				sample_size: An integer indicating the number of nodes of the sampled graph.
				seed: An integer indicating the id of the first node. 
		
		Returns:
			An igraph.Graph that is the sampled graph.
		"""
		sample_size, seed = (parameters if isinstance(parameters, tuple)
				else parameters, numpy.random.randint(graph.vcount(), size=1)[0])
		if sample_size <= 1:
			sample_size = math.floor(graph.vcount() * sample_size)

		sampled_vertices = []
		sampled_graph = igraph.Graph()
		index_map = {}
		index_reverse_map = []
		queue = set([seed])

		count = 1
		curr_node = seed
		sampled_graph.add_vertex()
		index_map[curr_node] = 0
		index_reverse_map.append(curr_node)
		reached_sample_size = False
		while queue and not reached_sample_size:
			curr_node = queue.pop()
			neighbours = graph.neighbors(curr_node)
			for next_node in neighbours:
				if next_node not in index_map:
					index_map[next_node] = count
					index_reverse_map.append(next_node)
					count += 1
					sampled_graph.add_vertex()
					sampled_graph.add_edge(index_map[curr_node], index_map[next_node])
					sampled_vertices.append(next_node)
					queue.add(next_node)
				elif not sampled_graph.are_connected(index_map[curr_node],
						index_map[next_node]):
					sampled_graph.add_edge(index_map[curr_node], index_map[next_node])
				if count == sample_size:
					reached_sample_size = True
					break
		degrees = graph.degree(sampled_vertices)
		return sampled_graph, degrees

if __name__ == '__main__':
	size_of_network = 1000
	graph = igraph.GraphBase.Erdos_Renyi(size_of_network, 0.01)
	sampled_graph, degrees = BreadthFirstSearchSampling.sample(graph, 100)
	pyplot.hist(degrees)
	pyplot.show()
