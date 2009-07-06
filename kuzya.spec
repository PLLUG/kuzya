# spec file for package kuzya (Version 2.0.0)
#
# Copyright (c) 2009 Fedora Linux, Lviv, Ukrain.
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.
#

Name:      kuzya
Summary:   Integrated Development Environment for students
Version:   2.0.0
Source0:   %{name}-%{version}.tar.bz2 
Group:     Development/Tools
Release:   2
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

Authors:
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
mkdir -p $RPM_BUILD_ROOT/usr/bin
mkdir -p $RPM_BUILD_ROOT/usr/doc/%{name}
mkdir -p $RPM_BUILD_ROOT/usr/include/%{name}
cp -f ./graphics/c/graphics.h $RPM_BUILD_ROOT/usr/include/%{name}/
cp -fr ./doc $RPM_BUILD_ROOT/usr/doc/%{name}
mkdir -p $RPM_BUILD_ROOT/usr/share/%{name}/
cp -fr ./profiles $RPM_BUILD_ROOT/usr/share/%{name}/
cp -fr ./resources $RPM_BUILD_ROOT/usr/share/%{name}/
mkdir -p $RPM_BUILD_ROOT/usr/share/%{name}/resources/translations
cp -fr ./src/images $RPM_BUILD_ROOT/usr/share/%{name}/src/
cp -fr ./src/qss $RPM_BUILD_ROOT/usr/share/%{name}/resources/

install -Dpm 755 ./bin/%{name}graph $RPM_BUILD_ROOT/usr/bin/%{name}graph
install -Dpm 755 ./bin/%{name}      $RPM_BUILD_ROOT/usr/bin/%{name}
install -Dpm 755 ./src/translations/*.qm $RPM_BUILD_ROOT/usr/share/%{name}/src/translations

%files
%defattr(-,root,root,0755)
/usr/bin/
#/usr/bin/%{name}/graph
/usr/doc/%{name}
/usr/include/%{name}
/usr/share/%{name}
#/usr/share/%{name}/resources
#/usr/share/%{name}/src/translations
#/usr/share/%{name}/src/images
#/usr/share/%{name}/src/qss
%clean
rm -rf $RPM_BUILD_ROOT
rm -fr /root/rpmbuild/BUILD/%{name}-%{version}

