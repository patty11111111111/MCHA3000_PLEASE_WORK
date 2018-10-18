#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <inttypes.h> // For PRIxx and SCNxx macros

#include "cmd_line_buffer.h"
#include "cmd_parser.h"

#include "potentiometer.h"
#include "encoder.h"
#include "led_rgb.h"
#include "light.h"
#include "dimmer_task.h"
#include "heartbeat_task.h"
#include "controller.h"
#include "sin_table.h"
#include "log_data.h"
#include "stepper_task.h"

static void _cmd_help(void);
static void _print_chip_pinout(void);

// Values
static float x = 0, y = 0, ref, Wheel_angle, Pend_angle, *u, input[3], t, s_speed =0;


#ifdef NO_LD_WRAP
void cmd_parse(char *) __asm__("___real_cmd_parse");
#endif

//cmd table
static CMD_T cmd_table[] =
{
	{"help", cmd_help, "Displays this help message"},
	{"pot", cmd_pot, "Reads the angle of the potentiometer"},
	{"enc", cmd_enc, "enc [reset] Get or reset encoder count\n"},
	{"light" ,cmd_light, "light [<hue> <sat> <val>] Get or set light HSV values\n"},
	{"dimmer",cmd_dimmer, "dimmer [start|stop] Get status or start/stop dimmer task\n"},
	{"heartbeat",cmd_heartbeat,"heartbeat [start|stop] Get status or start/stop heartbeat task\n"},
	{"get",			cmd_get,			"Query the value of x or y" 				},
	{"set", 		cmd_set, 			"set the value of x,y,ref or meas" 			},
	{"mulxy", 		cmd_mulxy, 		"Query the value of x times y" 				},
	{"ctrl",		cmd_ctrl,			""	},
	{"sin",			cmd_sin, 			""},
	{"log",			cmd_log, 			""},
	
};
enum {CMD_N = sizeof(cmd_table)/sizeof(CMD_T)};

void cmd_parse(char * cmd)
{
	char* argv[11];
	int argc;
    if (cmd == NULL)
    {
        printf("ERROR: Tried to parse NULL command pointer\n");
        return;
    }
    else if (*cmd == '\0') // Empty command string
    {
        return;
    }
    else 
	{
		argc = makeargv(cmd , argv, 10);
		for (size_t i = 0; i < CMD_N; i++)
		{
			
			if (strcmp(argv[0], cmd_table[i].cmd)== 0)
			{
				cmd_table[i].func(argc, argv);
				break;
			}
			else if (i == CMD_N-1)
			{
				printf("Unknown command: \"%s\"\n", cmd);
			}
		}
	}
}
   
    

int makeargv(char *string, char *argv[], int argvsize)
{
	char *p = string;
	int  i;
	int argc = 0;

	for(i = 0; i < argvsize; i++) {
		/* skip leading whitespace */
		while(isspace(*p))
			p++;

		if(*p != '\0')
			argv[argc++] = p;
		else {
			argv[argc] = 0;
			break;
		}

		/* scan over arg */
		while(*p != '\0' && !isspace(*p))
			p++;
		/* terminate arg: */
		if(*p != '\0' && i < argvsize-1)
			*p++ = '\0';
	}

	return argc;
}

