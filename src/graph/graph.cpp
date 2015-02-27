#include <vector>
#include <set>

using namespace std;

class Graph {
/** This class implements graph using an adjacency list.
**/
public:
	bool isUndirected;
	vector<set<int> > adjacencyList;
	vector<pair<int, int> > edges;

	Graph() {
		isUndirected = true;
	}

	Graph(bool isUndirected) {
		this->isUndirected = isUndirected;
	}

	int vertexCount(void) {
		return (int) adjacencyList.size();
	}

	int edgeCount(void) {
		return (int) edges.size();
	}

	void addVertex(void) {
		adjacencyList.push_back(set<int>());
	}

	void addVertices(int quantity) {
		for (int i = 0; i < quantity; i++)
			addVertex();
	}

	void addEdge(int vId1, int vId2) {
		edges.push_back(make_pair(vId1, vId2));
		adjacencyList[vId1].insert(vId2);
		if (isUndirected)
			adjacencyList[vId2].insert(vId1);
	}

	void addEdge(pair<int, int> vIds) {
		addEdge(vIds.first, vIds.second);
	}

	void addEdges(vector<pair<int, int> > & vIds) {
		for (int i = 0; i < vIds.size(); i++)
			addEdge(vIds[i]);
	}

	bool areConnected(int vId1, int vId2) {
		return adjacencyList[vId1].count(vId2) > 0;
	}
};

#include <iostream>

int main(void) {
	Graph g;
	g.addVertices(3);
	g.addEdge(0, 1);
	g.addEdge(0, 2);
	cout << g.areConnected(0, 1) << " " << g.areConnected(1, 2) << endl;
	cout << g.edges.size() << endl;
}
