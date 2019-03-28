#include <mach.h>
#include <mach_error.h>
#include <mach/mig_errors.h>
#include <mach/thread_status.h>
#include <mach/processor_info.h>
#include <mach/message.h>
#include <stdio.h>
#include <stdlib.h>
#include <hurd.h>


#define PAG_SIZE 16384
#define MAXDATA 3
#define SLEEP_TIME 20

struct{
	mach_msg_header_t header;
}msg;

struct {
	    mach_msg_header_t header;
} message;

int main(){
	int pid = fork();
	int err;
	mach_port_t parent = mach_task_self();
	if(pid < 0) {
		printf("Error forking");
	}
	if(pid == 0){		
		msg.header.msgh_remote_port = parent;
		msg.header.msgh_local_port = MACH_PORT_NULL;
		msg.header.msgh_bits = MACH_MSGH_BITS (MACH_MSG_TYPE_COPY_SEND, 0);
		msg.header.msgh_size = sizeof msg;
		mach_msg_return_t err = mach_msg_send(&msg.header); 
		if (err != MACH_MSG_SUCCESS){
		      fprintf(stderr, "Couldn't send uint: 0x%x\n", err); 
		}
		exit(0);
	}
	else if(pid > 0){
		sleep(5);
		mach_msg_return_t err = mach_msg(&message.header, MACH_RCV_MSG | MACH_RCV_INTERRUPT,0, sizeof message, parent,MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL); 
		if(err == MACH_MSG_SUCCESS){
			printf("Message recived");
		}
		printf("%d\n",err);
		int status;
		waitpid(-1,&status,0); 
	
	}
}
