/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/
#pragma once

#include "SensorBase.h"
#include "RobotState.h"
#include "Task.h"
#include "HAL/HAL.hpp"

struct HALControlWord;
class AnalogInput;

/**
 * Provide access to the network communication data to / from the Driver Station.
 */
class DriverStation : public SensorBase, public RobotStateInterface
{
public:
	enum Alliance
	{
		kRed,
		kBlue,
		kInvalid
	};

	virtual ~DriverStation();
	static DriverStation *GetInstance();

	static const uint32_t kJoystickPorts = 4;
	static const uint32_t kJoystickAxes = 6;

	float GetStickAxis(uint32_t stick, uint32_t axis);
	short GetStickButtons(uint32_t stick);

	bool IsEnabled();
	bool IsDisabled();
	bool IsAutonomous();
	bool IsOperatorControl();
	bool IsTest();
	bool IsNewControlData();
	bool IsFMSAttached();

	Alliance GetAlliance();
	uint32_t GetLocation();
	void WaitForData();
	double GetMatchTime();
	float GetBatteryVoltage();

	MUTEX_ID GetUserStatusDataSem()
	{
		return m_statusDataSemaphore;
	}

	/** Only to be used to tell the Driver Station what code you claim to be executing
	 *   for diagnostic purposes only
	 * @param entering If true, starting disabled code; if false, leaving disabled code */
	void InDisabled(bool entering)
	{
		m_userInDisabled = entering;
	}
	/** Only to be used to tell the Driver Station what code you claim to be executing
	 *   for diagnostic purposes only
	 * @param entering If true, starting autonomous code; if false, leaving autonomous code */
	void InAutonomous(bool entering)
	{
		m_userInAutonomous = entering;
	}
	/** Only to be used to tell the Driver Station what code you claim to be executing
	 *   for diagnostic purposes only
	 * @param entering If true, starting teleop code; if false, leaving teleop code */
	void InOperatorControl(bool entering)
	{
		m_userInTeleop = entering;
	}
	/** Only to be used to tell the Driver Station what code you claim to be executing
	 *   for diagnostic purposes only
	 * @param entering If true, starting test code; if false, leaving test code */
	void InTest(bool entering)
	{
		m_userInTest = entering;
	}

protected:
	DriverStation();

	void GetData();
	void SetData();

private:
	static void InitTask(DriverStation *ds);
	static DriverStation *m_instance;

	void Run();

	HALControlWord m_controlWord;
	HALAllianceStationID m_allianceStationID;
	HALJoystickAxes m_joystickAxes[kJoystickPorts];
	HALJoystickButtons m_joystickButtons[kJoystickPorts];

	MUTEX_ID m_statusDataSemaphore;
	Task m_task;
	SEMAPHORE_ID m_newControlData;
	MUTEX_ID m_packetDataAvailableSem;
	MULTIWAIT_ID m_waitForDataSem;
	double m_approxMatchTimeOffset;
	bool m_userInDisabled;
	bool m_userInAutonomous;
	bool m_userInTeleop;
	bool m_userInTest;
};