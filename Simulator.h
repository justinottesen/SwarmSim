#include "Logger.h"
#include "Blockchain.h"
#include "Metrics.h"
#include <random>
#include <vector>

struct Params {
  // General
  unsigned int simulation_length = 60;
  unsigned int rng_seed = 0;

  // Contracts
  double new_contract_probability = 0.5;
  std::normal_distribution<double> contract_price_dist{10, 0};
  double collateral_percentage = 1;
};

class Simulator {
  public:
    Simulator(const Params& params = {}) : m_params(params), m_rng(params.rng_seed) {
      LOG(INFO) << "Initializing simulation";

      LOG(TRACE) << "Seeded rng with value " << params.rng_seed;
    }

    int run() {
      LOG(INFO) << "Running simulation with total length " << m_params.simulation_length;
      for (unsigned int time = 0; time < m_params.simulation_length; ++time) {
        LOG(TRACE) << "Simulating timestep " << time;
        Metrics& metrics = m_metricMgr.nextFrame();
        
        // Create a new contract
        if (m_rng() < m_params.new_contract_probability * m_rng.max()) {
          double price = m_params.contract_price_dist(m_rng);
          const Contract& contract = m_blockchain.emplace_back(price, time + std::round(price));
          LOG(INFO) << "Created new contract (price: " << contract.price << ", deadline: " << contract.deadline << ")";
          metrics.contracts_created++;
          metrics.contracts_created_price += contract.price;
        }

      }
      return 0;
    }

  private:
    Params m_params;

    std::mt19937 m_rng;

    MetricManager m_metricMgr;

    Blockchain m_blockchain;

};