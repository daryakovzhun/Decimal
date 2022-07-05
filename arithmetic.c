#include "s21_decimal.h"

void balance(s21_decimal* value_1, s21_decimal* value_2) {
  int deg_v1 = get_degree(*value_1);
  int deg_v2 = get_degree(*value_2);

  int min_deg = (deg_v1 < deg_v2) ? deg_v1 : deg_v2;
  int max_deg = (deg_v1 > deg_v2) ? deg_v1 : deg_v2;
  s21_decimal* min_val = (deg_v1 < deg_v2) ? value_1 : value_2;
  s21_decimal* max_val = (deg_v1 > deg_v2) ? value_1 : value_2;
  s21_decimal ten;
  s21_from_int_to_decimal(10, &ten);

  while (min_deg != max_deg) {
    if (!s21_mul_simple(*min_val, ten, min_val)) {
      min_deg++;
      set_degree(min_val, min_deg);
    } else {
      s21_bank_rounding(max_val, max_deg - min_deg);
      break;
    }
  }
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
    balance(&value_1, &value_2);
    int sign_1 = get_sign(value_1);
    int sign_2 = get_sign(value_2);

    if (!(sign_1 ^ sign_2)) {                         // value_1 > 0 && value_2 > 0 || value_1 < 0 && value_2 < 0
        s21_add_simple(value_1, value_2, result);
        set_sign(result, sign_1);
    } else if (!sign_1 && sign_2) {                   // value_1 > 0 && value_2 < 0
        if (s21_is_less_or_equal_simple(value_1, value_2)) {
            s21_sub_simple(value_2, value_1, result);  // value_1 <= value_2
            set_sign(result, 1);
        } else {
            s21_sub_simple(value_1, value_2, result);  // value_1 > value_2
            set_sign(result, 0);
        }
    } else {                                           // value_1 < 0 && value_2 > 0
        if (s21_is_less_or_equal_simple(value_2, value_1)) {
            s21_sub_simple(value_1, value_2, result);  // value_2 <= value_1
            set_sign(result, 1);
        } else {
            s21_sub_simple(value_2, value_1, result);  // value_2 > value_1
            set_sign(result, 0);
        }
    }

    set_degree(result, get_degree(value_1));
    
    return OK;
}