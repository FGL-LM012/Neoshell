# Neoshell 

A simple shell implemented in C, created to explore how shells work under the hood.

## Concepts

**Command Parsing & Tokenization**
- `read_line()` - reads user input with dynamic buffer resizing
- `get_arguments()` - tokenizes input into command and arguments by replacing spaces with null terminators

**The Heart of the Shell - Command Execution**
```c
int execute(char **args){
    // Check if it's a builtin command first
    for(int i = 0; i<num_builtins(); i++){
        if(strcmp(args[0], builtin_str[i]) == 0){
            return (*builtin_functions[i])(args);
        }
    }
    // If not builtin, execute as external command
    return external_functions(args);
}
```

**Function Pointer Arrays** - Clean way to map command strings to their functions:
```c
char *builtin_str[] = {"cd", "help", "exit", "rm", ...};
int (*builtin_functions[]) (char **) = {&cd, &help, &quit, &rm, ...};
```

**Process Management** - How external commands actually work:
- `fork()` creates a child process
- Child process calls `execvp()` to replace itself with the new program that way the shell can continue running
- Parent process waits with `waitpid()` until child finishes
- This is why you can run any system command like `nvim`, `fzf`...

## How to Run

```bash
make
./neoshell
```

## Commands

**Built-ins I implemented:**
`cd`, `ls`, `mkdir`, `rmdir`, `touch`, `rm`, `cat`, `cp`, `mv`, `rn`, `help`, `exit`

**Path shortcuts:**
- `add_path shortname /some/long/path ` - save a path
- `ac shortname` - jump to saved path
- `rm_path shortname` - remove saved path

**External commands work automatically** - anything not in the builtin list gets forked and exec'd (explained above)

## File Structure
- `main.c` - main loop, initialization
- `functions.c` - all the command implementations  
- `header.h` - declarations and color definitions

