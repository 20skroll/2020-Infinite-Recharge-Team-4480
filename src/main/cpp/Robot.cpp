/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Robot.h"
#include<rev/CANSparkMax.h>

rev::CANSparkMax d_frontLeft(5, rev::CANSparkMax::MotorType::kBrushless);
rev::CANSparkMax d_frontRight(4, rev::CANSparkMax::MotorType::kBrushless);
rev::CANSparkMax d_rearLeft(11, rev::CANSparkMax::MotorType::kBrushless);
rev::CANSparkMax d_rearRight(6, rev::CANSparkMax::MotorType::kBrushless);

frc::MecanumDrive d_mechanum{d_frontLeft,d_rearLeft,d_frontRight,d_rearRight};


void Robot::RobotInit() {
    i_distance.SetAutomaticMode(true);
    i_topSpeed = frc::Shuffleboard::GetTab("Driver Station").Add("Top Spinner Speed", 0.0).WithWidget("Text View").GetEntry();
    i_bottomSpeed = frc::Shuffleboard::GetTab("Driver Station").Add("Bottom Spinner Speed",0.0).WithWidget("Text View").GetEntry();
    frc::Shuffleboard::GetTab("Driver Station").Add("Is SuperUser", isSuper);
    i_dist = frc::Shuffleboard::GetTab("Driver Station").Add("Distance", 0).GetEntry();
    i_switch = frc::Shuffleboard::GetTab("Driver Station").Add("Button",0).GetEntry();
    if(frc::DriverStation::GetInstance().GetAlliance() == frc::DriverStation::kBlue){
        LED.Set(.87);
    }
    else
    {
        LED.Set(.59);
    }
}

void Robot::RobotPeriodic() {
    i_dist.SetDouble(i_distance.GetRange());
    i_switch.ForceSetBoolean(i_shooterSwitch.Get());
    if (c_driverController.GetStartButtonPressed()){
        rumbleTime.Start();
        isPartnerEnabled = !isPartnerEnabled;
        c_driverController.SetRumble(frc::GenericHID::RumbleType::kLeftRumble,.5);
        #if isTwoDrivers
            c_partnerController.SetRumble(frc::GenericHID::RumbleType::kRightRumble,0.5);
        #endif
        }
   
    #if isTwoDrivers
    if (c_partnerController.GetStartButtonPressed()){
     rumbleTime.Start();
     isPartnerEnabled = false;
     c_driverController.SetRumble(frc::GenericHID::RumbleType::kRightRumble,0.5);
     c_partnerController.SetRumble(frc::GenericHID::RumbleType::kLeftRumble,0.5);
     }
   #endif
   if (rumbleTime.Get()>2){
     c_driverController.SetRumble(frc::GenericHID::RumbleType::kLeftRumble,0);
     c_driverController.SetRumble(frc::GenericHID::RumbleType::kRightRumble,0);
     #if isTwoDrivers
     c_partnerController.SetRumble(frc::GenericHID::RumbleType::kRightRumble,0);
     c_partnerController.SetRumble(frc::GenericHID::RumbleType::kLeftRumble,0);
     #endif
     rumbleTime.Stop();
     rumbleTime.Reset();
   }
}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {
    if(i_distance.GetRange()<30 && i_distance.IsRangeValid()){
        d_mechanum.DriveCartesian(0,0,0);
        isAtTower =  true;

    }
    else{
        d_mechanum.DriveCartesian(0,0.25,0);
        isAtTower = false;
    }
    if(isAtTower){
    shootSequence();
    }
       
    
}

