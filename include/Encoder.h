#ifndef ENCODER_H_
#define ENCODER_H_

enum EncoderEdgeType {
  RISING_EDGE_A = 0,
  RISING_EDGE_B = 1,
  FALLING_EDGE_A = 2,
  FALLING_EDGE_B = 3,
  NONE = -1
};

void ISR_rising_edge_a_left();
void ISR_falling_edge_a_left();
void ISR_rising_edge_b_left();
void ISR_falling_edge_b_left();
void ISR_rising_edge_a_right();
void ISR_falling_edge_a_right();
void ISR_rising_edge_b_right();
void ISR_falling_edge_b_right();
void update_tick_count(EncoderEdgeType current_edge_type,
                       volatile EncoderEdgeType *last_edge_type,
                       volatile int *tick_count);

#endif // Encoder.h