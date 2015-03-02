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

bool Graph::areConnected(int vId1, int vId2) {
  return adjacencyList[vId1].count(vId2) > 0;
}

set<int> & Graph::getNeighbours(int vId) {
  return adjacencyList[vId];
}

void Graph::readFromFile(string filename) {
  ifstream file;
  file.open(filename);

  int vId1, vId2;
  while (file >> vId1 >> vId2) {
    addVertices(max(vId1, vId2) - getVertexCount() + 1);
    addEdge(vId1, vId2);
    if (isUndirected) {
      addEdge(vId2, vId1);
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
