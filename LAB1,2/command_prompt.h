#ifndef FOS_KERN_MONITOR_H
#define FOS_KERN_MONITOR_H
#ifndef FOS_KERNEL
# error "This is a FOS kernel header; user programs should not #include it"
#endif

// Function to activate the kernel command prompt
int run_command_prompt();

// Declaration of functions that implement command prompt commands.
int command_help(int , char **);
int command_kernel_info(int , char **);
int command_rep(int , char **);
int command_halt(int , char **);
#endif	// !FOS_KERN_MONITOR_H
