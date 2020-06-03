
#include "LSM6DS33.h"
#include "Motor.h"
#include <chrono>
#include <errno.h>
#include <iostream>
#include <signal.h>
#include <thread>

static volatile bool keep_running = true;

void siginterrupt_handler(int sig) { keep_running = false; }

int main() {
  signal(SIGINT, siginterrupt_handler);

  while (keep_running) {
  }

  return 0;
}
