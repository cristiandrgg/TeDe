//https://github.com/cristiandrgg/TeDe

//de compilat: gcc -o tede main.c parser.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "parser.h"

#define MAX 32768
char buffer[MAX];

void reset(){
    date.nod = 0;
    date.line_count = 0;
}

void print_at_command_data(){

    printf("\n AT_COMMANDS read: \n");
    for(int i = 0; i < date.line_count; i++){
            printf(" %s \n", date.data[i]);
    }
}

int main(int argc, char* argv[]) {
   
    char c;
    char charr;
    bool x = true;

    int flags[20];
    int flag_coded[15] = {0,0,0,1,1,0,0,0,0,0,0,1,0,1,0};
    int i_flag;

    if(argc>=3){
        for(int i = 2; i < argc; i++) 
            flags[i-2]=atoi(argv[i]); 
    }
    else{
        for(int i = 0; i < 15; i++)
            flags[i]=flag_coded[i];
    }


    if(argc < 2){
        printf("\n Numar insuficient de argumente. \n");
    }
	else{

        FILE *f;
        f = fopen(argv[1], "rb");

        if (f == NULL) { 
			perror("\n Fisierul nu este valid. \n"); 
		}

        c = fgetc(f);
        x = true;
        reset();
        STATE_MACHINE_RETURN_VALUE status = at_command_parse(c, flags[i_flag]);

        /*
        int n = fread(buffer, 1, MAX, f);
        buffer[n]='\0';
        */

        //int status = at_command_parse(buffer);
        //print_at_command_data();

        /*
        if(status!=1){
            print_at_command_data();
            printf("\n NOT OK / ERROR \n");
        }
        else if(status==1)
        {
            print_at_command_data();
            printf("\n OK! \n");
        }
        */

        while(x){   
            if((c==EOF) || (charr==EOF)){
                x = false;
            }

            switch(status){

                case STATE_MACHINE_NOT_READY:
                { 
                    c = fgetc(f);
                    charr = c;
                    status = at_command_parse(c,  flags[i_flag]);
                    break;
                }

                case STATE_MACHINE_READY_OK:
                {
                    print_at_command_data();
                    printf("\n OK! \n");
                    
                    reset();
                    i_flag = i_flag + 1;
                    status=at_command_parse(charr, flags[i_flag]);
                    break;
                }
                case STATE_MACHINE_READY_WITH_ERROR:
                {
                    print_at_command_data();
                    printf("\n NOT OK / ERROR \n");
                    
                    reset();
                    status=at_command_parse(charr, flags[i_flag]);
                    i_flag = i_flag +1;
                    break;
                }
                case STATE_MACHINE_NOT_READY_WITH_ERROR:
                {
                    print_at_command_data();
                    printf("\n NOT OK / ERROR \n");
                   
                    x=false;
                    break;
                }
                case STATE_MACHINE_LINES:
                {
                    print_at_command_data();
                    printf("\n S-a depasit nr de linii !! \n");
                    
                    reset();
                    status=at_command_parse(charr,flags[i_flag]);
                    break;
                }
            }
        }   

    fclose(f);
    }
    return 0;
}