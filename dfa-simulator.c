#include <stdio.h>
#include <string.h>

#define maxchar 100
#define maxstate 50
#define maxalpha 75

const char msg_accepted[] = "ACCEPTED";
const char msg_declined[] = "DECLINED";
const char msg_err_eof[]    = "ERROR : Unexpected EOF\n";
const char msg_err_nq[]     = "ERROR : Wrong formatted number of states, integer expected \"%s\" found\n";
const char msg_err_na[]     = "ERROR : Wrong formatted number of transitions, integer expected \"%s\" found\n";
const char msg_err_nf[]     = "ERROR : Wrong formatted number of final states, integer expected \"%s\" found\n";
const char msg_err_trans[]  = "ERROR : Wrong formatted transitions, state index expected \"%s\" found\n";
const char msg_err_state[]  = "ERROR : Wrong formatted state index, integer expected \"%s\" found\n";
const char msg_err_outnq[]  = "ERROR : Out of bound state index, 0 until %d expected, %d found\n";
const char msg_err_outnf[]  = "ERROR : Out of bound final states, 1 until %d expected, %d found\n";
const char msg_err_alpha[]  = "No trasnition %s found\n";

struct DFA {
  int nq; /* number of states */
  int na; /* number of alphabets */
  int nf; /* number of final states */
  char tstate[maxstate][maxchar];
  char talpha[maxalpha][maxchar];
  int ttrans[maxstate][maxalpha];
  int start_state;
  int final_state[maxstate];
  /* 0 if not final state, 1 otherwise */
};

int checkDFA(FILE* fp);
/* return 1 if fp is correct DFA file, 0 otherwise */
int readFromFile(FILE* fp, char* out);
/* read from file fp, stored in out. return 0 if EOF, 1 otherwise */
void readDFA(FILE* fp, struct DFA* dfa);
/* read DFA from file */
void readStates(FILE* fp, int* nq, char tstate[maxstate][maxchar]);
/* read states from file */
void readAlphas(FILE* fp, int* na, char talpha[maxalpha][maxchar]);
/* read alphabet from file */
void readTranss(FILE* fp, int nq, int na, int ttrans[maxstate][maxalpha]);
/* read alphabet from file */
void readStartState(FILE* fp, int* start_state);
/* read start state from file */
void readFinalState(FILE* fp, int nq, int* nf, int final_state[maxstate]);
/* read final state from file */
int idxOfAlpha(struct DFA dfa, char input[maxchar]);
/* return the index of input in talpha, -1 if not found */
void runDFA(struct DFA dfa);
/* run dfa */
int readAlpha(struct DFA dfa, char input[maxchar]);
/* read state from input, return 0 if EOF, 1 otehrwise */
void writeState(struct DFA dfa, int idxState);
/* print idxState in dfa */
int nextState(struct DFA dfa, int state_now, int idxAlpha);
/* return result of transition from state_now with idxAlpha */
int toUnInt(char* str);
/* convert array of char to integer, return -1 if NaN */

int main(int argc, char* argv[]) {
  if (argc == 2) {
    FILE* fp;
    struct DFA dfa;
    int correct_dfa;

    fp = fopen(argv[1],"r");
    correct_dfa = checkDFA(fp);
    fclose(fp);

    if (correct_dfa) {
      fp = fopen(argv[1],"r");
      readDFA(fp, &dfa);
      runDFA(dfa);
      fclose(fp);
    } else {
      printf("DFA File not formatted correctly, terminating.\n");
    }

  } else {
    printf("wrong usage, ex:\n");
    printf("  ./dfa-sumlator graph.dfa\n");
  }
}

int checkDFA(FILE* fp) {
  char stemp[maxchar];
  int nq, na, nf, ss, t;
  int i, j;

  /* check states */
  if (!readFromFile(fp, stemp)) { printf(msg_err_eof); return 0; }
  nq = toUnInt(stemp);
  if (nq == -1) { printf(msg_err_nq, stemp);  return 0; }
  for (i=0; i<nq; i++) { if (!readFromFile(fp, stemp)) { printf(msg_err_eof); return 0; } }

  /* check alphas */
  if (!readFromFile(fp, stemp)) { printf(msg_err_eof); return 0; }
  na = toUnInt(stemp);
  if (na == -1) { printf(msg_err_na, stemp);  return 0; }
  for (i=0; i<na; i++) { if (!readFromFile(fp, stemp)) { printf(msg_err_eof); return 0; } }

  /* check transition tables */
  for (i=0; i<nq; i++) {
    for (j=0; j<na; j++) {
      if (!readFromFile(fp, stemp)) { printf(msg_err_eof); return 0; }
      t = toUnInt(stemp);
      if (t == -1)          { printf(msg_err_trans, stemp);   return 0; }
      if (t < 0 || t >= nq) { printf(msg_err_outnq, nq-1, t); return 0; }
    }
  }

  /* check start state */
  if (!readFromFile(fp, stemp)) { printf(msg_err_eof); return 0; }
  ss = toUnInt(stemp);
  if (ss == -1)           { printf(msg_err_state, stemp);   return 0; }
  if (ss < 0 || ss >= nq) { printf(msg_err_outnq, nq-1, ss); return 0; }

  /* check final state */
  if (!readFromFile(fp, stemp)) { printf(msg_err_eof); return 0; }
  nf = toUnInt(stemp);
  if (nf == -1)          { printf(msg_err_nf, stemp);     return 0; }
  if (nf < 1 || nf > nq) { printf(msg_err_outnf, nq, nf); return 0; }
  for (i=0; i<nf; i++) {
    if (!readFromFile(fp, stemp)) { printf(msg_err_eof); return 0; }
    t = toUnInt(stemp);
    if (t == -1)          { printf(msg_err_state, stemp); return 0; }
    if (t < 0 || t >= nq) { printf(msg_err_outnq, nq-1, t); return 0; }
  }

  return 1;
}

