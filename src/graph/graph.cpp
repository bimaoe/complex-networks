#include "graph.h"

Graph::Graph() {
  isUndirected = true;
}

Graph::Graph(bool isUndirected) {
  this->isUndirected = isUndirected;
}

int Graph::getVertexCount(void) {
  return (int) adjacencyList.size();
}

int Graph::getEdgeCount(void) {
  return (int) edges.size();
}

void Graph::addVertex(void) {
  adjacencyList.push_back(set<int>());
}

void Graph::addVertices(int quantity) {
  for (int i = 0; i < quantity; i++)
  addVertex();
}

void Graph::addEdge(int vId1, int vId2) {
  edges.push_back(make_pair(vId1, vId2));
  adjacencyList[vId1].insert(vId2);
  if (isUndirected) {
    adjacencyList[vId2].insert(vId1);
  }
}

void Graph::addEdge(pair<int, int> vIds) {
  addEdge(vIds.first, vIds.second);
}

void Graph::addEdges(vector<pair<int, int> > & vIds) {
  for (int i = 0; i < vIds.size(); i++) {
    addEdge(vIds[i]);
  }
}

void Graph::clear(void) {
  adjacencyList.clear();
  edges.clear();
}

bool Graph::areConnected(int vId1, int vId2) {
  return adjacencyList[vId1].count(vId2) > 0;
}

set<int> & Graph::getNeighbours(int vId) {
  return adjacencyList[vId];
}

void Graph::readFromFile(string filename) {
  ifstream file;
  file.open(filename);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open file " << filename << std::endl;
    return;
  }

  int vId1, vId2;
  string line;
  while (getline(file, line)) {
    // Lines that start with # are comments.
    if (line[0] == '#') continue;
    stringstream s(line);
    s >> vId1 >> vId2;
    addVertices(max(vId1, vId2) - getVertexCount() + 1);
    addEdge(vId1, vId2);
    if (isUndirected) {
      addEdge(vId2, vId1);
    }
  }
}

void Graph::SSSP(int source, vector<int> & distance) {
  // Initialize distance vector.
  distance.resize(getVertexCount(), -1);
  queue<int> q;

  // Initialize distance of the source.
  distance[source] = 0;
  q.push(source);

  // Run BFS.
  while(!q.empty()) {
    int curr = q.front(); q.pop();
    for (auto it = getNeighbours(curr).begin();
        it != getNeighbours(curr).end(); it++) {
      if (distance[*it] == -1) {
        distance[*it] = distance[curr]+1;
        q.push(*it);
      }
    }
  }
}

// #include <iostream>

// int main(void) {
//   Graph g;
//   g.addVertices(3);
//   g.addEdge(0, 1);
//   g.addEdge(0, 2);
//   cout << g.areConnected(0, 1) << " " << g.areConnected(1, 2) << endl;
//   cout << g.edges.size() << endl;
// }
