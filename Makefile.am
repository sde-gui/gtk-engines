SUBDIRS = engines themes test schema
DIST_SUBDIRS = $(SUBDIRS) po

EXTRA_DIST = \
	autogen.sh	\
	COPYING.GPL	\
	COPYING.LGPL	\
	gtk-engines-2.pc.in \
	gtk-engines.spec.in \
	gtk-engines.spec \
	intltool-extract.in \
	intltool-merge.in \
	intltool-update.in

DISTCLEANFILES = \
	intltool-extract \
	intltool-merge \
	intltool-update

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = gtk-engines-2.pc


test:	all
	cd test && $(MAKE) $(AM_MAKEFLAGS) test

rpm : dist
	rpmbuild --nodeps -ta $(PACKAGE)-$(VERSION).tar.gz
	@rm -f $(PACKAGE)-$(VERSION).tar.gz
