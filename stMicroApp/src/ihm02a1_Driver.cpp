#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdfloat>

#include <iocsh.h>
#include <epicsThread.h>

#include <asynOctetSyncIO.h>

#include <epicsExport.h>
#include "ihm02a1_Driver.h"

#define NINT(f) (int)((f)>0 ? (f)+0.5 : (f)-0.5)

int32_t revEndian(int32_t x){
	int32_t y=0;
	for(int i = 0; i < 4; i++){
		y <<= 8; //shift LSB over one; shifts zeroes on first run
		y += x & 255; //copy LSB of x into LSB of y
		x >>= 8; //discard LSB
	}
	return y;
}

int32_t packInt(int32_t x, uint8_t regLength, bool isSigned){

  uint8_t sign = x >> 31; //true for negative numbers
  if(sign && !isSigned){
    print("Sign error\n");
    return 0;
  }

  x &= (1 << regLength-isSigned) - 1; //zero out unwanted bits; leave room for a sign bit if needed
  if(isSigned){
    x |= sign << regLength - 1;
  }
	x <<= 8*(3-(regLength-1)/8)); //rounds up to 24 (1-byte arg), 16, 8 or 0 (4-byte arg)
	int32_t arg = revEndian(x);
	return arg;
}

int32_t unpackInt(int32_t resp, uint8_t regLength, bool isSigned){

  int32_t x = revEndian(resp);
  x >>= 8*(3-(regLength-1)/8));
  uint8_t sign;
  if(isSigned){
    sign = x >> regLength - 1;
    x |= sign << 31;
  }
  return x;
}

ihm02a1Controller::ihm02a1Controller(const char *portName, const char *ihm02a1PortName, int numAxes, 
                                 double movingPollPeriod, double idlePollPeriod)
  :  asynMotorController(portName, numAxes, NUM_ihm02a1_PARAMS,
                         0, // No additional interfaces beyond those in base class
                         0, // No additional callback interfaces beyond those in base class
                         ASYN_CANBLOCK | ASYN_MULTIDEVICE,
                         1, // autoconnect
                         0, 0)  // Default priority and stack size
{
  int axis;
  asynStatus status;
  ihm02a1Axis *pAxis;
  static const char *functionName = "ihm02a1Controller::ihm02a1Controller";

  /* Connect to ihm02a1 controller */
  status = pasynOctetSyncIO->connect(ihm02a1PortName, 0, &pasynUserController_, NULL);
  if (status) {
    asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR, 
      "%s: cannot connect to IHM02A1 controller\n",
      functionName);
  }
  for (axis=0; axis<numAxes; axis++) {
    pAxis = new ihm02a1Axis(this, axis);
  }

  startPoller(movingPollPeriod, idlePollPeriod, 2);
}

/** Creates a new ihm02a1Controller object.
  * Configuration command, called directly or from iocsh
  * \param[in] portName          The name of the asyn port that will be created for this driver
  * \param[in] ihm02a1PortName    The name of the drvAsynIPPPort that was created previously to connect to the ihm02a1 controller 
  * \param[in] numAxes           The number of axes that this controller supports
  * \param[in] movingPollPeriod  The time in ms between polls when any axis is moving
  * \param[in] idlePollPeriod    The time in ms between polls when no axis is moving
  */

extern "C" int ihm02a1CreateController(const char *portName, const char *ihm02a1PortName, int numAxes, 
                                    int movingPollPeriod, int idlePollPeriod)
{
  ihm02a1Controller *pihm02a1Controller
    = new ihm02a1Controller(portName, ihm02a1PortName, numAxes, movingPollPeriod/1000., idlePollPeriod/1000.);
  pihm02a1Controller = NULL;

  return(asynSuccess);
}

/** Reports on status of the driver
  * \param[in] fp The file pointer on which report information will be written
  * \param[in] level The level of report detail desired
  *
  * If details > 0 then information is printed about each axis.
  * After printing controller-specific information it calls asynMotorController::report()
  */

void ihm02a1Controller::report(FILE *fp, int level)
{
  fprintf(fp, "IHM02A1 motor driver %s, numAxes=%d, moving poll period=%f, idle poll period=%f\n", 
  this->portName, numAxes_, movingPollPeriod_, idlePollPeriod_);

  // Call the base class method
  asynMotorController::report(fp, level);
}

/** Returns a pointer to an ihm02a1Axis object.
  * Returns NULL if the axis number encoded in pasynUser is invalid.
  * \param[in] pasynUser asynUser structure that encodes the axis index number. */

ihm02a1Axis* ihm02a1Controller::getAxis(asynUser *pasynUser)
{
  return static_cast<ihm02a1Axis*>(asynMotorController::getAxis(pasynUser));
}

/** Returns a pointer to an ihm02a1Axis object.
  * Returns NULL if the axis number encoded in pasynUser is invalid.
  * \param[in] axisNo Axis index number. */

ihm02a1Axis* ihm02a1Controller::getAxis(int axisNo)
{
  return static_cast<ihm02a1Axis*>(asynMotorController::getAxis(axisNo));
}


asynStatus ihm02a1Controller::writeReadFrame(uint8_t* input, uint8_t* output, uint8_t len, uint8_t mask)
{
  asynStatus status;
  uint8_t rx[32] = {0};
  uint8_t tx[32] = {0};
  size_t nwrite, nread;
  int eomReason;

  //interlace the data to be sent
  
  for(uint8_t i = 0; i < numAxes_; i++){

  //order of devices; remember device directly connected to host MOSI
  //is last in chain and corresponds to mask msb. I love SPI too

    if((mask >> i) & 1){ //extract corresponding mask bit with bitwise ops

      for(uint8_t j = 0; j < len; j++){
        memcpy(tx+i+j*numAxes_, output+j, 1);

        /* if 4 devices connected:
        device 0 will read from bits 0, 4, 8, 12 etc
        device 1 will read from bits 1, 5, 9, 13
        etc */
      }
    }
  } //braces formally optional

  //do the data transfer

  for(int i = 0; i < len; i++){
    status = pasynOctetSyncIO->writeRead(pasynUserController_, rx+i*numAxes_,
                                         numAxes_, tx+i*numAxes_, sizeof(rx),
                                         DEFAULT_CONTROLLER_TIMEOUT, &nwrite, &nread, &eomReason);

  // Repeated calls to pasynOctetSyncIO which will hopefully propagate through SPI as desired.
  }

  //deinterlace the received data

  for(uint8_t i = 0; i < numAxes_; i++){
    for (uint8_t j = 0; j < len; j++){
      memcpy(input+j+i*len, rx+j*len+i, 1); //basically transposing a matrix here
    }
  }

  return status;
}
