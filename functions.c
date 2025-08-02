#include "header.h"

char path[2*MAX_PATH_SIZE];

char *read_line(char *line, int *line_size){
    int c; 
    int i = 0;
    while((c = getchar()) != EOF && c != '\n'){
        line[i++] = c;
        if(i >= (*line_size) - 1){
            *line_size *= 2;
            line = realloc(line, sizeof(*line) * (*line_size));
            if (line == NULL) {
                fprintf(stderr, RED "Memory reallocation failed\n" RESET);
                exit(1);}
        }
        
    }
    line[i] = '\0';
    return line;
}

char **get_arguments(char *line, char **args, int *args_size){
    int new_word = 1,  counter = 0;
    int len = strlen(line);
    for(int i = 0; i<len; i++){
        if(line[i] != ' ' && new_word){
            args[counter++] = &line[i];
            if(counter >= (*args_size) - 1){
                *args_size *= 2;
                args = realloc(args, sizeof(*args) * (*args_size) );
                if (args == NULL){
                    fprintf(stderr, RED "Memory reallocation failed\n" RESET);
                    exit(1); }
            }
            new_word = 0;
        }else if(line[i] == ' ' && line[i-1] != ' ' && !new_word){
            new_word = 1;
            line[i] = '\0';
        }
    }
    args[counter] = NULL;
    return args;
}

