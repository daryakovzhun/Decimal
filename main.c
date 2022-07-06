#include "s21_decimal.h"





int main() {
  s21_decimal num; //, num2, res;
//   s21_from_int_to_decimal(5, &num);
//   s21_from_int_to_decimal(500, &num2);
//   set_degree(&num, 0);
//   set_degree(&num2, 2);
//   s21_add(num, num2, &res);
//   print_decimal_binary_top(res);
//   printf("exp = %d\n", get_degree(res));

//   print_decimal_binary_top(num);
//   print_decimal_binary_top(num2);

  //   s21_from_int_to_decimal(1, &num);
  //   s21_from_int_to_decimal(2, &num2);
    float a = 5.52;
  //   printf("float = %f\n", a);

    s21_from_float_to_decimal(a, &num);
  // ostatok(num, num2, &res);
  print_decimal_binary_top(num);

  //   num.bits[0] = 465;
  //   num.bits[1] = 0;
  //   num.bits[2] = 0;
  //   num.bits[3] = 1946531;

  // // num2.bits[0] = 2;
  // // num2.bits[1] = 0;
  // // num2.bits[2] = 0;
  // // num2.bits[3] = 0;

  //   print_decimal_binary(num);

  // print_decimal_binary(num2);
  // s21_mul_simple(num, num2, &res);
  //   shift_right(&num);
  //   print_decimal_binary(num);
  // print_decimal_binary(res);

  // // printf("\n%d", get_bit(num, 33));

  return 0;
}