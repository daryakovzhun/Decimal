#include "s21_decimal.h"

int get_sign(s21_decimal number) {
  int sign;
  if ((number.bits[3] & (1 << 31)) == 0) {
    sign = 0;
  } else {
    sign = 1;
  }
  return sign;
}

void set_sign(s21_decimal* number, int sign) {
  if (sign == 0) {
    number->bits[3] = number->bits[3] & ~(1 << 31);
  } else {
    number->bits[3] = number->bits[3] | (1 << 31);
  }
}

int get_degree(s21_decimal number) {
  int degree_bin = 0, shift = 16, k = 1;
  while (shift <= 23) {
    int bit = ((number.bits[3] & (1 << shift)) == 0) ? 0 : 1;
    degree_bin += bit * k;
    k *= 10;
    shift++;
  }

  int degree = 0, i = 0;
  while (degree_bin > 0) {
    degree += degree_bin % 10 * pow(2, i);
    i++;
    degree_bin /= 10;
  }

  return degree;
}

void set_degree(s21_decimal* number, int degree) {
  int shift = 16;
  while (shift <= 23) {
    if (degree > 0 && degree % 2 == 1) {
      number->bits[3] = number->bits[3] | (1 << shift);
    } else {
      number->bits[3] = number->bits[3] & ~(1 << shift);
    }
    degree /= 2;
    shift++;
  }
}

void print_decimal_binary_top(s21_decimal number) {
  char line[256];
  int in = 0;
  for (int i = 3; i >= 0; i--) {
    int k = 31;
    while (k >= 0) {
      sprintf(line + in++, "%c", '0' + ((number.bits[i] & (1 << k)) != 0));
      k--;
    }
    sprintf(line + in++, "%c", '.');
  }
  char command[512] = "echo \"";
  strcat(command, line);
  strcat(command, "\" | python3 a.py\n");
  printf(command);
  system(command);
}

void print_decimal_binary(s21_decimal number) {
  for (int i = 3; i >= 0; i--) {
    int k = 31;
    while (k >= 0) {
      putchar('0' + ((number.bits[i] & (1 << k)) != 0));
      k--;
    }
    printf(".");
  }
  printf("\n");
}

int get_bit(s21_decimal number, int bit) {
  int sign;
  if ((number.bits[bit / 32] & (1 << bit % 32)) == 0) {
    sign = 0;
  } else {
    sign = 1;
  }
  return sign;
}

void set_bit(s21_decimal* number, int bit, int sign) {
  if (sign == 0) {
    number->bits[bit / 32] = number->bits[bit / 32] & ~(1 << bit % 32);
  } else {
    number->bits[bit / 32] = number->bits[bit / 32] | (1 << bit % 32);
  }
}

int shift_left(s21_decimal* number) {
  int is_overflow = get_bit(*number, 95);
  for (int i = 95; i >= 0; i--) {
    if (i != 0) {
      set_bit(number, i, get_bit(*number, i - 1));
    } else {
      set_bit(number, i, 0);
    }
  }
  return is_overflow;
}

int shift_right(s21_decimal* number) {
  int is_overflow = get_bit(*number, 1);
  for (int i = 0; i < 96; i++) {
    if (i != 95) {
      set_bit(number, i, get_bit(*number, i + 1));
    } else {
      set_bit(number, i, 0);
    }
  }
  return is_overflow;
}

void initial_num(s21_decimal* number) {
  for (int i = 0; i < 128; i++) {
    set_bit(number, i, 0);
  }
}

void s21_add_simple(s21_decimal value_1, s21_decimal value_2,
                    s21_decimal* result) {
  initial_num(result);
  int rank = 0;
  for (int i = 0; i < 96; i++) {
    int bit_val1 = get_bit(value_1, i);
    int bit_val2 = get_bit(value_2, i);

    set_bit(result, i, bit_val1 ^ bit_val2 ^ rank);

    rank = (bit_val1 && bit_val2) || (bit_val1 && rank) || (bit_val2 && rank);
  }
}

