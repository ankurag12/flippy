#ifndef CONFIGREADER_H_
#define CONFIGREADER_H_

#include <string>

struct PidConfig {
  double p;
  double i;
  double d;
};

class ConfigReader {
public:
  ConfigReader();
  PidConfig get_pid_config(std::string cont_name);

private:
  std::string _default_config_file = "../config.txt";
};

#endif // CONFIGREADER_H_