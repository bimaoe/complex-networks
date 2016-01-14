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
    long long getSpreaderCount(void); /* Get the number of spreader vertices.
                                        Includes stifler vertices. */
    void operator += (Evolution *e);
  };

  static void initialize(void);

  /** Initialize the infected list and the status vector. **/
  static void initializeSpreader(vector<int> & initiallySpreader, list<int> & spreaderVertices,
      vector<Status> & status);

  /** Run the ISR Maki-Thompson model in a graph.
    Parameters:
      graph: The graph where the rumor will be spread.
      delta: The infection probability, must be in [0, 1].
      nu: The recovery probability, must be in [0, 1].
      iniciallyInfected: The initially infected vertices' IDs.
      maxIterations: Maximum number of iterations to run.
  **/
  static Rumor::Evolution * runISR(Graph & graph, double delta, double nu,
      vector<int> & initiallySpreader, int maxIterations);

  /** Run the ISR Maki-Thompson model in a graph.
    Parameters:
      graph: The graph where the rumor will be spread.
      delta: The infection probability, must be in [0, 1].
      nu: The recovery probability, must be in [0, 1].
      firstInfected: The initially infected vertex's ID.
      maxIterations: Maximum number of iterations to run.
  **/
  static Rumor::Evolution * runISR(Graph & graph, double delta, double nu, int firstInfected,
      int maxIterations);
};