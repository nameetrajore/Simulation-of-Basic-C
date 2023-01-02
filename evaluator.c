#include <math.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_TOKENS (100000)
#define MAX_LINE_LENGTH (100000)
#define MAX_VARIABLES (100000)
#define EXIT_SUCCESS 0

char tokens[MAX_TOKENS];
int num_tokens;
char *state;
int parse_equality(void);
void statements();
typedef struct map {
  char var_name[MAX_TOKENS];
  int value;
} map;

map variables[MAX_VARIABLES]; // to store all the defined variables

// Integer to String
char *itoa(int value, char *result, int base) {
  // check that the base if valid
  if (base < 2 || base > 36) {
    *result = '\0';
    return result;
  }

  char *ptr = result, *ptr1 = result, tmp_char;
  int tmp_value;

  do {
    tmp_value = value;
    value /= base;
    *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrst"
             "uvwxyz"[35 + (tmp_value - value * base)];
  } while (value);

  // Apply negative sign
  if (tmp_value < 0)
    *ptr++ = '-';
  *ptr-- = '\0';
  while (ptr1 < ptr) {
    tmp_char = *ptr;
    *ptr-- = *ptr1;
    *ptr1++ = tmp_char;
  }
  return result;
}

// checks if pre starts with str
int startsWith (char* pre, char * str){
  return strncmp(pre, str, strlen(pre)) == 0;
}

// checks if a particular statement is an assignment operator
int isAssignment(){
  char* copy_state = state;
/* printf("%s", state); */
  while(*copy_state!=';' && strlen(state)!=0 ){
    if(*copy_state=='=' && *(copy_state+1)!= '=')
      return 1;
    /* printf("hello \n"); */
    copy_state++;
  }
    return 0;
  }

//for specific isAssignment check
int isAssignment_for_loop(char* string){
  char* local_state = string;
  while(*local_state!=';' && strlen(local_state)!=0 ){
    if(*local_state=='=' && *(local_state+1)!= '=')
      return 1;
    local_state++;
  }
    return 0;
  }

int get_value_from_variable(char *var_name) {
  for (int i = 0; i < MAX_VARIABLES; i++) {
    if (strcmp(variables[i].var_name,var_name)==0)
      return variables[i].value;
  }
      printf("%s is not declared. \n", var_name);
      exit(0);
  return -1;
  }

int get_variable_index(char* var_name){
  for(int i=0;i<MAX_VARIABLES;i++){
    if(strcmp(variables[i].var_name, var_name)==0)
      return i;
  }

  return -1;
}

char* get_statement(char* destination, char* source){
  char* copy_state = source;
  char var_name [MAX_TOKENS] = "";

  /* copy_state++; */
  /* printf("copy state: %c state: %c", *copy_state, *state); */
  while(*copy_state!= EOF && *copy_state!=';' && *(copy_state+1)!='{' && strlen(copy_state)!=0 ){
    strncat(destination, copy_state, 1);
    /* printf("%s", var_name); */
    copy_state++;
  }

  return destination;
}

char* get_expression(char* destination, char* source){
  char* copy_state = source;
  char var_name [MAX_TOKENS] = "";

  /* copy_state++; */
  /* printf("copy state: %c state: %c", *copy_state, *state); */
  while(*copy_state!= EOF && *copy_state!=';'&& strlen(copy_state)!=0 ){
    strncat(destination, copy_state, 1);
    /* printf("%s", var_name); */
    copy_state++;

  }
  return destination;
}
char* get_for_statements(char* destination, char* source){
  char* copy_state = source;
  char var_name [MAX_TOKENS] = "";

  /* copy_state++; */
  /* printf("copy state: %c state: %c", *copy_state, *state); */
  while( *copy_state!= EOF && *copy_state!='}'){
    strncat(destination, copy_state, 1);
    /* printf("%s", var_name); */
    copy_state++;
  }

  while(*copy_state=='}' && *(copy_state+1)!='}'){
    strncat(destination, copy_state, 1);
    /* printf("%s", var_name); */
    copy_state++;
}
  return destination;
}

char* next_statement(){
  char* copy_state = state;
  /* printf("%s\n", state); */
  if(*copy_state==EOF)
    return copy_state;
  while(*copy_state!=';' && *copy_state!='}' && *(copy_state+1)!='{'){
    /* printf("copy_state: %c \n",*copy_state); */
    copy_state++;
  }
  if(*copy_state!=EOF)
    copy_state++;
  /* printf("copy state: %s\n" , copy_state); */
  /* printf("exiting copy_state: %c" ,*copy_state); */
  return copy_state;
}

