# Neoshell 

A simple shell implemented in C, built for learning purposes to explore how shells work under the hood.

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
- Child process calls `execvp()` to replace itself with the new program allowing the shell can continue running independently
- Parent process waits with `waitpid()` until child finishes
- This allows the shell to run any system command like `vim` or `fzf` while staying interactive
- This demonstrates multiprocessing: multiple independent processes running in parallel, each with their own memory space.

## How to Run

```bash
make
./neoshell
```

## Commands

**Builtin commands:**
`cd`, `ls`, `mkdir`, `rmdir`, `touch`, `rm`, `cat`, `cp`, `mv`, `rn`, `help`, `exit`

**Path shortcuts:**
- `add_path shortname /some/long/path ` - save a path
- `ac shortname` - jump to saved path
- `rm_path shortname` - remove saved path

**External commands work automatically** - anything not in the builtin list gets forked and executed (explained above)

## File Structure
- `main.c` - main loop, initialization
- `functions.c` - all the command implementations  
- `header.h` - declarations and color definitions

