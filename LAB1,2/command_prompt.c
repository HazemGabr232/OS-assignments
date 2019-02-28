// Simple command-line kernel prompt useful for
// controlling the kernel and exploring the system interactively.

/*
KEY WORDS
==========
CONSTANTS:	WHITESPACE, NUM_OF_COMMANDS
VARIABLES:	Command, commands, name, description, function_to_execute, number_of_arguments, arguments, command_string, command_line, command_found
FUNCTIONS:	readline, cprintf, execute_command, run_command_prompt, command_kernel_info, command_help, strcmp, strsplit, start_of_kernel, start_of_uninitialized_data_section, end_of_kernel_code_section, end_of_kernel
=====================================================================================================================================================================================================
*/

#include <inc/stdio.h>
#include <inc/string.h>
#include <inc/memlayout.h>
#include <inc/assert.h>
#include <inc/x86.h>

#include <kern/console.h>
#include <kern/command_prompt.h>

//Structure for each command
struct Command 
{
	char *name;
	char *description;
	// return -1 to force command prompt to exit
	int (*function_to_execute)(int number_of_arguments, char** arguments);
};

//Array of commands. (initialized)
struct Command commands[] = 
{
	{ "help", "Display this list of commands", command_help },
	{ "kernel_info", "Display information about the kernel", command_kernel_info },
	{ "rep", "Echo the given string N times -- usage:rep string n ", command_rep },
	{ "halt", "exit the kernel", command_halt },
};

//Number of commands = size of the array / size of command structure
#define NUM_OF_COMMANDS (sizeof(commands)/sizeof(commands[0]))

//Functions Declaration
int execute_command(char *command_string);
//=====================


//invoke the command prompt
int run_command_prompt()
{
	char *command_line;

	cprintf("\nWelcome to the FOS kernel command prompt!\n");
	cprintf("Type 'help' for a list of commands.\n");


	while (1==1) 
	{
		//get command line
		command_line = readline("FOS> ");
		
		//parse and execute the command
		if (command_line != NULL)
			if (execute_command(command_line) < 0)
				break;
	}
	return 0;
}

/***** Kernel command prompt command interpreter *****/

//define the white-space symbols 
#define WHITESPACE "\t\r\n "
#define ANDSYMBOL "&"
//Function to parse any command and execute it 
//(simply by calling its corresponding function)
int execute_command(char *command_string)
{
	// Split the command string into whitespace-separated arguments
	int number_of_arguments;
	int number_of_commands;
	//allocate array of char* of size MAX_ARGUMENTS = 16 defined in "inc/string.h" 	
	char *arguments[MAX_ARGUMENTS];
	char *commands_in_line[NUM_OF_COMMANDS];
	
	strsplit(command_string, ANDSYMBOL, commands_in_line, &number_of_commands) ;
	if (number_of_commands == 0)
		return 0;
	
	int i ;
	
	for (i = 0; i < number_of_commands; i++){
		strsplit(commands_in_line[i], WHITESPACE, arguments, &number_of_arguments) ;
		//if (number_of_arguments == 0)
			//return 0;
		
		// Lookup in the commands array and execute the command
		int command_found = 0;
		
		for (i = 0; i < NUM_OF_COMMANDS; i++)
		{
			if (strcmp(arguments[0], commands[i].name) == 0)
			{
				command_found = 1;
				break;
			}
		}
		
		if(command_found)
		{
			int return_value;
			return_value = commands[i].function_to_execute(number_of_arguments, arguments);			
			if (return_value<0)
				return return_value;
		}
		else
		{
			//if not found, then it's unknown command
			cprintf("Unknown command '%s'\n", arguments[0]);
			
		}
	}
	return 0;
}

/***** Implementations of basic kernel command prompt commands *****/

//print name and description of each command
int command_help(int number_of_arguments, char **arguments)
{
	if(number_of_arguments>1){
		cprintf("invalid number of arguments\n");
		return 0;
	}			
	
	int i;

	for (i = 0; i < NUM_OF_COMMANDS; i++)
		cprintf("%s - %s\n", commands[i].name, commands[i].description);
	return 0;
}

//print information about kernel addresses and kernel size
int command_kernel_info(int number_of_arguments, char **arguments )
{
	if(number_of_arguments>1){
		cprintf("invalid number of arguments\n");
		return 0;
	}
	
	extern char start_of_kernel[], end_of_kernel_code_section[], start_of_uninitialized_data_section[], end_of_kernel[];

	cprintf("Special kernel symbols:\n");
	cprintf("  Start Address of the kernel 			%08x (virt)  %08x (phys)\n", start_of_kernel, start_of_kernel - KERNEL_BASE);
	cprintf("  End address of kernel code  			%08x (virt)  %08x (phys)\n", end_of_kernel_code_section, end_of_kernel_code_section - KERNEL_BASE);
	cprintf("  Start addr. of uninitialized data section 	%08x (virt)  %08x (phys)\n", start_of_uninitialized_data_section, start_of_uninitialized_data_section - KERNEL_BASE);
	cprintf("  End address of the kernel   			%08x (virt)  %08x (phys)\n", end_of_kernel, end_of_kernel - KERNEL_BASE);
	cprintf("Kernel executable memory footprint: %d KB\n",
		(end_of_kernel-start_of_kernel+1023)/1024);
	return 0;
}

int command_rep(int number_of_arguments, char **arguments)
{
	if(number_of_arguments!=3){
		cprintf("invalid number of arguments\n");
		return 0;
	}
	int rep=strtol(arguments[2],NULL,10);	
	int i ;
	for(i=0 ; i<rep ;i++){
		cprintf("%s\n",arguments[1]);
	}
	return 0;
}

int command_halt(int number_of_arguments, char **arguments)
{	if(number_of_arguments>1){
		cprintf("invalid number of arguments\n");
		return 0;
	}
	return -1; 
}	