"""This module implements the epidemic spread SIR, SIS and SI models."""

import igraph
import random
import matplotlib.pyplot as pyplot

class Epidemics(object):
	"""Epidemic spread in a graph."""
	def __init__(self, graph, model, model_parameters, first_infected, max_iterations):
		"""Initializes the epidemic spread model.

		Parameters:
			graph: An igraph.Graph that is the network where the epidemy will spread.
			model: A string indicating the model to be run.
			model_parameters: A tuple (infection_probability, recovery_probability) for the models SIR and SIS or infection_probability for the model SI.
				infection_probability: A double between 0 and 1 indicating the probability that a node will be infected by one infected neighbour.
				recovery_probability: A double between 0 and 1 indicating the recovery probability of a node(None for SI model).
			first_infected: An integer indicating the index of the node that will start the epidemy.
			max_iterations: An integer indicating the maximum number of iterations.
		"""
		self.graph = graph;
		self.infection_probability = model_parameters if model == 'SI' else model_parameters[0]
		self.recovery_probability = None if model == 'SI' else model_parameters[1]
		self.recovery_status = None if model == 'SI' else 's' if model == 'SIS' else 'r'
		self.infected_nodes = [first_infected]
		self.node_status = ['s' for _ in xrange(self.graph.vcount())]
		self.node_status[first_infected] = 'i'
		self.evolution = {'s': [graph.vcount()-1], 'i': [1], 'r': [0]} if model == 'SIR' else {'s': [graph.vcount()-1], 'i': [1]}
		self.max_iterations = max_iterations

	def run_infection(self):
		"""Runs the infection stage of the epidemic spread."""
		newly_infected = []
		for infected_node in self.infected_nodes:
			neighbours = self.graph.neighbors(infected_node)
			for neighbour in neighbours:
				if self.node_status[neighbour] == 's' and random.random() < self.infection_probability:
					self.node_status[neighbour] = 'i'
					newly_infected.append(neighbour)
		return list(set(newly_infected))

	def run_recovery(self):
		"""Runs the recovery stage of the epidemic spread."""
		for infected_node in self.infected_nodes:
			if random.random() < self.recovery_probability:
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
			True if the spread ended because there were no more infected nodes and False if it ended because of the maximum number of iterations.
		"""
		for _ in xrange(self.max_iterations):
			self.run_step()
			if len(self.infected_nodes) == 0:
				return True
		return False

if __name__ == '__main__':
	epi = Epidemics(igraph.GraphBase.Erdos_Renyi(100, 0.1), 'SIR', [0.05, 0.1], 0, 300)
	epi.run()
	size = len(epi.evolution['i'])
	x = range(0, size)
	pyplot.plot(x, epi.evolution['s'])
	pyplot.plot(x, epi.evolution['i'])
	pyplot.plot(x, epi.evolution['r'])
	pyplot.show()
