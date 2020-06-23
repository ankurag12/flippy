#ifndef CONFIGREADER_H_
#define CONFIGREADER_H_

#include <string>

struct PidConfig {
  double p;
  double i;
  double d;
};

struct FilterConfig {
  double alpha;
};

class ConfigReader {
public:
  ConfigReader();
  PidConfig get_pid_gains(std::string cont_name);
  FilterConfig get_filter_weights(std::string filter_name);

private:
  std::string _default_config_file = "../config.txt";
};

#endif // CONFIGREADER_H_