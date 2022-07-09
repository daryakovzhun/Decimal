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
    int count_str = 0, k = 1;
    char str[100];
    sprintf(str, "%f", src);

    for (int i = (int) strlen(str) - 1; i >= 0; i--) {
        if (str[i] == '0' && k == 1) {
            str[i] = '\0';
            continue;
        } else {
            k = -1;
        }
        if (str[i] == '.') {
            break;
        }
        count_str++;
    }

    strcpy(str_src, str);

    return count_str;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
    char str_src[100];
    int count_str = count_str_float(src, str_src), is_overfull = 0;
    initial_num(dst);

    s21_decimal ten;
    s21_from_int_to_decimal(10, &ten);

    for (size_t i = 0; i < strlen(str_src); i++) {
        if (str_src[i] != '.' && str_src[i] != '-') {
            s21_decimal add;
            s21_from_int_to_decimal(str_src[i] - '0', &add);
            s21_add_simple(*dst, add, dst);
            is_overfull = s21_mul_simple(*dst, ten, dst);
        }
    }

    if (!is_overfull) {
        s21_div_simple(*dst, ten, dst);
    }

    set_sign(dst, src < 0);
    set_degree(dst, count_str);

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

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
    int minus = get_sign(src);
    long double a = 0;
    s21_decimal res = {0}, ten = {0};
    s21_from_int_to_decimal(10, &ten);
    s21_truncate(src, &res);
    int sign = 0;
    for(int i = 0; i < 96; i++) {
        sign = get_bit(res, i);
        a += sign * pow(2, i);
    }
    s21_decimal float_part = {0};
    s21_sub(src, res, &float_part);
    int degree = get_degree(src);
    long double f_part = 0;
    int count = 0;
    for(int i = 0; i < 96; i++) {
        sign = get_bit(float_part, i);
        count++;
        f_part += sign * pow(2, i);
    }
    *dst = f_part / pow(10, degree) + a;
    if (minus == 1) {
        *dst = -(*dst);
    }
}
