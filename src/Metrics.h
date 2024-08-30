#include <vector>

struct Metrics {
  // Contracts
  unsigned int contracts_created       = 0;    // Number of contracts added this timestep
  unsigned int contracts_created_price = 0;    // Total value added this timestep

  unsigned int contracts_expired       = 0;    // Number of contracts expired this timestep
  unsigned int contracts_expired_price = 0;    // Total value expired this timestep
};

class MetricManager {
  public:
  // TODO: Dump metrics to file

  Metrics& nextFrame() { return m_frames.emplace_back(); }

  private:
  std::vector<Metrics> m_frames;
};