int external_functions(char **args){
    pid_t pid, wpid;
    int status;
    pid = fork();
    if(pid < 0){
        fprintf(stderr, RED);
        perror("Fork failed ");
        fprintf(stderr, RESET);
    }else if(pid == 0){
        //Child process
        if(execvp(args[0], args) == -1){
            fprintf(stderr, RED);
            perror("Invalid command ");
            fprintf(stderr, RESET);
            exit(1);
        }
    }else{
        //parent process
        do{
            if(strcmp(args[0],"open") != 0){
            wpid = waitpid(pid, &status, WUNTRACED);}
        }while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

char *builtin_str[] = {
    "cd",
    "help",
    "exit",
    "rn",
    "rm",
    "touch",
    "ls",
    "mkdir",
    "rmdir",
    "cat",
    "add_path",
    "rm_path",
    "ac",
    "cp",
    "mv"
};



int (*builtin_functions[]) (char **) = {
    &cd,
    &help,
    &quit,
    &rn,
    &rm,
    &touch,
    &ls,
    &mkdir_command,
    &rmdir_command,
    &cat,
    &add_path,
    &rm_path,
    &ac,
    &cp,
    &mv
};

int num_builtins(){
    return sizeof(builtin_str) / sizeof(char *);
}

int execute(char **args){
    if (args[0] == NULL) {
        return 1;
    }
    for(int i = 0; i<num_builtins(); i++){
        if(strcmp(args[0], builtin_str[i]) == 0){
            return (*builtin_functions[i])(args);
        }
    }
    return external_functions(args);
}

int cd(char **args){
  if(args[1] == NULL){
    fprintf(stderr, RED "Too few arguments for command `cd`\n" RESET);
  }else{
    if (chdir(args[1]) != 0) {
        perror(RED "Invalid path " RESET);
    }
  }
  getcwd(path, sizeof(path));
  return 1;
}

int help(char **args) {
    printf(CYAN "List of builtin functions:\n" RESET);
    printf(YELLOW "\tcd [DIRECTORY]\n" RESET);
    printf(GRAY "\t\tChange the current directory to DIRECTORY.\n" RESET);
    printf(YELLOW "\thelp\n" RESET);
    printf(GRAY "\t\tDisplay this help message.\n" RESET);
    printf(YELLOW "\texit\n" RESET);
    printf(GRAY "\t\tExit the shell.\n" RESET);
    printf(YELLOW "\trn OLDFILENAME NEWFILENAME\n" RESET);
    printf(GRAY "\t\tRename OLDFILENAME to NEWFILENAME.\n" RESET);
    printf(YELLOW "\trm FILENAME\n" RESET);
    printf(GRAY "\t\tDelete the file named FILENAME.\n" RESET);
    printf(YELLOW "\ttouch FILENAME\n" RESET);
    printf(GRAY "\t\tCreate a new file named FILENAME.\n" RESET);
    printf(YELLOW "\tls\n" RESET);
    printf(GRAY "\t\tList the contents of the current directory.\n" RESET);
    printf(YELLOW "\tmkdir DIRECTORY\n" RESET);
    printf(GRAY "\t\tCreate a new directory named DIRECTORY.\n" RESET);
    printf(YELLOW "\trmdir DIRECTORY\n" RESET);
    printf(GRAY "\t\tRemove the directory named DIRECTORY.\n" RESET);
    printf(YELLOW "\tcat FILENAME\n" RESET);
    printf(GRAY "\t\tDisplay the content of the file named FILENAME.\n" RESET);
    printf(YELLOW "\tadd_path FULLPATH PATHNAME\n" RESET);
    printf(GRAY "\t\tCreates a shortcut using the file shortcuts.txt in ~/.shortcuts.txt\n" RESET);
    printf(YELLOW "\trm_path PATHNAME\n" RESET);
    printf(GRAY "\t\tRemoves a shorcut from shorcuts.txt by the PATHNAME\n" RESET);
    printf(YELLOW "\tac FULLPATH or ac *PATHNAME\n" RESET);
    printf(GRAY "\t\tAccess command, access by using the fullpath or more easily with *PATHNAME (created with path_add)\n" RESET);
    printf(YELLOW "\tcp FILENAME COPY_FILENAME\n" RESET);
    printf(GRAY "\t\tCopy the contents of FILENAME to COPY_FILENAME.\n" RESET);
    printf(YELLOW "\tmv FILENAME FULLPATH or mv FILENAME *PATHNAME\n" RESET);
    printf(GRAY "\t\tMove FILENAME using a FULLPATH or simply by *PATHNAME (added by add_path).\n" RESET);
    return 1;
}


int quit(char **args){
    return 0;
}


int rn(char **args){
    if(args[1] != NULL && args[2] != NULL){
        char *OLDFILENAME = custom_strcat(path, args[1]);
        if(!rename(OLDFILENAME, args[2])){
            printf(MAGENTA "File renamed successfully.\n" RESET);
        }else{
            fprintf(stderr, RED);
            perror("Failed to rename the file ");
            fprintf(stderr, RESET);}
        free(OLDFILENAME);
    }else fprintf(stderr, RED "Too few arguments for command `rn`\n" RESET);
    return 1;
}


int touch(char **args){
    if(args[1] != NULL){
        char *FILENAME = custom_strcat(path, args[1]);
        if(strlen(args[1]) > 4){
            FILE *f = NULL;
            if(strncmp(FILENAME + strlen(FILENAME) - 4, ".bin", 4) == 0)  f = fopen(FILENAME, "wb");
            else f = fopen(FILENAME, "w");
            
            if(f == NULL){
                fprintf(stderr, RED);
                perror("Unable to create the file ");
                fprintf(stderr, RESET);
                return 1;
            }
            fclose(f);

        }else{
            FILE *f = fopen(FILENAME, "w");
            if(f == NULL){
                fprintf(stderr, RED);
                perror("Unable to create the file ");
                fprintf(stderr, RESET);
                return 1;
            }
            fclose(f);
        }
        free(FILENAME);
        printf(MAGENTA "File created successfully.\n" RESET);
    }else fprintf(stderr, RED "Too few arguments for command `touch`\n" RESET);
    return 1;
}


int rm(char **args){
    if(args[1] != NULL){
        char *FILENAME = custom_strcat(path, args[1]);
        if(!remove(FILENAME)){
            printf(MAGENTA "File deleted successfully.\n" RESET);
        }else{
            fprintf(stderr, RED);
            perror("Failed to delete the file ");
            fprintf(stderr, RESET);}
        free(FILENAME);
    }else fprintf(stderr, RED "Too few arguments for command `rm`\n" RESET);
    return 1;
}

int ls(char **args) {
    if(args[1] == NULL){
        struct dirent *entry;
        DIR *dp = opendir(path);
        if (dp == NULL) {
            fprintf(stderr, RED "Could not open the directory\n" RESET);
            return 1;
        }

        while ((entry = readdir(dp))) {
            if (entry->d_name[0] != '.') {
                printf(GRAY "%s\n" RESET, entry->d_name);}
        }
        closedir(dp);
    }else fprintf(stderr, RED "Too many arguments for command `ls`\n" RESET);
    return 1;
}

int mkdir_command(char **args) {
    if(args[1] != NULL){
        if (mkdir(args[1], 0755) != 0){
                fprintf(stderr, RED);
                perror("Failed to create the directory ");
                fprintf(stderr, RESET);
        }else printf(MAGENTA "Directory created successfully.\n" RESET);
    }else fprintf(stderr, RED "Too few arguments for command `rmdir`\n" RESET);
    return 1;
}

int rmdir_command(char **args) {
    if(args[1] != NULL){
        if (rmdir(args[1]) != 0){
            fprintf(stderr, RED);
            perror("Failed to delete the directory ");
            fprintf(stderr, RESET);
        }else printf(MAGENTA "Directory deleted successfully.\n" RESET);
    }else fprintf(stderr, RED "Too few arguments for command `rmdir`\n" RESET);
    return 1;
}

int cat(char **args){
    if(args[1] == NULL){
        fprintf(stderr, RED "Too few arguments for command `cat`\n" RESET);
    }else{
        int count = 0;
        while (args[count] != NULL){
            count++;
        }
        char *FILENAME; FILE *f;
        int byte; char line[1024];
        for(int i = 1; i<count; i++){
            FILENAME = custom_strcat(path, args[i]);
            if(strncmp(FILENAME + strlen(FILENAME) - 4, ".bin", 4) == 0){
                f = fopen(FILENAME, "rb");
                if(f == NULL){
                    fprintf(stderr, RED "Unable to open the file `%s`\n" RESET, args[i]);
                }else{
                    while ((byte = fgetc(f)) != EOF) {
                        printf("%02X", byte);
                    }
                    fclose(f);
                }
            }else{
                f = fopen(FILENAME, "r");
                printf("FILENAME = %s\n", FILENAME);
                if(f == NULL){
                    fprintf(stderr, RED "Unable to open the file `%s`\n" RESET, args[i]);
                }else{
                    while (fgets(line, sizeof(line), f) != NULL) {
                        printf("%s", line);
                    }
                    fclose(f);
                }   
            }
            
        }
        free(FILENAME);
    }
    return 1;
}


int add_path(char **args){
   if(args[1] == NULL || args[2] == NULL){
       fprintf(stderr, RED "Too few arguments for command `add_path`\n" RESET);
   }else{
       char shortcuts[500];
       uid_t uid = getuid();
       struct passwd *pw;
       pw = getpwuid(uid);
       
       if(pw == NULL){
           perror(RED "Error getting user info" RESET);
           return 1;
       }
       
       memset(shortcuts, 0, sizeof(shortcuts));
       strcpy(shortcuts, "/home/");
       strcat(shortcuts, pw->pw_name);
       strcat(shortcuts, "/.shortcuts.txt");
       FILE *f = fopen(shortcuts, "a");
       if(f == NULL){
           perror(RED "Unable to open `shortcuts.txt`" RESET);
       }else{
           if(strlen(args[1]) > MAX_PATH_SIZE || strlen(args[2]) > MAX_PATHNAME_SIZE){
               fprintf(stderr, RED "The fullpath or the pathname is too long\n" RESET);
               fclose(f);
           }else{
               fprintf(f, "%s\t%s\n", args[1], args[2]);
               fclose(f);
               printf(MAGENTA "Path added successfully.\n" RESET);
           }
       }
   }
   
   return 1;
}


int rm_path(char **args){
    if(args[1] == NULL){
        fprintf(stderr, RED "Too few arguments for command `rm_path`\n" RESET);
    }else{
        char shortcuts[500];
        char temp_path[500];  
        struct passwd *pw;
        uid_t uid = getuid();
        pw = getpwuid(uid);
        
        if(pw == NULL){  
            perror(RED "Error getting user info" RESET);
            return 1;
        }
        
        memset(shortcuts, 0, sizeof(shortcuts));
        strcpy(shortcuts, "/home/");  
        strcat(shortcuts, pw->pw_name);  
        strcpy(temp_path, shortcuts);   
        strcat(shortcuts, "/.shortcuts.txt");
        strcat(temp_path, "/.temp.txt");
        
        FILE *f = fopen(shortcuts, "r");
        if(f == NULL){
            perror(RED "Error " RESET);
            return 1;
        }
        FILE *temp_file = fopen(temp_path, "w");
        if(temp_file == NULL){
            perror(RED "Error " RESET);
            fclose(f);
            return 1;
        }
        Pa p;
        while(fscanf(f, "%s\t%s\n", p.FULLPATH, p.PATHNAME) == 2){
            if(strcmp(p.PATHNAME, args[1]) != 0){
                fprintf(temp_file, "%s\t%s\n", p.FULLPATH, p.PATHNAME);
            }
        }
        fclose(f);
        fclose(temp_file);
        remove(shortcuts);
        rename(temp_path, shortcuts);
        printf(MAGENTA "Path deleted successfully.\n" RESET);
    }
    return 1;
}

int ac(char **args){
    if(args[1] == NULL){
        fprintf(stderr, RED "Too few arguments for command `acc`\n" RESET);
    }else{
        if(*args[1] != '*'){
            if(chdir(args[1]) != 0){
                perror(RED "Invalid path " RESET);
            }else{
                getcwd(path, sizeof(path));
            }    
        }else{
            access_byshortcut(args[1]);
        }
    }
    return 1;
}


//cp FILENAME COPYFILE_NAME
int cp(char **args){
    if(args[1] == NULL || args[2] == NULL){
        fprintf(stderr, RED "Too few arguments for command `cp`\n" RESET);
    }else{
        char *FILENAME = custom_strcat(path, args[1]);
        char *COPY_FILENAME = custom_strcat(path, args[2]);
        if(copy_file(FILENAME, COPY_FILENAME)){
            printf(MAGENTA "File copied successfully.\n" RESET); 
        }
        free(FILENAME);
        free(COPY_FILENAME);
    }

    return 1;
}


int mv(char **args){
    if(args[1] == NULL || args[2] == NULL){
        fprintf(stderr, RED "Too few arguments for command `mv`\n" RESET);
    }else{
        char *FILENAME = custom_strcat(path, args[1]);
        if(*args[2] == '*'){
            access_byshortcut(args[2]);
        }else{
            if(chdir(args[2]) != 0){
                perror(RED "Invalid path " RESET);
                return 1;
            }
            getcwd(path, sizeof(path));
        }
        char *extension = get_extension(args[1]);
        char temp[40] = "temp";
        if(extension != NULL){
            strcat(temp, extension);
            free(extension);}
        if(copy_file(FILENAME, temp)){
            remove(FILENAME);
            FILENAME = custom_strcat(path, args[1]);
            rename(temp, args[1]);     
            free(FILENAME);
            printf(MAGENTA "File moved successfully.\n" RESET); 
        }
          
    }
    return 1;
}




char *custom_strcat(char *str1, char *str2){
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    char *str = malloc(sizeof(*str) * (len1 + len2 + 1) );
    for(int i = 0; i<len1; i++) str[i] = str1[i];
    str[len1] = '/';
    for(int i = 0; i<len2; i++) str[len1 + 1 + i] = str2[i];
    str[len1 + len2 + 1] = '\0';
    return str;
}

void display_username_hostname(){
    //Username
    struct passwd *pw;
    uid_t uid;

    uid = getuid();
    pw = getpwuid(uid);
    if (pw != NULL) {
        printf(GREEN "%s@", pw->pw_name);
    } else {
        fprintf(stderr, RED);
        perror("Unable to get username ");
        fprintf(stderr, RESET);
    }
    //Hostname
    char hostname[1024];  
    int result;
    result = gethostname(hostname, sizeof(hostname));
    if (result == 0) {
        printf("%s:" RESET, hostname);
    } else {
        fprintf(stderr, RED);
        perror("Unable to get hostname ");
        fprintf(stderr, RESET);
    }
}

int copy_file(char *FILENAME, char *COPY_FILENAME){
    FILE *f = NULL;
    FILE *f_copy = NULL;

    if (strlen(FILENAME) > 4) {
        if (strncmp(FILENAME + strlen(FILENAME) - 4, ".bin", 4) == 0) {
            // Binary file
            f = fopen(FILENAME, "rb");
            if (f == NULL) {
                perror(RED "Unable to open the file " RESET);
                return 1;
            }
            f_copy = fopen(COPY_FILENAME, "wb");
            if (f_copy == NULL) {
                perror(RED "Unable to create the copy file " RESET);
                fclose(f);
                return 1;
            }
            // Copy
            char buffer[1024];
            size_t bytes;
            while ((bytes = fread(buffer, 1, sizeof(buffer), f)) > 0) {
                fwrite(buffer, 1, bytes, f_copy);
            }
            fclose(f);
            fclose(f_copy);
            return 1;
        }
    }
    // Not a binary file
    f = fopen(FILENAME, "r");
    if (f == NULL) {
        perror(RED "Unable to open the file " RESET);
        return 1;
    }
    f_copy = fopen(COPY_FILENAME, "w");
    if (f_copy == NULL) {
        perror(RED "Unable to create the copy file " RESET);
        fclose(f);
        return 1;
    }
    // Copy
    char line[1024];
    while (fgets(line, sizeof(line), f) != NULL) {
        fprintf(f_copy, "%s", line);
    }
    fclose(f);
    fclose(f_copy);
    return 1;
}

void access_byshortcut(char *arg){
   char shortcuts[500];
   uid_t uid = getuid();
   struct passwd *pw;
   pw = getpwuid(uid);
   
   if(pw == NULL){
       perror(RED "Error getting user info" RESET);
       return;
   }
   
   memset(shortcuts, 0, sizeof(shortcuts));
   strcpy(shortcuts, "/home/");
   strcat(shortcuts, pw->pw_name);
   strcat(shortcuts, "/.shortcuts.txt");
   
   FILE *f = fopen(shortcuts, "r");
   if(f == NULL){
       perror(RED "Unable to open `paths.txt`" RESET);
   }else{
       Pa p;
       int found = 0;
       while(fscanf(f, "%s\t%s\n", p.FULLPATH, p.PATHNAME) == 2){
           if(strcmp(p.PATHNAME, arg + 1) == 0){
               if(chdir(p.FULLPATH) != 0){
                   perror(RED "Invalid path " RESET);
               }else{
                   getcwd(path, sizeof(path));
               }
               found = 1;
               break;
           }
       }
       if(!found){
           fprintf(stderr, RED "Could not find `%s` path\n" RESET, arg + 1);
       }
       fclose(f);
   }
}

char *get_extension(char *arg) {
    size_t len = strlen(arg);
    const char *dot = strrchr(arg, '.');
    
    if (!dot || dot == arg) {
        return NULL;
    }
    
    char *ext = malloc(strlen(dot) + 1); 
    
    if (ext) {
        strcpy(ext, dot); 
    }
    
    return ext;
}



