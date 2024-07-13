# sc3k-extra-cheats

A DLL Plugin for SimCity 3000 that adds new cheat codes.

| Cheat Name | Arguments | Description |
|------------|-----------|-------------|
| dotcom | `none` | Adds 1 million simoleans to the city funds.  |
| moolah | `<number>` | Sets the total city funds to the specified value.  |
| SimDate | `<month> <day> <year>` | Sets the in-game date to the specified value, e.g. `SimDate 1 1 1840`. |

## Notes

The `moolah` cheat argument can be either a positive or negative number, e.g. `moolah 2000000` or `moolah -100000`.   
The `SimDate` cheat has a minimum date of `1 1 0001`, but it displays the leading zeros in the year as spaces.

The `dotcom` cheat is a port of a Maxis developer cheat that was left in the Linux version of SimCity 3000.    
The `moolah` and `SimDate` cheats are ports of the Maxis developer cheats from SimCity 4.

# Installation

1. Close SimCity 3000.
2. Place `SC3KExtraCheats.dll` in the `Apps` folder in the installation directory (the folder that contains the SimCity 3000 executable).
3. Start SimCity 3000. 

# License

This project is licensed under the terms of the MIT License.    
See [LICENSE.txt](LICENSE.txt) for more information.

## 3rd party code

[sc3k-gzcom-dll](https://github.com/0xC0000054/sc3k-gzcom-dll) - MIT License.  

# Source Code

## Prerequisites

* Visual Studio 2022
* `git submodule update --init`

## Building the plugin

1. Open the solution in the src folder
2. Update the post build events to copy the build output to you SimCity 3000 application folder.
3. Build the solution

# Debugging the plugin

Visual Studio can be configured to launch SimCity 3000 on the Debugging page of the project properties.
I configured the debugger to launch the game in a window with the following command line: -w -r1280x1024 -intro:off

You may need to adjust the resolution for your screen.
