"""This module implements the epidemic spread SIR, SIS and SI models."""

import igraph

class Epidemics(object):
	"""Epidemic spread in a graph."""
	status = {'s': 0, 'i': 1, 'r': 2}
	def __init__(self, graph, model, model_parameters, first_infected):
		"""Initializes the epidemic spread model.

		Parameters:
			graph: An igraph.Graph that is the network where the epidemy will spread.
			model: A string indicating the model to be run.
			model_parameters: A tuple (infection_probability, recovery_probability) for the models SIR and SIS or infection_probability for the model SI.
				infection_probability: A double between 0 and 1 indicating the probability that a node will be infected by one infected neighbour.
				recovery_probability: A double between 0 and 1 indicating the recovery probability of a node(None for SI model).
			first_infected: An integer indicating the index of the node that will start the epidemy.
		"""
		self.graph = graph;
		self.infection_probability = model_parameters if model == 'SI' else model_parameter[0]
		self.recovery_probability = None if model == 'SI' else model_parameters[1]
		self.infected_nodes = [first_infected]
		self.node_status = [Epidemics.status['s'] for _ in xrange(self.graph.vcount())]
		self.evolution = {}

if __name__ == '__main__':
	Epidemics(igraph.GraphBase.Barabasi(100, 3), 'SI', 0.1, 0)
