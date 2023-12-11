#include "headers.h"

int cmf(const struct dirent** a, const struct dirent** b)
{
  int l_a = strlen((*a)->d_name);
  char* s_a = calloc(l_a, sizeof(char));
  if (s_a == NULL)
  {
    perror("Error");
    return 1;
  }
  strcpy(s_a, (*a)->d_name);

  int l_b = strlen((*b)->d_name);
  char* s_b = calloc(l_b, sizeof(char));
  if (s_b == NULL)
  {
    perror("Error");
    return 1;
  }  
  strcpy(s_b, (*b)->d_name);

  if (s_a[0] >= 'A' && s_a[0] <= 'Z') s_a[0] = s_a[0] + 32;
  if (s_b[0] >= 'A' && s_b[0] <= 'Z') s_b[0] = s_b[0] + 32;
  if (strcmp(&s_a[0], &s_b[0]) > 0) return -1;
  if (strcmp(&s_a[0], &s_b[0]) == 0) return 0;
  if (strcmp(&s_a[0], &s_b[0]) < 0) return 1;
}

int ls_l(char *loc, char* path, int flag)
{
  struct stat st;
  struct group* gid;
  long long int blocks;

  char* u_name = calloc(MAX, sizeof(char));
  if (u_name == NULL)
  {
    perror("Error");
    return 1;
  }
  char* tm = calloc(MAX, sizeof(char));
  if (tm == NULL)
  {
    perror("Error");
    return 1;
  }

  if (stat(loc, &st) == 0)
  {
    if (S_ISREG(st.st_mode))
    {
      if (S_ISDIR(st.st_mode)) printf("d");
      else printf("-");
      if (st.st_mode & S_IRUSR) printf("r");
      else printf("-");
      if (st.st_mode & S_IWUSR) printf("w");
      else printf("-");
      if (st.st_mode & S_IXUSR) printf("x");
      else printf("-");
      if (st.st_mode & S_IRGRP) printf("r");
      else printf("-");
      if (st.st_mode & S_IWGRP) printf("w");
      else printf("-");
      if (st.st_mode & S_IXGRP) printf("x");
      else printf("-");
      if (st.st_mode & S_IROTH) printf("r");
      else printf("-");
      if (st.st_mode & S_IWOTH) printf("w");
      else printf("-");
      if (st.st_mode & S_IXOTH) printf("x");
      else printf("-");
      gid = getgrgid(st.st_gid);
      strncpy(tm, ctime(&st.st_mtime) + 4, 12);
      
      if (S_ISDIR(st.st_mode)) printf("%3lu %4s %8s %8ld %12s" BLUE "\t%s\n" RESET, st.st_nlink, u_name, gid->gr_name, st.st_size, tm, loc);
      else if ((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH))printf("%3lu %4s %8s %8ld %12s" BLUE "\t%s\n" RESET, st.st_nlink, u_name, gid->gr_name, st.st_size, tm, loc);
      else printf("%3lu %4s %8s %8ld %12s\t%s\n", st.st_nlink, u_name, gid->gr_name, st.st_size, tm, loc);
      
      return 0;
    }
  }
  chdir(loc);
  
  struct dirent *ent, **ent_l;
  DIR* dir = opendir(".");
  if (dir == NULL)
  {
    perror("Error");
    return 1;
  }
  blocks = 0;
  while ((ent = readdir(dir)) != NULL)
  {
    if (stat(ent->d_name, &st) == 0)
    {
      if (flag == 0) if (ent->d_name[0] == '.') continue;
      blocks += st.st_blocks;
    }
  }
  printf("total %lld\n", blocks / 2);
  
  int n = scandir(".", &ent_l, NULL, cmf);
  if (n == -1)
  {
    perror("scandir");
    return 1;
  }
  while (n--)
  {
    if (ent_l[n] == NULL) continue;
    if (flag == 0) if (ent_l[n]->d_name[0] == '.') continue;
    if (stat(ent_l[n]->d_name, &st) != 0)
    {
      perror("Error");
      continue;
    }
    if (S_ISDIR(st.st_mode)) printf("d");
    else printf("-");
    if (st.st_mode & S_IRUSR) printf("r");
    else printf("-");
    if (st.st_mode & S_IWUSR) printf("w");
    else printf("-");
    if (st.st_mode & S_IXUSR) printf("x");
    else printf("-");
    if (st.st_mode & S_IRGRP) printf("r");
    else printf("-");
    if (st.st_mode & S_IWGRP) printf("w");
    else printf("-");
    if (st.st_mode & S_IXGRP) printf("x");
    else printf("-");
    if (st.st_mode & S_IROTH) printf("r");
    else printf("-");
    if (st.st_mode & S_IWOTH) printf("w");
    else printf("-");
    if (st.st_mode & S_IXOTH) printf("x");
    else printf("-");
    gid = getgrgid(st.st_gid);
    strncpy(tm, ctime(&st.st_mtime) + 4, 12);     

    if (S_ISDIR(st.st_mode)) printf("%3lu %4s %8s %8ld %12s" BLUE "\t%s\n" RESET, st.st_nlink, u_name, gid->gr_name, st.st_size, tm, ent_l[n]->d_name);
    else if ((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH)) printf("%3lu %4s %8s %8ld %12s" GREEN "\t%s\n" RESET, st.st_nlink, u_name, gid->gr_name, st.st_size, tm, ent_l[n]->d_name);
    else printf("%3lu %4s %8s %8ld %12s\t%s\n", st.st_nlink, u_name, gid->gr_name, st.st_size, tm, ent_l[n]->d_name);
  }
    
  closedir(dir);
  chdir(path);
  
  return 0;
}

