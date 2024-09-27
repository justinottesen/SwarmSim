#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <shared_mutex>
#include <sstream>
#include <string>

#define LOG(level) \
  if (true) Logger(level, __FILE__, __LINE__, __FUNCTION__)

enum LogLevel { FATAL, ERROR, WARN, INFO, DEBUG, TRACE };

std::string_view toStr(LogLevel level);

class Logger {
 public:
  Logger(LogLevel level, const std::filesystem::path& path, int line, const char* function);
  ~Logger();

  static void addFile(const std::string& path, LogLevel level = INFO) {
    Workers::addWorker(path, level);
  }

  static void addConsole(LogLevel level = INFO) { Workers::addWorker("", level); }

  static void setConsoleLevel(LogLevel level) { Workers::setLevel("", level); }

  static void setLevel(const std::filesystem::path& path, LogLevel level) {
    Workers::setLevel(path, level);
  }

  static void removeFile(const std::filesystem::path& path) { Workers::removeWorker(path); }

  static void removeConsole() { Workers::removeWorker(""); }

  template <class T> std::ostream& operator<<(const T& msg) { return m_buffer << msg; }

 private:
  LogLevel m_level;

  std::ostringstream m_buffer;

  class Workers {
   public:
    static void addWorker(const std::filesystem::path& path, LogLevel level = INFO);

    static void setLevel(const std::filesystem::path& path, LogLevel level);

    static void removeWorker(const std::filesystem::path& path);

    static void log(LogLevel level, std::string_view msg);

   private:
    class Worker {
     public:
      Worker(LogLevel level)
          : m_level(level) {}

      virtual ~Worker() {}

      void log(LogLevel level, std::string_view msg);

      void setLevel(LogLevel level);

     protected:
      void logTime();

      void logOpeningMessage(std::string_view location);

     private:
      virtual std::ostream& stream() = 0;

      std::mutex m_mutex;
      LogLevel   m_level;
    };

    class ConsoleWorker : public Worker {
     public:
      ConsoleWorker(LogLevel level)
          : Worker(level) {
        logOpeningMessage("Console");
      }
      ~ConsoleWorker() {
        logTime();
        stream() << "Closing log" << std::endl;
      }

     private:
      std::ostream& stream() override { return std::cout; }
    };

    class FileWorker : public Worker {
     public:
      FileWorker(LogLevel level, std::filesystem::path path);
      ~FileWorker() {
        logTime();
        stream() << "Closing log" << std::endl;
      }

     private:
      std::ostream& stream() override { return m_stream; }

      std::ofstream m_stream;
    };

    static std::shared_mutex& mutex() {
      static std::shared_mutex mutex;
      return mutex;
    }

    static std::map<std::string, std::unique_ptr<Worker>>& workers() {
      static std::map<std::string, std::unique_ptr<Worker>> workers;
      return workers;
    }
  };
};
