#include "s21_decimal.h"

void balance(s21_decimal* value_1, s21_decimal* value_2) {
    int deg_v1 = get_degree(*value_1);
    int deg_v2 = get_degree(*value_2);

    int v1 = value_1->bits[3];
    int v2 = value_2->bits[3];

    int min_deg = (deg_v1 < deg_v2) ? deg_v1 : deg_v2;
    int max_deg = (deg_v1 > deg_v2) ? deg_v1 : deg_v2;
    s21_decimal* min_val = (deg_v1 < deg_v2) ? value_1 : value_2;
    s21_decimal* max_val = (deg_v1 > deg_v2) ? value_1 : value_2;
    s21_decimal ten;
    s21_from_int_to_decimal(10, &ten);

    while (min_deg != max_deg) {
        if (min_deg < 28 && !s21_mul_simple(*min_val, ten, min_val)) {
            min_deg++;
            set_degree(min_val, min_deg);
        } else {
            s21_bank_rounding(max_val, max_deg - min_deg);
        break;
        }
    }

    value_1->bits[3] = v1;
    value_2->bits[3] = v2;
    set_degree(min_val, min_deg);
    set_degree(max_val, min_deg);
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
    int is_overfull = OK;

    balance(&value_1, &value_2);
    int exp = get_degree(value_1);
    int sign_1 = get_sign(value_1);
    int sign_2 = get_sign(value_2);

    if (!(sign_1 ^ sign_2)) {
        is_overfull = s21_add_simple(value_1, value_2, result);
        set_sign(result, sign_1);
        if (is_overfull && sign_1 && sign_2) {
            is_overfull = NUMBER_MIN_INF;
        }
    } else if (s21_is_less_or_equal_simple(value_1, value_2)) {
        s21_sub_simple(value_2, value_1, result);
        set_sign(result, sign_2);
    } else {
        s21_sub_simple(value_1, value_2, result);
        set_sign(result, sign_1);
    }

    if (is_overfull && get_degree(value_1)) {
        s21_bank_rounding(&value_1, 1);
        s21_bank_rounding(&value_2, 1);
        is_overfull = s21_add(value_1, value_2, result);
    } else {
        set_degree(result, exp);
    }

  return is_overfull;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int is_overfull = OK;
    set_sign(&value_2, !get_sign(value_2));
    is_overfull = s21_add(value_1, value_2, result);

    return is_overfull;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    s21_mul_simple(value_1, value_2, result);

    return OK;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    s21_decimal nul = {0};
    if (s21_is_equal(value_2, nul)) {
        return DEV_BY_ZERO;
    }
    s21_div_simple(value_1, value_2, result);

    return OK;
}

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    s21_decimal nul = {0};
    if (s21_is_equal(value_2, nul)) {
        return DEV_BY_ZERO;
    }
    *result = s21_div_simple(value_1, value_2, NULL);

    return OK;
}
