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
// int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
//     int is_overfull = OK;
//     set_sign(&value_2, !get_sign(value_2));
//     is_overfull = s21_add(value_1, value_2, result);

//     return is_overfull;
// }

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
    // printf("SRC = \n");
    // print_decimal_binary(src);
    // printf("EXP = %d\n", get_degree(src));

    int exp = get_degree(src), sign;
    sign = (get_sign(src)) ? -1 : 1;
    s21_decimal ten;
    s21_from_int_to_decimal(10, &ten);
    while (exp > 0) {
        // print_decimal_binary_top(src);
        s21_div_simple(src, ten, &src);
        exp--;
    }
    // printf("\nSRC = \n");
    // print_decimal_binary(src);

    // for (int i = 0; i < 31; i++) {
    //     *dst += get_bit(src, i) * (int) pow(2, i);
    //     printf("%lf ", pow(2, i));
    // }

    set_bit(&src, 31, 0);
    *dst = src.bits[0];

    *dst *= sign;

    // *dst *= (get_sign(src)) ? -src.bits[0] : src.bits[0];
    // printf("dst = %d\t bit[0] = %d\n", *dst, src.bits[0]);
    // *dst *= src.bits[0];
    return OK;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
    double a = 0;
    s21_decimal whole = {0};
    s21_decimal res = {0};
    s21_truncate(src, &res);
    int sign = 0;
    for(int i = 0; i < 95; i++) {
        sign = get_bit(res, i);
        a += sign * pow(2, i);
    }
    s21_decimal float_part = {0};
    s21_sub(src, res, &float_part);
    long double f_part = 0;
    int count = 0;
    for(int i = 0; i < 95; i++) {
        sign = get_bit(float_part, i);
        count++;
        f_part += sign * pow(2, i);
    }
    int degree = (int)(ceil(log10(f_part)));
    printf("main_part = %lf\n", a);
    double dst1 = f_part/ pow(10, degree) + a;
    *dst = (float) dst1;
    printf("lalala = %lf\n", dst1);
    printf("res_float = %f\n", *dst);
    float asdasd = 340216.561045;
    printf("A ETO CHTO %f\n", asdasd);
    // printf("finish = %lf\n", dst1 + a);
}