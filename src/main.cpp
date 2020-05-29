
#include <thread>
#include <errno.h>
#include <chrono>
#include <iostream>
#include "LSM6DS33.h"
#include "motor.h"
#include <signal.h>

static volatile bool keep_running = true;

void siginterrupt_handler(int sig) {
    keep_running = false;
    }

int main() {
    signal(SIGINT, siginterrupt_handler);
    
    while (keep_running) {
        
    }
    
    

    return 0;
}
