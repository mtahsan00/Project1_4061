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
//
// int main(){
//   char *argv[] = {
//     "ls",
//     "-F",
//     "-a ",
//     "-1",
//     "test_data/stuff",
//     NULL
//   };
//   cmd_t *cmd = cmd_new(argv);
//   for(int i=0; i<ARG_MAX+1; i++){
//     printf("  [%3d] : %s\n",i,cmd->argv[i]);
//     if(cmd->argv[i] == NULL){
//       break;
//     }
//   }
//   cmd_free(cmd);
//
// }


// Allocates a new cmd_t with the given argv[] array. Makes string
// copies of each of the strings contained within argv[] using
// strdup() as they likely come from a source that will be
// altered. Ensures that cmd->argv[] is ended with NULL. Sets the name
// field to be the argv[0]. Sets finished to 0 (not finished yet). Set
// str_status to be "INIT" using snprintf(). Initializes the remaining
// fields to obvious default values such as -1s, and NULLs.
cmd_t *cmd_new(char *argv[]){
cmd_t * newcmd = malloc(sizeof(cmd_t));
for(int i =0;i<ARG_MAX+1;i++){
  if(argv[i] == NULL){
    newcmd->argv[i] = NULL;
    break;
  }
  newcmd->argv[i] = strdup(argv[i]);
}
//strcpy(newcmd->name, argv[0]);
newcmd->finished = 0;
char* s = "INIT";
snprintf(newcmd->str_status,5, "%s", s);
newcmd->pid = -1;
newcmd->out_pipe[0] = -1;
newcmd->out_pipe[1] = -1;
newcmd->finished = -1;
newcmd->status = -1;
newcmd->output = NULL;
newcmd->output_size = -1;
return newcmd;
}

// Deallocates a cmd structure. Deallocates the strings in the argv[]
// array. Also deallocats the output buffer if it is not
// NULL. Finally, deallocates cmd itself.
void cmd_free(cmd_t *cmd){
  for(int i =0;i<ARG_MAX+1;i++){
    if(cmd->argv[i] == NULL){
      free(cmd->argv[i]);
      break;
    }
    free(cmd->argv[i]);
  }
  if(cmd->output != NULL){
    free(cmd->output);
  }
  free(cmd);
}


// Forks a process and executes command in cmd in the process.
// Changes the str_status field to "RUN" using snprintf(). Creates a
// pipe for out_pipe to capture standard output.  In the parent
// process, ensures that the pid field is set to the child PID. In the
// child process, directs standard output to the pipe using the dup2()
// command. For both parent and child, ensures that unused file
// descriptors for the pipe are closed (write in the parent, read in
// the child).
void cmd_start(cmd_t *cmd){

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
//   int inputStatus;
//   if(cmd->finished==1){
//     return;
//   }else{
//     waitpid(cmd->pid, &inputStatus, block);
//     if(WIFEXITED(inputStatus)){
//       cmd->finished = 1;
//       cmd->status = WEXITSTATUS(inputStatus);
//     /////  cmd_fetch_output();       -----------UNCOMMENT ONCE FUNCTION WRITTEN
// }
//   }
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
  char *c = "";
    return c;
  // int total = 0;
  // int max_size = 1, cur_pos = 0;
  // int readIn;
  // char *buf = malloc(max_size*sizeof(char));
  // while(1){
  //  readIn = read(fd,buf,BUFSIZE-1);
  //  if(readIn == 0){
  //    nread = &total;
  //    return buf;
  //  }
  //  cur_pos++;
  //  if(cur_pos == max_size){
  //    max_size *= 2;                               // double size of buffer
  //    char *new_buf =                              // pointer to either new or old location
  //      realloc(buf, max_size*sizeof(char));
  //      if(new_buf == NULL){                         // check that re-allocation succeeded
  //        printf("ERROR: reallocation failed\n");    // if not...
  //        free(buf);                                 // de-allocate current buffer
  //        exit(1); //do we need THIS___                                  // bail out
  //      }
  //      buf = new_buf;
  //  }
  //  buf[readIn] = '\0';
  //  total += readIn;
  //  printf("read: '%s'\n",buf);
  //  }
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
     // if(cmd->finished == 0){
     //   printf("ls[#%d] not finished yet\n",cmd->pid); // Is this cmd->pid? -----------
     // }else{
     //   //pipe stuff. Figure out! -----------
     // }
}

// Prints the output of the cmd contained in the output field if it is
// non-null. Prints the error message
//
// ls[#17251] : output not ready
//
// if output is NULL. The message includes the command name and PID.
void cmd_print_output(cmd_t *cmd){ ///FIgure out PID
// if(cmd->output == NULL){
//   printf("ls[#%d] : output not ready\n", cmd->pid); // Is this cmd->pid? -----------
// }
}
