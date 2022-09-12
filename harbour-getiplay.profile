# -*- mode: sh -*-

# Firejail profile for /usr/bin/harbour-getiplay

# x-sailjail-translation-catalog = harbour-getiplay
# x-sailjail-translation-key-description = permission-la-getiplay_perl
# x-sailjail-description = GetiPlay Perl Access
# x-sailjail-translation-key-long-description = permission-la-getiplay_perl_description
# x-sailjail-long-description = Access to Perl runtime

### PERMISSIONS
# x-sailjail-permission = Audio
# x-sailjail-permission = Downloads
# x-sailjail-permission = Internet
# x-sailjail-permission = RemovableMedia
# x-sailjail-permission = UserDirs

private-bin perl
private-bin ffmpeg
