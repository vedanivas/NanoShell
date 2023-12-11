#include "headers.h"

int cmd_type(char* tkn, int a)
{
  if (a && tkn[0] == '\"')
  {
    strcpy(tkn, tkn + 1);
    tkn[strlen(tkn)  - 1] = '\0';
  }

  struct stat st;
  int er_flag = stat(tkn, &st);
  
  if (!strcmp("-f", tkn)) return 0;
  else if (!strcmp("-d", tkn)) return 1;
  else if ((er_flag != -1 && S_ISDIR(st.st_mode))) return 2;
  else if ((tkn[0] == '.') || (tkn[0] == '~')) return 2;
  else return 3;
}

int search(char* dir_name, char* file, int f)
{
    if(f) return 0;
    
    struct dirent *ent;
    DIR *dir = opendir(dir_name);

    int er_flag;
    struct stat st;
    while ((ent = readdir(dir)) != NULL)
    {
      char *trans = calloc(MAX, sizeof(char));
      if (trans == NULL)
      {
        perror("Error");
        return 1;
      }
      strcpy(trans, dir_name);
      strcat(trans, ent->d_name);
      if (trans[strlen(trans) - 1] != '/') strcat(trans, "/");
      er_flag = stat(trans, &st);
      if (strcmp(ent->d_name, ".") && strcmp(ent->d_name, "..") && (er_flag != -1) && S_ISDIR(st.st_mode)) search(trans, file, f);
      else if (!strcmp(ent->d_name, file))
      {
        printf("%s%s\n", dir_name, file);
        f = 1;
      }
    }
    closedir(dir);
}

int find(char* dir_name, int f, int d)
{
  struct dirent *ent;
  DIR *dir = opendir(dir_name);
  
  int er_flag;
  struct stat st;
  while ((ent = readdir(dir)) != NULL)
  {
    char *trans = calloc(MAX, sizeof(char));
    if (trans == NULL)
    {
      perror("Error");
      return 1;
    }
    strcpy(trans, dir_name);
    strcat(trans, ent->d_name);
    er_flag = stat(trans, &st);
    if (strcmp(ent->d_name, ".") && strcmp(ent->d_name, "..") && er_flag != -1 && S_ISDIR(st.st_mode))
    {
      char *senti = calloc(MAX, sizeof(char));
      if (senti == NULL)
      {
        perror("Error");
        return 1;
      }
      strcpy(senti, trans);
      if (trans[strlen(trans) - 1] != '/') strcat(trans, "/");
      if (d) printf("%s\n", senti);
      find(trans, f, d);
    }
    else if(strcmp(ent->d_name, ".") && strcmp(ent->d_name, "..") && f) printf("%s\n",trans);
  }
  
  closedir(dir);
}

int cmd_discover(char** cmds, char* home)
{
  int in = -1, out = 9;
  if (cmd_iodi(cmds, &in, &out)) return 1;

  int f = 1, d = 1, k = 0, flag  = 0;
  char *req_file;
  char curr[MAX] = "./";
  for (int i = 1; cmds[i] != NULL; i++)
  {
    if (!strcmp(cmds[i], "-d"))
    {
      k = 1;
      break;
    }
  }
  for (int i = 1; cmds[i] != NULL; i++)
  {
    int senti = cmd_type(cmds[i], k);
    if (senti == 0)
    {
      if(f && d) d = 0;
      else if(!f && d) f=1;
    }
    else if (senti == 1)
    {
      if(f && d) f = 0; 
      else if(f && !d) d = 1;      
    }
    else if(senti == 2)
    {
      strcpy(curr, cmds[i]);
      if (cmds[i][0] == '~')
      {
        strcpy(curr, home);
        strcat(curr, cmds[i] + 1);
      }
        
      if(curr[strlen(curr) - 1] != '/') strcat(curr, "/");
    }
    else
    {
      req_file = calloc(strlen(cmds[i]), sizeof(char));
      if (req_file == NULL)
      {
        perror("Error");
        return 1;
      }
      strcpy(req_file, cmds[i] + 1);
      req_file[strlen(req_file) - 1] = '\0';
      f = d = 0;
      flag = 1;
    }
  }  
  if (!flag)
  {
    if (d) printf("%s\n", curr);    
    find(curr, f, d);
  }
  else search(curr, req_file, 0);
  
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