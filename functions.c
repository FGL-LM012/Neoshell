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
            wpid = waitpid(pid, &status, WUNTRACED);
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
    printf(CYAN BOLD "=== BUILTIN SHELL COMMANDS ===\n" RESET);
    printf("\n");
    
    // File and Directory Operations
    printf(YELLOW BOLD "📁 FILE & DIRECTORY OPERATIONS:\n" RESET);
    printf(GREEN "  ls" RESET " [path]           - List directory contents (current dir if no path)\n");
    printf(GREEN "  mkdir" RESET " <name>        - Create a new directory\n");
    printf(GREEN "  rmdir" RESET " <name>        - Remove an empty directory\n");
    printf(GREEN "  touch" RESET " <filename>    - Create a new empty file\n");
    printf(GREEN "  rm" RESET " <filename>       - Delete a file\n");
    printf(GREEN "  cat" RESET " <file1> [file2...] - Display file contents\n");
    printf("\n");
    
    // File Management
    printf(YELLOW BOLD "🔄 FILE MANAGEMENT:\n" RESET);
    printf(GREEN "  cp" RESET " <source> <dest>  - Copy file from source to destination\n");
    printf(GREEN "  mv" RESET " <source> <dest>  - Move/rename file or directory\n");
    printf(GREEN "  rn" RESET " <old> <new>      - Rename file (supports path/old_name new_name)\n");
    printf("\n");
    
    // Navigation
    printf(YELLOW BOLD "🧭 NAVIGATION:\n" RESET);
    printf(GREEN "  cd" RESET " <path>           - Change to specified directory\n");
    printf(GREEN "  ac" RESET " <shortcut>       - Access directory using saved shortcut\n");
    printf("\n");
    
    // Path Shortcuts
    printf(YELLOW BOLD "🔗 PATH SHORTCUTS:\n" RESET);
    printf(GREEN "  add_path" RESET " <path> <name> - Save a path with a shortcut name\n");
    printf(GREEN "  rm_path" RESET " <name>        - Remove a saved path shortcut\n");
    printf(GRAY "  " RESET "Shortcuts are stored in ~/.shortcuts.txt\n");
    printf("\n");
    
    // System
    printf(YELLOW BOLD "⚙️  SYSTEM:\n" RESET);
    printf(GREEN "  help" RESET "                 - Show this help message\n");
    printf(GREEN "  exit" RESET "                 - Exit the shell\n");
    printf("\n");
    
    // Examples
    printf(CYAN BOLD "💡 EXAMPLES:\n" RESET);
    printf(MAGENTA "  ls /home/user" RESET "       # List contents of /home/user\n");
    printf(MAGENTA "  cp file.txt backup.txt" RESET " # Copy file.txt to backup.txt\n");
    printf(MAGENTA "  add_path /home/user/docs mydocs" RESET " # Save shortcut\n");
    printf(MAGENTA "  ac mydocs" RESET "           # Navigate using shortcut\n");
    printf(MAGENTA "  rn old.txt new.txt" RESET "  # Rename file\n");
    printf("\n");
    
    printf(BLUE "💭 External commands (like gcc, nano, etc.) are also supported!\n" RESET);
    
    return 1;
}


int quit(char **args){
    return 0;
}



// returns the index of the last occurance of a character in a string
int find(char *str, char c){
    int i=0;
    int c_index = -1;
    while(str[i] && i<MAX_PATH_SIZE){
        if(str[i] == c) c_index = i;
        i++;
    }
    return c_index;
}

/* Basically two possibilities :
    p1. rn old_name new_name
    p2. rn path/to/old_name new_name
*/
int rn(char **args){
    if(args[1] != NULL && args[2] != NULL){
        int fs_index = find(args[1], '/');
        char new_name[1000];
        memset(new_name, 0, sizeof(new_name));
        if(fs_index != -1){
            // handles p2
            strncpy(new_name, args[1], fs_index+1);
        }
        strcat(new_name, args[2]);
        if(!rename(args[1], new_name)){
            printf(MAGENTA "File renamed successfully.\n" RESET);
        }else{
            fprintf(stderr, RED);
            perror("Failed to rename the file ");
            fprintf(stderr, RESET);}
    }else fprintf(stderr, RED "Too few arguments for command `rn`\n" RESET);
    return 1;
}


int touch(char **args){
    if(args[1] != NULL){
        char FILENAME[MAX_PATH_SIZE];
        strcpy(FILENAME, args[1]);
        FILE *f = fopen(FILENAME, "w");
        if(f == NULL){
            fprintf(stderr, RED);
            perror("Unable to create the file ");
            fprintf(stderr, RESET);
            return 1;
        }
        fclose(f);
        printf(MAGENTA "File created successfully.\n" RESET);
    }else{
        fprintf(stderr, RED "Too few arguments for command `touch`\n" RESET);
    }
    return 1;
}