char* next_statement_assignment(){
  char* copy_state = state;
  /* printf("%s\n", state); */
  if(*copy_state==EOF)
    return copy_state;
  while(*copy_state!=';'){
    /* printf("copy_state: %c \n",*copy_state); */
    copy_state++;
  }
  if(*copy_state!=EOF)
    copy_state++;
  /* printf("copy state: %s\n" , copy_state); */
  /* printf("exiting copy_state: %c" ,*copy_state); */
  return copy_state;
}
//Checks if state is a valid alphabet
int is_alphabet(void) { return  *state >= 'a' && *state <= 'z'; }

// Checks if state is a valid digit
int is_digit(void) { return *state >= '0' && *state <= '9'; }

// Get the digit value of state
int digit(void) { return *state - '0'; }

// Parses a number
int number(void) {
  int result = 0;
  while (is_digit()) {
    int n = digit();

    result *= 10;
    result += n;

    ++state;
  }
  return result;
}

// Parses a factor parenthesi and numbers
int parse_factor(void) {
  char var_name[MAX_TOKENS] = "";
  if (*state == '(') {
    ++state;
    int result = parse_equality();
    ++state; // eat )
    return result;
  } else if(is_digit()){
    return number();
    }
  else {
        /* printf(" in %s \n", var_name); */
      while(is_alphabet()){
        strncat(var_name, state, 1);
        state++;
      }
        /* printf(" out %s \n", var_name); */
      int variable_value = get_value_from_variable(var_name);
      return variable_value;
  }
}

// Parses * and /
int parse_mul(void) {

  int left = parse_factor();

  while (*state == '*' || *state == '/') {
    char op = *state++;
    int right = parse_factor();
    if (op == '*')
      left *= right;
    else
      left /= right;
  }

  return left;
}

// Parses + and -
int parse_add(void) {

  int left = parse_mul();
  while (*state == '+' || *state == '-') {
    char op = *state++;
    int right = parse_mul();

    if (op == '+')
      left += right;
    else
      left -= right;
  }
  return left;
}

// Parse <
int parse_inequality() {
  int left = parse_add();
  while (*state == '>') {
    state++;
    int right = parse_add();
    left = left > right;
  }
  return left;
}

// Parses ==
int parse_equality(void) {
  int left = parse_inequality();
  while (*state == '=' && *(state + 1) == '=') {
    state += 2;
    int right = parse_inequality();
    left = left == right;
  }

  return left;
}

// TODO: Include variables in expression
int evaluate_expression(char *string) {
  state = string;
  return parse_equality();
}

void tokenize(char *filename) {
  FILE *in_file = fopen(filename, "r");

  char line[MAX_TOKENS];
  while (fgets(line, MAX_LINE_LENGTH, in_file)) {
    for (int i = 0; i < strlen(line); ++i) {
      if (line[i] == ' ' || line[i] == '\t' || line[i] == 10 || line[i] == '\n')
        continue;
      tokens[num_tokens] = line[i];
      num_tokens++;
    }
  }
}

void assignment(){
  char expression [MAX_TOKENS] = "";
  /* printf(" hello %s", expression); */
  char var_name[MAX_TOKENS] = "";
  while(*state!='='){
        strncat(var_name, state, 1);
        state++;
      /* if(!is_alphabet()){ */
      /*   break; */
      /* } */
  }
  state++;
  char* copy_state = 0;
  get_expression(expression, state);
  int variable_index = get_variable_index(var_name);
  copy_state = state;
  variables[variable_index].value = evaluate_expression(expression);
  state = copy_state;
  state = next_statement_assignment();
  /* while(!is_alphabet()) */
  /*   state++; */
  /* printf("%s", state); */
  statements();
  }

// for specific assignment function
void assignment_for_loop(char* string){
  char expression [MAX_TOKENS] = "";
  char var_name[MAX_TOKENS] = "";
  char* local_state = string;
  while(*local_state!='='){
        strncat(var_name, local_state, 1);
        local_state++;
      /* if(!is_alphabet()){ */
      /*   break; */
      /* } */
  }
  // to skip '='
  local_state++;
  char* copy_state = 0;
  get_statement(expression, local_state);
  /* printf("hello"); */
  int variable_index = get_variable_index(var_name);
  variables[variable_index].value = evaluate_expression(expression);
}

