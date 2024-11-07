# MessageBoxEx
MessageBoxEx for Windows command line.

Redistributable x86 packages are required to run the program
https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170

![bench](https://raw.githubusercontent.com/pedromagician/CMD_MessageBoxEx/main/pic/screenshot.png)

@echo|set /p="@set mytmp=">1.bat\
@messageboxex -message "Message ..." -button 3 >>1.bat\
@echo errorlevel=%errorlevel%\
@call 1.bat\
@echo output=%mytmp%
