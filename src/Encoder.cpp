#include "Encoder.h"
#include "Motor.h"
#include <iostream>
#include <wiringPi.h>

// Globals
volatile int g_tick_count_left;
volatile int g_tick_count_right;

volatile int g_last_enc_ab_state_left = -1;
volatile int g_last_enc_ab_state_right = -1;

const int quad_enc_increment_map[4][4] = {
    {0, -1, 1, 2}, {1, 0, 2, -1}, {-1, 2, 0, 1}, {2, 1, -1, 0}};

// ISRs cannot be class methods as there is no `this` argument with ISR so there
// is no way to know what instance of the class the method belongs to
void ISR_any_edge_left() {
  update_tick_count(&g_last_enc_ab_state_left, &g_tick_count_left,
                    motor_pin_mapping.at(MotorSide::LEFT).enc_a,
                    motor_pin_mapping.at(MotorSide::LEFT).enc_b);
}

void ISR_any_edge_right() {
  update_tick_count(&g_last_enc_ab_state_right, &g_tick_count_right,
                    motor_pin_mapping.at(MotorSide::RIGHT).enc_a,
                    motor_pin_mapping.at(MotorSide::RIGHT).enc_b);
}

void update_tick_count(volatile int *last_enc_ab_state,
                       volatile int *tick_count, int pin_a, int pin_b) {

  int enc_a_state = digitalRead(pin_a);
  int enc_b_state = digitalRead(pin_b);
  int enc_ab_state = (enc_a_state << 1) | enc_b_state;
  if (*last_enc_ab_state == -1) {
    *tick_count = 0;
  } else {
    int increment = quad_enc_increment_map[*last_enc_ab_state][enc_ab_state];
    if (increment == 2) {
      std::cout << "WARNING: Skip in encoder detected!" << std::endl;
    } else {
      *tick_count += increment;
    }
  }
  *last_enc_ab_state = enc_ab_state;
}
