#include "s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
    s21_decimal base = {0}, zero = {0}, one = {0};
    s21_from_int_to_decimal(10, &base);
    s21_from_int_to_decimal(0, &zero);
    s21_from_int_to_decimal(1, &one);
    int sign = get_sign(value);
    s21_truncate(value, result);

    if (sign == 1 && !s21_is_equal(value, *result)) {
        s21_add_simple(*result, one, result);
    }
    set_sign(result, sign);
    return OK;
}

int s21_round(s21_decimal value, s21_decimal *result) {
    int sign = get_sign(value);
    s21_decimal base = {0}, mul = {0}, one = {0};
    s21_decimal copy = value;
    s21_truncate(value, result);
    s21_from_int_to_decimal(10, &base);
    s21_from_int_to_decimal(1, &one);
    int degree = get_degree(value);

    if (degree != 0)
        degree--;
    while (degree != 0) {
        s21_div(value, base, &value);
        degree--;
    }
    mul = s21_div_simple(value, base, &value);
    if (mul.bits[0] >= 5 && !s21_is_equal(copy, *result)) {
        s21_add_simple(*result, one, result);
    }
    set_sign(result, sign);
    return OK;  // исправить return
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
    int sign = get_sign(value);
    int degree = get_degree(value);
    s21_decimal base = {0};
    *result = value;
    s21_from_int_to_decimal(10, &base);

    while (degree != 0) {
        s21_div_simple(*result, base, result);
        degree--;
    }
    set_sign(result, sign);
    return OK;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
    *result = value;
    set_sign(result, !get_sign(value));
    return OK;
}
