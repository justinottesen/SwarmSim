#pragma once

#include <string>
#include <unordered_map>

class JSON_Node {
 public:
  JSON_Node(const std::string& label)
      : label(label) {}

 private:
  const std::string label;
};

template <class T> class JSON_Value : JSON_Node {
  JSON_Value(const T& value)
      : value{T}
      , JSON_Node() {}
  JSON_Value(const T& value)
      : value{T}
      , JSON_Node() {}
  virtual ~JSON_Value() {}

  const T value;
};

// Config(const std::filesystem::path& path) {
//   if (load(path)) {
//     LOG(INFO) << "Successfully loaded config from " << path;
//   } else {
//     LOG(ERROR) << "Unable to load config from " << path;
//     LOG(INFO) << "Using default config";
//   }
// }

// bool load(const std::filesystem::path& path) const {
//   std::ifstream json_file(path);
//   if (!json_file.good()) {
//     LOG(ERROR) << "Unable to open config at path " << path;
//     return false;
//   }
//   char buffer_char;
//   std::string buffer_str;
//   while (isspace(json_file.peek())) { json_file.get(); }
//   buffer_char = json_file.get();
//   if (buffer_char != '{') {
//     LOG(ERROR) << "Improper JSON file provided, expected {, found " << buffer_char;
//     return false;
//   }
//   while (json_file >> buffer_str) {
//     if (buffer_str == "simulation" || buffer_str == "simulation:") {
//       if (buffer_str == "simulation") {
//         while (isspace(json_file.peek())) { json_file.get(); }
//         if ((buffer_char = json_file.get()) != ':') {
//           LOG(ERROR) << "Improper JSON file provided, expected :, found " << buffer_char;
//           return false;
//         }
//       }
//       while (isspace(json_file.peek())) { json_file.get(); }
//       if ((buffer_char = json_file.get()) != '{') {
//         LOG(ERROR) << "Improper JSON file provided, expected {, found " << buffer_char;
//         return false;
//       }
//       while (json_file >> buffer_str) {

//       }
//     } else {
//       LOG(ERROR) << "Improper JSON file provided, expected simulation, found " << buffer_str;
//       return false;
//     }
//     return true;
//   }
// }
