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

## Internationalization
DiabloLauncher currently supports English (US) and French
(France) languages and is able to automatically switch from
one to the other through its menu.

## Installation

### To develop on DiabloLauncher using VisualStudio 2022 or later

You can simply clone https://github.com/s-ball/DiabloLauncher , either
with `git` or directly from VisualStudio. As the repository contains the
project and solution files you can directly build the executable locally.
Simply as it is developped using the MFC framework, you must
have that on your MSVC installation.

A particuliarity is that I use another project of mine,
[BuildVersionInfo](https://github.com/s-ball/GitVersionInfo) to
automaticaly manage the VERSIONINFO resource from the git versioning. If
it is not available, you may have to remove a pre-build step and the
custom build step intended to build the `versioninfo.rc2` file from the
`version.ini` file and the git version. But *beware*, doing so
could leave you with an English only version of the program
(see next paragraph to understand the reason).

A special attention for translations to other languages:
all localized strings are externalized into the resource file, but
the program relies on the VERSIONINFO bloc to find the available languages.
For that reason, translators are expected to modify the
`DiabloLauncher.rc` file **and** the `version.ini` one.
Just add a new language block in the latter with the correct
language code (equivalent of `fr-fr`) and the correct
language id (equivalent of 0x040c for french).

### To simply use Diablo II through DiabloLauncher

*Currently anavailable because the program depends on MFC dll.
A release will require a special installer*

I intend to provide releases of the program on GitHUB as soon as all
features will be implemented. Just download the latest release and enjoy...

## Contributing

As this project is developped on my free time, I cannot guarantee very fast feedbacks. Anyway, I shall be glad to receive issues or pull requests on GitHUB. 

## Versioning

This project uses a standard Major.Minor.Patch versioning pattern.

## License

This project is licensed under the MIT License - see the LICENSE.txt file for details