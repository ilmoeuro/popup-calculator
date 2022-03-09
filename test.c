#include <stdio.h>
#include <string.h>
#include <math.h>
#include "engine.h"

void test_creation_destruction(void) {
    calc_state_t *state = calc_new();
    calc_destroy(state);
    printf("test_creation_destruction() passed if no memory leaks.\n");
}

void test_sum(void) {
    calc_state_t *c = calc_new();
    const char *expected =
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                 100\n"
        "+                 50\n"
        "=                150\n"
        "                    \n"
        "                   0\n";
    
    calc_input(c, '1');
    calc_input(c, '0');
    calc_input(c, '0');
    calc_input(c, '+');
    calc_input(c, '5');
    calc_input(c, '0');
    calc_input(c, '=');

    if (strcmp(calc_output(c), expected) != 0) {
        printf("test_sum() failed. Expected:\n%sActual:\n%s",
                expected,
                calc_output(c));
    } else {
        printf("test_sum() passed.\n");
    }

    calc_destroy(c);
}

void test_diff(void) {
    calc_state_t *c = calc_new();
    const char *expected =
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                 100\n"
        "-                 50\n"
        "=                 50\n"
        "                    \n"
        "                   0\n";
    
    calc_input(c, '1');
    calc_input(c, '0');
    calc_input(c, '0');
    calc_input(c, '-');
    calc_input(c, '5');
    calc_input(c, '0');
    calc_input(c, '=');

    if (strcmp(calc_output(c), expected) != 0) {
        printf("test_diff() failed. Expected:\n%sActual:\n%s",
                expected,
                calc_output(c));
    } else {
        printf("test_diff() passed.\n");
    }

    calc_destroy(c);
}

void test_multiple_sum(void) {
    calc_state_t *c = calc_new();
    const char *expected =
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                 200\n"
        "+                100\n"
        "=                300\n"
        "+                 50\n"
        "=                350\n"
        "                    \n"
        "                   0\n";
    
    calc_input(c, '2');
    calc_input(c, '0');
    calc_input(c, '0');
    calc_input(c, '+');
    calc_input(c, '1');
    calc_input(c, '0');
    calc_input(c, '0');
    calc_input(c, '+');
    calc_input(c, '5');
    calc_input(c, '0');
    calc_input(c, '=');

    if (strcmp(calc_output(c), expected) != 0) {
        printf("test_multiple_sum() failed. Expected:\n%sActual:\n%s",
                expected,
                calc_output(c));
    } else {
        printf("test_multiple_sum() passed.\n");
    }

    calc_destroy(c);
}

void test_division_by_zero(void) {
    calc_state_t *c = calc_new();
    const char *expected =
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                   1\n"
        "/                  0\n"
        "=                inf\n"
        "                    \n"
        "                   0\n";
    calc_input(c, '1');
    calc_input(c, '/');
    calc_input(c, '0');
    calc_input(c, '=');

    if (strcmp(calc_output(c), expected) != 0) {
        printf("test_division_by_zero() failed. Expected:\n%sActual:\n%s",
                expected,
                calc_output(c));
    } else {
        printf("test_division_by_zero() passed.\n");
    }

    calc_destroy(c);
}

void test_big_numbers(void) {
    calc_state_t *c = calc_new();
    const char *expected =
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "              123456\n"
        "*              12345\n"
        "=         1524064320\n"
        "                    \n"
        "                   0\n";
    calc_input(c, '1');
    calc_input(c, '2');
    calc_input(c, '3');
    calc_input(c, '4');
    calc_input(c, '5');
    calc_input(c, '6');
    calc_input(c, '*');
    calc_input(c, '1');
    calc_input(c, '2');
    calc_input(c, '3');
    calc_input(c, '4');
    calc_input(c, '5');
    calc_input(c, '=');

    if (strcmp(calc_output(c), expected) != 0) {
        printf("test_big_numbers() failed. Expected:\n%sActual:\n%s",
                expected,
                calc_output(c));
    } else {
        printf("test_big_numbers() passed.\n");
    }

    calc_destroy(c);
}

