typedef struct calc_state_s calc_state_t;

calc_state_t *calc_new(void);
void calc_destroy(calc_state_t *);
void calc_input(calc_state_t *, char);
char *calc_output(calc_state_t *);
