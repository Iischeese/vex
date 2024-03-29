/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       ianloftis                                                 */
/*    Created:      2/20/2024, 5:37:00 PM                                     */
/*    Description:  IQ2 project                                               */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#pragma region VEXcode Generated Robot Configuration
// Make sure all required headers are included.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


#include "vex.h"

using namespace vex;

// Brain should be defined by default
brain Brain;


// START IQ MACROS
#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)
// END IQ MACROS


// Robot configuration code.
inertial BrainInertial = inertial();
motor LeftDriveSmart = motor(PORT1, 2, false);
motor RightDriveSmart = motor(PORT6, 2, true);
drivetrain Drivetrain = drivetrain(LeftDriveSmart, RightDriveSmart, 200, 70, 70, mm, 1);
motor ElevatorMotorA = motor(PORT5, true);
motor ElevatorMotorB = motor(PORT2, false);
motor_group Elevator = motor_group(ElevatorMotorA, ElevatorMotorB);

controller Controller = controller();
gyro Gyros = gyro(PORT9);
motor LiftMotorA = motor(PORT11, true);
motor LiftMotorB = motor(PORT12, false);
motor_group Lift = motor_group(LiftMotorA, LiftMotorB);




// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
// define variables used for controlling motors based on controller inputs
bool DrivetrainLNeedsToBeStopped_Controller = true;
bool DrivetrainRNeedsToBeStopped_Controller = true;

// define a task that will handle monitoring inputs from Controller
int rc_auto_loop_function_Controller() {
  // process the controller input every 20 milliseconds
  // update the motors based on the input values
  while(true) {
    if(RemoteControlCodeEnabled) {
      
      // calculate the drivetrain motor velocities from the controller joystick axies
      // left = AxisA
      // right = AxisD
      int drivetrainLeftSideSpeed = Controller.AxisA.position();
      int drivetrainRightSideSpeed = Controller.AxisD.position();
      
      // check if the value is inside of the deadband range
      if (drivetrainLeftSideSpeed < 5 && drivetrainLeftSideSpeed > -5) {
        // check if the left motor has already been stopped
        if (DrivetrainLNeedsToBeStopped_Controller) {
          // stop the left drive motor
          LeftDriveSmart.stop();
          // tell the code that the left motor has been stopped
          DrivetrainLNeedsToBeStopped_Controller = false;
        }
      } else {
        // reset the toggle so that the deadband code knows to stop the left motor nexttime the input is in the deadband range
        DrivetrainLNeedsToBeStopped_Controller = true;
      }
      // check if the value is inside of the deadband range
      if (drivetrainRightSideSpeed < 5 && drivetrainRightSideSpeed > -5) {
        // check if the right motor has already been stopped
        if (DrivetrainRNeedsToBeStopped_Controller) {
          // stop the right drive motor
          RightDriveSmart.stop();
          // tell the code that the right motor has been stopped
          DrivetrainRNeedsToBeStopped_Controller = false;
        }
      } else {
        // reset the toggle so that the deadband code knows to stop the right motor next time the input is in the deadband range
        DrivetrainRNeedsToBeStopped_Controller = true;
      }
      
      // only tell the left drive motor to spin if the values are not in the deadband range
      if (DrivetrainLNeedsToBeStopped_Controller) {
        LeftDriveSmart.setVelocity(drivetrainLeftSideSpeed, percent);
        LeftDriveSmart.spin(forward);
      }
      // only tell the right drive motor to spin if the values are not in the deadband range
      if (DrivetrainRNeedsToBeStopped_Controller) {
        RightDriveSmart.setVelocity(drivetrainRightSideSpeed, percent);
        RightDriveSmart.spin(forward);
      }
    }
    // wait before repeating the process
    wait(20, msec);
  }
  return 0;
}

task rc_auto_loop_task_Controller(rc_auto_loop_function_Controller);

#pragma endregion VEXcode Generated Robot Configuration

// Include the IQ Library
#include "vex.h"
  
// Allows for easier use of the VEX Library
using namespace vex;

void myCalibrate(){
  Gyros.calibrate(calExtended);
  Gyros.setHeading(0,degrees);

  // Set Speeds
  Drivetrain.setDriveVelocity(70, percent);
  Drivetrain.setTurnVelocity(20, percent);
  Elevator.setVelocity(100, percent);

  // Set Torque
  Elevator.setMaxTorque(100, percent);
  Elevator.setVelocity(100,percent);

  // Set Braking methods
  Drivetrain.setStopping(coast);
  Elevator.setStopping(coast);

}

void logo(){
  Brain.Screen.clearScreen();

  Brain.Screen.setFont(prop30);
  Brain.Screen.print("Raindow Men");

  Brain.Screen.newLine();
  Brain.Screen.newLine();

  Brain.Screen.setFont(mono20);
  Brain.Screen.print("v.2.7.a");
}


//  72 x 96
void program() {
  Elevator.spin(forward);

  // Collect starting 4 greens
  Drivetrain.driveFor(forward, 36.5, inches);
  Drivetrain.driveFor(reverse, 17, inches);
  Drivetrain.turnFor(right, 90, degrees);

  // Collect 4G 1P square
  Drivetrain.driveFor(forward, 26, inches);
  Drivetrain.turnFor(left, 90, degrees);

  // Backup into first depot & release blocks 
  Drivetrain.driveFor(reverse, 16.5, inches);
  Lift.spin(forward);
  wait(4, seconds);
  Lift.stop();
  wait(4, seconds);
  Lift.spin(reverse);
  wait(4, seconds);
  Lift.stop();

  // Come out of depot
  Drivetrain.driveFor(forward, 16. , inches);
  Drivetrain.turnFor(left, 90, degrees);
  Drivetrain.driveFor(forward, 24, inches);
}

int main() {
  myCalibrate();
  Brain.playSound(headlightsOn);
  logo();
  program();

  return 0;
}