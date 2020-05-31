#ifndef ENCODER_H_
#define ENCODER_H_

volatile int g_tick_count_left = 0;
volatile int g_tick_count_right = 0;

enum EdgeType {
    RISING_EDGE_A = 0,
    RISING_EDGE_B = 1,
    FALLING_EDGE_A = 2,
    FALLING_EDGE_B = 3,
    NONE = -1
};

EdgeType g_last_edge_type_left = EdgeType::NONE;
EdgeType g_last_edge_type_right = EdgeType::NONE;

void ISR_rising_edge_a_left();
void ISR_falling_edge_a_left();
void ISR_rising_edge_b_left();
void ISR_falling_edge_b_left();
void ISR_rising_edge_a_right();
void ISR_falling_edge_a_right();
void ISR_rising_edge_b_right();
void ISR_falling_edge_b_right();
void update_tick_count();

#endif    // Encoder.h