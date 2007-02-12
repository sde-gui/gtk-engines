SUBDIRS = engines themes test

EXTRA_DIST = \
	autogen.sh	\
	COPYING.GPL	\
	COPYING.LGPL	\
	gtk-engines-2.pc.in \
	gtk-engines.spec.in \
	gtk-engines.spec

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = gtk-engines-2.pc


test:	all
	cd test && $(MAKE) $(AM_MAKEFLAGS) test

rpm : dist
	rpmbuild --nodeps -ta $(PACKAGE)-$(VERSION).tar.gz
	@rm -f $(PACKAGE)-$(VERSION).tar.gz
