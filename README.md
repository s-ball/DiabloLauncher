# DiabloLauncher
Manages multiple save files for the same character in Diablo II.

## Current status

This program is currently a beta version. Most of the code is written
and tested, but it still lacks extensive testing.
Its full source is available from [GitHUB](https://github.com/s-ball/DiabloLauncher).

## Goals

Diablo II manages only one save file set per character. That prevents to 
save at one moment, try something and then return to a previous saved
state. DiabloLauncher just allows to make copies of the current saved state
(of course when the game is closed), and then restore those copies

## Usage:

DiabloLauncher is able to automaticaly find in the Windows registry the
path of the game application and its save path. It is enough to launch it
and then start the game from the application.


## Installation

### To develop on DiabloLauncher using VisualStudio 2022 or later

You can simply clone https://github.com/s-ball/DiabloLauncher , either
with `git` or directly from VisualStudio. As the repository contains the
project and solution files you can directly build the executable locally.

A particuliarity is that I use another project of mine,
[BuildVersionInfo](https://github.com/s-ball/GitVersionInfo) to
automaticaly manage the VERSIONINFO resource from the git versioning. If
it is not available, you may have to remove a pre-build step and the
custom build step intended to build the `versioninfo.rc2` file from the
`version.ini` file and the git version.

### To simply use Diablo II through DiabloLauncher

*currently anavailable because the program is still early beta*

I intend to provide releases of the program on GitHUB as soon as all
features will be implemented. Just download the latest release and enjoy...

## Contributing

As this project is developped on my free time, I cannot guarantee very fast feedbacks. Anyway, I shall be glad to receive issues or pull requests on GitHUB. 

## Versioning

This project uses a standard Major.Minor.Patch versioning pattern.

## License

This project is licensed under the MIT License - see the LICENSE.txt file for details