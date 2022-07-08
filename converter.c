#include "s21_decimal.h"
#include "string.h"

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

int count_str_float(float src, char* str_src) {
    int count_str = 0, k = -1;
    char str[100];
    sprintf(str, "%.7f", src);

    for (int i = (int) strlen(str) - 1; i >= 0; i--) {
        if (str[i] == '0') {
            str[i] = '\0';
            continue;
        }
        if (str[i] == '.') {
            break;
        }
        count_str++;
    }

    for (size_t i = 0; i < strlen(str); i++) {
        if (str[i] == '.') {
            k = 0;
            continue;
        }
        if (k >= 0) {
            str_src[k] = str[i];
            k++;
        }
    }
    str_src[k] = '\0';

    return count_str;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
    char str_src[100];
    int count_str = count_str_float(src, str_src);
    // printf("str_src = %s\n", str_src);

    initial_num(dst);
    s21_from_int_to_decimal(1, dst);
    set_sign(dst, src < 0);
    int *src_int = (int *)&src;

    int exp = get_exp(*src_int) - 127, temp_exp = exp;

    while (temp_exp > 0) {
        shift_left(dst);
        temp_exp--;
    }

    s21_decimal exp_d = *dst, float_dst = {0}, ten;
    s21_from_int_to_decimal(10, &ten);

    int mantisa = 0;
    for (int i = 22; i >= 0; i--) {
        int bit_mantisa = get_bit_int(*src_int, i);
        if (bit_mantisa && exp > 0) {
            s21_decimal temp = exp_d;
            for (int j = 0; j <= mantisa; j++) {
              shift_right(&temp);
            }
            s21_add_simple(*dst, temp, dst);
        }
        mantisa++;
        exp--;
    }

    for (size_t i = 0; i < strlen(str_src); i++) {
        s21_decimal add;
        // printf("i = %d\n", str_src[i] - '0');
        s21_from_int_to_decimal(str_src[i] - '0', &add);

        s21_add_simple(float_dst, add, &float_dst);
        s21_mul_simple(float_dst, ten, &float_dst);
    }
    s21_div_simple(float_dst, ten, &float_dst);

    // printf("float_dst\n");
    // print_decimal_binary_top(float_dst);


    int temp = count_str;
    while (temp > 0) {
        s21_mul_simple(*dst, ten, dst);
        temp--;
    }

    // print_decimal_binary_top(*dst);

    s21_add_simple(*dst, float_dst, dst);

    set_degree(dst, count_str);

    // printf("dst\n");
    // print_decimal_binary_top(*dst);

    return 0;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
    if (!dst) {
        return ERROR_CONVERT;
    }

    int exp = get_degree(src), sign;
    sign = (get_sign(src)) ? -1 : 1;
    s21_decimal ten;
    s21_from_int_to_decimal(10, &ten);
    while (exp > 0) {
        s21_div_simple(src, ten, &src);
        exp--;
    }

    set_bit(&src, 31, 0);
    *dst = src.bits[0];
    *dst *= sign;

    // s21_decimal temp;
    // s21_truncate(src, &temp);
    // *dst = temp.bits[0];
    // *dst *= (get_sign(src)) ? -1 : 1;

    return OK;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst);
