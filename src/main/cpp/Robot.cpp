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
    frc::Shuffleboard::GetTab("Driver Station").Add("Is SuperUser", isSuper).GetEntry();
    i_dist = frc::Shuffleboard::GetTab("Driver Station").Add("Distance", 0).GetEntry();
    i_switch = frc::Shuffleboard::GetTab("Driver Station").Add("Button",0).GetEntry();
    frc::Shuffleboard::GetTab("Driver Station").Add("Gear", -gear+5).GetEntry();

    if(frc::DriverStation::GetInstance().GetAlliance() == frc::DriverStation::kBlue){
        if(isPartnerEnabled)
            LED.Set(.87);
        else
            LED.Set(-0.09);
        
    }
    else
    {
        if(isPartnerEnabled)
            LED.Set(.59);
        else
            LED.Set(-0.11);
    }
}

void Robot::RobotPeriodic() {
    if(c_driverController.GetBumper(frc::GenericHID::JoystickHand::kRightHand)&& c_driverController.GetBumper(frc::GenericHID::JoystickHand::kLeftHand)&&
        c_driverController.GetStickButton(frc::GenericHID::JoystickHand::kRightHand) && c_driverController.GetStickButton(frc::GenericHID::JoystickHand::kLeftHand)){
            isSuper = true;
        }
    i_dist.SetDouble(i_distance.GetRange());
    i_switch.ForceSetBoolean(i_shooterSwitch.Get());
    if (c_driverController.GetStartButtonPressed()){
        rumbleTime.Start();
        isPartnerEnabled = !isPartnerEnabled;
        c_driverController.SetRumble(frc::GenericHID::RumbleType::kLeftRumble,.5);
        }
   
   if (rumbleTime.Get()>2){
     c_driverController.SetRumble(frc::GenericHID::RumbleType::kLeftRumble,0);
     c_driverController.SetRumble(frc::GenericHID::RumbleType::kRightRumble,0);
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
    if(m_topShooter.Get()>0 || m_bottomShooter.Get()>0)
        shooterStart();
}
void Robot::TeleopPeriodic() {

    isPartnerEnabled ?  d_mechanum.DriveCartesian(c_driverController.GetX(frc::GenericHID::JoystickHand::kLeftHand)/gear, -c_driverController.GetY(frc::GenericHID::JoystickHand::kLeftHand)/gear,c_driverController.GetX(frc::GenericHID::JoystickHand::kRightHand)/gear) :
                        d_mechanum.DriveCartesian(-c_driverController.GetX(frc::GenericHID::JoystickHand::kLeftHand)/gear, c_driverController.GetY(frc::GenericHID::JoystickHand::kLeftHand)/gear,c_driverController.GetX(frc::GenericHID::JoystickHand::kRightHand)/gear);
    if(c_driverController.GetBackButtonPressed()){
        shooterStart();
    }
    c_driverController.GetBumper(frc::GenericHID::kRightHand)? m_ballArticulator.Set(.50):m_ballArticulator.Set(0);
    c_driverController.GetTriggerAxis(frc::GenericHID::kRightHand)? m_shooterSet.Set(.25):m_shooterSet.Set(0);
    //c_driverController.GetTriggerAxis(frc::GenericHID::kLeftHand)? m_ballArticulator.Set(-0.25):m_ballArticulator.Set(0);
    (c_driverController.GetPOV()==180)? m_endgameLift.Set(1): m_endgameLift.Set(0);
    (c_driverController.GetPOV(0)==0)? m_endgameDeploy.Set(frc::Relay::Value::kReverse) : m_endgameDeploy.Set(frc::Relay::Value::kOff);
    if(c_driverController.GetPOV()==90){
        m_colorSpinner.Set(0.05);
    }
    else if(c_driverController.GetPOV()== 270){
        m_colorSpinner.Set(-0.25);
    }
    else{
        m_colorSpinner.Set(0);
    }
    transmissionSet(c_driverController.GetAButtonPressed(), c_driverController.GetXButtonPressed());
    if(isSuper){
        transmissionSet(c_driverController.GetStickButtonPressed(frc::GenericHID::JoystickHand::kRightHand),
                        c_driverController.GetStickButtonPressed(frc::GenericHID::JoystickHand::kLeftHand));
    }
}


void Robot::TestInit() {}
void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif