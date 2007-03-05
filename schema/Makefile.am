xmldir = $(pkgdatadir)

EXTRA_DIST= \
	clearlooks.xml.in.in	\
	hc.xml.in.in		\
	redmond.xml.in.in	\
	crux.xml.in.in		\
	industrial.xml.in.in	\
	smooth.xml.in.in	\
	glide.xml.in.in		\
	mist.xml.in.in		\
	thinice.xml.in.in

xml_DATA = $(BUILD_SCHEMAS)
xml_in_files = $(xml_DATA:.xml=.xml.in)
xml_in_in_files = $(xml_DATA:.xml=.xml.in.in)

@INTLTOOL_XML_RULE@

CLEANFILES = $(xml_DATA) $(xml_in_files)
