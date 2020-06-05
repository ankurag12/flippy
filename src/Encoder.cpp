#include "Encoder.h"
#include <iostream>

// Globals
volatile int g_tick_count_left = 0;
volatile int g_tick_count_right = 0;

volatile EncoderEdgeType g_last_edge_type_left = EncoderEdgeType::NONE;
volatile EncoderEdgeType g_last_edge_type_right = EncoderEdgeType::NONE;

// ISRs cannot be class methods as there is no `this` argument with ISR so there
// is no way to know what instance of the class the method belongs to
void ISR_rising_edge_a_left() {
  update_tick_count(EncoderEdgeType::RISING_EDGE_A, &g_last_edge_type_left,
                    &g_tick_count_left);
}

void ISR_falling_edge_a_left() {
  update_tick_count(EncoderEdgeType::FALLING_EDGE_A, &g_last_edge_type_left,
                    &g_tick_count_left);
}

void ISR_rising_edge_b_left() {
  update_tick_count(EncoderEdgeType::RISING_EDGE_B, &g_last_edge_type_left,
                    &g_tick_count_left);
}

void ISR_falling_edge_b_left() {
  update_tick_count(EncoderEdgeType::FALLING_EDGE_B, &g_last_edge_type_left,
                    &g_tick_count_left);
}

void ISR_rising_edge_a_right() {
  update_tick_count(EncoderEdgeType::RISING_EDGE_A, &g_last_edge_type_right,
                    &g_tick_count_right);
}

void ISR_falling_edge_a_right() {
  update_tick_count(EncoderEdgeType::FALLING_EDGE_A, &g_last_edge_type_right,
                    &g_tick_count_right);
}

void ISR_rising_edge_b_right() {
  update_tick_count(EncoderEdgeType::RISING_EDGE_B, &g_last_edge_type_right,
                    &g_tick_count_right);
}

void ISR_falling_edge_b_right() {
  update_tick_count(EncoderEdgeType::FALLING_EDGE_B, &g_last_edge_type_right,
                    &g_tick_count_right);
}

void update_tick_count(EncoderEdgeType current_edge_type,
                       volatile EncoderEdgeType *last_edge_type,
                       volatile int *tick_count) {

  if ((*last_edge_type + 1) % 4 == current_edge_type) {
    (*tick_count)++;
  } else if ((current_edge_type + 1) % 4 == *last_edge_type) {
    (*tick_count)--;
  } else if (*last_edge_type == EncoderEdgeType::NONE) {
    *tick_count = 0;
  } else {
    // Note that apart from actual errors due to skipping interrupts, a change
    // in direction of the motor could also produce this warning
    std::cout << "WARNING: Skip in encoder detected!" << std::endl;
  }
  *last_edge_type = current_edge_type;
}