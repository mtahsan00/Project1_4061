#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "commando.h"

int main(int argc, char **argv){
  setvbuf(stdout, NULL, _IONBF, 0);
  int echo_enabled = 0;
  char *echo_char = getenv("COMMANDO_ECHO");
  if(argc>1){
    if((strncmp(argv[1],"--echo",6)==0)){
    setenv("COMMANDO_ECHO","1",1);
  }
  }

  char *tokenArray[MAX_LINE]; //free in end
  int tokens;
  cmdcol_t cmdcol_actual = {};
  cmdcol_t *cmdcol = &cmdcol_actual;
  while(1){
    echo_char = getenv("COMMANDO_ECHO");
    if(echo_char !=NULL){
      echo_enabled = 1;
    }else if(echo_char == NULL){
      echo_enabled = 0;
    }
    printf("@> ");
    char buf[MAX_LINE];
    if(fgets(buf, MAX_LINE, stdin) == NULL){
      printf("\n%s","End of input" );
      break;
    }
    parse_into_tokens(buf,tokenArray,&tokens);
    if(tokenArray[0] == NULL){
      if(echo_enabled){
        printf("\n" );
      }
    }else if(strncmp(tokenArray[0], "help",4)==0){
      if(echo_enabled){
        printf("help\n" );
      }
      printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n","COMMANDO COMMANDS",
      "help               : show this message",
      "exit               : exit the program",
      "list               : list all jobs that have been started giving information on each",
      "pause nanos secs   : pause for the given number of nanseconds and seconds",
      "output-for int     : print the output for given job number",
      "output-all         : print output for all jobs",
      "wait-for int       : wait until the given job number finishes",
      "wait-all           : wait for all jobs to finish",
      "command arg1 ...   : non-built-in is run as a job");
    }else if(strncmp(tokenArray[0], "list",4)==0){
      if(echo_enabled){
        printf("%s\n","list");
      }
      cmdcol_print(cmdcol);
    }else if(strncmp(tokenArray[0], "exit",4)==0){
      if(echo_enabled){
        printf("%s\n","exit");
      }
      break;
    }else if(strncmp(tokenArray[0], "wait-for",8)==0){
      if(echo_enabled){
        printf("%s %s\n",tokenArray[0],tokenArray[1]);
      }
      cmd_update_state(cmdcol->cmd[atoi(tokenArray[1])],DOBLOCK);
    }else if(strncmp(tokenArray[0], "output-for",10)==0){
      int point = atoi(tokenArray[1]);
      if(echo_enabled){
        printf("%s %s\n",tokenArray[0],tokenArray[1]);
      }
      printf("\n@<<< Output for %s[%%%d] (%d bytes):\n",cmdcol->cmd[point]->name,point,
      cmdcol->cmd[point]->output_size);
      printf("----------------------------------------\n");
      cmd_print_output(cmdcol->cmd[point]);
      printf("----------------------------------------\n");
    }else if(strncmp(tokenArray[0], "output-all",10)==0){
      if(echo_enabled){
        printf("%s\n",tokenArray[0]);
      }
      for(int i =0;i<cmdcol->size;i++){
        printf("@<<< Output for %s[#%d] (%d bytes):\n",cmdcol->cmd[i]->name,cmdcol->cmd[i]->pid,
        cmdcol->cmd[i]->output_size);
        printf("----------------------------------------\n");
        cmd_print_output(cmdcol->cmd[i]);
        printf("----------------------------------------\n");
      }
    }else if(strncmp(tokenArray[0], "wait-all",8)==0){
      if(echo_enabled){
        printf("%s\n",tokenArray[0]);
      }
      for(int i =0;i<cmdcol->size;i++){
        cmd_update_state(cmdcol->cmd[i],DOBLOCK);
      }
    }else if(strncmp(tokenArray[0], "pause",5)==0){
      if(echo_enabled){
        printf("%s %s %s\n",tokenArray[0],tokenArray[1],tokenArray[2]);
      }
    //  printf("%d\n",atoi(tokenArray[1]));
      pause_for(atol(tokenArray[1]),atoi(tokenArray[2]));
      cmdcol_update_state(cmdcol,NOBLOCK);
    }else{
      if(echo_enabled){
        for(int i =0;i<tokens;i++){
          printf("%s ",tokenArray[i]);
        }
        printf("\n");
      }
      cmd_t *cmd = cmd_new(tokenArray);
      cmdcol_add(cmdcol, cmd);
      cmd_start(cmd);
    }
  }
  cmdcol_freeall(cmdcol);
return 0;
}
//run < test-results/cmds.tmp
// ls -a -F test_data/stuff/
// wait-for 0
// list
// output-for 0