void Robot::TeleopInit() {
    shooterStart();
}
void Robot::TeleopPeriodic() {

    #if !isTwoDrivers
        isPartnerEnabled ?  d_mechanum.DriveCartesian(c_driverController.GetX(frc::GenericHID::JoystickHand::kLeftHand)/gear, -c_driverController.GetY(frc::GenericHID::JoystickHand::kLeftHand)/gear,c_driverController.GetX(frc::GenericHID::JoystickHand::kRightHand)/gear) :
                            d_mechanum.DriveCartesian(-c_driverController.GetX(frc::GenericHID::JoystickHand::kLeftHand)/gear, c_driverController.GetY(frc::GenericHID::JoystickHand::kLeftHand)/gear,c_driverController.GetX(frc::GenericHID::JoystickHand::kRightHand)/gear);
        if(c_driverController.GetBackButtonPressed()){
            shooterStart();
        }
        c_driverController.GetBumper(frc::GenericHID::kRightHand)? m_ballArticulator.Set(.50):m_ballArticulator.Set(0);
        c_driverController.GetTriggerAxis(frc::GenericHID::kRightHand)? m_shooterSet.Set(.25):m_shooterSet.Set(0);
        //c_driverController.GetTriggerAxis(frc::GenericHID::kLeftHand)? m_ballArticulator.Set(-0.25):m_ballArticulator.Set(0);
        (c_driverController.GetPOV()==180)? m_endgameLift.Set(1): m_endgameLift.Set(0);
        c_driverController.GetAButton()? m_colorSpinner.Set(0.5): m_colorSpinner.Set(0);
        (c_driverController.GetPOV(0)==0)? m_endgameDeploy.Set(frc::Relay::Value::kReverse) : m_endgameDeploy.Set(frc::Relay::Value::kOff);
        transmissionSet(c_driverController.GetAButtonPressed(), c_driverController.GetXButtonPressed());
    #endif

    #if isTwoDrivers
    ////////////////////Drive Modes/////////////////////////
    isPartnerEnabled ?  d_mechanum.DriveCartesian(c_partnerController.GetX(frc::GenericHID::JoystickHand::kLeftHand)/gear, -c_partnerController.GetY(frc::GenericHID::JoystickHand::kLeftHand)/gear,c_partnerController.GetX(frc::GenericHID::JoystickHand::kRightHand)/gear) :
                        d_mechanum.DriveCartesian(-c_driverController.GetX(frc::GenericHID::JoystickHand::kLeftHand)/gear, c_driverController.GetY(frc::GenericHID::JoystickHand::kLeftHand)/gear,c_driverController.GetX(frc::GenericHID::JoystickHand::kRightHand)/gear);
    if(c_driverController.GetBackButtonPressed()|| c_partnerController.GetBackButtonPressed()){
        shooterStart();
    }
    /////////////////////Driver Only/////////////////////////////
    if (!isPartnerEnabled){
        c_driverController.GetBumper(frc::GenericHID::kRightHand)? m_ballArticulator.Set(.50):m_ballArticulator.Set(0);
        c_driverController.GetTriggerAxis(frc::GenericHID::kRightHand)? m_shooterSet.Set(.25):m_shooterSet.Set(0);
        //c_driverController.GetTriggerAxis(frc::GenericHID::kLeftHand)? m_ballArticulator.Set(-0.25):m_ballArticulator.Set(0);
        c_driverController.GetBButton()? m_endgameLift.Set(0.5): m_endgameLift.Set(0);
        c_driverController.GetAButton()? m_colorSpinner.Set(0.5): m_colorSpinner.Set(0);
    }
    ///////////////////////Partner Only////////////////////////////
    if (isPartnerEnabled){
        c_partnerController.GetBumper(frc::GenericHID::kRightHand)? m_ballArticulator.Set(.50):m_ballArticulator.Set(0);
        c_partnerController.GetTriggerAxis(frc::GenericHID::kRightHand)? m_shooterSet.Set(.25):m_shooterSet.Set(0);
        //c_driverController.GetTriggerAxis(frc::GenericHID::kLeftHand)? m_ballArticulator.Set(-0.25):m_ballArticulator.Set(0);
        c_partnerController.GetBButton()? m_endgameLift.Set(0.5): m_endgameLift.Set(0);
        c_partnerController.GetAButton()? m_colorSpinner.Set(0.5): m_colorSpinner.Set(0);
    }
    #endif

}


void Robot::TestInit() {}
void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif


/*

TODO
LED
BUTTON
POV
*/