void cmd_help(int argc, char*argv[])
{
	printf("Command Description\n""--------------------------\n");
	
	for (size_t i = 0; i < CMD_N; i++)
	{
		printf("%-14s %s\n", cmd_table[i].cmd, cmd_table[i].help);
	}
}
void cmd_pot(int argc, char *argv[])
{
	if (argc == 1)
	{
		printf("Potentiometer ADC value is %" PRIu16 "\n", pot_get_value());
	}
	else 
	{
		printf("pot: invalid argument \"%s\", syntax is: pot\n", argv[1]);
	}
}
void cmd_enc(int argc, char *argv[])
{
	if (argc == 1)
    {
        printf("Encoder count is %" PRId32 "\n", encoder_get_count());
    }
    else if (!strcmp(argv[1] , "reset"))
    {
        encoder_set_count(0);
        printf("Encoder count reset to 0\n");
    }
	else 
	{
		printf("enc: invalid argument \"%s\", syntax is: enc [reset]\n", argv[1]);
	}
}
void cmd_light(int argc, char *argv[])
{
	if (argc == 1)
	{
		printf("Current LED hue = %"PRIi32", sat = %"PRIi32", val = %"PRIi32"\n",light_get_hue()/128,light_get_saturation(),light_get_brightness());
	}
	else if (argc == 4)
    {
		int32_t h_l, sat_l, bright;
		h_l = atof(argv[1]);
		sat_l = atof(argv[2]);
		bright = atof(argv[3]);
		
		if (h_l > 360 || h_l < -360)
		{
			h_l =  h_l % 360;
		}
		if (h_l < 0)
		{
			h_l = 360 + h_l;
		}
		if (sat_l > 65535)
		{
			sat_l = 65535;
		}
		else if (sat_l < 0)
		{
			sat_l = 0;
		}
		if (bright > 65535)
		{
			bright = 65535;
		}
		else if (bright < 0)
		{
			bright = 0;
		}

		
		light_set_hue(128*h_l);
		light_set_saturation(sat_l);
		light_set_brightness(bright);
		
		// Update LED brightness
        led_rgb_set_red(light_get_red());
        led_rgb_set_green(light_get_green());
        led_rgb_set_blue(light_get_blue());
		printf("Set LED hue = %d, sat = %d, val = %d\n", h_l, sat_l, bright);
		
	}
	else 
	{
		printf("light: invalid argument \"%s\", syntax is: light [<hue> <saturation> <brightness>]\n", argv[1]);
	}
}
void cmd_dimmer(int argc, char *argv[])
{
	if (argc ==1)
    {
        if (dimmer_task_is_running())
            printf("Dimmer is currently running. Better catch it.\n");
        else
            printf("Dimmer is not currently running.\n");
    }
    else if (!strcmp(argv[1], "start"))
    {
        dimmer_task_start();
        printf("Dimmer has started\n");
    }
    else if (!strcmp(argv[1], "stop"))
    {
		dimmer_task_stop();
        printf("Dimmer has stopped\n");
    }
	else 
	{
		printf("dimmer: invalid argument \"%s\", syntax is: dimmer [start|stop]\n", argv[1]);
	}

}
void cmd_heartbeat(int argc, char *argv[])
{
    if (argc <= 1)
    {
        if (heartbeat_task_is_running())
            printf("Heartbeat is currently running\n");
        else
            printf("Heartbeat is not currently running\n");
    }
	else if (!strcmp(argv[1], "start"))
    {
        heartbeat_task_start();
        printf("Heartbeat has started\n");
    }
    else if (!strcmp(argv[1], "stop"))
    {
        heartbeat_task_stop();
        printf("Heartbeat has stopped\n");
    }
	else 
	{
		printf("heartbeat: invalid argument \"%s\", syntax is: heartbeat [start|stop]\n", argv[1]);
	}

}


void cmd_set(int argc, char *argv[])
{	
//	if (argc == 2) //2 means 2nd word input
	
		if (!strncmp(argv[1],"x", 1)) //1st placeholder after initial search
			{
				x = atof(argv[2]);
			}
		else if (!strncmp(argv[1],"y", 1))
			{
				y = atof(argv[2]);
			}
		else if (!strncmp(argv[1],"ref", 3))
			{
				ref = atof(argv[2]);
			}
		else if (!strncmp(argv[1],"s_set", 5))
			{
				stepper_update(0);
			}
		else if (!strncmp(argv[1],"period", 6))
			{
				stepper_period(atoi(argv[2]));
				printf("PERIOD is %d\n", stepper_period_get()); 

			}
			else if (!strncmp(argv[1],"dir", 3)) {
			stepper_dir();
			}
		/*else if (!strncmp(argv[1],"meas", 4)) //1st placeholder after initial search
			{
				Wheel_angle = atof(argv[2]);
				Pend_angle = atof(argv[3]);	
				
			}*/
		//else if (argc == 3) //2 means 2nd word input
			
				if (!strncmp(argv[1],"meas", 4)) //1st placeholder after initial search
				{
				Wheel_angle = atof(argv[2]);
				Pend_angle = atof(argv[3]);
				}
			
			
	
}

