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

void s21_add_simple(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int rank = 0;
    for (int i = 0; i < 95; i++) {
        int bit_val1 = get_bit(value_1, i);
        int bit_val2 = get_bit(value_2, i);

        set_bit(result, i, bit_val1 ^ bit_val2 ^ rank);

        rank = (bit_val1 && bit_val2) || (bit_val1 && rank) || (bit_val2 && rank);
    }
}

void s21_sub_simple(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    for (int i = 0; i < 95; i++) {
        int bit_val1 = get_bit(value_1, i);
        int bit_val2 = get_bit(value_2, i);

        set_bit(result, i, ((bit_val1 && !bit_val2) || (!bit_val1 && bit_val2)));

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

void s21_mul_simple(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    for (int i = 0; i < 95; i++) {
        int bit_val1 = get_bit(value_2, i);
        int bit_val2 = get_bit(value_2, i);

        s21_decimal temp;

    }
}

int main() {
    s21_decimal num = {0}, num2 = {0}, res = {0}; 
    num.bits[0] = 0;
    num.bits[1] = 0;
    num.bits[2] = 1;
    num.bits[3] = 0;

    num2.bits[0] = 2;
    num2.bits[1] = 0;
    num2.bits[2] = 0;
    num2.bits[3] = 0;
    // set_degree(&num, 28);
    // printf("degree = %d\n", get_degree(num));

    // printf("num = %d", num.bits[3]);
    // set_bit(&num, 2, 0);


    print_decimal_binary(num);
    print_decimal_binary(num2);
    s21_sub_simple(num, num2, &res);
    print_decimal_binary(res);

    // printf("\n%d", get_bit(num, 33));

    return 0;
}