#include "headers.h"

int main()
{
  int er_flag;
  
  char *h_name = calloc(HOST_NAME_MAX, sizeof(char));
  if (h_name == NULL)
  {
    perror("Error");
    return 1;
  }
  er_flag = gethostname(h_name, HOST_NAME_MAX);
  if (er_flag)
  {
    perror("Error");
    return 1;
  }
  char *u_name = calloc(LOGIN_NAME_MAX, sizeof(char));
  if (u_name == NULL)
  {
    perror("Error");
    return 1;
  }
  er_flag = getlogin_r(u_name, LOGIN_NAME_MAX);
  if (er_flag)
  {
    perror("Error");
    return 1;
  }
  char *home = calloc(MAX, sizeof(char)); 
  if (home == NULL)
  {
    perror("Error");
    return 1;
  }
  char *prev = calloc(MAX, sizeof(char));
  if (prev == NULL)
  {
    perror("Error");
    return 1;
  }
  
  signal(SIGCHLD, handler);

  int tm = 0;
  getcwd(home, MAX);     /* ####################   HANDLE ERROR HERE!!!!!! ####################### */
  strcpy(prev, home);
  int len = strlen(home);
  for (;;)
  {
    char *cmd_main = calloc(MAX, sizeof(char));
    if (cmd_main == NULL)
    {
      perror("Error");
      return 1;
    }
    char *path = calloc(MAX, sizeof(char));
    if (path == NULL)
    {
      perror("Error");
      return 1;
    }
    char dlm[4] = " \t\n";
    getcwd(path, MAX);          /* ####################   HANDLE ERROR HERE!!!!!! ####################### */
    
    int bgn = 0, ind = 0, end = 0;
    if (strlen(path) >= len) 
    {
      if (tm) printf(BOLDMAGENTA "<%s@%s" RESET BOLDWHITE ":~took %ds" RESET BOLDMAGENTA"%s> " RESET , u_name, h_name, tm, &path[len]);
      else printf(BOLDMAGENTA "<%s@%s:~%s> " RESET , u_name, h_name, &path[len]);
    }
    else 
    {
      if (tm) printf(BOLDMAGENTA "<%s@%s" RESET BOLDWHITE ":~took %ds" RESET BOLDMAGENTA"%s> " RESET, u_name, h_name, tm, path);
      else printf(BOLDMAGENTA "<%s@%s:~%s> " RESET, u_name, h_name, path);
    }
    
    fgets(cmd_main, MAX, stdin);
    cmd_history(cmd_main, 0);
    strcat(cmd_main, "\n");
  
    while(cmd_main[ind] != '\0')
    {
      for (;;)
      {
        if (cmd_main[ind] == ';' || cmd_main[ind] == '\n') 
        {
          end = ind;
          break;
        }
        else if (cmd_main[ind] == '&')
        {
          end = ind + 1;
          break;
        }
        ind++;
      }
      if (bgn == ind) 
      {
        ind++;
        continue;
      }
      ind++;
        
      char *cmd_m = calloc(MAX, sizeof(char));
      if (cmd_m == NULL)
      {
        perror("Error");
        return 1;
      }
      strncpy(cmd_m, &cmd_main[bgn], end - bgn);
      bgn = ind;

      int ini = 0, mv = 0, cnt = 0;
      // while(cmd_m[mv] != '\0')
      // {

      // }

      int num_cmds = 0;
      char **cmd_tkns = calloc(MAX / 10, sizeof(char *));
      if (cmd_tkns == NULL)
      {
        perror("Error");
        return 1;
      } 
      cmd_tkns[num_cmds++] = strtok(cmd_m, dlm);
      while(cmd_tkns[num_cmds - 1] != NULL) cmd_tkns[num_cmds++] = strtok(NULL, dlm);

      tm = 0;
      if (!strcmp(cmd_tkns[0], "echo")) cmd_echo(cmd_tkns, num_cmds);
      else if (!strcmp(cmd_tkns[0], "pwd")) cmd_pwd(cmd_tkns);
      else if (!strcmp(cmd_tkns[0], "cd")) cmd_cd(cmd_tkns, &prev, path, home, num_cmds);
      else if (!strcmp(cmd_tkns[0], "ls")) cmd_ls(cmd_tkns, num_cmds);
      else if (!strcmp(cmd_tkns[0], "pinfo")) cmd_pinfo(cmd_tkns, home);
      else if (!strcmp(cmd_tkns[0], "discover")) cmd_discover(cmd_tkns, home);
      else if (!strcmp(cmd_tkns[0], "history")) cmd_history(cmd_m, 1);
      else if (!strcmp(cmd_tkns[0], "exit")) return 0;
      else tm = cmd_spl(cmd_tkns, num_cmds);
    }
  }

  return 0;
}