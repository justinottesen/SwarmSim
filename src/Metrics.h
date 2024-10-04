#pragma once

#include <filesystem>
#include <vector>

#include "Logger.h"

#define CASE_TO_STRING(x) \
  case x: return #x

class FrameMetrics {
 public:
  enum Type { CONTRACTS_CREATED, CONTRACTS_EXPIRED, CONTRACTS_COMPLETED, NUM_TYPES };

  static std::string_view toStr(Type t) {
    switch (t) {
      CASE_TO_STRING(CONTRACTS_CREATED);
      CASE_TO_STRING(CONTRACTS_EXPIRED);
      CASE_TO_STRING(CONTRACTS_COMPLETED);
      default: return "";
    }
  }

  void         increment(Type t) { metrics[t]++; }
  void         reset(Type t) { metrics[t] = 0; }
  void         set(Type t, unsigned int val) { metrics[t] = val; }
  unsigned int get(Type t) { return metrics[t]; }

 private:
  std::array<unsigned int, NUM_TYPES> metrics;
};

class MetricManager {
 public:
  FrameMetrics& getFrame(unsigned int t) {
    if (t >= m_metrics.size()) { m_metrics.resize(t + 1); }
    LOG(INFO) << "GETTING FRAME " << t << ", SIZE " << m_metrics.size();
    return m_metrics[t];
  }

  void dump(const std::filesystem::path& path) {
    LOG(INFO) << "Dumping metrics to path " << path;
    if (!std::filesystem::exists(path) && path.has_parent_path()) {
      std::filesystem::create_directories(path.parent_path());
    }
    std::ofstream csv_file(path);
    if (!csv_file.good()) {
      LOG(ERROR) << "Unable to dump metrics to path " << path;
      return;
    }
    csv_file << "FRAME,";
    for (unsigned int metric = 0; metric < FrameMetrics::NUM_TYPES; metric++) {
      csv_file << FrameMetrics::toStr(FrameMetrics::Type(metric)) << ",";
    }
    csv_file << "\n";
    LOG(INFO) << "NUM FRAMES IN METRICS: " << m_metrics.size();
    for (unsigned int frame = 0; frame < m_metrics.size(); frame++) {
      LOG(TRACE) << "METRICS FRAME " << frame;
      csv_file << frame << ",";
      for (unsigned int metric = 0; metric < FrameMetrics::NUM_TYPES; metric++) {
        csv_file << m_metrics[frame].get(FrameMetrics::Type(metric)) << ",";
      }
      csv_file << "\n";
    }
  }

 private:
  std::vector<FrameMetrics> m_metrics;
};
