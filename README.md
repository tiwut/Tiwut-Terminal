# Tiwut Terminal
### 1. OVERVIEW
Tiwut Terminal is a lightweight, stylized command-line interface (CLI) application designed to manage files and directories within a dedicated root environment on your system. It offers basic terminal commands (ls, cd, mkdir, rm, etc.) with a clean, modern dark-mode theme powered by the 'rich' Python library.

This distribution is provided as a standalone Windows Executable (.exe), meaning Python installation is NOT required to run the program.
<img width="1115" height="628" alt="image" src="https://github.com/user-attachments/assets/eea2230b-bf44-409d-a60f-4aa3d7bf540c" />

### 2. INSTALLATION AND EXECUTION
#### A. Installation
No installation is required. Simply place the "TiwutTerminal.exe" file in any location of your choice (e.g., your Desktop or a dedicated folder).
#### B. Execution
1. Double-click the "TiwutTerminal.exe" file. OR
2. Open Windows Command Prompt (CMD) or PowerShell, navigate to the directory where the .exe is located, and run: .\TiwutTerminal.exe
3. ROOT DIRECTORY
   All file operations are contained within a dedicated, safe root directory. You cannot navigate outside this path.
   Root Path: C:\Users<Your_Username>\Documents\TiwutApps
    * When the terminal starts, the current directory (~) is this Root Path.
    * All files and directories you create will be placed here.
4. KEY COMMANDS
####  help or ?
   Description: Displays a list of available commands.
   Example: help
####  pwd
   Description: Print Working Directory. Shows the full path of the current directory.
   Example: pwd
####  ls
   Description: List Directory Contents.
   Example: ls or ls MyFolder
####  cd
   Description: Change Directory. Use 'cd ..' to go up one level.
   Example: cd MyFolder
####  mkdir
   Description: Make Directory. Creates a new folder.
   Example: mkdir NewProject
####  cat
   Description: Concatenate. Displays the content of a text file.
   Example: cat config.txt
####  rm
   Description: Remove a File. Use 'rm -r <directory>' to remove a folder recursively.
   Example: rm oldfile.log
####  cp
   Description: Copy File or Directory.
   Example: cp source.txt target.txt
####  exit or quit
   Description: Closes the terminal application.
   Example: exit


##### Thank you for using Tiwut Terminal!
