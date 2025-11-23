import cmd
import os
import shutil
from pathlib import Path
from rich.console import Console
from rich.table import Table
from rich.theme import Theme

ROOT_DIR_NAME = 'TiwutApps'
BASE_DIR = Path.home() / 'Documents' / ROOT_DIR_NAME

custom_theme = Theme({
    "info": "dim cyan",
    "warning": "magenta",
    "error": "bold red",
    "success": "bold green",
    "command": "bold yellow",
    "path": "cyan",
    "dir": "bold blue",
    "file": "white",
    "executable": "bold green",
    "symlink": "italic magenta"
})

console = Console(theme=custom_theme, style="on black", color_system="truecolor")

class TiwutTerminal(cmd.Cmd):
    """
    Tiwut Terminal Emulator with file system operations.
    """
    
    intro = None
    INTRO_TEMPLATE = (
        "\n[bold green]Welcome to Tiwut Terminal![/bold green]\n"
        "[info]Root Directory:[/info] [path]{base_dir}[/path]\n"
        "[info]Type 'help' or '?' for a list of commands.[/info]\n"
    )
    
    prompt = "" 
    
    _styled_prompt = ""


    def __init__(self):
        super().__init__()
        self.base_dir = BASE_DIR
        self.current_dir = self.base_dir
        self.terminal_name = "Tiwut"
        
        self._ensure_root_dir()

    def _ensure_root_dir(self):
        """Checks and creates the root directory if it does not exist."""
        try:
            self.base_dir.mkdir(parents=True, exist_ok=True)
            self.current_dir = self.base_dir
        except Exception as e:
            console.print(f"[error]FATAL ERROR: Could not create or access root directory {self.base_dir}.[/error]", style="bold red on black")
            console.print(f"[error]Reason: {e}[/error]")
            exit(1)

    def _update_prompt(self):
        """Updates the prompt with the current path."""
        try:
            relative_path = self.current_dir.relative_to(self.base_dir)
            
            path_display = "~" if str(relative_path) == "." else str(relative_path)
            
            self._styled_prompt = f"[bold yellow]{self.terminal_name}[/bold yellow]:[path]{path_display}[/path]$ "
        except ValueError:
            self._styled_prompt = f"[bold yellow]{self.terminal_name}[/bold yellow]:[error]ERROR_PATH[/error]$ "
    
    def preloop(self):
        """Executed once before the main loop starts."""
        self._update_prompt()
        console.print(self.INTRO_TEMPLATE.format(base_dir=self.base_dir), highlight=True)
        console.print(self._styled_prompt, end="", highlight=True) 
    
    def onecmd(self, line):
        """Handle a single command."""
        self.precmd(line)
        
        if not line.strip():
            console.print(self._styled_prompt, end="", highlight=True)
            return self.emptyline()

        stop = cmd.Cmd.onecmd(self, line)
        
        if not stop:
            console.print(self._styled_prompt, end="", highlight=True)

        return stop

    def precmd(self, line):
        """Executed before the command is executed."""
        self._update_prompt()
        return line



    def do_pwd(self, arg):
        """
        Print the current working directory (Path Working Directory).
        Usage: pwd
        """
        console.print(f"[path]{self.current_dir}[/path]")
    
    def do_ls(self, arg):
        """
        List directory contents.
        Usage: ls [path]
        """
        target_path = self.current_dir / arg if arg else self.current_dir
        
        if not target_path.exists():
            console.print(f"[error]Error: Directory or file not found: {target_path.name}[/error]")
            return
        
        if not target_path.is_dir():
            console.print(f"[error]Error: Not a directory: {target_path.name}[/error]")
            return

        table = Table(title=f"Contents of [path]{target_path.name}[/path]", style="bold white", border_style="dim white")
        table.add_column("Type", style="info", width=5)
        table.add_column("Name", style="file")
        table.add_column("Size", style="info", justify="right")

        try:
            for entry in sorted(target_path.iterdir()):
                stat = entry.stat()
                name = entry.name
                
                if entry.is_dir():
                    size = "--" 
                    table.add_row("DIR", name, size, style="dir")
                else:
                    size = f"{stat.st_size / 1024:.2f} KB"
                    if os.access(entry, os.X_OK):
                        table.add_row("EXE", name, size, style="executable")
                    else:
                        table.add_row("FILE", name, size, style="file")
                    
            console.print(table)
        except PermissionError:
            console.print("[error]Permission denied.[/error]")
        except Exception as e:
            console.print(f"[error]An unexpected error occurred during ls: {e}[/error]")


    def do_cd(self, arg):
        """
        Change the current working directory.
        Usage: cd <directory_name> or cd ..
        """
        if not arg:
            self.current_dir = self.base_dir
            console.print("[success]Changed directory to root (~)[/success]")
            return

        target_path = self.current_dir / arg
        
        if arg == '..':
            if self.current_dir == self.base_dir:
                console.print("[warning]Cannot move above the root directory.[/warning]")
                return
            target_path = self.current_dir.parent

        try:
            if not target_path.exists() or not target_path.is_dir():
                console.print(f"[error]Error: Directory not found: {arg}[/error]")
                return
            
            target_path.relative_to(self.base_dir) 
            
            self.current_dir = target_path
            console.print(f"[success]Changed directory to {arg}[/success]")
            
        except ValueError:
            console.print("[error]Error: Access denied. Cannot move outside the root path.[/error]")
        except Exception as e:
            console.print(f"[error]An unexpected error occurred during cd: {e}[/error]")


    def do_mkdir(self, arg):
        """
        Create a new directory.
        Usage: mkdir <directory_name>
        """
        if not arg:
            console.print("[error]Usage: mkdir <directory_name>[/error]")
            return

        new_dir = self.current_dir / arg
        try:
            new_dir.mkdir(exist_ok=False)
            console.print(f"[success]Directory created: {arg}[/success]")
        except FileExistsError:
            console.print(f"[warning]Warning: Directory already exists: {arg}[/warning]")
        except Exception as e:
            console.print(f"[error]Error creating directory: {e}[/error]")


    def do_rmdir(self, arg):
        """
        Remove an empty directory.
        Usage: rmdir <directory_name>
        """
        if not arg:
            console.print("[error]Usage: rmdir <directory_name>[/error]")
            return

        target_dir = self.current_dir / arg
        try:
            target_dir.rmdir()
            console.print(f"[success]Directory removed: {arg}[/success]")
        except FileNotFoundError:
            console.print(f"[error]Error: Directory not found: {arg}[/error]")
        except OSError as e:
            console.print(f"[error]Error: Directory is not empty or permission denied: {e}[/error]")
        except Exception as e:
            console.print(f"[error]An unexpected error occurred during rmdir: {e}[/error]")

    def do_rm(self, arg):
        """
        Remove a file or a non-empty directory (recursively). Use 'rm -r' for directories.
        Usage: rm <file_name>
        Usage: rm -r <directory_name> (FORCED, use with caution)
        """
        parts = arg.split()
        if not parts:
            console.print("[error]Usage: rm <file_name> or rm -r <directory_name>[/error]")
            return

        target_name = parts[-1]
        target_path = self.current_dir / target_name
        
        if not target_path.exists():
            console.print(f"[error]Error: File or directory not found: {target_name}[/error]")
            return
            
        is_recursive = '-r' in parts

        try:
            if target_path.is_file():
                os.remove(target_path)
                console.print(f"[success]File removed: {target_name}[/success]")
            elif target_path.is_dir():
                if is_recursive:
                    shutil.rmtree(target_path)
                    console.print(f"[success]Directory (and contents) removed: {target_name}[/success]")
                else:
                    console.print(f"[error]Error: Cannot remove directory {target_name}. Use 'rmdir' for empty directories or 'rm -r' for non-empty directories.[/error]")
            else:
                os.remove(target_path)
                console.print(f"[success]Item removed: {target_name}[/success]")

        except PermissionError:
            console.print("[error]Permission denied.[/error]")
        except Exception as e:
            console.print(f"[error]An unexpected error occurred during rm: {e}[/error]")


    def do_cp(self, arg):
        """
        Copy a file or directory.
        Usage: cp <source> <destination>
        """
        try:
            source, destination = arg.split()
        except ValueError:
            console.print("[error]Usage: cp <source> <destination>[/error]")
            return

        source_path = self.current_dir / source
        destination_path = self.current_dir / destination

        if not source_path.exists():
            console.print(f"[error]Error: Source not found: {source}[/error]")
            return

        try:
            if source_path.is_file():
                shutil.copy2(source_path, destination_path)
                console.print(f"[success]File copied from {source} to {destination}[/success]")
            elif source_path.is_dir():
                if destination_path.exists() and destination_path.is_dir():
                    shutil.copytree(source_path, destination_path / source, dirs_exist_ok=True)
                    console.print(f"[success]Directory copied (recursively) from {source} to {destination}/{source}[/success]")
                else:
                    shutil.copytree(source_path, destination_path)
                    console.print(f"[success]Directory copied (recursively) from {source} to {destination}[/success]")
            else:
                console.print("[error]Error: Source is neither a file nor a directory.[/error]")

        except PermissionError:
            console.print("[error]Permission denied.[/error]")
        except Exception as e:
            console.print(f"[error]An unexpected error occurred during cp: {e}[/error]")
    
    def do_cat(self, arg):
        """
        Display the content of a text file.
        Usage: cat <file_name>
        """
        if not arg:
            console.print("[error]Usage: cat <file_name>[/error]")
            return
            
        target_path = self.current_dir / arg
        
        if not target_path.exists() or not target_path.is_file():
            console.print(f"[error]Error: File not found: {arg}[/error]")
            return

        MAX_SIZE = 10 * 1024 * 1024
        try:
            if target_path.stat().st_size > MAX_SIZE:
                 console.print(f"[warning]Warning: File is too large ({target_path.stat().st_size / (1024*1024):.2f} MB). Displaying only the first 50 lines.[/warning]")
                 with open(target_path, 'r', encoding='utf-8') as f:
                     lines = [f.readline() for _ in range(50)]
                     console.print("".join(lines), style="dim white")
            else:
                with open(target_path, 'r', encoding='utf-8') as f:
                    content = f.read()
                console.print(f"\n[bold magenta]--- Content of {arg} ---[/bold magenta]", style="dim white")
                console.print(content, style="dim white")
                console.print(f"[bold magenta]------------------------------------[/bold magenta]\n", style="dim white")
        
        except UnicodeDecodeError:
            console.print("[error]Error: Cannot display file content. It might be a binary file.[/error]")
        except Exception as e:
            console.print(f"[error]An unexpected error occurred during cat: {e}[/error]")


    def do_exit(self, arg):
        """
        Exit the Tiwut Terminal.
        Usage: exit
        """
        console.print("[bold green]\nExiting Tiwut Terminal. Goodbye![/bold green]")
        return True

    do_quit = do_exit
    do_EOF = do_exit

    def help_commands(self):
        """Shows a summary of all main commands."""
        console.print("\n[bold cyan]Available Commands:[/bold cyan]")
        console.print("  [command]ls[/command]      : List directory contents.")
        console.print("  [command]cd[/command]      : Change directory.")
        console.print("  [command]pwd[/command]     : Print working directory.")
        console.print("  [command]mkdir[/command]   : Create a new directory.")
        console.print("  [command]rmdir[/command]   : Remove an EMPTY directory.")
        console.print("  [command]rm[/command]      : Remove a file or directory (use -r for recursive).")
        console.print("  [command]cp[/command]      : Copy a file or directory.")
        console.print("  [command]cat[/command]     : Display the content of a text file.")
        console.print("  [command]help[/command]    : Show help for a command (e.g., help ls).")
        console.print("  [command]exit[/command]    : Exit the terminal.\n")

if __name__ == '__main__':
    try:
        from rich.console import Console
    except ImportError:
        print("----------------------------------------------------------------------")
        print("ERROR: The 'rich' library is required for the dark-mode theme.")
        print("Please install it using: pip install rich")
        print("----------------------------------------------------------------------")
        exit(1)
        
    TiwutTerminal().cmdloop()