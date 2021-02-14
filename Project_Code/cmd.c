#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "commando.h"

#define BUFSIZE 255

cmd_t *cmd_new(char *argv[]){
cmd_t * newcmd_t = (cmd_t *) malloc(sizeof(cmd_t));
newcmd_t->argv[] = strdup(argv[]);
newcmd_t->name = argv[0];
newcmd_t->finished = 0;
char* s = "INIT";
snprintf(newcmd_t->str_status,4, "%s", s);  //Is 4 enough?
newcmd_t->pid_t = NULL;
newcmd_t->out_pipe[0] = -1;
newcmd_t->out_pipe[1] = -1;
newcmd_t->finished = -1;
newcmd_t->status = -1;
newcmd_t->output = Null;
newcmd_t->output_size = -1;
return newcmd_t;
}

// Deallocates a cmd structure. Deallocates the strings in the argv[]
// array. Also deallocats the output buffer if it is not
// NULL. Finally, deallocates cmd itself.
void cmd_free(cmd_t *cmd){
free(cmd);
}

// If the finished flag is 1, does nothing. Otherwise, updates the
// state of cmd.  Uses waitpid() and the pid field of command to wait
// selectively for the given process. Passes block (one of DOBLOCK or
// NOBLOCK) to waitpid() to cause either non-blocking or blocking
// waits.  Uses the macro WIFEXITED to check the returned status for
// whether the command has exited. If so, sets the finished field to 1
// and sets the cmd->status field to the exit status of the cmd using
// the WEXITSTATUS macro. Calls cmd_fetch_output() to fill up the
// output buffer for later printing.
void cmd_update_state(cmd_t *cmd, int block){
  int inputStatus;
  if(cmd->finished==1){
    return;
  }else{
    waitpid(cmd->pid, &inputStatus, block);
    if(WIFEXITED(inputStatus)){
      cmd->finished = 1;
      cmd->status = WEXITSTATUS(inputStatus);
    /////  cmd_fetch_output();       -----------UNCOMMENT ONCE FUNCTION WRITTEN
}
  }
}

// Reads all input from the open file descriptor fd. Assumes
// character/text output and null-terminates the character output with
// a '\0' character allowing for printf() to print it later. Stores
// the results in a dynamically allocated buffer which may need to
// grow as more data is read.  Uses an efficient growth scheme such as
// doubling the size of the buffer when additional space is
// needed. Uses realloc() for resizing.  When no data is left in fd,
// sets the integer pointed to by nread to the number of bytes read
// and return a pointer to the allocated buffer. Ensures the return
// string is null-terminated. Does not call close() on the fd as this
// is done elsewhere.
char *read_all(int fd, int *nread){
  int max_size = 1; cur_pos = 0;
   char *buf = malloc(max_size*sizeof(char));
   while(1){
   nread = read(fd,buf,BUFSIZE-1);
   if(nread == 0){
     nread = total;
     return buf;
   }
   cur_pos++;
   if(cur_pos == max_size){
     max_size *= 2;                               // double size of buffer
     char *new_buf =                              // pointer to either new or old location
       realloc(buf, max_size*sizeof(char));
       if(new_buf == NULL){                         // check that re-allocation succeeded
         printf("ERROR: reallocation failed\n");    // if not...
         free(buf);                                 // de-allocate current buffer
         exit(1);                                   // bail out
       }
       buf = new_buf;
   }
   buf[nread] = '\0';
   total += nread;
   printf("read: '%s'\n",buf);
   }
}

// If cmd->finished is zero, prints an error message with the format
//
// ls[#12341] not finished yet
//
// Otherwise retrieves output from the cmd->out_pipe and fills
// cmd->output setting cmd->output_size to number of bytes in
// output. Makes use of read_all() to efficiently capture
// output. Closes the pipe associated with the command after reading
// all input.
void cmd_fetch_output(cmd_t *cmd){
     if(cmd->finished == 0){
       printf("ls[#%d] not finished yet\n",cmd->pid); // Is this cmd->pid? -----------
     }else{
       //pipe stuff. Figure out! -----------
     }
}

// Prints the output of the cmd contained in the output field if it is
// non-null. Prints the error message
//
// ls[#17251] : output not ready
//
// if output is NULL. The message includes the command name and PID.
void cmd_print_output(cmd_t *cmd){ ///FIgure out PID
if(cmd->output == NULL){
  printf("ls[#%d] : output not ready\n", cmd->pid); // Is this cmd->pid? -----------
}
}