int rm(char **args){
    if(args[1] != NULL){
        char FILENAME[MAX_PATH_SIZE];
        strcpy(FILENAME, args[1]);
        if(!remove(FILENAME)){
            printf(MAGENTA "File deleted successfully.\n" RESET);
        }else{
            fprintf(stderr, RED);
            perror("Failed to delete the file ");
            fprintf(stderr, RESET);}
    }else fprintf(stderr, RED "Too few arguments for command `rm`\n" RESET);
    return 1;
}

int ls(char **args) {
    char *target_path;
    
    if(args[1] == NULL) {
        target_path = path; // use current directory
    } else {
        target_path = args[1]; // use provided path
    }
    
    struct dirent *entry;
    DIR *dp = opendir(target_path);
    if (dp == NULL) {
        fprintf(stderr, RED "Could not open the directory '%s'\n" RESET, target_path);
        return 1;
    }
    
    while ((entry = readdir(dp))) {
        if (entry->d_name[0] != '.') {
            printf(GRAY "%s\n" RESET, entry->d_name);
        }
    }
    closedir(dp);
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
        char FILENAME[MAX_PATH_SIZE]; FILE *f;
        int byte; char line[1024];
        for(int i = 1; i<count; i++){
            strcpy(FILENAME, args[i]);
            f = fopen(FILENAME, "r");  
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
            perror(RED "Error opening shortcuts file" RESET);
            return 1;
        }
        FILE *temp_file = fopen(temp_path, "w");
        if(temp_file == NULL){
            perror(RED "Error creating temp file" RESET);
            fclose(f);
            return 1;
        }
        
        Pa p;
        int found = 0;  
        while(fscanf(f, "%s\t%s\n", p.PATHNAME, p.FULLPATH) == 2){
            if(strcmp(p.PATHNAME, args[1]) == 0){
                found = 1;
            } else {
                fprintf(temp_file, "%s\t%s\n", p.FULLPATH, p.PATHNAME);
            }
        }
        fclose(f);
        fclose(temp_file);
        
        if(found) {
            remove(shortcuts);
            rename(temp_path, shortcuts);
            printf(MAGENTA "Path deleted successfully.\n" RESET);
        } else {
            remove(temp_path);  
            fprintf(stderr, RED "Shortcut '%s' not found.\n" RESET, args[1]);
        }
    }
    return 1;
}

int ac(char **args){
    if(args[1] == NULL){
        fprintf(stderr, RED "Too few arguments for command `ac`\n" RESET);
    }else{
        access_byshortcut(args[1]);
    }
    return 1;
}

int cp(char **args){
    if(args[1] == NULL || args[2] == NULL){
        fprintf(stderr, RED "Too few arguments for command `cp`\n" RESET);
    }else{
        if(copy_file(args[1], args[2])){
            printf(MAGENTA "File copied successfully.\n" RESET); 
        }
    }

    return 1;
}



int mv(char **args){
    if(args[1] == NULL || args[2] == NULL){
        fprintf(stderr, RED "Too few arguments for command `mv`\n" RESET);
    }else{
        
        // Check if destination is a directory
        struct stat dst_stat;
        if(stat(args[2], &dst_stat) == 0 && S_ISDIR(dst_stat.st_mode)) {
            char dst[MAX_PATH_SIZE];
            strcpy(dst, args[2]);
            // this way both mv x dir and mv x dir/ work
            if(dst[strlen(dst)-1] != '/') dst[strlen(dst)] = '/';
            int fs_index = find(args[1], '/');
            if(fs_index == -1){
                // args[1] only contains the filename
                strcat(dst, args[1]);
            }else{
                // we extract the filename only
                strcat(dst, args[1] + fs_index + 1);
            }
            if(rename(args[1], dst) == 0) {
                printf(MAGENTA "File moved successfully.\n" RESET);
            } else {
                fprintf(stderr, RED);
                perror("Failed to move file");
                fprintf(stderr, RESET);
            }
        } else {
            if(rename(args[1], args[2]) == 0) {
                printf(MAGENTA "File moved successfully.\n" RESET);
            }else{
                fprintf(stderr, RED);
                perror("Failed to move file");
                fprintf(stderr, RESET);
            }
        }
    }
    return 1;
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
       while(fscanf(f, "%s\t%s\n", p.PATHNAME, p.FULLPATH) == 2){
           if(strcmp(p.PATHNAME, arg) == 0){
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


