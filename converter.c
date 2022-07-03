#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
    initial_num(dst);
    dst->bits[0] = (src > 0) ? src : -src;
    set_sign(dst, src < 0);
    return OK;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
    
}

int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);