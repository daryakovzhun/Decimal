#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
    initial_num(dst);
    dst->bits[0] = (src >= 0) ? src : -src;
    set_sign(dst, src < 0);
    return OK;
}

int get_exp(int number) {
    int shift = 23;
    int degree = 0, i = 0;
    while (shift <= 30) {
        int bit = ((number & (1 << shift)) != 0);
        degree += bit * pow(2, i);
        shift++;
        i++;
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
    int* src_int = (int*)&src;
    
    int exp = get_exp(*src_int) - 127, temp_exp = exp;
    printf("exp = %d\n", exp);
    dst->bits[0] = 1;

    while (temp_exp > 0) {
        shift_left(dst);
        temp_exp--;
    }
    
    s21_decimal exp_d = *dst;
    printf("src = %f\n", src);

    int mantisa = 0;
    for (int i = 22; i >=0; i--) {
        if (get_bit_int(*src_int, i)) {
            s21_decimal temp = exp_d;
            for (int j = 0; j <= mantisa; j++) {
                shift_right(&temp);
            }
            printf("TEMP\n");
            print_decimal_binary(temp);
            s21_add_simple(*dst, temp, dst);
        }
        mantisa++;
    }

    printf("dst\n");
    print_decimal_binary(*dst);
    print_decimal_binary_top(*dst);

    return 0;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
    *dst = (get_sign(src)) ? -src.bits[0]: src.bits[0];
    return OK;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst);