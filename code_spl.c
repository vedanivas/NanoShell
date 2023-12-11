#include "headers.h"

int check_amb(char** cmds)
{
  for (int i = 0; cmds[i] != NULL; i++) for (int j = 0; cmds[i][j] != '\0'; j++) if (cmds[i][j] == '&') return i;

  return -1;
}

int cmd_spl(char** cmds, int a)
{
  int in = 8, out = 9;
  if (cmd_iodi(cmds, &in, &out)) return 1;

  int f_bg = check_amb(cmds);
  
  if (f_bg != -1) cmds[f_bg] = strtok(cmds[f_bg], "&");
  
  int flag = fork();
  int end, bgn = time(0);
  if (flag == 0)
  {
    if (f_bg != -1) setpgid(0, 0);
    if (execvp(cmds[0], cmds) == -1) 
    {
      printf("Invalid command\n");
      return 1;
    }
  }
  else if (flag > 0)
  {
    if (f_bg == -1) waitpid(flag, NULL, 0);
    else printf("[%d]\n", flag);
    end = time(0);
  }
  else
  {
    perror("Error");
    return 1;
  }
  
  if (in > 0)
  {
    if (dup2(in, STDIN_FILENO) < 0)
    {
      // perror("Error");
      // return 1;
    }
    close(in);
  }
  if (out > 0)
  {
    if (dup2(out, STDOUT_FILENO) < 0)
    {
      perror("Error");
      return 1;
    }
    close(out);
  }

  int delay = end - bgn;
  return delay >= 1 ? delay : 0;
}