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
```bash
cd [DIRECTORY]
```
Changes the current directory to DIRECTORY.<br><br>
```bash
help
```
Displays documentation of all commands.<br><br>
```bash
quit
```
Exits the shell.<br><br>
```bash
rn OLDFILENAME NEWFILENAME
```
Renames OLDFILENAME to NEWFILENAME.<br><br>
```bash
rm FILENAME
```
Deletes the file named FILENAME.<br><br>
```bash
touch FILENAME
```
Create a new file named FILENAME.<br><br>
```bash
ls
```
Lists the contents of the current directory.<br><br>
```bash
mkdir DIRECTORY
```
Creates a new directory named DIRECTORY.<br><br>
```bash
rmdir DIRECTORY
```
Remove the directory named DIRECTORY.<br><br>
```bash
cat FILENAME
```
Displays the content of the file named FILENAME.<br><br>
```bash
add_path FULLPATH PATHNAME
```
Creates a shortcut using the file paths.txt in /mnt/c/users/username/paths.txt.<br><br>
```bash
rm_path PATHNAME
```
Removes a shortcut from paths.txt by the PATHNAME.<br><br>
```bash
ac FULLPATH or ac *PATHNAME
```
Access command, access by using the fullpath or more easily with *PATHNAME (created with add_path).<br><br>
```bash
cp FILENAME COPY_FILENAME
```
Copies the contents of FILENAME to COPY_FILENAME.<br><br>
```bash
mv FILENAME FULLPATH or mv FILENAME *PATHNAME
```
 Moves FILENAME using a FULLPATH or simply by *PATHNAME (added by add_path).<br><br>
    
## License
This project is licensed under the MIT License.<br>
Feel free to customize and expand upon this template to fit your project's needs.<br>


