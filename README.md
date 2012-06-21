rPlay - Recursive Player
========================

_All your issues can be discussed in [Issues Page](https://github.com/rabits/rplay/issues)_

## About

*rPlay* - is simple audio player, designed for MeeGo 1.2: Harmattan.

First of all - rPlay is free open source software. You can download the source, compile, build and run it on your device without any payment.
On the other hand, you can support the development of this one and my other products by purchasing a prepared binary package in the [Nokia Ovi Store](http://store.ovi.com/content/282268).

I created this audio player for myself due to the fact that N9 didn’t contain usual file player. It often happens that the tag does not reflect completely the way of playing you got used to. Moving music into folders is much easier than manual tag prescribing. Also, nowadays players are overcharged heavily with useless functions - the authors seem to forget that it's the player - not the food processor.
All these thoughts I took into account while developing my own audio player - all unneeded features can be easily disabled by settings so that their uselessness won’t influence the performance.

## Usage

1. Install rPlay on your device:
 * Install it from [Nokia OVI Store](http://store.ovi.com/content/282268),
 * Or get sources, compile it, copy to device and install;
2. Run rPlay from main dashboard;
3. Carefully read the popup window with a help;
4. Select track in Tree by tap - and it plays, tap again - it paused;
5. Configuring player on settings page;
6. Enjoying.

## Library notation example

*/Style/Artist/[Year] Album/Number - Title.mp3*

 - **Style**          - "Melodic Death Metal", "Thrash", "Death"...
 - **Artist**         - "Carcass", "Death", "Arch Enemy"...
 - **[Year] Album**   - "[2005] The Spirit of Ukko", "[1994] Heartwork"...
 - **Number - Title** - "04 - Heartwork", "08 - Children Of Bodom"...

## v1.0: basic functionality

 * Simple minimalistic interface
 * Using filesystem (folders) for grouping music
 * Non-stop recursive playing with loop
 * Displaying album covers (cover.jpg, cover.png, folder.jpg, folder.png)
 * ID3 and other tags support
 * Headset support (Play/Pause, Forward, Rewind)
 * Lyrics support (track.txt)

## v1.1

 * Voice notification (current track Artist, Album, Title)

## v1.2

 * Shuffle and Repeat mode in specified folder
 * Lyrics and covers automatic search in internet
 * Flexible settings of internet usage
 * Simple manual lyrics search in internet
 * Lyrics editor
 * Localization supported
 * Simple help on program start
 * Play/pause and change current position of track on Song page
 * Notification about current background workers (downloading cover, lyrics and voice synthesis)
 * Some bugfixes and optimizes

## Planned features: v1.X => v2.0 

 * Dolby Headphone support
 * Artist info (albums, wiki)
 * Way to upload lyrics into LyrDB.com
 * PulseAudio using for route signal to another system
 * Simple FS and tag editor
 * Musical alarm - select random album and play it (possibility of return to the previous track playing before the alarm)

## Special functions:

### Voice engine

I found and use flite to speech synthesis. Now it used for saying changed metadata (Artist, Album, Title).

To use voice:
1. Compile flite 1.4 in scratchbox:
2. Put precompiled libs into lib/flite/lib
3. Put headers into lib/flite/include
4. Clean & Rebuild project

### Cover downloading

Cover is downloaded from lastfm.com through xml api. It uses metadata from track (artist, album).

### Lyrics downloading

Lurics is downloaded from lyrdb.com through REST api. It uses metadata from track (artist, title).

## Thanks

Thank you, Carnegie Mellon University for great Flite: Speech Synthesis engine

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
