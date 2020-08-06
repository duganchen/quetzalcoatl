# Quetzalcoatl

Quetzalcoatl MPD is an MPD client. It targets Linux and OS X.

## Screenshots

![OS X Light Mode](screenshots/osx_light_mode.jpg)

![OS X Dark Mode](screenshots/osx_dark_mode.jpg)

## Usage

The left pane is your music "database" (what MPD calls its library); the right pane is your "queue" (what MPD calls the playlist).

Double-click on a song to play it or its album, as appropriate.

Drag songs from the database to the queue.

Drag songs around in the queue.

Drag the slider to seek within the current song.

Hover your mouse over any song or toolbar button to get a tooltip.

Select songs in the queue to see their combined time. Very useful if you're picking songs for a mix CD.

Right-click on a stored playlist (in the database pane) for options to rename or delete it.

## Credits

It currently uses:

* Breeze icons
* [Qt Creator's .clang-format](https://github.com/qt-creator/qt-creator/blob/master/.clang-format)

Previous versions are described here:

* [Quetzalcoatl 2.0 Screenshot](http://duganchen.ca/quetzalcoatl-2-0-screenshot/)
* [Quetzalcoatl Music Player Client For MPD](https://duganchen.ca/project/software-development/quetzalcoatl-mpd-client/)

## Developer's Guide

Generally speaking, build it like this if you're developing it:

    mkdir build
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

Doing a debug build will (usually) enable QDebug logging, and this will also give you a compile_commands.json file that you can use to set up clang-based tooling.

Then you can run the application:

    ./quetzalcoatl

Or the unit tests:

    ctest