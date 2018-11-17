#define LEFT_MOTOR motorB
#define RIGHT_MOTOR motorA
#define SYNC_MOTOR synchAB

#define TURN_LEFT 1
#define WATER_MOTOR motorC

#define DIRECTION -1
#define INIT_POWER 50
#define SLOW_POWER 15

#define TICKTOMM 37
#define TICK90DEG 234

#pragma config(Sensor, S4,     touchSensor,         sensorTouch)
#pragma DebuggerWindows("nxtLCDScreen")

#define FORWARD 0
#define BACKWARD 1
#define RIGHT 2
#define LEFT 3
#define FORWARD_TO_STOP 4
#define SLEEP 5

#define COMMAND_LENGTH 3*2

int path[COMMAND_LENGTH] = { FORWARD, 100, SLEEP, 200, BACKWARD, 100};

void driveStraightDistance(int centimeter, int masterPower)
{
	nSyncedMotors = synchNone;
  int tickGoal = centimeter * TICKTOMM;

  //This will count up the total encoder ticks despite the fact that the encoders are constantly reset.
  int totalTicks = 0;

  //Initialise slavePower as masterPower - 5 so we don't get huge error for the first few iterations. The
  //-5 value is based off a rough guess of how much the motors are different, which prevents the robot from
  //veering off course at the start of the function.
  int slavePower = masterPower - 0;

  int error = 0;

  int kp = 1;

  SensorValue[LEFT_MOTOR] = 0;
  SensorValue[RIGHT_MOTOR] = 0;

  //Monitor 'totalTicks', instead of the values of the encoders which are constantly reset.
  while(abs(totalTicks) < tickGoal /*&& !SensorValue[sensorTouch]*/)
  {
    //Proportional algorithm to keep the robot going straight.
    motor[LEFT_MOTOR] = masterPower;
    motor[RIGHT_MOTOR] = slavePower;

    error = SensorValue[LEFT_MOTOR] - SensorValue[RIGHT_MOTOR];

    slavePower += error / kp;

    SensorValue[LEFT_MOTOR] = 0;
    SensorValue[RIGHT_MOTOR] = 0;

    wait1Msec(100);

    //Add this iteration's encoder values to totalTicks.
    totalTicks+= SensorValue[LEFT_MOTOR];
  }
  motor[LEFT_MOTOR] = 0; // Stop the loop once the encoders have counted up the correct number of encoder ticks.
  motor[RIGHT_MOTOR] = 0;
}

void turn45Degree(int turnDirection)
{
	nSyncedMotors = SYNC_MOTOR;
	// Rotate in place to turn robot
	nSyncedTurnRatio = -100;              // rotate in place
	nMotorEncoderTarget[RIGHT_MOTOR] = TICK90DEG;    // move 200 encoder counts and stop
	motor[RIGHT_MOTOR] = 50 * turnDirection;

	while(nMotorRunState[RIGHT_MOTOR] != runStateIdle)  // while Motor B is still running:
	{
  	// Do nothing
	}
	motor[RIGHT_MOTOR] = 0;
}

task main()
{
  int i = 0;
  for (i = 0; i < COMMAND_LENGTH; i += 2)
  {
  	switch (path[i])
    {
    case FORWARD:
  		driveStraightDistance(path[i+1], INIT_POWER * DIRECTION);
      break;
    case BACKWARD:
  		driveStraightDistance(path[i+1], INIT_POWER * -DIRECTION);
      break;
    case RIGHT:
      turn45Degree(-TURN_LEFT);
      break;
    case LEFT:
    	turn45Degree(TURN_LEFT);
    	break;
    case FORWARD_TO_STOP:
  		driveStraightDistance(path[i+1], SLOW_POWER * DIRECTION);
    	break;
    case SLEEP:
    	wait1Msec(path[i+1]);
    	break;
    default:
      displayTextLine(4, "BAD COMMAND!");
      break;
    }
  }
}
