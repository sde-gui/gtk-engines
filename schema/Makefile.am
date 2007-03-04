xmldir = $(pkgdatadir)

xml_DATA = $(BUILD_SCHEMAS)
xml_in_files = $(xml_DATA:.xml=.xml.in)
xml_in_in_files = $(xml_DATA:.xml=.xml.in.in)

@INTLTOOL_XML_RULE@

EXTRA_DIST=$(xml_in_in_files)

CLEANFILES = $(xml_DATA) $(xml_in_files)
