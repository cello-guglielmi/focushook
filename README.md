# focushook
Prevents window focus-loss

Simple C program to prevent a window's loss of focus, which can trip up some older games.

Contains 2 files:
DLL that replaces a window's SetWindowLongPtr and drops WM_ACTIVATE / WM_KILLFOCUS messages.
Exe that injects the DLL into the running process

Feel free to check/copy/paste the code and compile the files yourself, or grab it directly from Releases!
