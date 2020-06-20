#include "ConfigReader.h"
#include <fstream>
#include <sstream>
#include <string>

ConfigReader::ConfigReader() {}

PidConfig ConfigReader::get_pid_config(std::string cont_name) {
  std::ifstream config_file_stream(_default_config_file);
  std::string line;
  PidConfig pid_config;

  while (std::getline(config_file_stream, line)) {
    std::istringstream iss(line);
    std::string token;
    std::getline(iss, token, ' ');
    if (token == cont_name) {
      iss >> pid_config.p >> pid_config.i >> pid_config.d;
      break;
    }
  }
  return pid_config;
}