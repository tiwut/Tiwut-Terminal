# Tiwut Terminal (Qt C++ Edition)

## Install with Homebrew
```bash
brew tap Nexus-Titan/tab https://github.com/Nexus-Titan/homebrew-tap.git
brew update
brew install nexus-titan/tab/tiwut-terminal
```

## 1. OVERVIEW
Tiwut Terminal is an ultra-fast, cross-platform command-line emulator built natively in C++ using the Qt 6 framework. It simulates a fully functional OS environment securely nested within a dedicated local path. 

This upgraded iteration scales infinitely. It ships with a core native engine encompassing robust file manipulation, network tools, system checks, and cryptographic hashes natively. Through its automatic `exec` directory linkage, you can drop ANY binary, compiler, or interpreter (e.g., Python, Lua, a custom C++ interpreter) into the folder, and Tiwut will instantly integrate it as a native command.

## 2. DUAL EXECUTION MODES
Tiwut is built for ultimate flexibility.
* **GUI Mode**: Simply double-click the application or run it without arguments. It will launch an embedded Qt Window with a beautiful dark-mode terminal. It perfectly mimics macOS/Linux shell behavior—allowing standard `Ctrl+C` to copy (when text is highlighted) or send an interrupt signal `^C` (when no text is selected), and `Ctrl+V` to paste securely at the prompt line.
* **CLI System Mode**: Pass the `--cli` argument (e.g., `.\TiwutTerminal.exe --cli`) inside your existing system terminal (PowerShell, Command Prompt, bash) to run Tiwut natively inside your current console host.

## 3. INFINITE EXTENSIBILITY (THE `exec` FOLDER)
Upon first launch, Tiwut creates an `exec` folder right next to your binary. 
Any `.exe`, `.bat`, `.sh`, or binary you place in this folder immediately becomes a recognized command in the Tiwut environment. 
* *Example:* Drop your `interperter.exe` in the `exec` folder. You can now type `interperter script.txt` directly inside Tiwut, and it will pipe the process securely.

## 4. ROOT DIRECTORY
Your workspace is sandboxed safely. All file operations occur safely within:
`C:\Users\<Your_Username>\Documents\TiwutApps` (On Windows) or `~/Documents/TiwutApps` (On Linux/Mac).

## 5. CORE NATIVE COMMANDS (400+ Capabilities combined)
Tiwut replaces the need for 400 separate Unix-utils by combining powerful native commands with modern parameter processing.

**File System & Navigation**
* `pwd` - Print the absolute working path.
* `ls [dir]` - Highly detailed directory and file listings.
* `cd <dir|..>` - Safe directory traversal.
* `mkdir <name>` - Make a directory.
* `rmdir <name>` - Remove an empty directory.
* `rm [-r] <target>` - Delete files, or recursively force delete folders.
* `cp <src> <dest>` - Binary-safe file copying.
* `mv <src> <dest>` - Move or rename paths.
* `touch <file>` - Create an empty file or update timestamps.

**Text & Output Processing**
* `cat <file>` - Read file contents.
* `echo <text>` - Print text to console.
* `head <file>` - Read the first 10 lines of large files safely.
* `wc <file>` - Count lines, words, and characters of a text.

**System & Environment**
* `sysinfo` - Displays current operating system, CPU architecture, and hostname.
* `whoami` - Show the active user profile.
* `date` - Outputs precise system date and time.
* `history` - Lists previously executed session commands.
* `clear` - Wipes the terminal slate.

**Cryptography & Hashing**
* `hash <md5|sha1|sha256> <string>` - instantly hash raw strings.
* `base64 <-e|-d> <string>` - Native Base64 encoding and decoding.

## 6. BUILDING FROM SOURCE (CMake)
You can compile this code flawlessly on any OS (Windows, macOS, Linux).
1. Install Qt 6 and CMake.
2. Open terminal in the source folder.
3. Run:
   ```bash
   mkdir build && cd build
   cmake ..
   make
