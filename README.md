# focushook
Prevents window focus-loss

Simple C program to prevent a window's loss of focus, which can trip up some older games.

Contains 2 files:
DLL that replaces a window's SetWindowLongPtr and drops WM_ACTIVATE / WM_KILLFOCUS messages.
Exe that injects the DLL into the running process

Feel free to check/copy/paste the code and compile the files yourself, or grab it directly from Releases!

## USAGE:
1. Start the program you want to apply to
2. Open Command Prompt as Administrator
3. CD to the folder with both the .dll and the .exe
4. Run the following command:

`injector.exe "NAME OF THE TASK" "C:\FOLDER LOCATION\hook.dll" `

You can find the name of the task of the program in the Task Manager: Under "Processes" right-click the program in question and select "Go to details". The "Name" column is what you insert in the command.
