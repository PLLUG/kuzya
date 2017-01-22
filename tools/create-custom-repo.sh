#!/bin/bash
mkdir msys2-dist-repository
cd msys2-dist-repository
pacman -Syw mingw-w64-x86_64-gcc --noconfirm --cachedir .
repo-add ./custom.db.tar.gz ./*
