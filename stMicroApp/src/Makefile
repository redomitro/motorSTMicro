TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# build a support library

LIBRARY_IOC += stMicroMotor

# xxxRecord.h will be created from xxxRecord.dbd
#DBDINC += xxxRecord
# install stMicro.dbd into <top>/dbd
DBD += stMicro.dbd
INC += ihm02a1_Driver.h

# specify all source files to be compiled and added to the library
#stMicro_SRCS += xxx

stMicroMotor_SRCS += ihm02a1_Driver.cpp

stMicro_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

