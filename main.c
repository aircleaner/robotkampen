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


void resetRegualtor()
{
	masterPower = INIT_POWER * DIRECTION;
	slavePower = INIT_POWER * DIRECTION;
	error = 0;
	kp = 4;
	//Reset the encoders.
  SensorValue[LEFT_MOTOR] = 0;
  SensorValue[RIGHT_MOTOR] = 0;
}

void driveStraight(int timeToRunMs)
{
	// time run in ms
	int timeRunMs = 0;

  //Repeat ten times a second.
  while(timeToRunMs > timeRunMs)
  {
    //Set the motor powers to their respective variables.
    motor[LEFT_MOTOR] = masterPower;
    motor[RIGHT_MOTOR] = slavePower;

    //This is where the magic happens. The error value is set as a scaled value representing the amount the slave
    //motor power needs to change. For example, if the left motor is moving faster than the right, then this will come
    //out as a positive number, meaning the right motor has to speed up.
    error = SensorValue[LEFT_MOTOR] - SensorValue[RIGHT_MOTOR];

    //This adds the error to slavePower, divided by kp. The '+=' operator literally means that this expression really says
    //"slavePower = slavepower + error / kp", effectively adding on the value after the operator.
    //Dividing by kp means that the error is scaled accordingly so that the motor value does not change too much or too
    //little. You should 'tune' kp to get the best value. For us, this turned out to be around 5.
    slavePower += error / kp;

    //Reset the encoders every loop so we have a fresh value to use to calculate the error.
    SensorValue[LEFT_MOTOR] = 0;
    SensorValue[RIGHT_MOTOR] = 0;

    //Makes the loop repeat ten times a second. If it repeats too much we lose accuracy due to the fact that we don't have
    //access to floating point math, however if it repeats to little the proportional algorithm will not be as effective.
    //Keep in mind that if this value is changed, kp must change accordingly.
    wait1Msec(100);
    timeRunMs += 100;
  }
}

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
