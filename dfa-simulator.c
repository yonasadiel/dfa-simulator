#include <stdio.h>
#include <string.h>
#define boolean unsigned char
#define true 1
#define false 0

#define maxchar 100
#define maxstate 50
#define maxalpha 75
#define undef -1
int mat[maxchar][maxchar];

void GetIdx(int nq, int na, char now[maxchar],char prev[maxchar],int *idx, int *idy);
/* get index for now and prev state */
int readFromFile(FILE* fp, char* out);
/* read from file fp, stored in out. return 0 if EOF, 1 otherwise */
void readStates(FILE* fp, int* nq, char tstate[maxstate][maxchar]);
/* read states from file */
void readAlphas(FILE* fp, int* na, char talpha[maxalpha][maxchar]);
/* read alphabet from file */
void readTranss(FILE* fp, int nq, int na, int ttrans[maxstate][maxalpha]);
/* read alphabet from file */
int toUnInt(char* str);
/* convert array of char to integer, return -1 if NaN */

int main(int argc, char* argv[]) {
  if (argc == 2) {
    FILE* fp;
    int nq, na;
    char tstate[maxstate][maxchar];
    char talpha[maxalpha][maxchar];
    int  ttrans[maxstate][maxalpha];

    fp = fopen(argv[1],"r");

    readStates(fp, &nq, tstate);
    readAlphas(fp, &na, talpha);
    readTranss(fp, nq, na, ttrans);

    // int start_state,final_state;

    // /*Read the start state*/
    // fscanf(fp,"%d",&start_state);

    // /*Read the final state*/
    // boolean state_AC[105];
    // for(i=0;i<maxchar;i++) state_AC[i]=false;
    // while(fscanf(fp,"%d",&final_state)!=EOF){
    //   state_AC[final_state]=true;
    // }

    // char input[maxchar],state_now[maxchar];
    // boolean accepted = true, first_input = true, second_input=true;
    // int idx,idy;
    // strcpy(state_now,dx[start_state]);
    // while(accepted && scanf("%s",input)!=EOF){
    //   /*General Case*/
    //   get index for [state,input(transition)] 
    //   GetIdx(state_now,input,&idx,&idy);
    //   /*if the destination is not valid (value = undef), stop*/
    //   if (mat[idx][idy]==undef) {
    //     accepted=false;
    //   } else {
    //     /*jump into next state*/
    //     strcpy(state_now,dx[mat[idx][idy]]);
    //   }
    //   if (accepted){
    //     if (!first_input)  printf(" ==> ");
    //     printf("%s", state_now);
    //   } else {
    //     printf("there's no state from %s to %s\n",dx[idx],dy[idy]);
    //   }
    //   first_input=false;
    // }
    // printf("\n");

    // if (accepted && state_AC[idx]) {
    //   printf("YES\n");
    // } else {
    //   printf("NO\n");
    // }

    fclose(fp);
  } else {
    printf("wrong usage, ex:\n");
    printf("  ./dfa-sumlator graph.dfa\n");
  }
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
      fscanf(fp, "%s", temp);
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

// void GetIdx(int nq, int na, char state_now[maxchar], char input[maxchar],int *idx, int *idy){
//   int i;
//   for(i=0;i<nq;i++){
//     if(strcmp(state_now,dx[i])==0){
//       *idx=i;
//     }
//   }
//   for(i=0;i<na;i++){
//     if(strcmp(input,dy[i])==0){
//       *idy=i;
//     }
//   }
// }