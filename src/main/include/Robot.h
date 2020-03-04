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
#include <frc/Spark.h>


#include <frc/drive/MecanumDrive.h>
#include <frc/XboxController.h>
#include <frc/GenericHID.h>

#include <frc/Timer.h>

#include <networktables/NetworkTableEntry.h>
#include<networktables/NetworkTableInstance.h>

#include<frc/shuffleboard/Shuffleboard.h>

#include<frc/DigitalInput.h>
#include<frc/Relay.h>

#include <frc/DriverStation.h>

#include<rev/Rev2mDistanceSensor.h>


class Robot : public frc::TimedRobot {
  private:

  rev::Rev2mDistanceSensor i_distance{rev::Rev2mDistanceSensor::Port::kOnboard, rev::Rev2mDistanceSensor::DistanceUnit::kInches};
  int gear = 2;
  bool isSuper = false;
  double distance;
  bool isAtTower = false;
  nt::NetworkTableEntry i_topSpeed, i_bottomSpeed, i_dist, i_switch;

  frc::Timer atonTime;


  frc::PWMSparkMax m_shooterSet{0}, m_ballArticulator{1}, m_endgameLift{2}, m_colorSpinner{3};
  frc::Relay m_endgameDeploy{0};

  frc::DigitalInput i_shooterSwitch{0};
  frc::Spark LED{9};

  WPI_TalonFX m_topShooter{3}, m_bottomShooter{2};

 
  frc::XboxController c_driverController{0};


  #if isTwoDrivers
  frc::XboxController c_partnerController{1};
  #endif

  bool isPartnerEnabled = false;
  frc::Timer rumbleTime;

  void shootSequence(){
    if(m_topShooter.Get()==0 && m_bottomShooter.Get()== 0){
      shooterStart();
      frc::Wait(1);
    }
    m_shooterSet.Set(.25);
    frc::Wait(.45);
    m_shooterSet.Set(0);
    m_ballArticulator.Set(.50);
    frc::Wait(1);
    m_ballArticulator.Set(0);
    frc::Wait(1.5);

  /*  m_shooterSet.Set(.25);
    frc::Wait(.5);
    if(i_shooterSwitch.Get()==true){
      m_shooterSet.Set(0);}
    m_ballArticulator.Set(.5);
    frc::Wait(2);
    m_ballArticulator.Set(0);*/
  }
  void oneRot(){
    m_shooterSet.Set(0);
  }

  void shooterStart(){
       
      if(m_topShooter.Get()==0||m_bottomShooter.Get()==0){
        m_topShooter.Set(.30);
        m_bottomShooter.Set(-0.12);
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
  if(gearDown && gear>(isSuper? 1:2))
    gear--;
 }

 void launchReset(){
  m_shooterSet.Set(.5);
  if(i_shooterSwitch.Get()==1){
    m_shooterSet.Set(0.0);
  }   
  return;
  // add bool to put in set or check mode, or use interrupts, cuz you can but probably shouldn't
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
