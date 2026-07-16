#include <cstdint>
#include "asynMotorController.h"
#include "asynMotorAxis.h"

//define registers
#define ABS_POS       0x01
#define EL_POS        0x02
#define MARK          0x03
#define SPEED         0x04
#define ACC           0x05
#define DEC           0x06
#define MAX_SPEED     0x07
#define MIN_SPEED     0x08
#define KVAL_HOLD     0x09
#define KVAL_RUN      0x0a
#define KVAL_ACC      0x0b
#define KVAL_DEC      0x0c
#define INT_SPEED     0x0d
#define ST_SLP        0x0e
#define FN_SLP_ACC    0x0f
#define FN_SLP_DEC    0x10
#define K_THERM       0x11
#define ADC_OUT       0x12
#define OCD_TH        0x13
#define STALL_TH      0x14
#define FS_SPD        0x15
#define STEP_MODE     0x16
#define ALARM_EN      0x17
#define CONFIG        0x18

//define commands
#define NOP           0x00
#define GET_PARAM     0x20
#define GO_HOME       0x70
#define GO_MARK       0x78
#define SOFT_HIZ      0xa0
#define HARD_HIZ      0xa8
#define SOFT_STOP     0xb0
#define HARD_STOP     0xb8
#define RESET_DEVICE  0xc0
#define RESET_POS     0xd8
#define GET_STATUS    0xd0

class epicsShareClass ihm02a1Axis : public asynMotorAxis{
public:
  /* These are the methods we override from the base class */
  ihm02a1Axis(class ihm02a1Controller *pC, int axis);
  void report(FILE *fp, int level);
  asynStatus move(double position, int relative, double min_velocity, double max_velocity, double acceleration);
  asynStatus moveVelocity(double min_velocity, double max_velocity, double acceleration);
  asynStatus home(double min_velocity, double max_velocity, double acceleration, int forwards);
  asynStatus stop(double acceleration);
  asynStatus poll(bool *moving);
  //  asynStatus setPosition(double position);
  //  asynStatus setClosedLoop(bool closedLoop);

private:
  ihm02a1Controller *pC_;  /**< Pointer to the asynMotorController to which this axis belongs.
                            *   Abbreviated because it is used very frequently */
  asynStatus setParameter(uint8_t param, int32_t value);
  asynStatus servoPower(bool power);

  friend class ihm02a1Controller;
};

class epicsShareClass ihm02a1Controller : public asynMotorController {
public:
  ihm02a1Controller(const char *portName, const char *ihm02a1PortName, int numAxes, double movingPollPeriod, double idlePollPeriod);

  void report(FILE *fp, int level);
  ihm02a1Axis* getAxis(asynUser *pasynUser);
  ihm02a1Axis* getAxis(int axisNozz);
  asynStatus writeReadController(const char *output, char *input, size_t maxChars, size_t *nread, double timeout);

private:
  uint8_t syncCounter;
  asynStatus writeReadFrame(uint8_t length, uint8_t frameType, uint8_t *payload);

  friend class ihm02a1Axis;
};
