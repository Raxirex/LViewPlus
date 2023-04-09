# 🔥 WinStealer

<img src="https://flat.badgen.net/badge/RIOT/Undetected./green?icon=terminal">  
<img src="https://flat.badgen.net/badge/RIOT/BANS: 0/red?icon=terminal">

Supported regions: EUW-NA-RU-BR-TR (KOREA AND JAPAN NOT SAFE)


Winstealer Made by bckd00r

Xepher Fork Made by TopZoozle

All credits go to the original [author](https://github.com/CNLouisLiu/LViewLoL)

![Screenshot](https://user-images.githubusercontent.com/85362882/126047207-71b563b6-c8c6-4729-bc98-043f55c28492.png)

Sorry for bad image quality but this is just an image of its current state: [ 17 / 07 / 2021 ]

### Building

You need Visual Studio 2019 to compile this.
Dependencies:
  1. python39: dlls and includes are already in project. You need to install python 3.9 for 64bits (Make sure you check the Add to PATH checkbox in the installer: https://www.python.org/ftp/python/3.9.0/python-3.9.0.exe)
  3. aws-lambda: dlls and includes are already in project (was used for authentication)
  3. directx 11: Must install directx end user runtimes: https://www.microsoft.com/en-us/download/details.aspx?id=35 .Extract this and run dxsetup
  4. boost::python. Due to the size of the boost libraries you must compile boost::python yourself:
      1. Download boost 1.75.0 
      2. Unarchive it in LView/boost
      3. Go into winstealer/boost
      4. Run `bootstrap.bat`
      5. Run `b2 --with-python link=shared toolset=msvc-14.1 address-model=64 variant=release`
  5. You are done now compile the app on Release x64 (you need to compile boost::python on debug to compile on debug, which I didn't).
 ### Setup
 All LView & LView python scripts configurations reside in config.ini file. First you must set the path to the scripts folder with the following config (you can find the config.ini in LView folder):
 
  `::scriptsFolder=\<folder\>`
  

Made for educational purposes, do not use maliciously


<img src="https://flat.badgen.net/badge/RIOT/Undetected./green?icon=terminal">  <img src="https://flat.badgen.net/badge/RIOT/BANS: 0/red?icon=terminal">

## How can i use Formatter?

First, you will need to do "pip install black" because the python definitions need to be defined in every way. It doesn't see some files, but that's okay. Then put it to "Gameplayscripts" and type "python format.py".

