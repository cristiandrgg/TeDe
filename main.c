//de compilat: gcc -o tede main.c parser.c

#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "parser.h"

#define MAX 32768
char buffer[MAX];

void print_at_command_data(){

    printf("AT_COMMANDS read: \n");
    for(int i = 0 ;i < date.line_count; i++){
            printf("%s \n", date.data[i]);
    }
}

int main(int argc, char* argv[]) {
   
	//int  aux;
	//char ch;

    if( argc < 2 ){
        printf("\n Numar insuficient de argumente. \n");
    }
	else{

        FILE *f;
        f = fopen(argv[1], "rb");

        if (f == NULL) { 
			perror("\n Fisierul nu este valid. \n"); 
		}

        int n = fread(buffer, 1, MAX, f);
        buffer[n]='\0';
        int status=at_command_parse(buffer);
        print_at_command_data();

        if(status==2){
            printf("NOT OK! \n");
        }
        else if(status==1)
        {
            printf("OK! \n");
            
        }
    fclose(f);
    }
    return 0;
}