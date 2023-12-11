#include "headers.h"

int cmd_pwd(char** cmds)
{
  int in = -1, out = 9;
  if (cmd_iodi(cmds, &in, &out)) return 1;

  char *path = calloc(MAX, sizeof(char));
  if (path == NULL)
  {
    perror("Error");
    return 1;
  }
  getcwd(path, MAX);
  printf("%s\n", path);
  
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