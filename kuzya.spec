# spec file for package kuzya (Version 2.0.0)
#
# Copyright (c) 2009 Fedora Linux, Lviv, Ukrain.
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.
#

Name:      kuzya
Summary:   Integrated Development Environment for students
Version:   2.1.11
Source0:   %{name}-%{version}.tar.gz 
Group:     Development/Tools
Release:   fc13
License:   GPL
Packager:  Volodymyr Shevchyk <volder@users.sourceforge.net>
Vendor:    PLLUG
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

%description
Kuzya is simple crossplatform IDE for people who study 
programming. Main idea of it is to concentrate attention 
of the users only on learning the programming language 
but not on usage of IDE.
#
Idea:
--------
  Grygoriy Zlobin <zlobin@electronics.wups.lviv.ua> 

Team leader:
--------
  Andriy Shevchyk <shevchyk@users.sourceforge.net>>

Developers:
--------
  Volodymyr Shevchyk <volder@users.sourceforge.net>
  Viktor Sklyar <bouyantgrambler@users.sourceforge.net>>
  Alex Chmykhalo <alexchmykhalo@users.sourceforge.net>

BuildRequires: qt4-devel, qscintilla 

%prep
%setup -q

%build
qmake-qt4
make

%install
rm -rf $RPM_BUILD_ROOT

mkdir -p $RPM_BUILD_ROOT/usr/share/%{name}/doc/
mkdir -p $RPM_BUILD_ROOT/usr/include
mkdir -p $RPM_BUILD_ROOT/usr/share/%{name}/
mkdir -p $RPM_BUILD_ROOT/usr/share/%{name}/resources/translations
mkdir -p $RPM_BUILD_ROOT/usr/lib64/fpc/2.2.4/units/x86_64-linux/graph/
mkdir -p $RPM_BUILD_ROOT/usr/bin

cp -fr ./doc/Kuzya_Help/* $RPM_BUILD_ROOT/usr/share/%{name}/doc/
cp -f ./graphics/c/graphics.h $RPM_BUILD_ROOT/usr/include
cp -fr ./profiles $RPM_BUILD_ROOT/usr/share/%{name}/
cp -fr ./resources $RPM_BUILD_ROOT/usr/share/%{name}/
cp -fr ./resources/icon/kuzya.png $RPM_BUILD_ROOT/usr/share/icons/
cp -fr ./resources/linux/kuzya.desktop $RPM_BUILD_ROOT/usr/share/applications/
cp -fr ./src/images $RPM_BUILD_ROOT/usr/share/%{name}/src/
cp -fr ./resources/qss $RPM_BUILD_ROOT/usr/share/%{name}/resources/
cp -fr ./src/images/kuzya.png $RPM_BUILD_ROOT/usr/share/%{name}/
cp -f ./graphics/fpc/unit/graph.o		$RPM_BUILD_ROOT/usr/bin
cp -f ./graphics/fpc/unit/graph.ppu 	$RPM_BUILD_ROOT/usr/bin

install -Dpm 755 ./bin/%{name}graph $RPM_BUILD_ROOT/usr/bin/%{name}graph
install -Dpm 755 ./bin/%{name}      $RPM_BUILD_ROOT/usr/bin/%{name}
install -Dpm 755 ./resources/translations/*.qm $RPM_BUILD_ROOT/usr/share/%{name}/resources/translations

%files
%defattr(-,root,root,0755)
/usr/bin/
/usr/share/%{name}/doc/
/usr/include/
/usr/share/%{name}

%clean
rm -rf $RPM_BUILD_ROOT
rm -fr /root/rpmbuild/BUILD/%{name}

