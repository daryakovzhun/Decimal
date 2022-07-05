#include "../s21_decimal.h"

int s21_is_less_simple(s21_decimal value_1, s21_decimal value_2) {
    int is_less = FALSE;
    for (int i = 95; i >= 0; i--) {
        if (get_bit(value_1, i) ^ get_bit(value_2, i)) {
            is_less = get_bit(value_2, i);
            break;
        }
    }

    return is_less;
}

int s21_is_equal_simple(s21_decimal value_1, s21_decimal value_2) {
    int is_equal = TRUE;
    for (int i = 95; i >= 0; i--) {
        if (get_bit(value_1, i) != get_bit(value_2, i)) {
            is_equal = FALSE;
            break;
        }
    }

    return is_equal;
}

int s21_is_less_or_equal_simple(s21_decimal value_1, s21_decimal value_2) {
  return s21_is_less_simple(value_1, value_2) ||
         s21_is_equal_simple(value_1, value_2);
}

int s21_is_greater_simple(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_less_or_equal_simple(value_1, value_2);
}

int s21_is_greater_or_equal_simple(s21_decimal value_1, s21_decimal value_2) {
  return s21_is_greater_simple(value_1, value_2) ||
         s21_is_equal_simple(value_1, value_2);
}

int s21_is_not_equal_simple(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_equal_simple(value_1, value_2);
}
