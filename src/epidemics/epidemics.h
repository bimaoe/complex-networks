#include "../graph/graph.h"
#include "../stat/stat.h"
#include <vector>
#include <list>

class Epidemics {
/** This class implements the SI, SIS and SIR epidemic processes.
**/
public:
  enum Status {
    SUSCEPTIBLE = 0,
    INFECTED = 1,
    RECOVERED = 2
  };

  class Evolution {
  public:
    vector<long long> statusCount[3]; // Should be indexed with the Status enum.
    void add(long long sCount, long long iCount, long long rCount);
  };

  /** Initialize the infected list and the status vector. **/
  static void initializeInfected(vector<int> & initiallyInfected, list<int> & infectedVertices,
      vector<Status> & status);

  /** Run the SIR model in a graph.
    Parameters:
      graph: The graph where the epidemics will be spread.
      delta: The infection probability, must be in [0, 1].
      nu: The recovery probability, must be in [0, 1].
      iniciallyInfected: The initially infected vertices' IDs.
      maxIterations: Maximum number of iterations to run.
  **/
  static Epidemics::Evolution * runSIR(Graph & graph, double delta, double nu,
      vector<int> & initiallyInfected, int maxIterations);

  /** Run the SIR model in a graph.
    Parameters:
      graph: The graph where the epidemics will be spread.
      delta: The infection probability, must be in [0, 1].
      nu: The recovery probability, must be in [0, 1].
      first: The initially infected vertex's ID.
      maxIterations: Maximum number of iterations to run.
  **/
  static Epidemics::Evolution * runSIR(Graph & graph, double delta, double nu, int firstInfected,
      int maxIterations);
};