#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <stdbool.h>


#include "parser.h"

AT_COMMAND_DATA date;

static uint8_t Case = 0;

STATE_MACHINE_RETURN_VALUE at_command_parse(char* char_crt){

	static uint8_t Case = 0;
	uint8_t lines = 0;
	uint8_t line_size = 0;
	uint8_t data_index;
	//static uint8_t char_crt;
	int index_col = 0;
	//date.status = false;
	char *start=0;

	while(char_crt){

		if((lines >= AT_COMMAND_MAX_LINES) && (line_size >= AT_COMMAND_MAX_LINE_SIZE))
			{
				date.status = STATE_MACHINE_READY_WITH_ERROR;
				return date.status;
			}

		for(;;){	
		
			char c = *char_crt;
			switch(Case){	// Case -> starea
				
					case 0:
					{
						if(c == 13){		//CR -> incepe transmisia
							Case = 1;
							char_crt++;
						}
						else{
							date.status = STATE_MACHINE_READY_WITH_ERROR;
							return date.status;
						}
						break;
					}
					
					case 1:
					{
						if(c == 10){		//LF
							Case = 2;
							char_crt++;
						}
						else{
							date.status = STATE_MACHINE_READY_WITH_ERROR;
							return date.status;
						}
						break;
					}
					
					case 2:
					{
						if(c == 'O'){				
							Case = 3;
							char_crt++;
						}
						else if(c == 'E'){	
							Case = 7;
							char_crt++;
						}
						else if(c == '+'){	
							Case = 14;
							start = char_crt;
							char_crt++;
							line_size++;
						}
						else{
							date.status = STATE_MACHINE_READY_WITH_ERROR;
							return date.status;
						}
						break;
					}
					
					case 3:
					{
						if(c == 'K'){	
							Case = 4;
							char_crt++;
						}
						else{
							date.status = STATE_MACHINE_READY_WITH_ERROR;
							return date.status;
						}
						break;
					}
					
					case 4:
					{
						if(c == 13){		//CR
							Case = 5;
							char_crt++;
						}
						else{
							date.status = STATE_MACHINE_READY_WITH_ERROR;
							return date.status;
						}
						break;
					}
					
					case 5:
					{
						if(c == 10){		//LF
							Case = 6;
							char_crt++;
						}
						else{
							date.status = STATE_MACHINE_READY_WITH_ERROR;
							return date.status;
						}
						break;
					}
					
					case 6:	//STARE FINALA
					{
						date.status = STATE_MACHINE_READY_OK;
						return date.status;
					}
					
					case 7:
					{
						if(c == 'R'){	
							Case = 8;
							char_crt++;
						}
						else{
							date.status = STATE_MACHINE_READY_WITH_ERROR;
							return date.status;
						}
						break;
					}
					
					case 8:
					{
						if(c == 'R'){	
							Case = 9;
							char_crt++;
						}
						else{
							date.status = STATE_MACHINE_READY_WITH_ERROR;
							return date.status;
						}
						break;
					}
					
					case 9:
					{
						if(c == 'O'){	
							Case = 10;
							char_crt++;
						}
						else{
							date.status = STATE_MACHINE_READY_WITH_ERROR;
							return date.status;
						}
						break;
					}
					
					case 10:
					{
						if(c == 'R'){	
							Case = 11;
							char_crt++;
						}
						else{
							date.status = STATE_MACHINE_READY_WITH_ERROR;
							return date.status;
						}
						break;
					}
					
					case 11:
					{
						if(c == 13){		//CR
							Case = 12;
							char_crt++;
						}
						else{
							date.status = STATE_MACHINE_READY_WITH_ERROR;
							return date.status;
						}
						break;
					}
					
					case 12:
					{
						if(c == 10){		//LF
							Case = 13;
							char_crt++;
						}
						else{
							date.status = STATE_MACHINE_READY_WITH_ERROR;
							return date.status;
						}
						break;
					}
					
					case 13:	//STARE FINALA
					{
						date.status = STATE_MACHINE_READY_OK;
						return date.status;
					}
					
					case 14:
					{
						if((c > 31) && (c < 127)){
							//stocam char
							Case = 14;
							char_crt++;
							line_size++;
						}
						else if(c == 13){	//CR
							//terminator '\0'
							Case = 15;
							char_crt++;
						}
						else{
							date.status = STATE_MACHINE_READY_WITH_ERROR;
							return date.status;
						}
						break;
					}
					
					case 15:
					{
						if(c == 10){		//LF
							Case = 16;
							memset(date.data[data_index], '\0', AT_COMMAND_MAX_LINE_SIZE);
							memcpy(date.data[data_index++], start, line_size);
							printf("Copied: %s \n", date.data[data_index-1]);
							char_crt++;
							line_size++;
							lines++;
						}
						else{
							date.status = STATE_MACHINE_READY_WITH_ERROR;
							return date.status;
						}
						break;
					}
					
					case 16:
					{
						if(c == 13){		//CR
							Case = 17;
							char_crt++;
						}
						else if(c == '+'){
							Case = 14;
							start = char_crt;
							char_crt++;
							line_size++;
						}
						else{
							date.status = STATE_MACHINE_READY_WITH_ERROR;
							return date.status;
						}
						break;
					}
					
					case 17:
					{
						if(c == 10){		//LF
							Case = 18;
							date.status = STATE_MACHINE_READY_WITH_ERROR;
							return date.status;
						}
						break;
					}
					
					case 18:
					{
						if(c == 'E'){		
							Case = 7;
							char_crt++;
						}
						else if(c == 'O'){
							Case = 3;
							char_crt++;
						}
						else{
							date.status = STATE_MACHINE_READY_WITH_ERROR;
							return date.status;
						}
						break;
					}
			}

			date.line_count=lines;

		}

	}

	return date.status;

}