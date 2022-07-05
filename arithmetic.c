#include "s21_decimal.h"

void balance(s21_decimal* value_1, s21_decimal* value_2) {
    int deg_v1 = get_degree(*value_1);
    int deg_v2 = get_degree(*value_2);

    int min_deg = (deg_v1 < deg_v2) ? deg_v1 : deg_v2;
    int max_deg = (deg_v1 > deg_v2) ? deg_v1 : deg_v2;
    s21_decimal* min_val = (deg_v1 < deg_v2) ? value_1 : value_2;
    s21_decimal* max_val = (deg_v1 > deg_v2) ? value_1 : value_2;
    s21_decimal coef;
    s21_from_int_to_decimal(10, &coef);

    while (min_deg != max_deg) {
        if (!s21_mul_simple(*min_val, coef, min_val)) {
        min_deg++;
        set_degree(min_val, min_deg);
        } else {
        s21_bank_rounding(max_val, max_deg - min_deg);
        break;
        }
    }
}

// int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {}