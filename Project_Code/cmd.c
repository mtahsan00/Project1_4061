#include "commando.h"
#include <unistd.h>
#include <stdlib.h>




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
strcpy(newcmd->name, argv[0]);
newcmd->finished = 0;
char* s = "INIT";
snprintf(newcmd->str_status,5, "%s", s);
newcmd->pid = -1;
newcmd->out_pipe[0] = -1;
newcmd->out_pipe[1] = -1;
newcmd->finished = 0;
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
pipe(cmd->out_pipe);
pid_t child = fork();
snprintf(cmd->str_status,5, "%s", "RUN");
if(child==0){
  dup2(cmd->out_pipe[1],STDOUT_FILENO);
  close(cmd->out_pipe[0]);
  execvp(cmd->name, cmd->argv);
}else{
    cmd->pid = child;
    close(cmd->out_pipe[1]);
}

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
  if(cmd->finished!=1){
    int ret = waitpid(cmd->pid,&inputStatus,block);
    if(ret == -1){
      perror("waitpid() failed\n");
      exit(1);
    }else if(ret == cmd->pid){
      if(WIFEXITED(inputStatus)){
        cmd->finished = 1;
        cmd->status = WEXITSTATUS(inputStatus);
        snprintf(cmd->str_status,8, "EXIT(%d)", cmd->status);
        cmd_fetch_output(cmd);
        printf("@!!! %s[#%d]: EXIT(%d)\n",cmd->name,cmd->pid,cmd->status);
      }
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
  int total = 0;
  int max_size = 1, cur_pos = 0;
  int readIn;
  char *buf = malloc(max_size*sizeof(char));
  while(1){
   readIn = read(fd,&buf[cur_pos],1);
   if(readIn == 0){
     *nread = total;
     buf[total] = '\0';
     return buf;
   }
   cur_pos++;
   if(cur_pos == max_size){
     max_size *= 2;
     buf = realloc(buf, max_size*sizeof(char));
      if(buf == NULL){
        free(buf);
        exit(1);
       }
   }
   total += readIn;
   }

  // free(buf);
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
       cmd->output = read_all(cmd->out_pipe[0],&(cmd->output_size));
       close(cmd->out_pipe[0]);
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
    printf("%s[#%d] : output not ready\n",cmd->name,cmd->pid); // Is this cmd->pid? -----------
  }
  else{
    printf("%s", (char *)cmd->output);
  }
}
