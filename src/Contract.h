#pragma once

#include <ranges>
#include <vector>

#include "Logger.h"
#include "Metrics.h"
#include "Params.h"

struct Contract {
  unsigned int ID;

  double       price;
  double       difficulty;
  unsigned int created;
  unsigned int deadline;
  unsigned int open_adjudicators;

  bool available = true;

  static void dump_header(std::ostream& ostr) {
    ostr << "ID,PRICE,DIFFICULTY,CREATED,DEADLINE,ACCEPTED,\n";
  }

  void dump(std::ostream& ostr) const {
    ostr << ID << "," << price << "," << difficulty << "," << created << "," << deadline << "," << (available ? 0 : 1) << ",\n";
  }
};

class ContractManager {
 public:
  ContractManager(const ContractParams& params, std::mt19937& rng, MetricManager& metrics)
      : m_params(params)
      , m_rng(rng)
      , m_metrics(metrics)
      , m_createProb(params.creation_prob)
      , m_priceDist(params.price_dist.mean, params.price_dist.stdev)
      , m_difficultyDist(params.difficulty_dist.mean, params.difficulty_dist.stdev) {}
  ~ContractManager() {
    dump(std::filesystem::path("data") / program_time(true) / "contracts.csv");
  }

  void createContracts(unsigned int t) {
    // Create new contracts
    if (m_createProb(m_rng)) {
      LOG(INFO) << "Creating new contract, ID: " << m_count;
      m_metrics.getFrame(t).increment(FrameMetrics::CONTRACTS_CREATED);
      m_contracts.emplace_back(m_count++, m_priceDist(m_rng), m_difficultyDist(m_rng),
                               t, t + m_params.duration, m_params.num_adjudicators);
    }
  }

  std::vector<Contract>& getContracts() { return m_contracts; }

  void dump(const std::filesystem::path& path) {
    LOG(INFO) << "Dumping metrics to " << path;
    if (!std::filesystem::exists(path) && path.has_parent_path()) {
      std::filesystem::create_directories(path.parent_path());
    }
    std::ofstream csv_file(path);
    if (!csv_file.good()) {
      LOG(ERROR) << "Unable to dump metrics to path " << path;
      return;
    }
    Contract::dump_header(csv_file);
    for (const Contract& c : m_contracts) {
      c.dump(csv_file);
    }
  }

 private:
  unsigned int m_count = 0;

  ContractParams m_params;

  std::mt19937& m_rng;

  MetricManager& m_metrics;

  std::bernoulli_distribution      m_createProb;
  std::normal_distribution<double> m_priceDist;
  std::normal_distribution<double> m_difficultyDist;

  std::vector<Contract> m_contracts;
};

typedef std::vector<Contract> Blockchain;
