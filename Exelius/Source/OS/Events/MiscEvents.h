#pragma once

#include "Source/OS/Events/Event.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Sensors that the engine can manage.
	/// Currently these mimic the sensors available from SFML
	/// </summary>
	enum class SensorType
	{
		INVALID,
		Accelerometer,    //!< Measures the raw acceleration (m/s^2)
		Gyroscope,        //!< Measures the raw rotation rates (degrees/s)
		Magnetometer,     //!< Measures the ambient magnetic field (micro-teslas)
		Gravity,          //!< Measures the direction and intensity of gravity, independent of device acceleration (m/s^2)
		UserAcceleration, //!< Measures the direction and intensity of device acceleration, independent of the gravity (m/s^2)
		Orientation,      //!< Measures the absolute 3D orientation (degrees)
	};

	/// <summary>
	/// Event specific for the sensors the engine can manage.
	/// </summary>
	class SensorEvent
		: public Event
	{
		SensorType m_type;
		float m_x;
		float m_y;
		float m_z;
	public:
		/// <summary>
		/// Sensor event creation.
		/// The data is up for interpretation based on the reading sensor.
		/// </summary>
		/// <param name="x">Sensor X data.</param>
		/// <param name="y">Sensor Y data.</param>
		/// <param name="z">Sensor Z data.</param>
		/// <param name="type">Type of sensor triggering the event. Default Type is INVALID.</param>
		SensorEvent(float x, float y, float z, SensorType type = SensorType::INVALID)
			: m_type(type)
			, m_x(x)
			, m_y(y)
			, m_z(z)
		{

		}

		SensorEvent() = delete;
		SensorEvent(const SensorEvent&) = delete;
		SensorEvent(SensorEvent&&) = delete;
		SensorEvent& operator=(const SensorEvent&) = delete;
		SensorEvent& operator=(SensorEvent&&) = delete;
		virtual ~SensorEvent() = default;

		EVENT_CLASS_TYPE(Misc)
		EVENT_CLASS_CATEGORY((int)EventCategory::Misc | (int)EventCategory::Input)
	};
}