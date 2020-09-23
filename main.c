#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**
 * @file
 */

/**
 * Вычисляет дискриминант квадратного уравнения
 * @param[in] at_x2 Коэффициент при x^2
 * @param[in] at_x Коэффициент при x
 * @param[in] at_const Свободный коэффициент
 * @return Дискриминант квадратного уравнение с такими коэфффициентами
 */
double count_discriminant (double at_x2, double at_x, double at_const) {
    return at_x * at_x - 4 * at_x2 * at_const;
}

/**
 * Возвращает модуль вещественного числа
 * @param[in] num Вещественное число
 * @return Абсолютное значение этого числа
 */
double d_abs(double num) {
    return num >= 0 ? num : -num;
}

/**
 * Проверяет, является ли вещественное число нулём с точностью 0.001
 * @param[in] num Вещественное число
 * @return 1, если число равно нулю, 0 в противном случае
 */
int is_zero(double num) {
    return d_abs(num) < 0.001;
}

/**
 * Проверяет, является ли вещественное число целым с точностью 0.001
 * @param[in] num Вещественное число
 * @return 1, если целое число, 0 в противном случае
 */
int is_int(double num) {
    return is_zero(num - (int) num);
}

/**
 * Решает линейное уравнение
 * @param[in] at_x Коэффициент при x
 * @param[in] at_const Свободный коэффициент
 * @return Массив вещественных чисел, на нулевой позиции количество решений (10, если бесконечно много решений), дальше -- эти решения
 */
double * solve_linear (double at_x, double at_const) {
    double *res = (double *) malloc(2 * sizeof(double));

    if (is_zero(at_x)) {
        if (at_const == 0) res[0] = 10;
        else        res[0] = 0;
    } else {
        res[0] = 1;
        res[1] = -at_const / at_x;
    }

    return res;
}

/**
 * Решает квадратное уравнение
 * @param[in] at_x2 Коэффициент при x^2
 * @param[in] at_x Коэффициент при x
 * @param[in] at_const Свободный коэффициент
 * @return Массив вещественных чисел, на нулевой позиции количество решений (10, если бесконечно много решений), дальше -- эти решения в порядке убывания
 */
double * solve_square (double at_x2, double at_x, double at_const) {
    if (is_zero(at_x2)) return solve_linear(at_x, at_const);

    double *res = (double *) malloc(3 * sizeof(double));
    double discriminant = count_discriminant(at_x2, at_x, at_const);

    if (is_zero(discriminant)) {
        res[0] = 1;
        res[1] = -at_x / (2 * at_x2);
    } else if (discriminant < 0) {
        res[0] = 0;
    } else {
        res[0] = 2;
        res[1] = (-at_x + sqrt(discriminant)) / (2 * at_x2);
        res[2] = (-at_x - sqrt(discriminant)) / (2 * at_x2);
    }
    return res;
}

/**
 * Записывает в буффер вещественное число в виде целого, если оно целое, и с точностью 0.001 в противном случае
 * @param[out] buff Ссылка на буффер, в который нужно записать число
 * @param[in] num Вещественное число, которое нужно записать
 * @return 1 в случае ошибки, 0 в противном случае
 */
int write_res(char * buff, double num) {
    if (buff == 0) return 1;

    if (is_int(num)) {
        sprintf(buff + strlen(buff), "%d", (int) num);
    } else {
        sprintf(buff + strlen(buff), "%.3lf", num);
    }

    return 0;
}

/**
 * Приводит решение квадратного уравнения в читабельный вид
 * @param[out] Ссылка на буффер, в который нужно записать ответ
 * @param[in] res Массив-решение квадратного уравнения, требования: первое число - количество решений, дальше - эти решения
 * @return 1 в случае ошибки, 0 в противном случае
 */
int print_solution(char *output, double *res) {
    if (output == 0 || res == 0) return 1;

    char *res_string = (char *) malloc(300 * sizeof(char));
    memset(res_string, 0, 100);

    switch ((int) res[0]) {
        case 0: {
            sprintf(res_string, "The equation has no solutions.");
            break;
        }
        case 1: {
            sprintf(res_string, "The equation has one solution: ");

            write_res(res_string, res[1]);

            break;
        }
        case 2: {
            sprintf(res_string, "The equation has two solutions: ");

            write_res(res_string, res[1]);

            sprintf(res_string + strlen(res_string), " and ");

            write_res(res_string, res[2]);
            break;
        }
        default: sprintf(res_string, "Any real x is a solution.");
    }

    sprintf(output, "%s", res_string);

    return 0;
}

/**
 * Единичный тест
 * @param at_x2 Коэффициент при x^2
 * @param at_x Коэффициент при x
 * @param at_const Свободный коэффициент
 * @param expected Ожидаемая строка вывода решения
 * @param resp Полученная строка вывода решения
 * @return 0, если тест пройден, 1, если ошибка в тесте, 2, если ошибка в данных
 */
int test(double at_x2, double at_x, double at_const, char *expected, char *resp) {
    if(expected == 0 || resp == 0) return 2;

    char * res_string = (char *) malloc(300 * sizeof(char));

    if (print_solution(res_string, solve_square(at_x2, at_x, at_const)) != 0) return 2;

    if (strcmp(res_string, expected) != 0) {
        sprintf(resp, "Error with %lf %lf %lf coefficients!\n Expected: %s\n Got: %s", at_x2, at_x, at_const, expected, res_string);
        return 1;
    }
    return 0;
}

/**
 * Тестировщик
 * @return Строку-ответ от тестировщика, если все тесты пройдены успешно, выводит "Everything is fine!"
 */
char * tester() {
    char * tester_response = (char *) malloc(300 * sizeof(char));
    memset(tester_response, 0, 100);

    if (test(0, 0, 0, "Any real x is a solution.", tester_response) != 0) {
        return tester_response;
    }

    if (test(0, 0, 1, "The equation has no solutions.", tester_response) != 0) {
        return tester_response;
    }

    if (test(0, 1, -1, "The equation has one solution: 1", tester_response) != 0) {
        return tester_response;
    }

    if (test(0, 1, 0, "The equation has one solution: 0", tester_response) != 0) {
        return tester_response;
    }

    if (test(1, -2, 1, "The equation has one solution: 1", tester_response) != 0) {
        return tester_response;
    }

    if (test(1, -2.0001, 1, "The equation has one solution: 1", tester_response) != 0) {
        return tester_response;
    }

    if (test(1, -6, 5, "The equation has two solutions: 5 and 1", tester_response) != 0) {
        return tester_response;
    }

    sprintf(tester_response, "Everything is fine!");
    return tester_response;
}

int main() {
//    Interactive mode:
/*
    double at_x2, at_x, at_count;
    printf("Enter the equation coefficients:\n");
    scanf("%lf %lf %lf", &at_x2, &at_x, &at_count);
    char * solver_answer = print_solution(solve_square(at_x2, at_x, at_count));
    printf("%s", solver_answer);
    free(solver_answer);
*/

//    Tester mode:

    char * tester_answer = tester();
    printf("%s", tester_answer);
    free(tester_answer);
    return 0;
}
