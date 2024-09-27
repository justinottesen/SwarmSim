#pragma once

#include <vector>
#include <ranges>
#include "Params.h"

#include "Logger.h"

struct Contract {
  unsigned int ID;

  double       price;
  double       difficulty;
  unsigned int deadline;

  bool available = true;
};

class ContractManager {
 public:
  ContractManager(const ContractParams& params, std::mt19937& rng)
      : m_rng(rng)
      , m_createProb(params.creation_prob)
      , m_priceDist(params.price_dist.mean, params.price_dist.stdev)
      , m_difficultyDist(params.difficulty_dist.mean, params.difficulty_dist.stdev) {}

  void updateContracts(unsigned int t) {
    // Create new contracts
    if (m_createProb(m_rng)) {
      LOG(INFO) << "Creating new contract, ID: " << m_count;
      m_contracts.emplace_back(m_count++, m_priceDist(m_rng), m_difficultyDist(m_rng),
                               t + m_params.duration);
    }
  }

  std::vector<Contract>& getContracts() { return m_contracts; }

 private:
  unsigned int m_count = 0;

  ContractParams m_params;

  std::mt19937& m_rng;

  std::bernoulli_distribution      m_createProb;
  std::normal_distribution<double> m_priceDist;
  std::normal_distribution<double> m_difficultyDist;

  std::vector<Contract> m_contracts;
};

typedef std::vector<Contract> Blockchain;
