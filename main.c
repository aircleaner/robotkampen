#define LEFT_MOTOR motorB
#define RIGHT_MOTOR motorA
#define SYNC_MOTOR synchAB
#define DIRECTION -1
#define WATER_MOTOR motorC
#define INIT_POWER 30


//The powers we give to both motors. masterPower will remain constant while slavePower will change so that
//the right wheel keeps the same speed as the left wheel.
int masterPower = 0;
int slavePower = 0;
//Essentially the difference between the master encoder and the slave encoder. Negative if slave has
//to slow down, positive if it has to speed up. If the motors moved at exactly the same speed, this
//value would be 0.
int error = 0;
//'Constant of proportionality' which the error is divided by. Usually this is a number between 1 and 0 the
//error is multiplied by, but we cannot use floating point numbers. Basically, it lets us choose how much
//the difference in encoder values effects the final power change to the motor.
int kp = 5;

int path[100];


int driveForward(short degrees)
{
  nSyncedMotors = SYNC_MOTOR;
  nSyncedTurnRatio = +100;              // move in a straight line

  nMotorEncoder[motorB] = 0;
	nMotorEncoderTarget[RIGHT_MOTOR] = 360;

  motor[RIGHT_MOTOR] = INIT_POWER * DIRECTION;

	while(nMotorRunState[RIGHT_MOTOR] != runStateIdle)  // while Motor B is still running:
	{
  	// Do nothing
	}
	motor[RIGHT_MOTOR] = 0;
	return nMotorEncoder[motorB];
}

void turnRight()
{
	// TODO
}

task main()
{
  driveForward(360);
}
