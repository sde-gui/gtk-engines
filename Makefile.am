SUBDIRS = engines metal redmond95

EXTRA_DIST = \
	autogen.sh	\
	COPYING.GPL	\
	COPYING.LGPL	\
	gtk-engines-2.pc.in \
	gtk-engines.spec.in \
	gtk-engines.spec \
	makefile.msc \
	README.win32

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = gtk-engines-2.pc

rpm : dist
	rpmbuild --nodeps -ta $(PACKAGE)-$(VERSION).tar.gz
	@rm -f $(PACKAGE)-$(VERSION).tar.gz