void cmd_get(int argc, char *argv[])
{
	//if (argc == 1) //2 means 2nd word input
	
		if (!strcmp(argv[1],"x")) //1st placeholder after initial search
		{
			printf("x is %g\n", x); 
		}
		
		if (!strcmp(argv[1],"y"))
		{
			printf("y is %g\n", y); 
		}
}

void cmd_mulxy(int argc, char *argv[])
{
	 printf("%g\n", (x*y)); 
}

void cmd_ctrl(int argc, char *argv[])
{
	if (argc == 1) //number of words looking for
	{
		input[1] = ref;
		input[2] = Wheel_angle;
		input[3] = Pend_angle;
		u=ctrl_run(input);
		t=*u;
		printf("%g\n", t);
	}
	
	else if (argc == 2) //2 means 2nd word input
	{
		if (!strcmp(argv[1],"reset")) //1st placeholder after initial search
		{
			printf("ctrl reset\n"); 
		    ctrl_init();
		}
		
	}
}
void cmd_sin(int argc, char *argv[])
{
	if (argc == 4) //number of words looking for
	{
		sin_table_cmd(argc, argv);
	}
}
void cmd_log(int argc, char *argv[])
{
	if (argc == 3) //number of words looking for
	{
		log_data_cmd(argc, argv);
	}

}


void _print_chip_pinout(void)


/*
motor out 1
TIM3_CH3
PC8_2 - step
PC6_4 - dir

motor out 2
PB1_24 - step
PB15_26 - dir

915pg for __
HAL_TIM_GET_AUTORELOAD

877 perifera control func
*/

{
    printf(
           "Pin configuration:\n"
           "\n"
           "       .---------------------------------------.\n"
           " PC10--|  1  2 --PC11              PC9--  1  2 |--PC8\n"
           " PC12--|  3  4 --PD2               PB8--  3  4 |--PC6\n"
           "  VDD--|  5  6 --E5V               PB9--  5  6 |--PC5\n"
           "BOOT0--|  7  8 --GND              AVDD--  7  8 |--U5V\n"
           "   NC--|  9 10 --NC                GND--  9 10 |--NC\n"
           "   NC--| 11 12 --IOREF             PA5-- 11 12 |--PA12\n"
           " PA13--| 13 14 --RESET             PA6-- 13 14 |--PA11\n"
           " PA14--| 15 16 --+3v3              PA7-- 15 16 |--PB12\n"
           " PA15--| 17 18 --+5v               PB6-- 17 18 |--NC\n"
           "  GND--| 19 20 --GND               PC7-- 19 20 |--GND\n"
           "  PB7--| 21 22 --GND               PA9-- 21 22 |--PB2\n"
           " PC13--| 23 24 --VIN               PA8-- 23 24 |--PB1\n"
           " PC14--| 25 26 --NC               PB10-- 25 26 |--PB15\n"
           " PC15--| 27 28 --PA0               PB4-- 27 28 |--PB14\n"
           "  PH0--| 29 30 --PA1               PB5-- 29 30 |--PB13\n"
           "  PH1--| 31 32 --PA4               PB3-- 31 32 |--AGND\n"
           " VBAT--| 33 34 --PB0              PA10-- 33 34 |--PC4\n"
           "  PC2--| 35 36 --PC1               PA2-- 35 36 |--NC\n"
           "  PC3--| 37 38 --PC0               PA3-- 37 38 |--NC\n"
           "       |________                   ____________|\n"
           "                \\_________________/\n"
           );
}

