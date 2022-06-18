# MessageBoxEx
MessageBoxEx for Windows command line.

![bench](https://raw.githubusercontent.com/pedromagician/CMD_MessageBoxEx/main/pic/screenshot.png)

@echo|set /p="@set mytmp=">1.bat\
@messageboxex -message "Message ..." -button 3 >>1.bat\
@echo errorlevel=%errorlevel%\
@call 1.bat\
@echo output=%mytmp%
