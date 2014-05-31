"""This module creates networks based on complex networks models with the aid of the iGraph library."""

import igraph

class NetworkGenerator(object):
	"""Network generator."""

	@classmethod
	def generate(cls, size_of_network, type_of_network, parameter_list):
		"""Generates a network of the given type and with the given parameters.
		Parameters:
			size_of_network: An integer indicating the number of nodes in the network.
			type_of_network: An integer that represents the desired model for the network.
			parameter_list: A list of parameters for the given type.

		Returns:
			An igraph graph that is the generated network.
		"""

	@classmethod
	def generate_BA(cls, size_of_network, parameter_list):
		"""Generates a graph based on the Barabasi-Albert model.

		Parameters:
			size_of_network: An integer indicating the number of nodes in the network.
			parameter_list: A list [number_of_outgoing_edges, is_directed, power_of_nonlinear_model].
				number_of_outgoing_edges: A positive integer indicating the number of outgoing edges for each vertex.
				is_directed: A boolean indicating whether the graph is directed.
				power_of_non_linear_model: A double indicating the power of the model(optional).

		Returns:
			An igraph graph based on the Barabasi-Albert model.
		"""
		return igraph.GraphBase.Barabasi(n = size_of_network, m = parameter_list[0], directed = parameter_list[1], power = parameter_list[2] if len(parameter_list) > 2 else 1)
	
	@classmethod
	def generate_ER(cls, size_of_network, parameter_list):
		"""Generates a graph based on the Erdos-Renyi model.

		Parameters:
			size_of_network: An integer indicating the number of nodes in the network.
			parameter_list: A list [probability_of_connection, is_directed].
				probability_of_connection: A double indicating the probability of connecting any pair of nodes.
				is_directed: A boolean indicating whether the graph is directed.

		Returns:
			An igraph graph based on the Erdos-Renyi model.
		"""
		return igraph.GraphBase.Erdos_Renyi(n = size_of_network, p = parameter_list[0], directed = parameter_list[1])

	@classmethod
	def generate_WS(cls, size_of_network, parameter_list):
		"""Generates a graph based on the Watts-Strogatz model.

		Parameters:
			size_of_network: An integer indicating the number of nodes in the network.
			parameter_list: A list [distance_of_connection, rewiring_probability].
				distance_of_connection: A positive integer indicating the distance within which two vertices will be connected.
				rewiring_probability: A double indicating the rewiring probability.

		Returns:
			An igraph graph based on the Barabasi-Albert model.
		"""
		return igraph.GraphBase.Watts_Strogatz(dim = 1, size = size_of_network, nei = parameter_list[0], p = parameter_list[1], loops = False, multiple = False)

if __name__ == '__main__':
	print NetworkGenerator.generate_BA(100, [3, False])
	print NetworkGenerator.generate_ER(100, [0.5, False])
	print NetworkGenerator.generate_WS(100, [2, 0.5])
