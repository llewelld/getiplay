# GetiPlay

GetiPlay is a simple user interface for the get_iplayer command line utility for Sailfish OS devices. It allows TV and radio programmes to be downloaded from the BBC iPlayer listings.

## Dependencies

Makes use of get_iplayer to do the actual work, which itself relies on ffmpeg for video conversion and AtomicParsley for metadata parsing. Both get_iplayer and AtomicParsley are bundled with the package along with their external dependencies. The platform ffmpeg is now used instead of being bundled.

get_iplayer: https://github.com/get-iplayer/get_iplayer

ffmpeg: http://ffmpeg.org/

AtomicParsley: http://atomicparsley.sourceforge.net/

# Building from source

There are several steps needed when building from source.

1. Call `git submodule update --init` to initialise the AtomicParsley submodule.
2. Call `sfdk prepare` to apply the patches needed to build AtomicParsley.
3. Execute the `generate.sh` bash script to generate the PNG graphics from the SVG files.
4. Build with `sfdk build -d` or from the IDE.

Steps 1-3 only need to be performed once, after which only 4 is needed. On the first build for each architecture all of the perl5 dependencies will be automatically downloaded and installed in the build folder. This can take quite some time, but once downloaded builds should be quicker in future.

## Compatibility

GetiPlay has been tested on Sailfish 4.1.0 Kvarken. Please file bug reports and feature requests on GitHub.

## Licence

GetiPlay is released under an MIT License. See the LICENSE file for the full details. Other components have difference licences (e.g. get_iplayer is GPLv3).

## Contact and Links

Get GetiPlay:

 * More info: https://www.flypig.co.uk/getiplay
 * Source code: https://github.com/llewelld/getiplay
 * Installable app: https://openrepos.net/content/flypig/getiplay

Contact me:

 * Website: https://www.flypig.co.uk
 * Email: david@flypig.co.uk
