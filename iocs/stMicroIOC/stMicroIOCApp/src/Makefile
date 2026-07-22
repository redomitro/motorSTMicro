# SPDX-FileCopyrightText: 2003 Argonne National Laboratory
#
# SPDX-License-Identifier: EPICS

TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = stMicroIOC
# stMicroIOC.dbd will be created and installed
DBD += stMicroIOC.dbd

# stMicroIOC.dbd will be made up from these files:
stMicroIOC_DBD += base.dbd
stMicroIOC_DBD += drvAsynSPI.dbd
stMicroIOC_DBD += asyn.dbd
stMicroIOC_DBD += motorSupport.dbd
stMicroIOC_DBD += devihm02a1.dbd

# Include dbd files from all support applications:
#stMicroIOC_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#stMicroIOC_LIBS += xxx
stMicroIOC_LIBS += asyn motor stMicroMotor drvAsynSPI

# stMicroIOC_registerRecordDeviceDriver.cpp derives from stMicroIOC.dbd
stMicroIOC_SRCS += stMicroIOC_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
stMicroIOC_SRCS_DEFAULT += stMicroIOCMain.cpp
stMicroIOC_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#stMicroIOC_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
stMicroIOC_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
