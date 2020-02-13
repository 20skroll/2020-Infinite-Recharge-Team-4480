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
    i_topSpeed = frc::Shuffleboard::GetTab("Spinners").Add("Top Spinner Speed", 0.0).WithWidget("Text View").GetEntry();
    i_bottomSpeed = frc::Shuffleboard::GetTab("Spinners").Add("Bottom Spinner Speed",0.0).WithWidget("Text View").GetEntry();
    
}

void Robot::RobotPeriodic() {
    if (c_driverController.GetStartButtonPressed()){
        rumbleTime.Start();
        isPartnerEnabled = true;
        c_driverController.SetRumble(frc::GenericHID::RumbleType::kLeftRumble,.5);
        c_partnerController.SetRumble(frc::GenericHID::RumbleType::kRightRumble,0.5);
        }
    if (c_partnerController.GetStartButtonPressed()){
     rumbleTime.Start();
     isPartnerEnabled = false;
     c_driverController.SetRumble(frc::GenericHID::RumbleType::kRightRumble,0.5);
     c_partnerController.SetRumble(frc::GenericHID::RumbleType::kLeftRumble,0.5);
   }
   if (rumbleTime.Get()>2){
     c_driverController.SetRumble(frc::GenericHID::RumbleType::kLeftRumble,0);
     c_partnerController.SetRumble(frc::GenericHID::RumbleType::kRightRumble,0);
     c_driverController.SetRumble(frc::GenericHID::RumbleType::kRightRumble,0);
     c_partnerController.SetRumble(frc::GenericHID::RumbleType::kLeftRumble,0);
     rumbleTime.Stop();
     rumbleTime.Reset();
   }
}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {}
void Robot::TeleopPeriodic() {
    //////////////////////////////////DRIVER CONTROLLER//////////////////////////////
    if(!isPartnerEnabled){
        d_mechanum.DriveCartesian(-c_driverController.GetX(frc::GenericHID::JoystickHand::kLeftHand)/3, c_driverController.GetY(frc::GenericHID::JoystickHand::kLeftHand)/3,c_driverController.GetX(frc::GenericHID::JoystickHand::kRightHand)/3);
        if(c_driverController.GetBackButtonPressed()){
            shooterStart();
        }

    }

    /////////////////////////////////PARTNER CONTROLLER/////////////////////////////////
    if(isPartnerEnabled){
        d_mechanum.DriveCartesian(c_partnerController.GetX(frc::GenericHID::JoystickHand::kLeftHand)/3, -c_partnerController.GetY(frc::GenericHID::JoystickHand::kLeftHand)/3,c_partnerController.GetX(frc::GenericHID::JoystickHand::kRightHand)/3);
        c_partnerController.GetYButton() ? m_shooterSet.Set(.25): m_shooterSet.Set(0);
        
        if(c_partnerController.GetBackButtonPressed()){
            shooterStart();
        }
    }

}


void Robot::TestInit() {}
void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
