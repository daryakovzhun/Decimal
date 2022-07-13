#include "../s21_decimal.h"

int s21_add_simple(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
    initial_num(result);
    int rank = 0;
    for (int i = 0; i < 96; i++) {
        int bit_val1 = get_bit(value_1, i);
        int bit_val2 = get_bit(value_2, i);

        set_bit(result, i, bit_val1 ^ bit_val2 ^ rank);

        rank = (bit_val1 && bit_val2) || (bit_val1 && rank) || (bit_val2 && rank);
    }

    return rank;
}

void s21_sub_simple(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
    initial_num(result);
    for (int i = 0; i < 96; i++) {
        int bit_val1 = get_bit(value_1, i);
        int bit_val2 = get_bit(value_2, i);

        set_bit(result, i, bit_val1 ^ bit_val2);

        if (!bit_val1 && bit_val2) {
            int k = i + 1;
            while ((bit_val1 = get_bit(value_1, k)) != 1) {
                set_bit(&value_1, k, 1);
                k++;
            }
            set_bit(&value_1, k, 0);
        }
    }
}

int s21_mul_simple(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
    s21_decimal tmp;
    initial_num(&tmp);
    int is_owerfull = 0;
    for (int i = 0; i < 96 && !is_owerfull; i++) {
        if (get_bit(value_2, i) == 1) {
            s21_decimal temp = value_1;
            int k = 0;
            while (k < i) {
                if (shift_left(&temp)) {
                    is_owerfull = 1;
                    break;
                }
                k++;
            }
            if (is_owerfull || (is_owerfull = s21_add_simple(temp, tmp, &tmp))) {
                break;
            }
        }
    }

    if (!is_owerfull) *result = tmp;

  return is_owerfull;
}

s21_decimal s21_div_simple(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  // if (value_2.bits[0] + value_2.bits[1] + value_2.bits[2])
  //     return DEV_BY_ZERO;

    if (result) initial_num(result);
    s21_decimal fmod = {0};
    s21_decimal temp = {0};
    if (s21_is_greater_or_equal_simple(value_1, value_2)) set_bit(&temp, 0, 1);
    if (!s21_is_greater_simple(value_2, value_1)) {
        while (1) {
            s21_decimal copy_val_2 = value_2;
            while (s21_is_greater_or_equal_simple(value_1, copy_val_2) &&
                      !(get_bit(value_1, 95) && get_bit(copy_val_2, 95))) {
                shift_left(&copy_val_2);
                shift_left(&temp);
            }

            if (!(get_bit(value_1, 95) && get_bit(copy_val_2, 95)) ||
                    (s21_is_greater_or_equal_simple(copy_val_2, value_1))) {
                shift_right(&copy_val_2);
                shift_right(&temp);
            }

            s21_sub_simple(value_1, copy_val_2, &value_1);
            if (result) s21_add_simple(*result, temp, result);
            initial_num(&temp);
            set_bit(&temp, 0, 1);
            if (s21_is_less_simple(value_1, value_2)) {
                break;
            }
        }
    }
    fmod = value_1;
    return fmod;
}

// void ostatok(s21_decimal value1, s21_decimal value2, s21_decimal* res) {
//     s21_decimal zero = {0};
//     s21_decimal base = {0};
//     int i = 0;
//     s21_from_int_to_decimal(10, &base);
//     while (!s21_is_equal_simple(s21_div_simple(value1, value2, NULL), zero)) {
//         s21_mul_simple(value1, base, &value1);
//         s21_div_simple(value1, value2, res);
//         i++;
//         if (i == 28) break;
//     }
// }
