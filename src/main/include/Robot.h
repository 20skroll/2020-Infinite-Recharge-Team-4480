/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/
#define isTwoDrivers false

#pragma once

#include <frc/TimedRobot.h>

#include <ctre/Phoenix.h>
#include <frc/PWMSparkMax.h>


#include <frc/drive/MecanumDrive.h>
#include <frc/XboxController.h>
#include <frc/GenericHID.h>
//#include <frc/buttons/POVButton.h>

#include <frc/Timer.h>

#include <networktables/NetworkTableEntry.h>
#include<networktables/NetworkTableInstance.h>

#include<frc/shuffleboard/Shuffleboard.h>

#include<frc/DigitalInput.h>
#include<frc/Relay.h>



class Robot : public frc::TimedRobot {
  private:
  int gear = 2;
  nt::NetworkTableEntry i_topSpeed, i_bottomSpeed;

 

  frc::PWMSparkMax m_shooterSet{0}, m_ballArticulator{1}, m_endgameLift{2}, m_colorSpinner{3};
  frc::Relay m_endgameDeploy{0};

frc::DigitalInput i_shooterSwitch{0};
  
  WPI_TalonFX m_topShooter{3}, m_bottomShooter{2};

 
  frc::XboxController c_driverController{0};
  //frc::POVButton povUP(c_driverController,0);

  #if isTwoDrivers
  frc::XboxController c_partnerController{1};
  #endif

  bool isPartnerEnabled = false;
  frc::Timer rumbleTime;

  void shooterStart(){
       
      if(m_topShooter.Get()==0||m_bottomShooter.Get()==0){
        m_topShooter.Set(i_topSpeed.GetDouble(.33));
        m_bottomShooter.Set(-i_bottomSpeed.GetDouble(.30));
        return;
      }
      else{
        m_topShooter.Set(0);
        m_bottomShooter.Set(0);       
      }
  }

 void transmissionSet(bool gearUp, bool gearDown){
   if(gearUp && gear<4)
    gear++;
  if(gearDown && gear>0)
    gear--;
 }
 
 public:
  void RobotInit() override;

  void RobotPeriodic() override;

  void AutonomousInit() override;
  void AutonomousPeriodic() override;

  void TeleopInit() override;
  void TeleopPeriodic() override;

  void TestInit() override;
  void TestPeriodic() override;



};
