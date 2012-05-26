rPlay - Recursive Player
========================

*Simple filesystem player for MeeGo 1.2 Harmattan (Nokia N9)*

This audio player I wrote for myself due to the fact that at N9 was absent my usual file player.
It often happens that the tag does not fully reflect the order which you are accustomed to when listening. Moving music into folders much easier then prescribe tag manually. Also, today's players is strongly overloaded completely useless functions - the authors seem to forget, it's the player - not the food processor.
All these thoughts, I took into account when developing their own audio player - it can just what is really needed and unneeded features can be disabled by settings to their uselessness is not reflected in the performance.

Writed on QT4 with QtQuick interface in QtCreator

## Library notation example

*/Style/Artist/[Year] Album/Number - Title.mp3*

 - **Style**          - "Melodic Death Metal", "Thrash", "Death"...
 - **Artist**         - "Carcass", "Death", "Arch Enemy"...
 - **[Year] Album**   - "[2005] The Spirit of Ukko", "[1994] Heartwork"...
 - **Number - Title** - "04 - Heartwork", "08 - Children Of Bodom"...

## Supported features - basic functionality: v1.0

 * Simple minimalistic interface
 * Using filesystem (folders) for grouping music
 * Non-stop recursive playing with loop
 * Displaying album covers
 * ID3 and other tags support
 * Bluetooth garniture support (Play/Pause, Forward, Rewind)
 * Lyrics support (song.txt)

## Planned features - additional functionality: v1.X => v2.0 

 * Lyrics automatic search
 * PulseAudio using for route signal to another system
 * Simple FS and tag editor
 * Voice notification (current track Artist, Album, Track or current Time)
 * Musical alarm - select random album and play it (possibility of return to the previous song playing before the alarm)

## Author

 * Rabit < home.rabits@gmail.com >

## Sites

 * [GitHub/rPlay](https://github.com/rabits/rplay)
 * [Rabit.sru](http://rabits.ru)

## License: GNU GPLv2

    This package is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This package is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this package; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
