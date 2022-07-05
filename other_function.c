#include "s21_decimal.h"

void s21_bank_rounding(s21_decimal *value, int count) {
    while (count > 0) {
       
        s21_decimal base = {0}, one = {0}, two = {0}, two_res = {0};
        s21_from_int_to_decimal(10, &base);
        s21_from_int_to_decimal(1, &one);
        s21_from_int_to_decimal(2, &two);   
        s21_decimal dec_mod = s21_div_simple(*value, base, NULL);
        //  print_decimal_binary_top(dec_mod);
        if (dec_mod.bits[0] > 5) {
            s21_div_simple(*value, base, value);
            // print_decimal_binary_top(*value);
            s21_add_simple(*value, one, value);
        } else if (dec_mod.bits[0] == 5) {
            s21_div_simple(*value, base, value);
            two_res = s21_div_simple(*value, two, NULL);
            if (s21_is_equal_simple(one, two_res))
                s21_add_simple(*value, one, value);         
            // initial_num(&two_res);        
        }
        print_decimal_binary_top(*value);
        count--;
    }
}