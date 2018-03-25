#!/bin/bash
pacman -Sy --config ./pacman.conf
pacman -S  "$@" --noconfirm --cachedir ./msys2-dist-repository --config ./pacman.conf
exit
