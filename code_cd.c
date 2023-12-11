#include "headers.h"

int find_sl(char* str)
{
  int i = strlen(str);
  while(str[i--] != '/' && i) {}

  return i == 0 ? i : (i + 2);
}

int cmd_cd(char **cmds, char **prev, char *curr, char *home, int a)
{
  char *trans = calloc(MAX, sizeof(char));
  if (trans == NULL)
  {
    perror("Error");
    return 1;
  }

  if (a == 2)
  {
    if (chdir(home) == -1)
    {
      perror("Error");
      return 1;
    }
    strcpy(*prev, curr);
  }
  if (a == 3)
  {
    if (cmds[1][0] == '.')
    {
      if (cmds[1][1] == '/')
      {
        strcpy(trans, curr);
        strcat(trans, &cmds[1][1]);
        if (chdir(trans) == -1)
        {
          perror("Error");
          return 1;
        }
        strcpy(*prev, curr);
      }
      else if (cmds[1][1] == '\0') strcpy(*prev, curr);
      else if (cmds[1][1], '.')
      {
        int a = find_sl(curr);
        strncpy(trans, &curr[0], a - 1);
        if (cmds[1][2] == '/') strcat(trans, &cmds[1][2]);
        if (chdir(trans) == -1)
        {
          perror("Error");
          return 1;
        }
        strcpy(*prev, curr);
      }
    }
    else if (cmds[1][0] == '~')
    {
      strcpy(trans, home);
      if (cmds[1][1] == '/') strcat(trans, &cmds[1][1]);
      if (chdir(trans) == -1)
      {
        perror("Error");
        return 1;
      }
      strcpy(*prev, curr);
    }
    else if (cmds[1][0] == '-')
    {
      printf("%s\n", *prev);
      if (chdir(*prev) == -1)
      {
        perror("Error");
        return 1;
      }
      strcpy(*prev, curr);
    }
    else
    {
      strcpy(trans, curr);
      strcat(trans, "/");
      strcat(trans, cmds[1]);
      if(chdir(trans) == -1)
      {
        perror("Error");
        return 1;
      }
      strcpy(*prev, curr);
    }
  }

  return 0;
}