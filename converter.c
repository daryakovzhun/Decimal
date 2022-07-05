#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  initial_num(dst);
  dst->bits[0] = (src >= 0) ? src : -src;
  set_sign(dst, src < 0);
  return OK;
}

int get_exp(int number) {
  int shift = 23;
  int degree = 0, i = 0;
  while (shift <= 30) {
    int bit = ((number & (1 << shift)) != 0);
    degree += bit * pow(2, i);
    shift++;
    i++;
  }

  return degree;
}

int get_bit_int(int number, int bit) {
  int sign;
  if ((number & (1 << bit)) == 0) {
    sign = 0;
  } else {
    sign = 1;
  }
  return sign;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  initial_num(dst);
  s21_from_int_to_decimal(1, dst);
  set_sign(dst, src < 0);
  int *src_int = (int *)&src;

  int exp = get_exp(*src_int) - 127, temp_exp = exp;

  while (temp_exp > 0) {
    shift_left(dst);
    temp_exp--;
  }

  s21_decimal exp_d = *dst, float_dst = {0}, ten;
  s21_from_int_to_decimal(10, &ten);

  long double interger_src;
  long double float_src = modfl(src, &interger_src);
  int mantisa = 0, count_ten = 0;
  for (int i = 22; i >= 0; i--) {
    int bit_mantisa = get_bit_int(*src_int, i);
    if (bit_mantisa && exp > 0) {
      s21_decimal temp = exp_d;
      for (int j = 0; j <= mantisa; j++) {
        shift_right(&temp);
      }
      s21_add_simple(*dst, temp, dst);
    } else if (bit_mantisa) {
      float_src = modfl(float_src * 10, &interger_src);
    //   printf("ffff = %Lf\n", float_src);
      s21_decimal add;
      s21_from_int_to_decimal((int)interger_src, &add);

      s21_add_simple(float_dst, add, &float_dst);
      s21_mul_simple(float_dst, ten, &float_dst);

      // s21_decimal div;
      // s21_from_int_to_decimal(1, &div);
      // for (int j = 0; j <= mantisa; j++) {
      //     shift_left(&div);
      // }
      // print_decimal_binary_top(exp_d);
      // print_decimal_binary_top(div);
      // ostatok(exp_d, div, &div);
      // print_decimal_binary_top(div);
      // printf("\n");
      // s21_add_simple(float_dst, div, &float_dst);

      count_ten++;
    }
    mantisa++;
    exp--;
  }
  printf("float_dst\n");
  print_decimal_binary_top(float_dst);

  print_decimal_binary_top(*dst);

  int temp = count_ten;
  while (count_ten >= 0) {
    s21_mul_simple(*dst, ten, dst);
    count_ten--;
  }
  print_decimal_binary_top(*dst);

  s21_add_simple(*dst, float_dst, dst);
  set_degree(dst, temp);

  print_decimal_binary_top(float_dst);
  printf("dst\n");
  print_decimal_binary_top(*dst);
  printf("degree = %d\n", get_degree(*dst));

  return 0;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  *dst = (get_sign(src)) ? -src.bits[0] : src.bits[0];
  return OK;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst);