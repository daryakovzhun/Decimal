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
  int shift = 16;
  int degree = 0, i = 0;
  while (shift <= 23) {
    int bit = ((number.bits[3] & (1 << shift)) != 0);
    degree += bit * pow(2, i);
    shift++;
    i++;
  }

  if (degree > 28) {
    degree = 28;
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

void initial_num(s21_decimal* number) {
  for (int i = 0; i < 128; i++) {
    set_bit(number, i, 0);
  }
}

// int main() {
//   s21_decimal num, num2, res;
//   s21_from_int_to_decimal(5, &num);
//   s21_from_int_to_decimal(500, &num2);
//   set_degree(&num, 0);
//   set_degree(&num2, 2);
//   s21_add(num, num2, &res);
//   print_decimal_binary_top(res);
//   printf("exp = %d\n", get_degree(res));

// //   print_decimal_binary_top(num);
// //   print_decimal_binary_top(num2);

//   //   s21_from_int_to_decimal(1, &num);
//   //   s21_from_int_to_decimal(2, &num2);
//   //   float a = 5.41;
//   //   printf("float = %f\n", a);

//   //   s21_from_float_to_decimal(a, &num);
//   // ostatok(num, num2, &res);
//   // print_decimal_binary_top(res);

//   //   num.bits[0] = 465;
//   //   num.bits[1] = 0;
//   //   num.bits[2] = 0;
//   //   num.bits[3] = 1946531;

//   // // num2.bits[0] = 2;
//   // // num2.bits[1] = 0;
//   // // num2.bits[2] = 0;
//   // // num2.bits[3] = 0;

//   //   print_decimal_binary(num);

//   // print_decimal_binary(num2);
//   // s21_mul_simple(num, num2, &res);
//   //   shift_right(&num);
//   //   print_decimal_binary(num);
//   // print_decimal_binary(res);

//   // // printf("\n%d", get_bit(num, 33));

//   return 0;
// }