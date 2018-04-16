# spec file for package kuzya (Version 2.0.0)
#
# Copyright (c) 2009 Fedora Linux, Lviv, Ukraine.
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.
#

Name:      kuzya
Summary:   Integrated Development Environment for students
Version:   2.1.6
Source0:   %{name}-%{version}.tar.bz2 
Group:     Development/Tools
Release:   0
License:   GPL
Packager:  Andriy Shevchyk <shevchyk@users.sourceforge.net>
Vendor:    PLLUG
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:  qt-devel >= 4
BuildRequires:  qscintilla-devel >= 2.2
Requires:       qscintilla >= 2.2
Requires:       qt >= 4

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

BuildRequires: qt4-devel, qscintilla-devel 

%prep
%setup -q

%build
qmake-qt4
make

%install
rm -rf $RPM_BUILD_ROOT
make install INSTALL_ROOT=$RPM_BUILD_ROOT

%files
%defattr(-,root,root,0755)
/usr/bin/
/usr/include/
/usr/share/%{name}

%clean
rm -rf $RPM_BUILD_ROOT

