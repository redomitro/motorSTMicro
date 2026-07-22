#!../../bin/linux-x86_64/stMicroIOC

#- SPDX-FileCopyrightText: 2003 Argonne National Laboratory
#-
#- SPDX-License-Identifier: EPICS

#- You may have to change stMicroIOC to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/stMicroIOC.dbd"
stMicroIOC_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/stMicroIOC.db","user=rea")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=rea"