void s21_sub_simple(s21_decimal value_1, s21_decimal value_2,
                    s21_decimal* result) {
  initial_num(result);
  for (int i = 0; i < 96; i++) {
    int bit_val1 = get_bit(value_1, i);
    int bit_val2 = get_bit(value_2, i);

    set_bit(
        result, i,
        bit_val1 ^
            bit_val2);  // ((bit_val1 && !bit_val2) || (!bit_val1 && bit_val2))

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

// void s21_mul_simple(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
//   initial_num(result);
//   for (int i = 0; i < 96; i++) {
//     if (get_bit(value_2, i) == 1) {
//       s21_decimal temp = value_1;
//       int k = 0;
//       while (k < i) {
//         if (shift_left(&temp)) {
//           i = 96;
//           break;
//         }
//         k++;
//       }
//       s21_add_simple(temp, *result, result);
//     }
//   }
// }

void s21_mul_simple(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    initial_num(result);
    for (int i = 0; i < 96; i++) {
        if (get_bit(value_2, i) == 1) {
            s21_decimal temp = value_1;
            int k = 0;
            while (k < i) {
                if (shift_left(&temp)) {
                    i = 96;
                    break;
                }
                k++;
            }
            s21_add_simple(temp, *result, result);
        }
    }
}

s21_decimal s21_div_simple(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    // if (value_2.bits[0] + value_2.bits[1] + value_2.bits[2])
    //     return DEV_BY_ZERO;
    if(result) 
        initial_num(result);
    s21_decimal fmod = {0};
    s21_decimal temp = {0};
    if (s21_is_greater_or_equal_simple(value_1, value_2))
        set_bit(&temp, 0, 1);
    if (!s21_is_greater_simple(value_2, value_1)) {
        while(1) {
            s21_decimal copy_val_2 = value_2;
            while (s21_is_greater_or_equal_simple(value_1, copy_val_2)) {
                shift_left(&copy_val_2);
                shift_left(&temp);
            }
            shift_right(&copy_val_2);
            shift_right(&temp);
            s21_sub_simple(value_1, copy_val_2, &value_1);
            if (result) 
                s21_add_simple(*result, temp, result);
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

void s21_bank_rounding(s21_decimal *value, int count) {
    print_decimal_binary_top(*value);
    s21_decimal base = {0};
    s21_decimal res = {0};
    s21_decimal one = {0};
    s21_decimal two = {0};
    s21_from_int_to_decimal(10, &base);
    s21_from_int_to_decimal(1, &one);
    s21_from_int_to_decimal(2, &two);   
    s21_decimal two_res = {0};
    s21_decimal dec_mod = s21_div_simple(*value, base, NULL);
    if (dec_mod.bits[0] > 5) {
        s21_div_simple(*value, base, value);
        // print_decimal_binary_top(*value);
        s21_add_simple(*value, one, value);
    } else if (dec_mod.bits[0] == 5) {
        s21_div_simple(*value, base, value);
        two_res = s21_div_simple(*value, two, NULL);

        print_decimal_binary_top(two_res);
        if (s21_is_equal_simple(one, two_res))
            s21_add_simple(*value, one, value);         
        initial_num(&two_res);        
    }
    print_decimal_binary_top(*value);
}

int main() {
    s21_decimal num = {0}, num2 = {0}, res; 
    num.bits[0] = 115;
    num.bits[1] = 0;
    num.bits[2] = 0;
    num.bits[3] = 0;

    num2.bits[0] = 15;
    num2.bits[1] = 0;
    num2.bits[2] = 0;
    num2.bits[3] = 0;

    // set_degree(&num, 28);
    // printf("degree = %d\n", get_degree(num));
    // printf("num = %d", num.bits[3]);
    // set_bit(&num, 2, 0);
    // print_decimal_binary(num);
    // print_decimal_binary(num2);

    // printf("\n");
    // s21_div_simple(num, num2, &res);
    // // s21_mul_simple(num, num2, &res);
    // // shift_right(&num);
 
    // printf("div\n");
    // print_decimal_binary(res);
    // printf("fmod\n");
    // print_decimal_binary(s21_div_simple(num, num2, &res));
    s21_bank_rounding(&num, 1);
    // printf("\n%d", get_bit(num, 33));

    return 0;
}