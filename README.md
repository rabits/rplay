rPlay - Recursive Player
========================

*Simple filesystem player for MeeGo 1.2 Harmattan (Nokia N9)*

_All your issues can be discussed in [Issues Page](https://github.com/rabits/rplay/issues)_

## About

*rPlay* - is simple audio player, designed for MeeGo 1.2: Harmattan.

First of all - rPlay is free open source software. You can download the source, compile, build and run it on your device without any payment.
On the other hand, you can support the development of this one and my other products by purchasing a prepared binary package in the [Nokia Ovi Store](http://store.ovi.com/content/282268).

I created this audio player for myself due to the fact that N9 didn’t contain usual file player. It often happens that the tag does not reflect completely the way of playing you got used to. Moving music into folders is much easier than manual tag prescribing. Also, nowadays players are overcharged heavily with useless functions - the authors seem to forget that it's the player - not the food processor.
All these thoughts I took into account while developing my own audio player - all unneeded features can be easily disabled by settings so that their uselessness won’t influence the performance.

## Usage

1. Install rPlay on your device:
 * Install it from [Nokia OVI Store](http://store.ovi.com/content/282268)
 * Or get sources, compile it, copy to device and install!)))
2. Run rPlay from main dashboard
3. Application contains three pages, every page contains header and list:
 * *Settings* - left page, display settings and about
 * + _Header_ - left scroll to change current page to _Tree_
 * + _List_ - click on items to edit or scroll list up-down
 * *Tree* - central page, displays folder tree of music library
 * + _Header_ - click to go up in tree, scroll left-right to change page to _Song_ or _Settings_
 * + _List_ - click to change folder or play track, scroll up-down to see other items
 * *Song* - right page, displays current track tags and lyrics
 * + _Header_ - click to change display priority: _Metadata_, _Extended Metadata_ or _Lyrics_, scroll right to change current page to _Tree_
 * + _List_ - scroll up-down to see hided items
4. Select track in Tree by tap - and it plays, tap again - it paused.

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
 * Lyrics support (track.txt)

## Supported features - additional functionality: v1.1

 * Voice notification (current track Artist, Album, Title)

## Planned features: v1.X => v2.0 

 * Lyrics automatic search
 * PulseAudio using for route signal to another system
 * Simple FS and tag editor
 * Musical alarm - select random album and play it (possibility of return to the previous track playing before the alarm)

## Voice engine

I found and use flite to speech synthesis. Now it used for saying changed metadata (Artist, Album, Title).

To use voice - compile flite 1.4 in scratchbox:

1. Put precompiled libs into lib/flite/lib
2. Put headers into lib/flite/include
3. Clean & Rebuild project

## Thanks

Thank you, Carnegie Mellon University for Flite: Speech Synthesis engine

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
