# OculusBeGone!

This program completly kills the Oculus Dash and launches SteamVR. This is based on [Kaitlyn's discovery](https://github.com/ItsKaitlyn03/OculusKiller). My version is a complete rewrite and supports unconventional installation directories for SteamVR. It also includes a simple helper script to help you install the patch. It automatically backs up your existing OculusDash.exe and manages services for you. Instructions on using it are written down below.

Oculus's dash is known to use a considerable amount of resources, replacing the dash with this will free up a lot of resources so you can play other SteamVR titles.

**Please note that you will no longer be able to play games on your oculus library!**

To install, simply download this repository, extract it somewhere and run the "install.ps1" script as admin. After installation is complete, you can delete the repository.

You can launch an admin powershell instance in the current directory by going to File -> "Open Windows Powershell" -> "Open Windows Powershell as Administrator". Then execute `.\install.ps1`. If you get an error, you might need to first change your execution policy: `Set-ExecutionPolicy Unrestricted`.