void for_loop(){
  char expression1 [MAX_TOKENS] = "";
  char expression2 [MAX_TOKENS] = "";
  char expression3 [MAX_TOKENS] = "";
  char for_statements[MAX_TOKENS] = "";
  char* copy_state = 0;
  // skipping 'for('
  state+=4;
  copy_state = state;
/*   //parsing first statement of for loop */
  get_statement(expression1, state);
  if(isAssignment_for_loop(expression1)){
    assignment_for_loop(expression1);
  }
  else{
  evaluate_expression(expression1);
  }
  state = copy_state;
  state = next_statement();

/*   //parsing second statement of for loop */
  copy_state = state;
  get_statement(expression2, state);
  state = copy_state;
  state = next_statement();

/*   //parsing third statement */
  copy_state = state;
  get_statement(expression3, state);
  state = copy_state;
  state = next_statement();
  state+=1;
/* printf(" expression 1 %s\n", expression1); */
/* printf(" expression 2 %s\n", expression2); */
/* printf(" expression 3 %s\n", expression3); */
  get_for_statements(for_statements, state);
/* printf(" statements %s\n", for_statements); */
/* // skipping '){' */

 copy_state = state;
    /* printf("evaluate: %d",evaluate_expression(expression2)); */
  if(isAssignment_for_loop(expression3)){
  while(evaluate_expression(expression2)){
 state = for_statements;
  /* printf("hello %s\n", state); */
 /* printf("%s \n", state); */
    statements();
    assignment_for_loop(expression3);
  }
  }
 /*  else{ */
 /*    while(evaluate_expression(expression2)){ */
 /*      state = for_statements; */
 /*      statements(); */
 /*      evaluate_expression(expression3); */
 /*    } */
 /*  } */
  state = copy_state;
  state += strlen(for_statements);
  while(!is_alphabet() && strlen(state)!=0){
    state++;}
  /* printf("hello"); */
  statements();
}

void read(){
  char var_name[MAX_TOKENS] = "";
  while(startsWith("read", state)){
    state+=4;
    while(*state!=';'){
      while(is_alphabet()){
        strncat(var_name, state, 1);
        state++;
      }
      int variable_index = get_variable_index(var_name);
      /* printf("%d", variable_index); */
      scanf("%d", &variables[variable_index].value);
    }
    strcpy(var_name, "");
      if(*state==';') state++;
    }

  statements();
}

void write(){
  char var_name[MAX_TOKENS] = "";

  while(startsWith("write", state)){
      state+=5;
      if(is_alphabet()){
      while(is_alphabet()){
        strncat(var_name, state, 1);
        state++;
      }
    printf("%d \n",get_value_from_variable(var_name));
    strcpy(var_name, "");
      }
      else {
        int write_num = number();
printf("%d\n", write_num);
  }
    state++;
  }
  statements();
}

void statements(){
  /* printf("statements: %s \n", state); */
  if(*state!=EOF && *state!=';' && *state!='}'){
  if(isAssignment() && !startsWith("for(", state))
    assignment();
  else if(startsWith("read", state))
    read();
  else if(startsWith("write", state))
    write();
  else if(startsWith("for(", state)){
    for_loop();
  }
  else if(strlen(state)!=0){
    printf("Syntax Error: %s \n", state);
    exit(1);
  }
}
  }

void declare(char* string){
  state = string;
  char var_name[MAX_TOKENS] = "";
  int i =0;
  if(startsWith("int", string)){
    state+=3;
    while(*state!=';'){
      while(is_alphabet()){
        strncat(var_name, state, 1);
        state++;
      }
      if(*state==',') state++;
      strcpy(variables[i].var_name, var_name);
      i++;
      strcpy(var_name, "");
    }
  }
  if(i!=0){
  state++;
  }
  /* printf("%s", state); */
  statements();
}

void check_variables (char* string){
  state = string;
  for(int i=0;i<MAX_VARIABLES;i++){
    if(strlen(variables[i].var_name)==0)
      break;
    printf("var_name: %s \t value: %d\n", variables[i].var_name, variables[i].value);
  }
}



int main(int argc, char **argv) {

  if (argc < 2) {
    perror("Pass the name of the input file as the first parameter. e.g.: "
           "./simulator input.txt");
    exit(EXIT_FAILURE);
  }

  tokenize(argv[1]);
  /* printf("%d\n", evaluate_expression(tokens)); */
  declare(tokens);
  /* check_variables(tokens); */
  return 0;
}
