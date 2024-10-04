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
  unsigned int deadline;
  unsigned int open_adjudicators;

  bool available = true;
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

  void createContracts(unsigned int t) {
    // Create new contracts
    if (m_createProb(m_rng)) {
      LOG(INFO) << "Creating new contract, ID: " << m_count;
      m_metrics.getFrame(t).increment(FrameMetrics::CONTRACTS_CREATED);
      m_contracts.emplace_back(m_count++, m_priceDist(m_rng), m_difficultyDist(m_rng),
                               t + m_params.duration, m_params.num_adjudicators);
    }
  }

  std::vector<Contract>& getContracts() { return m_contracts; }

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
