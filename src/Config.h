#pragma once

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ratio>
#include <string>
#include <tuple>

#include "Logger.h"
#include "Params.h"
#include "TimeUtil.h"

template <std::intmax_t N, std::intmax_t D>
std::ostream& operator<<(std::ostream& ostr, const std::ratio<N, D>&) {
  ostr << N << "/" << D;
  return ostr;
}

template <unsigned int N> struct StringLiteral {
  char data[N]{};

  consteval StringLiteral(const char (&str)[N]) { std::copy_n(str, N, data); }

  consteval operator std::string_view() const { return std::string_view(data, N); }
};

template <StringLiteral Name, class ValueType, ValueType Default = ValueType{}> struct NamedValue {
  std::string_view name{Name};
  ValueType        value{Default};
};

template <StringLiteral Name, class ValueType, ValueType Default>
std::ostream& operator<<(std::ostream& ostr, const NamedValue<Name, ValueType, Default>& val) {
  ostr << '"' << Name.data << '"' << ": " << val.value;
  return ostr;
}

inline std::ostream& operator<<(std::ostream& ostr, const NormalDist& nd) {
  ostr << "N(" << nd.mean << ", " << nd.stdev << "^2)";
  return ostr;
}

class ConfigManager {
 public:
  ~ConfigManager() { dump(std::filesystem::path("data") / program_time(true) / "config.json"); }

  using SimulationConfig = std::tuple<NamedValue<"simulation_length", unsigned int, 60>,
                                      NamedValue<"rng_seed", unsigned int>>;
  using ContractConfig =
      std::tuple<NamedValue<"creation_probability", std::ratio<1, 2>>,
                 NamedValue<"price_distribution", NormalDist, NormalDist(10, 1)>,
                 NamedValue<"difficulty_distribution", NormalDist, NormalDist(1, 0.1)>,
                 NamedValue<"adjudicators_per_contract", unsigned int, 3>,
                 NamedValue<"collateral_ratio", std::ratio<1, 1>>,
                 NamedValue<"duration", unsigned int, 10>>;
  using AgentConfig = std::tuple<NamedValue<"num_workers", unsigned int, 10>,
                                 NamedValue<"ability_distribution", NormalDist, NormalDist(1, 0.1)>,
                                 NamedValue<"num_ajdudicators", unsigned int, 10>>;

  SimulationConfig simulation;
  ContractConfig   contract;
  AgentConfig      agent;

  void dump(const std::filesystem::path& path) const {
    LOG(INFO) << "Dumping config to path " << path;
    if (!std::filesystem::exists(path) && path.has_parent_path()) {
      std::filesystem::create_directories(path.parent_path());
    }
    std::ofstream json_file(path);
    if (!json_file.good()) {
      LOG(ERROR) << "Unable to dump config to path " << path;
      return;
    }
    json_file << "{\n"
              << "  \"simulation\": {\n";
    std::apply(
        [&](const auto&... param) {
          unsigned int n = 0;
          ((json_file << "    " << param
                      << (++n != std::tuple_size<SimulationConfig>() ? ",\n" : "\n")),
           ...);
        },
        simulation);
    json_file << "  }\n"
              << "  \"contract\": {\n";
    std::apply(
        [&](const auto&... param) {
          unsigned int n = 0;
          ((json_file << "    " << param
                      << (++n != std::tuple_size<ContractConfig>() ? ",\n" : "\n")),
           ...);
        },
        contract);
    json_file << "  }\n"
              << "  \"agent\": {\n";
    std::apply(
        [&](const auto&... param) {
          unsigned int n = 0;
          ((json_file << "    " << param
                      << (++n != std::tuple_size<AgentConfig>() ? ",\n" : "\n")),
           ...);
        },
        agent);
    json_file << "  }\n"
              << "}\n";
  }
};

