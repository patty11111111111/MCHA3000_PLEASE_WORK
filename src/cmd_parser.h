#ifndef CMD_PARSER_H
#define CMD_PARSER_H

#include "cmd_line_buffer.h"

void cmd_parse(char *);

// command table structur
typedef struct
{
const char * cmd;
void (*func)(int argc, char *argv[]);
const char * help;
} CMD_T;




// comand table functions
int makeargv(char *string, char *argv[], int argvsize);
void cmd_help(int argc, char *argv[]);
void cmd_pot(int argc, char *argv[]);
void cmd_enc(int argc, char *argv[]);
void cmd_light(int argc, char *argv[]);
void cmd_dimmer(int argc, char *argv[]);
void cmd_heartbeat(int argc, char *argv[]);
void cmd_get(int argc, char *argv[]);
void cmd_set(int argc, char *argv[]);
void cmd_mulxy(int argc, char *argv[]);
void cmd_ctrl(int argc, char *argv[]);
void cmd_sin(int argc, char *argv[]);
void cmd_log(int argc, char *argv[]);
#endif


