#include "headers.h"

int cmd_pinfo(char** cmds, char* home) 
{
  int in = -1, out = 9;
  if (cmd_iodi(cmds, &in, &out)) return 1;

  char status, file_name[MAX], dir[MAX] = "/proc/";
  char* path = calloc(MAX, sizeof(char));
  int pid, f_1, f_2, mem;

  if (cmds[1] != NULL) strcat(dir, cmds[1]);
  else strcat(dir, "self");

  strcpy(file_name, dir);
  
  strcat(file_name, "/stat");
  FILE* file = fopen(file_name, "r");
  if (file) fscanf(file, "%d %*s %c %*s %d %*s %*s %d", &pid, &status, &f_1, &f_2);
  else 
  {
    perror("Error");
    return 1;
  }
  fclose(file);
  
  strcpy(file_name, dir);
  strcat(file_name, "/statm");
  file = fopen(file_name, "r");
  if(file) fscanf(file, "%d", &mem);
  else 
  {
    perror("Error");
    return 1;
  }
  fclose(file);

  if (f_1 == f_2) printf("pid : %d\nProcess Status : %c+\nMemory : %d\n", pid, status, mem);
  else printf("pid : %d\nProcess Status : %c\nMemory : %d\n", pid, status, mem);

  strcpy(file_name, dir);
  strcat(file_name, "/exe");

  int len = readlink(file_name, path, MAX);
  if (len >= 0) 
  {
    int f_r = 0, i = 1;
   
    while(i++) 
    {
      if (home[--i] == '\0') 
      {
        f_r = 1;
        break;
      }
      if (len == i || path[i] != home[i++]) break;
    }
    if (f_r) 
    {
      path = path + strlen(home) - 1;
      path[0] = '~';
    }
    printf("Executable Path : %s\n", path);
  } 
  else 
  {
    perror("Error");
    return 1;
  }

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