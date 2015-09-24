#
# Makefile
#
# $Id$
#
# Makefile for macchina.io WebEvent bundle
#

BUNDLE_TOOL = $(POCO_BASE)/OSP/BundleCreator/$(POCO_HOST_BINDIR)/bundle

include $(POCO_BASE)/build/rules/global

objects = FileServerBundleActivator FileServerRequestHandler

target         = io.macchina.services.fileserver
target_version = 1
target_libs    = PocoOSPWebEvent PocoOSPWeb PocoOSP PocoRemotingNG PocoUtil PocoXML PocoNet PocoFoundation

postbuild      = $(BUNDLE_TOOL) -n$(OSNAME) -a$(OSARCH) -o../bundles FileServer.bndlspec

include $(POCO_BASE)/build/rules/dylib
