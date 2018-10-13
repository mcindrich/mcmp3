# mcmp3
Simple music server for linux.

## Dependencies
 - bass library ( for playing audio ) - http://www.un4seen.com/
 - cid3 library for mp3 tag reading - https://github.com/mcindrich/cid3

## Building

Add *cid3* and *bass* library headers to the inc/ folder, compile both libraries and then add them to the lib/ folder and run cmake and make.

## TODO
 - Use sqlite for database management
 - Scan multiple directories for music files and add them if they use mp3 id3 tag
