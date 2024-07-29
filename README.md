## Table of Contents
- [Run the shell](#Run_the_shell)
- [Built-in commands](#Built-in_Commands)
- [Licence](#License)

## Run_the_shell
```bash
make
./neoshell
```

## Built-in Commands

- <span style="color: yellow;">cd</span> <span style="color: blue;">[DIRECTORY]</span><br>
    Change the current directory to DIRECTORY.<br><br>
- <span style="color: yellow;">help</span><br>
    Display documentation of all commands.<br><br>
- <span style="color: yellow;">quit</span><br>
    Exit the shell.<br><br>
- <span style="color: yellow;">rn</span> <span style="color: blue;">OLDFILENAME NEWFILENAME</span><br>
    Rename OLDFILENAME to NEWFILENAME.<br><br>
- <span style="color: yellow;">rm</span> <span style="color: blue;">FILENAME</span><br>
    Delete the file named FILENAME.<br><br>
- <span style="color: yellow;">touch</span> <span style="color: blue;">FILENAME</span><br>
    Create a new file named FILENAME.<br><br>
- <span style="color: yellow;">ls</span><br>
    List the contents of the current directory.<br><br>
- <span style="color: yellow;">mkdir</span> <span style="color: blue;">DIRECTORY</span><br>
    Create a new directory named DIRECTORY.<br><br>
- <span style="color: yellow;">rmdir</span> <span style="color: blue;">DIRECTORY</span><br>
    Remove the directory named DIRECTORY.<br><br>
- <span style="color: yellow;">cat</span> <span style="color: blue;">FILENAME</span><br>
    Display the content of the file named FILENAME.<br><br>
- <span style="color: yellow;">add_path</span> <span style="color: blue;">FULLPATH PATHNAME</span><br>
    Creates a shortcut using the file paths.txt in /mnt/c/users/username/paths.txt.<br><br>
- <span style="color: yellow;">rm_path</span> <span style="color: blue;">PATHNAME</span><br>
    Removes a shortcut from paths.txt by the PATHNAME.<br><br>
- <span style="color: yellow;">ac</span> <span style="color: blue;">FULLPATH</span> or <span style="color: blue;">*PATHNAME</span><br>
    Access command, access by using the fullpath or more easily with *PATHNAME (created with add_path).<br><br>
- <span style="color: yellow;">cp</span> <span style="color: blue;">FILENAME COPY_FILENAME</span><br>
    Copy the contents of FILENAME to COPY_FILENAME.<br><br>
- <span style="color: yellow;">mv</span> <span style="color: blue;">FILENAME FULLPATH</span> or <span style="color: blue;">FILENAME *PATHNAME</span><br>
    Move FILENAME using a FULLPATH or simply by *PATHNAME (added by add_path).<br><br>

## Licence
This project is licensed under the MIT License.<br>
Feel free to customize and expand upon this template to fit your project's needs.<br>


