#include "Logger.h"

#include <sys/time.h>

Logger::Logger(LogLevel level, const std::filesystem::path& path, int line, const char* function) : m_level(level)
{
  m_buffer << "(" << toStr(level) << ") - " << path.filename().c_str() << ": " << line << " " << function << ": ";
}

Logger::~Logger() { Workers::log(m_level, m_buffer.str()); }

void Logger::Workers::addWorker(const std::filesystem::path& path, LogLevel level)
{
  std::lock_guard<std::shared_mutex> lock(mutex());
  if (path == "") {
    workers().try_emplace(std::move(path), std::make_unique<ConsoleWorker>(level));
  } else {
    workers().try_emplace(std::move(path), std::make_unique<FileWorker>(level, path));
  }
}

void Logger::Workers::setLevel(const std::filesystem::path& path, LogLevel level)
{
  std::lock_guard<std::shared_mutex> lock(mutex());
  workers().at(std::move(path))->setLevel(level);
}

void Logger::Workers::removeWorker(const std::filesystem::path& path)
{
  std::lock_guard<std::shared_mutex> lock(mutex());
  workers().erase(path);
}

void Logger::Workers::log(LogLevel level, std::string_view msg)
{
  std::shared_lock<std::shared_mutex> lock(mutex());
  for (auto& [_, worker] : workers()) { worker->log(level, msg); }
}

void Logger::Workers::Worker::log(LogLevel level, std::string_view msg)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  if (level > m_level) { return; }
  logTime();
  stream() << msg << std::endl;
}

void Logger::Workers::Worker::setLevel(LogLevel level)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  logTime();
  stream() << "Changing log level from " << toStr(m_level) << " to " << toStr(level) << std::endl;
  m_level = level;
}

void Logger::Workers::Worker::logTime()
{
  timeval tv;
  gettimeofday(&tv, nullptr);
  tm tm = *std::localtime(&tv.tv_sec);
  stream() << std::put_time(&tm, "%F %T") << "." << std::setw(3) << std::setfill('0')
           << tv.tv_usec / 1000 << " | ";
}

void Logger::Workers::Worker::logOpeningMessage(std::string_view location)
{
  stream() << std::string(25, '-') << std::endl;
  logTime();
  stream() << std::string(75, '-') << std::endl;
  logTime();
  stream() << "Opening \"" << location << "\" for logging at level " << toStr(m_level) << std::endl;
}

Logger::Workers::FileWorker::FileWorker(LogLevel level, std::filesystem::path path) : Worker(level)
{
  if (!std::filesystem::exists(path) && path.has_parent_path()) {
    std::filesystem::create_directories(path.parent_path());
  }
  m_stream.open(path, std::ios::app);
  if (!m_stream.good()) { std::cerr << "Unable to open log at " << path << std::endl; }
  logOpeningMessage(path.string());
}

std::string_view toStr(LogLevel level)
{
  switch (level) {
    case FATAL: return "FATAL";
    case ERROR: return "ERROR";
    case WARN: return "WARN";
    case INFO: return "INFO";
    case DEBUG: return "DEBUG";
    case TRACE: return "TRACE";
  }
}