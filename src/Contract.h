#pragma once

#include <ranges>
#include <vector>

#include "Logger.h"
#include "Metrics.h"
#include "Params.h"
#include "Config.h"

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
  ContractManager(const ConfigView<ContractConfig>&& config, std::mt19937& rng, MetricManager& metrics)
      : m_config(config)
      , m_rng(rng)
      , m_metrics(metrics)
      , m_createProb(config.get<double>("creation_probability"))
      , m_priceDist(config.get<NormalDist>("price_distribution"))
      , m_difficultyDist(config.get<NormalDist>("difficulty_distribution")) {}

  void createContracts(unsigned int t) {
    // Create new contracts
    unsigned int duration = m_config.get<unsigned int>("duration");
    unsigned int adjudicators_per_contract = m_config.get<unsigned int>("adjudicators_per_contract");
    if (m_createProb(m_rng)) {
      LOG(INFO) << "Creating new contract, ID: " << m_count;
      m_metrics.getFrame(t).increment(FrameMetrics::CONTRACTS_CREATED);
      m_contracts.emplace_back(m_count++, m_priceDist(m_rng), m_difficultyDist(m_rng),
                               t + duration, adjudicators_per_contract);
    }
  }

  std::vector<Contract>& getContracts() { return m_contracts; }

 private:
  unsigned int m_count = 0;

  ConfigView<ContractConfig> m_config;

  std::mt19937& m_rng;

  MetricManager& m_metrics;

  std::bernoulli_distribution      m_createProb;
  std::normal_distribution<double> m_priceDist;
  std::normal_distribution<double> m_difficultyDist;

  std::vector<Contract> m_contracts;
};

typedef std::vector<Contract> Blockchain;
