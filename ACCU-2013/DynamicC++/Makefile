#
# Makefile
#
# $Id: //poco/1.4/Foundation/samples/dir/Makefile#1 $
#
# Makefile for Poco dir
#

include $(POCO_BASE)/build/rules/global

INCLUDE += -I$(POCO_BOOST_INCLUDE) -I/home/parallels/Downloads/double-conversion/src
SYSLIBS += -lfolly
COMMONFLAGS += -std=c++11

objects = DynamicCpp variant variant_visitors TypeErasure folly_dynamic DynamicVar

target         = DynamicCpp
target_version = 1
target_libs    = PocoFoundation

include $(POCO_BASE)/build/rules/exec
