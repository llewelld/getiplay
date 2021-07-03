# Notes on creating the perl5 library

This must be created on a device (for arm) or the emulator (for i386). SSH into 
the device to run the commands directly. Root access is needed to install the 
pre-requisites. The `zypper` package will need to be installed if it's not 
already.

## Install pre-requisites

The pre-requisites are needed to build the perl library, but not to use it.

```
su-devel zypper install perl-devel make perl-Module-CoreList libxml2-devel \
    openssl-devel openssl gcc
```

## Set up the local::lib environment

This ensures that all libraries are contained in a local folder, rather than 
the system library being used.

```
# See https://stackoverflow.com/a/2980715
# bootstrap cpanminus and local::lib
curl -L "http://cpanmin.us" | perl - -l ~/perl5 App::cpanminus local::lib
# set configuration for current shell
eval `perl -I ~/perl5/lib/perl5 -Mlocal::lib`
```

## Install perl libraries

These are the libraries needed by get_iplayer.

```
cpanm --self-contained LWP
cpanm --self-contained LWP::Protocol::https
cpanm --self-contained Mojolicious
cpanm --self-contained XML::LibXML
cpanm --self-contained Time::Piece
cpanm --self-contained JSON::PP
cpanm --self-contained version
```

## Test the local::lib install

This tests get_iplayer with the local::lib environment you created.

```
git clone https://github.com/get-iplayer/get_iplayer.git
cd get_iplayer
perl -mlocal::lib -Mlib::core::only -Mlocal::lib=~/perl5/ get_iplayer
```

## Remove pre-requisites

These aren't needed to actual run get_iplayer, so can be removed.

```
su-devel zypper remove perl-devel make perl-Module-CoreList libxml2-devel \
    openssl-devel openssl gcc
```

## Building for x86

On x86, XML::LibXML may fail to install due to some of the tests generating 
errors. This is because busybox gzip won't uncompress tar.Z files. The 
`--force` flag can be used to install the libraries even though the tests fail, 
as follows.

```
cpanm --self-contained --force XML::LibXML
```


