"""This module creates networks based on complex networks models with the aid of
    the iGraph library."""

import igraph
import numpy
import matplotlib.pyplot as pyplot
import math
import sys

class NetworkGenerator(object):
  """Network generator."""

  GENERATOR_DICT_KEYS = ["BA", "ER", "WS", "SF2ER", "Waxman", "SpatialSF",
      "DegreeSequence", "ConfigurationSF"]
  RANDOM = numpy.random.RandomState()

  @classmethod
  def generate(cls, type_of_network, size_of_network, parameter_list):
    """Generates a network of the given type and with the given parameters.
    Parameters:
      type_of_network: An integer that represents the desired model for the
          network.
      size_of_network: An integer indicating the number of nodes in the network.
      parameter_list: A list of parameters for the given type.

    Returns:
      An igraph graph that is the generated network.
    """
    generator_dict = {
        "BA": cls.generate_BA,
        "BA_avg": cls.generate_BA_with_average_degree,
        "ER": cls.generate_ER,
        "ER_avg": cls.generate_ER_with_average_degree,
        "WS": cls.generate_WS,
        "WS_avg": cls.generate_WS_with_average_degree,
        "SF2ER": cls.generate_SF2ER,
        "Waxman": cls.generate_Waxman,
        "Waxman_avg": cls.generate_Waxman_with_average_degree,
        "SpatialSF": cls.generate_SpatialSF,
        "SpatialSF_avg": cls.generate_SpatialSF_with_average_degree,
        "DegreeSequence": cls.generate_DegreeSequence,
        "ConfigurationSF": cls.generate_ConfigurationSF}
    return generator_dict[type_of_network](size_of_network, parameter_list)

  @classmethod
  def generate_BA(cls, size_of_network, parameter_list):
    """Generates a graph based on the Barabasi-Albert model.

    Parameters:
      size_of_network: An integer indicating the number of nodes in the network.
      parameter_list: A list [number_of_outgoing_edges, is_directed,
          power_of_nonlinear_model].
        number_of_outgoing_edges: A positive integer indicating the number of
            outgoing edges for each vertex.
        is_directed: A boolean indicating whether the graph is directed.
        power_of_non_linear_model: A double indicating the power of the
            model(optional).

    Returns:
      An igraph graph based on the Barabasi-Albert model.
    """
    return igraph.GraphBase.Barabasi(n = size_of_network, m = parameter_list[0],
        directed = parameter_list[1], power = (parameter_list[2]
        if len(parameter_list) > 2 else 1))

  @classmethod
  def generate_BA_with_average_degree(cls, size_of_network, parameter_list):
    """Generates a graph based on the Barabasi-Albert model.
    Works badly when the average degree is odd.

    Parameters:
      size_of_network: An integer indicating the number of nodes in the network.
      parameter_list: A list [average_degree, is_directed,
          power_of_nonlinear_model].
        average_degree: A number indicating the average degree of the network.
        is_directed: A boolean indicating whether the graph is directed.
        power_of_non_linear_model: A double indicating the power of the
            model(optional).

    Returns:
      An igraph graph based on the Barabasi-Albert model.
    """
    number_of_outgoing_edges = (int(parameter_list[0]/2)
        if parameter_list[0] - int(parameter_list[0]/2)*2 < int((parameter_list[0]+1)/2) - parameter_list[0]
        else int((parameter_list[0]+1)/2))
    return cls.generate_BA(size_of_network, 
        [int(number_of_outgoing_edges), parameter_list[1], parameter_list[2]
        if len(parameter_list) > 2 else 1])
  
  @classmethod
  def generate_ER(cls, size_of_network, parameter_list):
    """Generates a graph based on the Erdos-Renyi model.

    Parameters:
      size_of_network: An integer indicating the number of nodes in the network.
      parameter_list: A list [probability_of_connection, is_directed, force_connected].
        probability_of_connection: A double indicating the probability of
            connecting any pair of nodes.
        is_directed: A boolean indicating whether the graph is directed.
        force_connected:  A boolean indicating if the graph must be connected(default=False).

    Returns:
      An igraph graph based on the Erdos-Renyi model.
    """
    p = parameter_list[0]
    force_connected = parameter_list[2] if len(parameter_list) > 2 else False

    # Create an undirected graph with size_of_network nodes.
    g = igraph.Graph(size_of_network)
    edges = []
    has_neighbor = [False for _ in xrange(size_of_network)]

    for i in xrange(size_of_network):
      for j in xrange(i+1, size_of_network):
        if cls.RANDOM.uniform() < p:
              has_neighbor[i] = True
              has_neighbor[j] = True
              edges.append((i, j))

      # Force the graph to be connected.
      if force_connected and not has_neighbor[i]:
        edges.append((i, cls.RANDOM.choice([_i for _i in xrange(size_of_network) if _i != i])))

    g.add_edges(edges)
    return g
    #return igraph.GraphBase.Erdos_Renyi(n = size_of_network,
    #    p = parameter_list[0], directed = parameter_list[1])

  @classmethod
  def generate_ER_with_average_degree(cls, size_of_network, parameter_list):
    """Generates a graph based on the Erdos-Renyi model.

    Parameters:
      size_of_network: An integer indicating the number of nodes in the network.
      parameter_list: A list [average_degree, is_directed].
        average_degree: A number indicating the average degree of the network.
        is_directed: A boolean indicating whether the graph is directed.
        force_connected:  A boolean indicating if the graph must be connected(default=False).

    Returns:
      An igraph graph based on the Erdos-Renyi model.
    """
    coef = 0.5 if parameter_list[1] else 1.0
    p = coef * parameter_list[0] / (size_of_network - 1)
    return cls.generate_ER(size_of_network, [p, parameter_list[1], 
        parameter_list[2] if len(parameter_list) > 2 else False])

  @classmethod
  def generate_WS(cls, size_of_network, parameter_list):
    """Generates a graph based on the Watts-Strogatz model.

    Parameters:
      size_of_network: An integer indicating the number of nodes in the network.
      parameter_list: A list [distance_of_connection, rewiring_probability].
        distance_of_connection: A positive integer indicating the distance
            within which two vertices will be connected.
        rewiring_probability: A double indicating the rewiring probability.

    Returns:
      An igraph graph based on the Barabasi-Albert model.
    """
    return igraph.GraphBase.Watts_Strogatz(dim = 1, size = size_of_network,
        nei = parameter_list[0], p = parameter_list[1], loops = False,
        multiple = False)

  @classmethod
  def generate_WS_with_average_degree(cls, size_of_network, parameter_list):
    """Generates a graph based on the Watts-Strogatz model.
    Works badly when the average degree is odd.

    Parameters:
      size_of_network: An integer indicating the number of nodes in the network.
      parameter_list: A list [average_degree, rewiring_probability].
        average_degree: A number indicating the average degree of the network.
        rewiring_probability: A double indicating the rewiring probability.

    Returns:
      An igraph graph based on the Barabasi-Albert model.
    """
    distance_of_connection = (int(parameter_list[0]/2)
        if parameter_list[0] - int(parameter_list[0]/2)*2 < int((parameter_list[0]+1)/2) - parameter_list[0]
        else int((parameter_list[0]+1)/2))
    return igraph.GraphBase.Watts_Strogatz(dim = 1, size = size_of_network,
        nei = distance_of_connection, p = parameter_list[1], loops = False,
        multiple = False)

  @classmethod
  def generate_SF2ER(cls, size_of_network, parameter_list):
    """Generates a graph based on a configuration model.
    Based on the model B in "From Scale-free to Erdos-Renyi Networks".

    Parameters:
      size_of_network: An integer indicating the number of nodes in the network.
      parameter_list: A list [alpha, m0, m].
        alpha: A double between 0 and 1 indicating the probability of choosing a
            node without taking the degree into account.
        m0: An integer indicating the number of nodes in the starting fully
            connected network.
        m: An integer indicating the number of outgoing edges for each node
            except the m0 starting ones.

    Returns:
      An igraph graph based on a configuration model.
    """
    alpha = parameter_list[0]
    m0 = parameter_list[1]
    m = parameter_list[2]

    # Number of edges generated by preferential attachment for each node
    pa_deg = [0] * size_of_network

    # Create an undirected graph with size_of_network nodes.
    g = igraph.Graph(size_of_network)

    # Create the fully connected graph.
    for i in xrange(0, m0):
      g.add_edges([(i, j) for j in range(0, m0) if j > i])
      pa_deg[i] = m0-1

    # Add the remaining nodes.
    for i in xrange(m0, size_of_network):
      all_nodes = [node for node in xrange(0, size_of_network) if node != i]
      if cls.RANDOM.random() < alpha:
        # Connect to m nodes with uniform probability distribution.
        g.add_edges([(i, j) for j in cls.RANDOM.choice(all_nodes, m, False)])
      else:
        # Connect to m nodes with probability proportional to the degree.
        deg = [pa_deg[node] for node in all_nodes]
        total = sum(deg)
        prob = [degree * 1.0 / total for degree in deg]
        chosen_nodes = cls.RANDOM.choice(all_nodes, m, False, prob)
        for node in chosen_nodes:
          pa_deg[node] += 1
          pa_deg[i] += 1
          g.add_edge(i, node)
    return g

  @classmethod
  def generate_Waxman(cls, size_of_network, parameter_list, point_list=None):
    """Generates a graph based on the Waxman model.
    The Waxman model creates a random graph when the nodes are points in space.
    The probability that two nodes are connected depends on the distance between
        the points.

    P(u, v) = \beta \exp{ \frac{ -d(u, v) }{ sqrt(2) \alpha } }
    
    Based on "Routing of Multipoint Connections".
    
    Parameters:
      size_of_network: An integer indicating the number of nodes in the network.
      parameter_list: A list [alpha, beta, force_connected].
        alpha: A double between 0 and 1 representing the density of short edges
            relative to long ones.
        beta: A double representing the edge density.
        force_connected: A boolean indicating if the graph must be connected(default=False).
      point_list: A list where the points will be inserted (optional).

    Returns:
      An igraph graph based on the Waxman model.
    """
    alpha = parameter_list[0]
    beta = parameter_list[1]
    force_connected=parameter_list[2] if len(parameter_list) > 2 else False

    # Create an undirected graph with size_of_network nodes.
    g = igraph.Graph(size_of_network)

    # Create random points in a 1x1 square.
    x = cls.RANDOM.uniform(size=size_of_network)
    y = cls.RANDOM.uniform(size=size_of_network)

    # Create the edges.
    sq2 = math.sqrt(2)
    edges = []
    has_neighbor = [False for _ in xrange(size_of_network)]
    for i in xrange(size_of_network):
      if point_list:
        point_list.append((x[i], y[i]))
      # Debug.
      if not (i%500):
        print >> sys.stderr, i

      for j in xrange(i+1, size_of_network):
        d = ((x[i] - x[j]) ** 2 + (y[i] - y[j]) ** 2) ** 0.5
        p = beta * math.exp(-d / (sq2 * alpha))
        if p > 1:
          raise Exception('P is larger than 1.')
        if cls.RANDOM.uniform() < p:
          has_neighbor[i] = True
          has_neighbor[j] = True
          edges.append((i, j))

      # Force the graph to be connected.
      if force_connected and not has_neighbor[i]:
        indices = []
        ps = []
        for j in xrange(0, size_of_network):
          if i == j:
            continue
          d = ((x[i] - x[j]) ** 2 + (y[i] - y[j]) ** 2) ** 0.5
          p = beta * math.exp(-d / (sq2 * alpha))
          ps.append(p)
          indices.append(j)
        tot = sum(ps)
        ps = [ps[_i]*1.0/tot for _i in xrange(0, size_of_network-1)]
        edges.append((i, cls.RANDOM.choice(indices, p=ps)))
    g.add_edges(edges)
    return g

  @classmethod
  def generate_Waxman_with_average_degree(cls, size_of_network, parameter_list):
    """Generates a graph based on the Waxman model.
    The Waxman model creates a random graph when the nodes are points in space.
    The probability that two nodes are connected depends on the distance between
        the points.

    P(u, v) = \beta \exp{ \frac{ -d(u, v) }{ sqrt(2) \alpha } }
    
    Based on "Routing of Multipoint Connections".

    Uses binary search to determine the beta parameter to produce the desired
    average degree.

    Warning: Super slow :(

    Parameters:
      size_of_network: An integer indicating the number of nodes in the network.
      parameter_list: A list [average_degree, alpha].
        average_degree: A number indicating the average degree of the network.
        alpha: A double between 0 and 1 representing the density of short edges
            relative to long ones.
        force_connected: A boolean indicating if the graph must be connected(default=False).

    Returns:
      An igraph graph based on the Waxman model.
    """
    alpha = parameter_list[1]
    force_connected = parameter_list[2] if len(parameter_list) > 2 else False
    eps = 0.05

    g = cls.generate_Waxman(size_of_network, [alpha, 0.3, force_connected])
    average_degree_03 = g.ecount() * 2.0 / g.vcount()
    g = cls.generate_Waxman(size_of_network, [alpha, 0.8, force_connected])
    average_degree_08 = g.ecount() * 2.0 / g.vcount()

    mid = parameter_list[0] * 0.5 / (average_degree_08 - average_degree_03)

    beg = max(mid-0.1, 0.0)
    end = min(mid+0.1, 1.0)
    while beg < end:
      mid = (beg + end)/2.0
      g = cls.generate_Waxman(size_of_network, [alpha, mid, force_connected])
      average_degree = g.ecount() * 2.0 / g.vcount()
      print >> sys.stderr, mid, average_degree
      if abs(average_degree - parameter_list[0]) / parameter_list[0] < eps:
        cnt = 0
        while not g.is_connected():
          print >> sys.stderr, 'ainda no', cnt
          cnt += 1
          g = cls.generate_Waxman(size_of_network, [alpha, mid, force_connected])
        print cnt
        break
      if average_degree < parameter_list[0]:
        beg = mid
      else:
        end = mid
    return g

  @classmethod
  def generate_SpatialSF(cls, size_of_network, parameter_list):
    """Generates a graph based on the geographic Scale Free model.
    The geographic Scale Free model creates a random graph when the nodes are
        points in space. The probability that two nodes are connected depends on
        the distance between the points and the degree of the nodes.

    P(u, v) = (k + 1) \exp{ \frac{ -d(u, v) }{ r_c } }
    
    Based on "Crossover from Scale-Free to Spatial Networks".
    
    Parameters:
      size_of_network: An integer indicating the number of nodes in the network.
      parameter_list: A list [n0, m, rc].
        n0: An integer indicating the number of initial active nodes.
        m: An integer indicating the number of outgoing edges for each node
            except the n0 starting ones.
        rc: A double between 0 and 1 representing the density of short edges
            relative to long ones.

    Returns:
      An igraph graph based on the spatial Scale Free model.
    """
    n0 = int(parameter_list[0])
    m = int(parameter_list[1])
    rc = parameter_list[2]

    # Create an undirected graph with size_of_network nodes.
    g = igraph.Graph(size_of_network)

    # Create random points in a 1x1 square.
    x = cls.RANDOM.uniform(size=size_of_network)
    y = cls.RANDOM.uniform(size=size_of_network)

    edges = []

    for i in xrange(n0):
      for j in xrange(i+1, n0):
        edges.append((i, j))

    for i in xrange(n0, size_of_network):
      deg = g.degree(range(0, i))
      prob = []
      for j in xrange(0, i):
        d = ((x[i] - x[j]) ** 2 + (y[i] - y[j]) ** 2) ** 0.5
        prob.append((deg[j] + 1) * math.exp(-d / rc))
      total = sum(prob)
      prob = [probability / total for probability in prob]
      edges += [(i, j) for j in \
          cls.RANDOM.choice(range(0, i), m, False, prob)]
    g.add_edges(edges)

    return g

  @classmethod
  def generate_SpatialSF_with_average_degree(cls, size_of_network, parameter_list):
    """Generates a graph based on the geographic Scale Free model.
    Works badly when the average degree is odd.
    The geographic Scale Free model creates a random graph when the nodes are
        points in space. The probability that two nodes are connected depends on
        the distance between the points and the degree of the nodes.

    P(u, v) = (k + 1) \exp{ \frac{ -d(u, v) }{ r_c } }
    
    Based on "Crossover from Scale-Free to Spatial Networks".
    
    Parameters:
      size_of_network: An integer indicating the number of nodes in the network.
      parameter_list: A list [average_degree, rc].
        average_degree: A number indicating the average degree of the network.
        rc: A double between 0 and 1 representing the density of short edges
            relative to long ones.

    Returns:
      An igraph graph based on the spatial Scale Free model.
    """
    m = (int(parameter_list[0]/2)
        if parameter_list[0] - int(parameter_list[0]/2)*2 < int((parameter_list[0]+1)/2) - parameter_list[0]
        else int((parameter_list[0]+1)/2))
    return cls.generate_SpatialSF(size_of_network, [m,
        m, parameter_list[1]])

  @classmethod
  def generate_DegreeSequence(cls, size_of_network, parameter_list):
    """Generates a simple connected graph with the given degree sequence.

      Parameters:
        size_of_network: An integer indicating the number of nodes in the network.
        parameter_list: A list [degree_list].
          degree_list: A list of integers indicating the degree of each node in
              the graph. The sum of the integers must be even.

      Returns:
        An igraph simple connected graph with the given degree sequence.
    """
    return igraph.GraphBase.Degree_Sequence(parameter_list[0], method="vl")

  @classmethod
  def generate_ConfigurationSF(cls, size_of_network, parameter_list):
    """Generates a simple connected SF network with a given degree distribution.

    Parameters:
      size_of_network: An integer indicating the number of nodes in the network.
      parameter_list: A list [gamma].
        gamma: A double indicating the gamma of the degree distribution.
            P(k) ~ k^(-gamma)
        m: An integer indicating the minimum degree.

    Returns:
      An igraph simple connected SF network with a degree distribution defined by
          the parameter.
    """
    gamma = parameter_list[0]
    m = parameter_list[1]
    probabilities = [i**(-gamma) for i in range(m, size_of_network)]
    total = sum(probabilities)
    probabilities = [probabilities[i]/total for i in range(len(probabilities))]
    degree_sequence = list(cls.RANDOM.choice(range(m, size_of_network),
        size_of_network, True, probabilities))
    if sum(degree_sequence) % 2 == 1:
      degree_sequence[0] += 1
    return cls.generate_DegreeSequence(size_of_network, [degree_sequence])

if __name__ == '__main__':
  # print NetworkGenerator.generate_BA_with_average_degree(100, [3, False, 1.5])
  # g = NetworkGenerator.generate_ER_with_average_degree(100, [3, False])
  # g = NetworkGenerator.generate_WS_with_average_degree(100, [3, 0.5])
  # g = NetworkGenerator.generate_SpatialSF_with_average_degree(100, [3, 0.3])
   g = NetworkGenerator.generate_Waxman_with_average_degree(100, [3, 0.6])
  # g = NetworkGenerator.generate_SF2ER(1000, [0.5, 5, 5]).degree(), 100)
  # g = NetworkGenerator.generate_Waxman(100, [0.4, 0.05])
  # g = NetworkGenerator.generate("SpatialSF", 500, [5, 5, 0.5])
  # g = NetworkGenerator.generate("ConfigurationSF", 1000, [2.3, 2])
  # g.write_edgelist("ConfigurationSF_1000_023.edgelist")
   print g
  # pyplot.hist(g.degree(), 100)
  # pyplot.show()
