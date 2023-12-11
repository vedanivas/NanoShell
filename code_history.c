#include "headers.h"

int num_cmds(char** cmds, char* path)
{
  FILE* file = fopen(path, "r");
  
  int i = 0;
  if (file == NULL) return i;
  
  while(fscanf(file, "%[^\n]s", cmds[i++]) != EOF) fseek(file, 1, 1);
  fclose(file);
  
  return (i - 1);
}

int swap_store(char** cmds, char* cmd, char* path)
{
  for (int i = 0; i < 19; i++) strcpy(cmds[i], cmds[i + 1]);
  strcpy(cmds[19], cmd);
  

  FILE* file = fopen(path, "w");
  if (file == NULL)
  {
    perror("Error");
    return 1;
  }
  for (int i = 0; i < 20; i++) fprintf(file, "%s\n", cmds[i]);
  fclose(file);
  
  return 0;
}

int cmd_history(char* cmd, int a)
{ 
  cmd[strlen(cmd) - 1] = '\0';
  
  char* path = calloc(MAX, sizeof(char));
  if (path == NULL)
  {
    perror("Error");
    return 1;
  }
  getcwd(path, MAX);
  strcat(path, "/history.txt");

  char **cmds = calloc(21, sizeof(char *));
  if (cmds == NULL)
  {
    perror("Error");
    return 1;
  }
  for (int i = 0; i < 21; i++)
  {
    cmds[i] = calloc(MAX, sizeof(char));
    if (cmds[i] == NULL)
    {
      perror("Error");
      return 1;
    }
  }
  
  int cnt = num_cmds(cmds, path);

  if (!a)
  {
    if (cnt != 0 && !strcmp(cmds[cnt - 1], cmd)) return 0;
    if (cnt >= 20) swap_store(cmds, cmd, path);
    else
    {
      FILE* file = fopen(path, "a");
      fprintf(file, "%s\n", cmd);
      fclose(file);
    }
  }
  if (a)
  {
    int in = -1, out = 9;
    if (cmd_iodi(cmds, &in, &out)) return 1;

    if (cnt <= 10) for (int i = 0; i < cnt; i++) printf("%s\n", cmds[i]);
    else for (int i = cnt - 10, j = 0; j < 10; j++) printf("%s\n", cmds[i++]);

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
  }

  return 0; 
}