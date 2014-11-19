"""This file tests the implemented features of the project"""

import sys
sys.path.insert(0, '../epidemics')
sys.path.insert(0, '../network_models')
import models
import network_generator
import igraph
import csv
import matplotlib.pyplot as pyplot

if __name__ == '__main__':
	epi = models.Epidemics(network_generator.NetworkGenerator.generate_ER(1000, [0.3, False]), 'SIR', [0.05, 0.05], 0, 100)
	epi.run()
	print epi.evolution
	f = open('teste.csv', 'w')
	writer = csv.writer(f)
	keys = epi.evolution.keys()
	writer.writerow(keys)
	size = len(epi.evolution[keys[0]])
	for i in xrange(0, size):
		row = []
		for key in keys:
			row.append(epi.evolution[key][i])
		writer.writerow(row)
	
	x = range(0, size)
	pyplot.plot(x, epi.evolution['i'])
	pyplot.show()