int readFromFile(FILE* fp, char* out) {
  char temp[maxchar];
  if (fscanf(fp, "%s", temp) == EOF) {
    return 0;
  } else {
    for (;;) {
      if (strcmp(temp, "#")) {
        break;
      } else {
        fscanf(fp, "%[^\n]", temp);
      }
      if (fscanf(fp, "%s", temp) == EOF) {
        return 0;
      }
    }
    strcpy(out, temp);
    return 1;
  }
}

int toUnInt(char* str) {
  int len = strlen(str);
  int out = 0;
  int success = 1;
  int i;

  for (i=0; i<len; i++) {
    out *= 10;
    out += (str[i] - '0');
    if (str[i] <'0' || str[i] > '9') {
      success = 0;
    }
  }
  if (success) {
    return out;
  } else {
    return -1;
  }
}

void readDFA(FILE* fp, struct DFA* dfa) {
  readStates(fp, &((*dfa).nq), (*dfa).tstate);
  readAlphas(fp, &((*dfa).na), (*dfa).talpha);
  readTranss(fp, (*dfa).nq, (*dfa).na, (*dfa).ttrans);
  readStartState(fp, &((*dfa).start_state));
  readFinalState(fp, (*dfa).nq, &((*dfa).nf), (*dfa).final_state);
}

void readStates(FILE* fp, int* nq, char tstate[maxstate][maxchar]) {
  int i;
  char stemp[maxchar];

  readFromFile(fp, stemp);
  *nq = toUnInt(stemp);
  for(i=0; i<*nq; i++){
    readFromFile(fp, stemp);
    strcpy(tstate[i], stemp);
  }
}

void readAlphas(FILE* fp, int* na, char talpha[maxalpha][maxchar]) {
  int i;
  char stemp[maxchar];

  readFromFile(fp, stemp);
  *na = toUnInt(stemp);
  for(i=0; i<*na; i++){
    readFromFile(fp, stemp);
    strcpy(talpha[i], stemp);
  }
}

void readTranss(FILE* fp, int nq, int na, int ttrans[maxstate][maxalpha]) {
  int i,j;
  char stemp[maxchar];

  for (i=0; i<nq; i++){
    for (j=0; j<na; j++) {
      readFromFile(fp, stemp);
      ttrans[i][j] = toUnInt(stemp);
    }
  }
}

void readStartState(FILE* fp, int* start_state) {
  char stemp[maxchar];

  readFromFile(fp, stemp);
  *start_state = toUnInt(stemp);
}

void readFinalState(FILE* fp, int nq, int* nf, int final_state[maxstate]) {
  char stemp[maxchar];
  int i;

  readFromFile(fp, stemp);
  *nf = toUnInt(stemp);

  for (i=0; i<nq; i++) {
    final_state[i] = 0;
  }

  for (i=0; i<*nf; i++) {
    readFromFile(fp, stemp);
    final_state[toUnInt(stemp)] = 1;
  }
}

int idxOfAlpha(struct DFA dfa, char input[maxchar]) {
  int found = -1;
  int i = 0;

  while (found == -1 && i < dfa.na) {
    if (!strcmp(dfa.talpha[i], input)) {
      found = i;
    }
    i++;
  }

  return found;
}

void runDFA(struct DFA dfa) {

  char input[maxchar];
  int state_now;
  int idxAlpha;
  
  state_now = dfa.start_state;
  writeState(dfa, state_now);
  while (readAlpha(dfa, input)) {
    idxAlpha = idxOfAlpha(dfa, input);
    state_now = nextState(dfa, state_now, idxAlpha);
    writeState(dfa, state_now);
  }

  if (dfa.final_state[state_now]) {
    printf("%s\n", msg_accepted);
  } else {
    printf("%s\n", msg_declined);
  }
}

int readAlpha(struct DFA dfa, char input[maxchar]) {
  printf("> ");
  for (;;) {
    if (scanf("%s",input) == EOF) {
      return 0;
    } else if (idxOfAlpha(dfa, input) == -1) {
      printf(msg_err_alpha, input);
      printf("> ");
    } else {
      break;
    }
  }
  return 1;
}

void writeState(struct DFA dfa, int idxState) {

  printf("Current state : %s\n", dfa.tstate[idxState]);
}

int nextState(struct DFA dfa, int state_now, int idxAlpha) {

  return dfa.ttrans[state_now][idxAlpha];
}