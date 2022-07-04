#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
    initial_num(dst);
    dst->bits[0] = (src >= 0) ? src : -src;
    set_sign(dst, src < 0);
    return OK;
}

int get_exp(int number) {
    int degree_bin = 0, shift = 23, k = 1;
    while (shift <= 30) {
        int bit = ((number & (1 << shift)) == 0) ? 0 : 1;
        degree_bin += bit * k;
        k *= 10;
        shift++;
    }

    int degree = 0, i = 0;
    while (degree_bin > 0) {
        degree += degree_bin % 10 * pow(2, i);
        i++;
        degree_bin /= 10;
    }

    return degree;
}

int get_bit_int(int number, int bit) {
    int sign;
    if ((number & (1 << bit)) == 0) {
        sign = 0;
    } else {
        sign = 1;
    }
    return sign;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
    initial_num(dst);
    set_sign(dst, src < 0);
    int src_int = *(int*)&src;
    
    int exp = get_exp(src_int) - 127;
    printf("exp = %d\n", exp);
    dst->bits[0] = 1;
    src -= pow(2, exp);
    while (exp > 0) {
        shift_left(dst);
        exp--;
    }
    


    // print_decimal_binary(*dst);

    return 0;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
    *dst = 0;
    *dst = (get_sign(src)) ? -src.bits[0]: src.bits[0];
    return OK;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst);