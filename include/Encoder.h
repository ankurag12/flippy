#ifndef ENCODER_H_
#define ENCODER_H_

void ISR_any_edge_left();
void ISR_any_edge_right();
void update_tick_count(volatile int *last_enc_ab_state,
                       volatile int *tick_count, int pin_a, int pin_b);
#endif // Encoder.h
