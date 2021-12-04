// https://github.com/cristiandrgg/TeDe

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define AT_COMMAND_MAX_LINES 100
#define AT_COMMAND_MAX_LINE_SIZE 100

typedef struct{
	uint8_t data[AT_COMMAND_MAX_LINES][AT_COMMAND_MAX_LINE_SIZE];
	uint8_t line_count;
	uint8_t nod;	
}AT_COMMAND_DATA;

AT_COMMAND_DATA date;

typedef enum{
	STATE_MACHINE_NOT_READY,
	STATE_MACHINE_READY_OK,
	STATE_MACHINE_READY_WITH_ERROR,
	STATE_MACHINE_NOT_READY_WITH_ERROR,
	STATE_MACHINE_LINES
}STATE_MACHINE_RETURN_VALUE;

// declaratii de functii
STATE_MACHINE_RETURN_VALUE at_command_parse(char char_crt, int flag);
