#include "s21_decimal.h"

void s21_bank_rounding(s21_decimal *value, int count) {
    int sign = get_sign(*value);    
    while (count > 0) {
        s21_decimal base = {0}, one = {0}, two = {0}, two_res = {0};
        s21_from_int_to_decimal(10, &base);
        s21_from_int_to_decimal(1, &one);
        s21_from_int_to_decimal(2, &two);
        s21_decimal dec_mod = s21_div_simple(*value, base, value);
        if (dec_mod.bits[0] > 5) {
            s21_add_simple(*value, one, value);
        } else if (dec_mod.bits[0] == 5) {
            two_res = s21_div_simple(*value, two, NULL);
        if (s21_is_equal_simple(one, two_res)) s21_add_simple(*value, one, value);
      // initial_num(&two_res);
    }
    // print_decimal_binary_top(*value);
    count--;
    }
    set_sign(value, sign);
}

int s21_floor(s21_decimal value, s21_decimal *result) {
    int sign = get_sign(value);    
    s21_decimal base = {0};
    s21_decimal one = {0};
    s21_from_int_to_decimal(10, &base);  
    s21_from_int_to_decimal(1, &one);
    s21_div_simple(value, base, result);
    if (sign == 1) {
        s21_add(*result, one, result);
    }
    set_sign(result, sign);
    // print_decimal_binary_top(*result);
    return 0; // исправить return 
}
int s21_round(s21_decimal value, s21_decimal *result) {
    int sign = get_sign(value);        
    s21_decimal base = {0}, one = {0}, two = {0}, two_res = {0};
    s21_from_int_to_decimal(10, &base);
    s21_from_int_to_decimal(1, &one);
    s21_from_int_to_decimal(2, &two);
    s21_decimal dec_mod = s21_div_simple(value, base, result);
    if (dec_mod.bits[0] >= 5) {
        s21_add_simple(*result, one, result);
    }
    set_sign(result, sign);
    return 0; // исправить return 
    // print_decimal_binary_top(*result);
}
int s21_truncate(s21_decimal value, s21_decimal *result) {
    s21_decimal one = {0};
    s21_from_int_to_decimal(1, &one);
    s21_div_simple(value, one, result);
}
// int s21_negate(s21_decimal value, s21_decimal *result) {

// }
