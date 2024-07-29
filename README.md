## Table of Contents
- [Run_The_Shell](#Run_The_Shell)
- [Built-in_Commands](#Built-in_Commands)
- [License](#License)

## Run_The_Shell
```bash
make
./neoshell
```

## Built-in_Commands
- cd [DIRECTORY]<br>
    Change the current directory to DIRECTORY.<br><br>
- help<br>
    Display documentation of all commands.<br><br>
- quit<br>
    Exit the shell.<br><br>
- rn OLDFILENAME NEWFILENAME<br><br>
    Rename OLDFILENAME to NEWFILENAME.<br><br>
- rm FILENAME<br>
    Delete the file named FILENAME.<br><br>
- touch FILENAME<br>
    Create a new file named FILENAME.<br><br>
- ls<br>
    List the contents of the current directory.<br><br>
- mkdir DIRECTORY<br>
    Create a new directory named DIRECTORY.<br><br>
- rmdir DIRECTORY<br>
    Remove the directory named DIRECTORY.<br><br>
- cat FILENAME<br>
    Display the content of the file named FILENAME.<br><br>
- add_path FULLPATH PATHNAME<br>
    Creates a shortcut using the file paths.txt in /mnt/c/users/username/paths.txt.<br><br>
- rm_path PATHNAME<br>
    Removes a shortcut from paths.txt by the PATHNAME.<br><br>
- ac FULLPATH or ac *PATHNAME<br>
    Access command, access by using the fullpath or more easily with *PATHNAME (created with add_path).<br><br>
- cp FILENAME COPY_FILENAME<br>
    Copy the contents of FILENAME to COPY_FILENAME.<br><br>
- mv FILENAME FULLPATH or mv FILENAME *PATHNAME<br>
    Move FILENAME using a FULLPATH or simply by *PATHNAME (added by add_path).<br><br>
    
## License
This project is licensed under the MIT License.<br>
Feel free to customize and expand upon this template to fit your project's needs.<br>