void test_overflow(void) {
    calc_state_t *c = calc_new();
    const char *expected =
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "          1234567890\n"
        "*         1234567890\n"
        "=    1.524157875e+18\n"
        "                    \n"
        "                   0\n";
    calc_input(c, '1');
    calc_input(c, '2');
    calc_input(c, '3');
    calc_input(c, '4');
    calc_input(c, '5');
    calc_input(c, '6');
    calc_input(c, '7');
    calc_input(c, '8');
    calc_input(c, '9');
    calc_input(c, '0');
    calc_input(c, '*');
    calc_input(c, '1');
    calc_input(c, '2');
    calc_input(c, '3');
    calc_input(c, '4');
    calc_input(c, '5');
    calc_input(c, '6');
    calc_input(c, '7');
    calc_input(c, '8');
    calc_input(c, '9');
    calc_input(c, '0');
    calc_input(c, '=');

    if (strcmp(calc_output(c), expected) != 0) {
        printf("test_overflow() failed. Expected:\n%sActual:\n%s",
                expected,
                calc_output(c));
    } else {
        printf("test_overflow() passed.\n");
    }

    calc_destroy(c);
}


void test_floating_result(void) {
    calc_state_t *c = calc_new();
    const char *expected =
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                   1\n"
        "/                  3\n"
        "=      0.33333333333\n"
        "                    \n"
        "                   0\n";
    calc_input(c, '1');
    calc_input(c, '/');
    calc_input(c, '3');
    calc_input(c, '=');

    if (strcmp(calc_output(c), expected) != 0) {
        printf("test_floating_result() failed. Expected:\n%sActual:\n%s",
                expected,
                calc_output(c));
    } else {
        printf("test_floating_result() passed.\n");
    }

    calc_destroy(c);
}

void test_decimal_point(void) {
    calc_state_t *c = calc_new();
    const char *expected =
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                 1.5\n"
        "+                1.5\n"
        "=                  3\n"
        "                    \n"
        "                   0\n";
    calc_input(c, '1');
    calc_input(c, '.');
    calc_input(c, '5');
    calc_input(c, '+');
    calc_input(c, '1');
    calc_input(c, '.');
    calc_input(c, '5');
    calc_input(c, '=');

    if (strcmp(calc_output(c), expected) != 0) {
        printf("test_decimal_point() failed. Expected:\n%sActual:\n%s",
                expected,
                calc_output(c));
    } else {
        printf("test_decimal_point() passed.\n");
    }

    calc_destroy(c);
}

void test_decimal_point_2(void) {
    calc_state_t *c = calc_new();
    const char *expected =
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "               1.125\n"
        "+              1.125\n"
        "=               2.25\n"
        "                    \n"
        "                   0\n";
    calc_input(c, '1');
    calc_input(c, '.');
    calc_input(c, '1');
    calc_input(c, '2');
    calc_input(c, '5');
    calc_input(c, '+');
    calc_input(c, '1');
    calc_input(c, '.');
    calc_input(c, '1');
    calc_input(c, '2');
    calc_input(c, '5');
    calc_input(c, '=');

    if (strcmp(calc_output(c), expected) != 0) {
        printf("test_decimal_point_2() failed. Expected:\n%sActual:\n%s",
                expected,
                calc_output(c));
    } else {
        printf("test_decimal_point_2() passed.\n");
    }

    calc_destroy(c);
}

void test_change_operation(void) {
    calc_state_t *c = calc_new();
    const char *expected =
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                    \n"
        "                   1\n"
        "/                  5\n"
        "=                0.2\n"
        "                    \n"
        "                   0\n";
    calc_input(c, '1');
    calc_input(c, '+');
    calc_input(c, '-');
    calc_input(c, '*');
    calc_input(c, '/');
    calc_input(c, '5');
    calc_input(c, '=');

    if (strcmp(calc_output(c), expected) != 0) {
        printf("test_change_operation() failed. Expected:\n%sActual:\n%s",
                expected,
                calc_output(c));
    } else {
        printf("test_change_operation() passed.\n");
    }

    calc_destroy(c);
}

int main(void) {
    test_creation_destruction();
    test_sum();
    test_diff();
    test_multiple_sum();
    test_division_by_zero();
    test_floating_result();
    test_big_numbers();
    test_overflow();
    test_decimal_point();
    test_decimal_point_2();
    test_change_operation();
    return 0;
}
