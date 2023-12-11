#include "headers.h"

void handler(int a)
{
  if (a != SIGCHLD) return;

  int pid, ppid, pid_l[MAX];
  char st, exe[MAX], *exe_l[MAX];
  
  struct dirent *ent; 
  DIR *dir = opendir("/proc");
  if (dir == NULL) 
  {
    perror("Error");
    return;
  }
  
  int l = 0;
  while((ent = readdir(dir)) != NULL)
  {
     if((ent->d_name[0] <= '9' && ent->d_name[0] >= '0') || !strcmp(ent->d_name, "self"))
     {
       char trans[MAX] = "/proc/";
       strcat(trans, ent->d_name);
       strcat(trans, "/stat");
       
       FILE *file = fopen(trans, "r");
       if (file != NULL)
       {
          fscanf(file, "%d %s %*s %d", &pid, exe, &ppid);
          if(ppid == getpid())
          {
            pid_l[l] = pid;
            exe_l[l] = calloc(strlen(exe) + 1, sizeof(char));
            strcpy(exe_l[l++], exe);
          }
        }
        fclose(file);
      }
  }
  closedir(dir);
  
  int ret, flag;
  while((pid = waitpid(-1, &ret, WNOHANG)) > 0)
  {
     flag = 0;
     if (WIFEXITED(ret)) flag = 1;
     
     strcpy(exe, "\0");
     
     int j = 0;
     while(j < l)
     {
       if (pid_l[j] == pid)
       {
         strcpy(exe, exe_l[j++]);
         exe[strlen(exe) - 1] = '\0';
         break;
       }   
     }
     if (flag) printf("\n%s with pid = %d exited normally\n", &exe[1], pid);        
     else printf("\n%s with pid = %d exited abnormally\n", &exe[1], pid);    
  }
}