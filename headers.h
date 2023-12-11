#ifndef _HEADERS_H
#define _HEADERS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <grp.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <limits.h>
#define RESET       "\e[0m"
#define GREEN       "\e[32m"      
#define BLUE        "\e[34m"      
#define BOLDMAGENTA "\e[1m\e[35m"      
#define BOLDWHITE   "\e[1m\e[37m"      
#define MAX 1000

int cmd_echo(char **cmds, int a);
int cmd_pwd(char** cmds);
int cmd_cd(char **cmds, char **prev, char *curr, char *home, int a);
int find_sl(char* str);
int cmd_ls(char** cmds, int a);
int cmf(const struct dirent** a, const struct dirent** b);
int ls_l(char *loc, char* path, int flag);
int ls(char* loc, int a);
int cmd_pinfo(char **cmds, char* home);
int cmd_discover(char** cmds,char* home);
int find(char* dir_name, int f, int d);
int search(char* dir_name, char* file, int f);
int cmd_type(char* tkn, int a);
int cmd_history(char* cmd, int a);
int swap_store(char** cmds, char* cmd, char* path);
int num_cmds(char** cmds, char* path);
int cmd_spl(char** cmds, int a);
void handler(int a);
int cmd_iodi(char** cmds, int* in, int* out);

#endif