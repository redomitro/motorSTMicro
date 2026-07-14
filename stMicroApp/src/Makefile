TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# build a support library

LIBRARY_IOC += stMicro

# xxxRecord.h will be created from xxxRecord.dbd
#DBDINC += xxxRecord
# install stMicro.dbd into <top>/dbd
DBD += stMicro.dbd

# specify all source files to be compiled and added to the library
#stMicro_SRCS += xxx

stMicro_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

