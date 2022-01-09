//https://github.com/cristiandrgg/TeDe

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

AT_COMMAND_DATA date;

static uint8_t Case = 0;

STATE_MACHINE_RETURN_VALUE at_command_parse(char* char_crt,  uint8_t special_command){

	static uint8_t Case = 0;
	static uint32_t cnt = 0;
	uint8_t lines = 0;
	uint8_t line_size = 0;

	int index_col = 0;
	char *start=0;

	char_crt = char_crt + cnt;

	while(*char_crt != '\0'){

		if((line_size >= AT_COMMAND_MAX_LINE_SIZE) || (lines >= AT_COMMAND_MAX_LINES))
				return STATE_MACHINE_READY_WITH_ERROR;
			
		for(;;){
		
			char c = *char_crt;

			switch(Case){		//Case -> starea
				
					case 0:
					{
						if(c == 13){		//CR -> incepe transmisia
								Case = 1;
								char_crt++;
							}
						else{
								return STATE_MACHINE_READY_WITH_ERROR;
						}
							break;
					}
					
					case 1:
					{
						if(c == 10){			//LF
							 if(!special_command){
                                Case = 2;
                            }
						    else{
						        Case = 14;
                                start = char_crt + 1;
                            }
							char_crt++;
						}
						else{
							return STATE_MACHINE_READY_WITH_ERROR;
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
							char_crt++;
                            start = char_crt;
						}
						else{
							return STATE_MACHINE_READY_WITH_ERROR;
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
							return STATE_MACHINE_READY_WITH_ERROR;
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
							return STATE_MACHINE_READY_WITH_ERROR;
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
							return STATE_MACHINE_READY_WITH_ERROR;
						}
						break;
					}
					
					case 6:	
					{
					    Case = 0;
						return STATE_MACHINE_READY_OK;
					}
					
					case 7:
					{
						if(c == 'R'){
							Case = 8;
							char_crt++;
						}
						else{
							return STATE_MACHINE_READY_WITH_ERROR;
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
							return STATE_MACHINE_READY_WITH_ERROR;
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
							return STATE_MACHINE_READY_WITH_ERROR;
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
							return STATE_MACHINE_READY_WITH_ERROR;
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
							return STATE_MACHINE_READY_WITH_ERROR;
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
							return STATE_MACHINE_READY_WITH_ERROR;
						}
						break;
					}
					
					case 13:
					{
                        Case = 0;
						return STATE_MACHINE_READY_OK;
					}
					
					case 14:
					{
						if(c > 31 && c < 127){
							Case = 14;
							char_crt++;
							line_size++;
						}
						else if(c == 13){		//CR
							Case = 15;
							char_crt++;
						}
						else{
							return STATE_MACHINE_READY_WITH_ERROR;
						}
						break;
					}
					
					case 15:
					{
						if(c == 10){		//LF

							Case = 16;
                            char_crt++;

                            if (lines < AT_COMMAND_MAX_LINES){
                                memset(date.data[lines], '\0', AT_COMMAND_MAX_LINE_SIZE);
                                memcpy(date.data[lines], start, line_size);
                                lines++;
								line_size = 0;
                            }
                        }
						else
                        {
							return STATE_MACHINE_READY_WITH_ERROR;
						}
						break;
					}
					
					case 16:
					{
					    if(!special_command)
					    {
                            if(c == 13)		//CR
                            {
                                Case = 17;
                                char_crt++;
                            }
                            else if(c == '+'){
                                Case = 14;
                                char_crt++;
                                start = char_crt;
                            }
                            else
                            {
                                return STATE_MACHINE_READY_WITH_ERROR;
                            }
					    }
					    else
                        {
					        if(c == 13)		//CR
                            {
                                Case = 17;
                                char_crt++;
                            }
                        }

						break;
					}
					
					case 17:
					{
						if(c == 10){		//LF
							Case = 18;
                            char_crt++;
						}
						else
                        {
                            return STATE_MACHINE_READY_WITH_ERROR;
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
							return STATE_MACHINE_READY_WITH_ERROR;
						}
						break;
					}
					default:
                    {
                        return STATE_MACHINE_NOT_READY;
                    }
			}

			date.line_count = lines;
			cnt++;

		}

	}

	return 0;

}
	

