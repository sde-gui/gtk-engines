%define  ver     0.1
%define  rel     SNAP
%define  prefix  /usr

Summary: Default GTK+ theme engines
Name: gtk-engines
Version: %ver
Release: %rel
Copyright: GPL
Group: X11/Libraries
Source: gtk-engines-%{PACKAGE_VERSION}.tar.gz
Url: http://gtk.themes.org
BuildRoot:/var/tmp/gtk-engines-%{PACKAGE_VERSION}-root
Docdir: %{prefix}/doc

%description
These are the graphical engines for the various GTK+ toolkit themes.
Included themes are:

  - Notif
  - redmond95
  - Pixmap
  - Metal (Java swing-like)


%prep
%setup 

%build
# Needed for snapshot releases.
if [ ! -f configure ]; then
  CFLAGS="$RPM_OPT_FLAGS" ./autogen.sh --prefix=%prefix
else
  CFLAGS="$RPM_OPT_FLAGS" ./configure --prefix=%prefix
fi


if [ "$SMP" != "" ]; then
  (make "MAKE=make -k -j $SMP"; exit 0)
  make
else
  make
fi

%install

rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT

#
# makefile is broken, sets exec_prefix explicitely.
#
make exec_prefix=$RPM_BUILD_ROOT/%{prefix} prefix=$RPM_BUILD_ROOT/%{prefix} install 

%clean
#rm -rf $RPM_BUILD_ROOT

%changelog

* Fri Nov 20 1998 Michael Fulbright <drmike@redhat.com>

- First try at a spec file

%files
%defattr(-, root, root)
%doc COPYING README ChangeLog
%{prefix}/lib/gtk/themes/engines/*
%{prefix}/share/themes/Pixmap/*
%{prefix}/share/themes/Metal/*
%{prefix}/share/themes/Notif/*
%{prefix}/share/themes/Redmond95/*