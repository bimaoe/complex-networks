"""This module reads a graph from a file and writes its largest connected component in a file."""
import argparse

def find(uf, a):
  if uf[a] == a:
    return a
  uf[a] = find(uf, uf[a])
  return uf[a]

def union(uf, a, b):
  if a not in uf:
    uf[a] = a
  if b not in uf:
    uf[b] = b
  a = find(uf, a)
  b = find(uf, b)
  uf[a] = b

def lcc(graph):
  """This function returns the largest connected component of a graph using
    disjoint set union.
    The input graph is considered undirected and the edges of the output
    graph maintain the same order as the ones in the input graph.
    The nodes are relabeled."""
  # Find the connected components.
  uf = {}
  for u in graph:
    for v in graph[u]:
      union(uf, u, v)

  # Count the number of vertices in each connected component.
  count = {}
  for u in uf:
    if find(uf, u) not in count:
      count[find(uf, u)] = 0
    count[find(uf, u)] += 1

  # Find the size of the largest connected component and its representative.
  mx = 0
  mxi = -1
  for u in count:
    if count[u] > mx:
      mx = count[u]
      mxi = u

  # Relabel the nodes in the largest connected component.
  relabel = {}
  for u in uf:
    if find(uf, u) == mxi:
      relabel[u] = len(relabel)

  # Create a graph with the largest connected component.
  graphp = {}
  for u in relabel:
    if u in graph:
      graphp[relabel[u]] = []
      for v in graph[u]:
        if v in relabel:
          graphp[relabel[u]].append(relabel[v])

  return graphp

def write_largest_connected_component(in_filename, out_filename):
  in_file = open(in_filename, 'r')
  g = {}
  for line in in_file:
    if line[0] == '#':
      continue
    u, v = line.split(' ')
    u = int(u)
    v = int(v)
    if u not in g:
      g[u] = []
    g[u].append(v)
  in_file.close()
  
  gp = lcc(g)

  out_file = open(out_filename, 'w')
  for u in gp:
    for v in gp[u]:
      out_file.write(str(u) + ' ' + str(v) + '\n')
  out_file.close()

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('-i', '--in_filename',
      help='File where the graph will be read from.', required=True)
  parser.add_argument('-o', '--out_filename',
      help='File where the largest connected component will be written to.', required=True)
  
  args = vars(parser.parse_args())
  in_filename = args['in_filename']
  out_filename = args['out_filename']

  write_largest_connected_component(in_filename, out_filename)

if __name__ == '__main__':
  main()