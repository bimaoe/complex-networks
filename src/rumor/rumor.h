#include "../graph/graph.h"
#include "../stat/stat.h"
#include <vector>
#include <list>

class Rumor {
/** This class implements the ISR rumor propagation.
**/
public:
  Stat stat;

  enum Status {
    IGNORANT = 0,
    SPREADER = 1,
    STIFLER = 2
  };

  class Evolution {
  public:
    int usedStatus; // Should be 3 for IRS.
    vector<long long> statusCount[3]; // Should be indexed with the Status enum.
    void add(long long iCount, long long sCount, long long rCount);
    long long getReliability(void); /* Get the number of spreader vertices.
                                        Includes stifler vertices. */
    void operator += (Evolution *e);
  };

  static void initialize(void);

  /** Initialize the spreader list and the status vector. **/
  static void initializeSpreader(vector<int> & initiallySpreader, list<int> & spreaderVertices,
      vector<Status> & status);

  /** Run the ISR Maki-Thompson model in a graph.
    Parameters:
      graph: The graph where the rumor will be spread.
      lambda: The spreading probability, must be in [0, 1].
      alpha: The recovery probability, must be in [0, 1].
      iniciallySpreader: The initially spreader vertices' IDs.
      maxIterations: Maximum number of iterations to run.
      status: Vector of status (in case one wants to know the status of every node).
  **/
  static Rumor::Evolution * runISR(Graph & graph, double lambda, double alpha,
    vector<int> & initiallySpreader, int maxIterations, vector<Status> &status);

  /** Run the ISR Maki-Thompson model in a graph.
    Parameters:
      graph: The graph where the rumor will be spread.
      lambda: The spreading probability, must be in [0, 1].
      alpha: The recovery probability, must be in [0, 1].
      iniciallySpreader: The initially spreader vertices' IDs.
      maxIterations: Maximum number of iterations to run.
  **/
  static Rumor::Evolution * runISR(Graph & graph, double lambda, double alpha,
      vector<int> & initiallySpreader, int maxIterations);

  /** Run the ISR Maki-Thompson model in a graph.
    Parameters:
      graph: The graph where the rumor will be spread.
      lambda: The spreading probability, must be in [0, 1].
      alpha: The recovery probability, must be in [0, 1].
      firstSpreader: The initially spreader vertex's ID.
      maxIterations: Maximum number of iterations to run.
      status: Vector of status (in case one wants to know the status of every node).
  **/
  static Rumor::Evolution * runISR(Graph & graph, double lambda, double alpha, int firstSpreader,
    int maxIterations, vector<Status> &status);

  /** Run the ISR Maki-Thompson model in a graph.
    Parameters:
      graph: The graph where the rumor will be spread.
      lambda: The infection probability, must be in [0, 1].
      alpha: The recovery probability, must be in [0, 1].
      firstSpreader: The initially spreader vertex's ID.
      maxIterations: Maximum number of iterations to run.
  **/
  static Rumor::Evolution * runISR(Graph & graph, double lambda, double alpha, int firstSpreader,
      int maxIterations);
};