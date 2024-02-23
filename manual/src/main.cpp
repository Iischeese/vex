/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       ianloftis                                                 */
/*    Created:      2/20/2024, 5:19:09 PM                                     */
/*    Description:  IQ2 project                                               */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


#include "vex.h"

using namespace vex;

// Setup systems

brain Brain;

motor LeftDriveSmart = motor(PORT1, 2, false);
motor RightDriveSmart = motor(PORT6, 2, true);
drivetrain Drivetrain = drivetrain(LeftDriveSmart, RightDriveSmart, 200, 173, 76, mm, 1);
motor ElevatorMotorA = motor(PORT5, true);
motor ElevatorMotorB = motor(PORT2, false);
motor_group Elevator = motor_group(ElevatorMotorA, ElevatorMotorB);

controller Controller = controller();
gyro Gyros = gyro(PORT9);
motor LiftMotorA = motor(PORT11, true);
motor LiftMotorB = motor(PORT12, false);
motor_group Lift = motor_group(LiftMotorA, LiftMotorB);

// Drivetrain Controls
bool RemoteControlCodeEnabled = true;

bool DrivetrainLNeedsToBeStopped_Controller = true;
bool DrivetrainRNeedsToBeStopped_Controller = true;

int rc_auto_loop_function_Controller()
{

  while (true)
  {
    if (RemoteControlCodeEnabled)
    {

      int drivetrainLeftSideSpeed = Controller.AxisA.position();
      int drivetrainRightSideSpeed = Controller.AxisD.position();

      if (drivetrainLeftSideSpeed < 5 && drivetrainLeftSideSpeed > -5)
      {

        if (DrivetrainLNeedsToBeStopped_Controller)
        {

          LeftDriveSmart.stop();
          DrivetrainLNeedsToBeStopped_Controller = false;
        }
      }
      else
      {
        DrivetrainLNeedsToBeStopped_Controller = true;
      }
      if (drivetrainRightSideSpeed < 5 && drivetrainRightSideSpeed > -5)
      {
        if (DrivetrainRNeedsToBeStopped_Controller)
        {
          RightDriveSmart.stop();
          DrivetrainRNeedsToBeStopped_Controller = false;
        }
      }
      else
      {
        DrivetrainRNeedsToBeStopped_Controller = true;
      }

      if (DrivetrainLNeedsToBeStopped_Controller)
      {
        LeftDriveSmart.setVelocity(drivetrainLeftSideSpeed, percent);
        LeftDriveSmart.spin(forward);
      }

      if (DrivetrainRNeedsToBeStopped_Controller)
      {
        RightDriveSmart.setVelocity(drivetrainRightSideSpeed, percent);
        RightDriveSmart.spin(forward);
      }
    }

    wait(20, msec);
  }
  return 0;
}

task rc_auto_loop_task_Controller(rc_auto_loop_function_Controller);

#include "vex.h"

using namespace vex;

// Macro Controls

void R_UP()
{
  Elevator.spin(forward);
}

void R_DOWN()
{
  Elevator.spin(reverse);
}

void L3()
{
  if (Controller.ButtonL3.pressing() && Controller.ButtonR3.pressing())
  {
    Drivetrain.stop();
    Elevator.stop();
    Lift.stop();

    Brain.playSound(headlightsOff);
  }
}

void L_DOWN()
{
  Lift.spin(forward);
}

void L_UP()
{
  Lift.spin(reverse);
}

void myCalibrate()
{
  Gyros.calibrate(calExtended);

  // Set Speeds
  Drivetrain.setDriveVelocity(100, percent);
  Drivetrain.setTurnVelocity(70, percent);
  Elevator.setVelocity(100, percent);
  Lift.setVelocity(100, percent);

  // Set Torque
  Elevator.setMaxTorque(100, percent);
  Lift.setMaxTorque(100, percent);

  // Set Braking methods
  Drivetrain.setStopping(brake);
  Elevator.setStopping(coast);
}

void logo()
{
  Brain.Screen.clearScreen();

  Brain.Screen.setFont(prop30);
  Brain.Screen.print("Raindow Men");

  Brain.Screen.newLine();
  Brain.Screen.newLine();

  Brain.Screen.setFont(mono20);

  /*
  Build Number NN-TT-YYMMDD.AAA
  N Name
  T Type (auto manual)
  YYMMDD year month date
  AAA number 001,002,003 ect every time its built
  */
  Brain.Screen.print("RMML240223.002");
}

void setMacros()
{
  // Elevator
  Controller.ButtonRUp.pressed(R_UP);
  Controller.ButtonRDown.pressed(R_DOWN);

  // Shutdown mechanic
  Controller.ButtonL3.pressed(L3);
  Controller.ButtonR3.pressed(L3);

  // Lift
  Controller.ButtonLDown.pressed(L_DOWN);
  Controller.ButtonLUp.pressed(L_UP);
}

int main()
{
  myCalibrate();
  setMacros();
  Brain.playSound(headlightsOn);
  logo();

  return 0;
}