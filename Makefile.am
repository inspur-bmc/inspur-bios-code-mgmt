AM_DEFAULT_SOURCE_EXT = .cpp

# Build these headers, don't install them
noinst_HEADERS = \
	version.hpp \
	item_updater.hpp \
	activation.hpp \
	flash.hpp

if HAVE_SYSTEMD
systemdsystemunit_DATA = inspur-bios-code-mgmt.service \
						 inspur-bios-updater.service
endif

sbin_PROGRAMS = \
	inspur-bios-updater

BUILT_SOURCES = \
	xyz/openbmc_project/Software/Image/error.cpp \
	xyz/openbmc_project/Software/Image/error.hpp \
	org/openbmc/Associations/server.cpp \
	org/openbmc/Associations/server.hpp

CLEANFILES = ${BUILT_SOURCES}

inspur_bios_updater_SOURCES = \
	activation.cpp \
	version.cpp \
	item_updater.cpp \
	item_updater_main.cpp

include static/Makefile.am.include

nodist_inspur_bios_updater_SOURCES = \
	org/openbmc/Associations/server.cpp

generic_cxxflags = \
	$(SYSTEMD_CFLAGS) \
	$(PHOSPHOR_DBUS_INTERFACES_CFLAGS) \
	$(SDBUSPLUS_CFLAGS) \
	$(PHOSPHOR_LOGGING_CFLAGS) \
	-flto
generic_ldflags = \
	$(SYSTEMD_LIBS) \
	$(PHOSPHOR_DBUS_INTERFACES_LIBS) \
	$(SDBUSPLUS_LIBS) \
	$(PHOSPHOR_LOGGING_LIBS) \
	-lstdc++fs \
	-lssl \
	-lcrypto

xyz/openbmc_project/Software/Image/error.hpp: ${top_srcdir}/xyz/openbmc_project/Software/Image.errors.yaml
	@mkdir -p `dirname $@`
	$(SDBUSPLUSPLUS) -r $(srcdir) error exception-header xyz.openbmc_project.Software.Image > $@

xyz/openbmc_project/Software/Image/error.cpp: ${top_srcdir}/xyz/openbmc_project/Software/Image.errors.yaml
	@mkdir -p `dirname $@`
	$(SDBUSPLUSPLUS) -r $(srcdir) error exception-cpp xyz.openbmc_project.Software.Image > $@

org/openbmc/Associations/server.cpp: org/openbmc/Associations.interface.yaml
	@mkdir -p `dirname $@`
	$(SDBUSPLUSPLUS) -r $(srcdir) interface server-cpp org.openbmc.Associations > $@

org/openbmc/Associations/server.hpp: org/openbmc/Associations.interface.yaml
	@mkdir -p `dirname $@`
	$(SDBUSPLUSPLUS) -r $(srcdir) interface server-header org.openbmc.Associations > $@

inspur_bios_updater_CXXFLAGS = $(generic_cxxflags)
inspur_bios_updater_LDFLAGS = $(generic_ldflags)
