#include "headers.h"

int cmd_iodi(char** cmds, int* in, int* out)
{
  int i = 0, fd;
  while(cmds[i] != NULL)
  {
    if (!strcmp(cmds[i], ">") || !strcmp(cmds[i], ">>"))
    {
      if (cmds[i + 1] == NULL)
      {
        printf("Invalid syntax\n");
        return 1;
      }
      fd = open(cmds[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if (!strcmp(cmds[i], ">>")) fd = open(cmds[i + 1], O_WRONLY | O_APPEND | O_CREAT, 0644);
      if (fd < 0)
      {
        perror("Error");
        return 1;
      }

      if (dup2(STDOUT_FILENO, *out) < 0)
      {
        perror("Error");
        return 1;
      }
      if (dup2(fd, STDOUT_FILENO) < 0)
      {
        perror("Error");
        return 1;
      }
      close(fd);
    }
    if (!strcmp(cmds[i], "<"))
    {
      if (cmds[i + 1] == NULL)
      {
        printf("Invalid syntax\n");
        return 1;
      }
      fd = open(cmds[i + 1], O_RDONLY);
      if (fd < 0)
      {
        perror("Error");
        return 1;
      }

      if (dup2(STDIN_FILENO, *in) < 0)
      {
        perror("Error");
        return 1;
      }
      if (dup2(fd, STDIN_FILENO) < 0)
      {
        perror("Error");
        return 1;
      }
      close(fd);
    }
    i++;
  }

  return 0;
}