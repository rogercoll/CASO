#include <mach.h>
#include <mach_error.h>
#include <mach/mig_errors.h>
#include <mach/thread_status.h>
#include <mach/processor_info.h>
#include <mach/mach_host.h>
#include <stdio.h>
#include <stdlib.h>
#include <hurd.h>

#define STACK_SIZE 8192
unsigned char threadStack[STACK_SIZE];

void bucle_infinit(){
	while(1){
		printf("Aloha");
	}
}



int main(){
	int res;
	struct i386_thread_state estat_thread;
	mach_msg_type_number_t more_info = i386_THREAD_STATE_COUNT;
	mach_port_t child_p;
	mach_port_t parent_p = mach_task_self();
	res = thread_create(parent_p, &child_p);
	if (res != KERN_SUCCESS){
		printf("Error creating thread");
	}
	thread_get_state(child_p, i386_THREAD_STATE,(thread_state_t)&estat_thread, &more_info); 
	estat_thread.uesp = &threadStack[STACK_SIZE] -100;
	estat_thread.eip = bucle_infinit;
	res = thread_set_state(child_p, i386_THREAD_STATE, (thread_state_t *)(&estat_thread), more_info);
	if (res != KERN_SUCCESS){
		printf("Error while setting state");
	}
	res = mach_setup_tls(child_p);
	if (res != KERN_SUCCESS){
		printf("Error while setting tls");
	}
	res = thread_resume(child_p);
	if (res != KERN_SUCCESS){
		printf("Error while running thread");
	}
	sleep(20);
	thread_terminate(child_p);
}