int ls(char* loc, int a)
{
  struct stat st;
  struct group *gp;
  if (stat(loc, &st) == 0)
  {
    if (S_ISREG(st.st_mode))
    {
      if ((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH)) printf(GREEN "%s\n" RESET, loc);
      else printf("%s\n", loc);
      return 0;
    }
  }
  
  struct dirent** ent_l;
  DIR *dir = opendir(loc);
  if (dir == NULL)
  {
    perror("Error");  
    return 1;
  }
  int n = scandir(loc, &ent_l, NULL, cmf);
  if (n == -1)
  {
    perror("Error");
    return 1;
  }
  while (n--)
  {
    if (ent_l[n] == NULL) continue;    
    if (!a) if (ent_l[n]->d_name[0] == '.') continue;   
    if (stat(ent_l[n]->d_name, &st) != 0)
    {
      perror("Error");
      continue;
    }
    if (S_ISDIR(st.st_mode)) printf(BLUE "%s\n" RESET, ent_l[n]->d_name);
    else if ((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH)) printf(GREEN "%s\n" RESET, ent_l[n]->d_name);
    else printf("%s\n", ent_l[n]->d_name);
  }
  
  closedir(dir);
  
  return 0;
}

int cmd_ls(char **cmds, int a)
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
  
  struct dirent** ent_l;
  struct stat st;
  a--;
  
  int f_v = 0, v = 0, f_cnt = 0;
  for (int i = 1; i < a; i++)
  {
    if (cmds[i][0] != '-') f_cnt++;  
    if (!strcmp(cmds[i], "-l")) v = 1;
    if (!strcmp(cmds[i], "-a")) f_v = 1;
    if (!strcmp(cmds[i], "-la") || !strcmp(cmds[i], "-al"))
    {
      f_v = 1;
      v = 1;
    }
  }

  if (!f_cnt)
  {
    if (v) ls_l(".", path, f_v);
    else ls(".", f_v);
  }
  else
  {
    for (int i = 1; i < a; i++)
    {
      if (cmds[i][0] != '-')
      {
        if (stat(cmds[i], &st) != 0)
        {
          perror("Error");
          continue;
        }
        if (f_cnt) printf("%s:\n", cmds[i]);
        if (v) ls_l(cmds[i], path, f_v);
        else ls(cmds[i], f_v);
      }
    }
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
