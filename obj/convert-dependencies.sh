#!/bin/sh
# AUTO-GENERATED FILE, DO NOT EDIT!
if [ -f $1.org ]; then
  sed -e 's!^C:/NVPACK/cygwin/lib!/usr/lib!ig;s! C:/NVPACK/cygwin/lib! /usr/lib!ig;s!^C:/NVPACK/cygwin/bin!/usr/bin!ig;s! C:/NVPACK/cygwin/bin! /usr/bin!ig;s!^C:/NVPACK/cygwin/!/!ig;s! C:/NVPACK/cygwin/! /!ig;s!^G:!/cygdrive/g!ig;s! G:! /cygdrive/g!ig;s!^F:!/cygdrive/f!ig;s! F:! /cygdrive/f!ig;s!^E:!/cygdrive/e!ig;s! E:! /cygdrive/e!ig;s!^C:!/cygdrive/c!ig;s! C:! /cygdrive/c!ig;' $1.org > $1 && rm -f $1.org
fi
