"""This module implements the epidemic spread SIR, SIS and SI models."""

import igraph
import numpy
import matplotlib.pyplot as pyplot

class Epidemics(object):
	"""Epidemic spread in a graph.
	The recovery step occurs after the infection step.
	Every infected node tries to infect its neighbours and then attempts to recover."""
	def __init__(self, graph, model, model_parameters, infected_nodes,
			max_iterations):
		"""Initializes the epidemic spread model.

		Parameters:
			graph: An igraph.Graph that is the network where the epidemy will spread.
			model: A string indicating the model to be run.
			model_parameters: A tuple (infection_probability, recovery_probability)
					for the models SIR and SIS or infection_probability for the model SI.
				infection_probability: A double between 0 and 1 indicating the 
						probability that a node will be infected by one infected neighbour.
				recovery_probability: A double between 0 and 1 indicating the recovery
						probability of a node(None for SI model).
			infected_list: A list of integers indicating the index of the nodes that
					will start the epidemy.
			max_iterations: An integer indicating the maximum number of iterations.
		"""
		self.graph = graph;
		self.infection_probability = (model_parameters if model == 'SI'
				else model_parameters[0])
		self.recovery_probability = None if model == 'SI' else model_parameters[1]
		self.recovery_status = None if model == 'SI' else ('s' if model == 'SIS'
				else 'r')
		self.infected_nodes = infected_nodes
		self.node_status = ['s' for _ in xrange(self.graph.vcount())]
		for infected_node in infected_nodes:
			self.node_status[infected_node] = 'i'
		infected_count = len(infected_nodes)
		self.evolution = ({'s': [graph.vcount() - infected_count], 
				'i': [infected_count], 'r': [0]} if model == 'SIR'
				else {'s': [graph.vcount() - infected_count], 'i': [infected_count]})
		self.max_iterations = max_iterations
		self.random = numpy.random.RandomState()

	def run_infection(self):
		"""Runs the infection stage of the epidemic spread."""
		newly_infected = []
		for infected_node in self.infected_nodes:
			neighbours = self.graph.neighbors(infected_node)
			for neighbour in neighbours:
				if (self.node_status[neighbour] == 's' and
						self.random.uniform() < self.infection_probability):
					self.node_status[neighbour] = 'i'
					newly_infected.append(neighbour)
		return list(set(newly_infected))

	def run_recovery(self):
		"""Runs the recovery stage of the epidemic spread."""
		for infected_node in self.infected_nodes:
			if self.random.uniform() < self.recovery_probability:
				self.node_status[infected_node] = self.recovery_status
				self.infected_nodes.remove(infected_node)

	def run_step(self):
		"""Runs a step of the epidemic spread."""
		newly_infected = self.run_infection()
		if self.recovery_probability:
			self.run_recovery()
		self.infected_nodes += newly_infected
		for status in self.evolution:
			self.evolution[status].append(self.node_status.count(status))

	def run(self):
		"""Runs the epidemic spread.
		
		Returns:
			True if the spread ended because there were no more infected nodes.
			False if it ended because of the maximum number of iterations.
		"""
		for _ in xrange(self.max_iterations):
			self.run_step()
			if len(self.infected_nodes) == 0:
				return True
		return False

if __name__ == '__main__':
	graph = igraph.GraphBase.Erdos_Renyi(1000, 0.01)
	infected_fractions = []
	ITERATIONS = 70
	for i in xrange(101):
		infected_fraction = 0
		for it in xrange(ITERATIONS):
			epi = Epidemics(graph, 'SIR', [0.01*i, 1], [0], 70)
			epi.run()
			infected_fraction += epi.evolution['i'][-1]+epi.evolution['r'][-1]
		infected_fractions.append(infected_fraction * 1.0 / ITERATIONS)
	x = range(0, len(infected_fractions))
	pyplot.plot(x, infected_fractions)
	pyplot.show()
