#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "engine.h"

#define NUM_LINES 20
#define LINE_LENGTH 20

struct calc_state_s {
    char operation;
    double intermediate;
    double current;
    int comma_offset;
  
    struct {
        double num;
        char operation;
    } history[NUM_LINES];
};

/*
   Create a new calculator object.

   This function creates and returns a new
   empty calculator object, with all values
   initialized to zero and empty history.
   The initial state is '+'.

   Returns
   A new calculator object.
*/
calc_state_t *calc_new(void) {
    calc_state_t *result = malloc(sizeof(calc_state_t));

    result->operation = '+';
    result->intermediate = 0.0;
    result->current = 0.0;
    result->comma_offset = INT_MIN;

    for (int i=0; i<NUM_LINES; i++) {
        result->history[i].num = 0.0;
        result->history[i].operation = ' ';
    }

    return result;
}

/*
   Destroy an existing calculator object.
*/
void calc_destroy(calc_state_t *state) {
    free(state);
}

/*
   Execute the current operation and
   clear the screen for next operation.
*/
static void execute(calc_state_t *state) {
    /* Perform the calculation */
    switch (state->operation) {
    case '+':
        state->intermediate += state->current;
        break;
    case '-':
        state->intermediate -= state->current;
        break;
    case '*':
        state->intermediate *= state->current;
        break;
    case '/':
        state->intermediate /= state->current;
        break;
    default:
        fprintf(stderr, "Can not execute() non-operation.");
        exit(-1);
        break;
    }

    /* Show the calculation */
    int move = 2;
    if (state->history[NUM_LINES-1].operation == ' ') {
        move = 1;
    }

    for (int i=0; i<NUM_LINES-move; i++) {
        state->history[i].num = state->history[i+move].num;
        state->history[i].operation = state->history[i+move].operation;
    }

    if (move == 2) {
        state->history[NUM_LINES-2].num = state->current;
        state->history[NUM_LINES-2].operation = state->operation;

        state->history[NUM_LINES-1].num = state->intermediate;
        state->history[NUM_LINES-1].operation = '=';
    } else {
        state->history[NUM_LINES-1].num = state->current;
        state->history[NUM_LINES-1].operation = state->operation;
    }

    /* Clear for next calculation */
    state->current = 0.0;
    state->comma_offset = INT_MIN;
}

/*
   Show the result of the calculations
   on screen and prepare for new
   calculations.
*/
static void finalize(calc_state_t *state) {
    /* Show the final result */
    for (int i=0; i<NUM_LINES-1; i++) {
        state->history[i].num = state->history[i+1].num;
        state->history[i].operation = state->history[i+1].operation;
    }
    state->history[NUM_LINES-1].num = 0.0;
    state->history[NUM_LINES-1].operation = ' '; /* blank line */

    /* Clear for next calculation */
    state->intermediate = 0.0;
    state->operation = '+';
}

/*
   Modify the calculator state
   according to numeric input.
*/
static void process_digit_or_comma(calc_state_t *state, char input) {
    if (isdigit(input)) {
        if (state->comma_offset < 0) {
            state->current *= 10.0;
            state->current += (double)(input - '0');
        } else {
            double val = (double)(input - '0');
            val *= pow((double)10, (double)-state->comma_offset);
            state->current += val;
            state->comma_offset++;
        }
    } else {
        if (state->comma_offset < 0) {
            state->comma_offset = 1;
        }
    }
}

/*
   Feed input to the calculator.
   The input is fed one character at a time,
   and the character can be one of the
   following:

   0-9:     digits
   ,.:      decimal point (comma)
   +,-,*,/: operators
   =:       show result
*/
void calc_input(calc_state_t *state, char input) {
    if (isdigit(input) || input == '.' || input == ',') {
        process_digit_or_comma(state, input);
    } else if (input == '+' || input == '-'
            || input == '*' || input == '/') {
        if (state->current != 0.0) {
            execute(state);
        }
        state->operation = input;
    } else if (input == '=') {
        execute(state);
        finalize(state);
    }
}

/*
   Dump the output of the calculator
   into a string and return it.
   The string is statically allocated,
   so you don't have to free it.
*/
char *calc_output(calc_state_t *state) {
    static char output[(NUM_LINES+1)*(LINE_LENGTH+1)+1];

    output[0] = '\0';
    for (int i=0; i<NUM_LINES; i++) {
        if (state->history[i].operation == ' ') {
            sprintf(output, "%s                    \n", output);
        } else {
            /* Remove the "+" if it's in the first line or just after empty line */
            char operation = state->history[i].operation;
            if (operation == '+') {
                if (i == 0 || state->history[i-1].operation == ' ') {
                    operation = ' ';
                }
            }
            
            /* operation + sign + 11 decimal digits + "0." + "e-300" = 20 chars */
            sprintf(output, "%s%c%19.11g\n",
                    output,
                    operation,
                    state->history[i].num);
        }
    }
    char operation = state->operation;
    /* Remove the '+' if it's just after empty line */
    if (operation == '+' && state->history[NUM_LINES-1].operation == ' ') {
        operation = ' ';
    }
    if (state->comma_offset < 1) { 
        snprintf(output,
				sizeof output,
				"%s%c%19.11g\n",
                output,
                operation,
                state->current);
    } else if (state->comma_offset == 1) {
        snprintf(output,
				sizeof output,
				"%s%c%#19.0f\n",
                output,
                operation,
                state->current);
    } else {
		char format_string[25];
		int precision = state->comma_offset - 1;
		int num_digits = (int)log10(abs(state->current));
		if (precision > 11 - num_digits) {
			precision = 11 - num_digits;
		}
		snprintf(format_string, 25, "%%s%%c%%19.%df\n", precision);
		format_string[24] = '\0';
		snprintf(output,
				sizeof output,
				format_string,
				output,
				operation,
				state->current);
	}
    return output;
}
