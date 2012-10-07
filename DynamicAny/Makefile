#
# Makefile
#
# $Id: //poco/svn/Foundation/samples/DynamicAny/Makefile#1 $
#
# Makefile for Poco DynamicAny test
#

include $(POCO_BASE)/build/rules/global

objects = DynamicAny ConversionCalls DynamicAnySizeConvert \
	DynamicAnySizeExtract AnySize lexical_cast_size

target         = DynamicAny
target_version = 1
target_libs    = PocoFoundation

RELEASEOPT_CC   = -DNDEBUG -O2
RELEASEOPT_CXX  = -DNDEBUG -O2

include $(POCO_BASE)/build/rules/exec
