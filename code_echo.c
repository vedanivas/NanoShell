#include "headers.h"

int cmd_echo(char **cmds, int a)
{
  int in = -1, out = 9;
  if (cmd_iodi(cmds, &in, &out)) return 1;

  for (int i = 1; cmds[i][0] != '>'; i++) printf("%s ", cmds[i]);
  printf("\n");

  if (in != -1)
  {
    close(in);
    if (dup2(in, STDIN_FILENO) < 0)
    {
      perror("Error");
      return 1;
    }
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

  return 0;
}