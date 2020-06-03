#include "Encoder.h"
#include "HwImpl.h"
#include "Motor.h"
#include <iostream>

// Globals
volatile int g_tick_count_left = 0;
volatile int g_tick_count_right = 0;

volatile int g_last_enc_ab_state_left;
volatile int g_last_enc_ab_state_right;

static constexpr int quad_enc_increment_map[4][4] = {
    {0, -1, 1, 2}, {1, 0, 2, -1}, {-1, 2, 0, 1}, {2, 1, -1, 0}};
static int enc_ab_state;
static int increment;
static int host_id = init_gpio();

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

  enc_ab_state =
      (read_digital_io(host_id, pin_a) << 1) | read_digital_io(host_id, pin_b);
  increment = quad_enc_increment_map[*last_enc_ab_state][enc_ab_state];
  if (increment == 2) {
    std::cout << "WARNING: Skip in encoder!" << std::endl;
  } else {
    *tick_count += increment;
  }
  *last_enc_ab_state = enc_ab_state;
}
