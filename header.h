#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/stat.h>


#define DEFAULT_LINE_SIZE 300
#define DEFAULT_ARGS_SIZE 3

#define RESET "\x1b[0m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define YELLOW  "\x1b[33m"
#define CYAN    "\x1b[36m"
#define BOLD    "\033[1m"
#define GRAY "\x1b[90m"


#define MAX_PATH_SIZE 1000
#define MAX_PATHNAME_SIZE 100
extern char path[2*MAX_PATH_SIZE];

typedef struct{
    char FULLPATH[MAX_PATH_SIZE];
    char PATHNAME[MAX_PATHNAME_SIZE];
}Pa;



char *read_line(char *line, int *line_size);
char **get_arguments(char *line, char **args, int *args_size);
int external_functions(char **args);
int execute(char **args);

int cd(char **args);
int help(char **args);
int quit(char **args);
int rn(char **args);
int rm(char **args);
int touch(char **args);
int ls(char **args);
int mkdir_command(char **args);
int rmdir_command(char **args);
int cat(char **args);
int add_path(char **args);
int rm_path(char **args);
int ac(char **args);
int cp(char **args);
int mv(char **args);
void display_username_hostname();



char *custom_strcat(char *str1, char *str2);
int copy_file(char *FILENAME, char *COPY_FILENAME);
void access_byshortcut(char *arg);
char *get_extension(char *arg);


#endif
