#ifndef GRAPH_H
#define GRAPH_H

#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <vector>


using namespace std;

class Graph {
/** This class implements a graph using an adjacency list.
**/
public:
  bool isUndirected;
  vector<set<int> > adjacencyList;
  vector<pair<int, int> > edges; // List of edges.

  Graph();

  Graph(bool isUndirected);

  /** Return the number of vertices in the graph. **/
  int getVertexCount(void);

  /** Return the number of edges in the graph. **/
  int getEdgeCount(void);

  /** Add a new vertex to the graph. Its ID will be the last ID + 1. **/
  void addVertex(void);

  void addVertices(int quantity);

  /** Add an edge between vertices with ID $vId1$ and $vId2$. If the graph is directed, the edge
      will be from $vId1$ to $vId2$. **/
  void addEdge(int vId1, int vId2);

  /** Add an edge between vertices with ID $vId.first$ and $vId.second$. If the graph is directed,
      the edge will be from $vId.first$ to $vId.second$. **/
  void addEdge(pair<int, int> vIds);

  void addEdges(vector<pair<int, int> > & vIds);

  /** Removes all the vertices and edges of the graph. **/
  void clear(void);

  /** Check whether vertices with IDs $vId1$ and $vId2$ are connected. If the graph is directed,
      check if there is an edge from $vId1$ to $vId2$.**/
  bool areConnected(int vId1, int vId2);

  set<int> & getNeighbours(int vId);

  /** Read a graph from a file.
    In each line there should be 2 integers indicating the ID's of two vertices to be connected.
  **/
  void readFromFile(string filename);

  /** Calculate the single source shortest path with the BFS algorithm. **/
  void SSSP(int source, vector<int> & distance);
};

#endif