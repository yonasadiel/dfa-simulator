#include <stdio.h>
#include <string.h>
#define boolean unsigned char
#define true 1
#define false 0

char dx[100][100],dy[100][100];
int mat[100][100];
int n,m;
int undef = -1;

void GetIdx(char now[100],char prev[100],int *idx, int *idy);
/*void Read_Comment();*/

int main(){
    
    FILE *fp = fopen("tubes.txt","r");
	int i,j;
    char ss[100],dummy;

    /*Read the specification*/
    /*Read_Comment();*/

    /*read the state*/
    fscanf(fp,"%d",&n);
    for(i=0;i<n;i++){
    	fscanf(fp,"%s",ss);
    	strcpy(dx[i],ss); /*Array for storing the name of states*/
    }

    /*read alphabet*/
    fscanf(fp,"%d",&m);
    for(i=0;i<m;i++){
    	fscanf(fp,"%s",ss);
    	strcpy(dy[i],ss); /*Array for storing the name of transition*/
    }
    /*read dfa-table*/
    for(i=0;i<n;i++){
    	for(j=0;j<m;j++){
    		fscanf(fp,"%d",&mat[i][j]);
    	}
    }
    int start_state,final_state;
    
    /*Read the start state*/
    fscanf(fp,"%d",&start_state);

    /*Read the final state*/
    boolean state_AC[105];
    for(i=0;i<100;i++) state_AC[i]=false;
    while(fscanf(fp,"%d",&final_state)!=EOF){
    	state_AC[final_state]=true;
    }
    
    char input[100],state_now[100];
    boolean accepted = true, first_input = true, second_input=true;
    int idx,idy;
    strcpy(state_now,dx[start_state]);
    while(accepted && scanf("%s",input)!=EOF){
    	/*General Case*/
    	/*get index for [state,input(transition)] */
    	GetIdx(state_now,input,&idx,&idy);
   		/*if the destination is not valid (value = undef), stop*/
   		if(mat[idx][idy]==undef){
   			accepted=false;
   		}
   		else{
   			/*jump into next state*/
   			strcpy(state_now,dx[mat[idx][idy]]);
   		}
   		if(accepted){
   			if(!first_input) printf(" ==> ");
   			printf("%s", state_now);
   		}
   		else
   			printf("there's no state from %s to %s\n",dx[idx],dy[idy]);
   		first_input=false;
    }
    printf("\n");

    if(accepted && state_AC[idx])
    	printf("YES\n");
    else
    	printf("NO\n");

    fclose(fp);
}


void GetIdx(char state_now[100],char input[100],int *idx, int *idy){
	int i;
	for(i=0;i<n;i++){
		if(strcmp(state_now,dx[i])==0){
			*idx=i;
		}
	}
	for(i=0;i<m;i++){
		if(strcmp(input,dy[i])==0){
			*idy=i;
		}
	}
}
/*
void Read_Comment(){
	boolean dump_sentence=true;
	char dummy;
	while(dump_sentence){
    	dummy = getc(fp);
    	if(dummy == '#'){
    		while(dummy!='\n'){
    			dummy = getc(fp);
    		}
    	}
    	else{
    		ungetc(dummy,fp);
    		dump_sentence=false;
    	}
    }
}*/