#pragma once

#include "Source/OS/Events/Event.h"

#include <list>

// Events in Exelius are currently blocking, meaning when an event occurs it
// immediately gets dispatched and must be dealt with right then an there.
// For the future, a better strategy might be to buffer events in an event
// bus and process them during the "event" part of the update stage.


/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// Anything with a "_" prefixed is private to the engine and is not recommended for use by client applications.
/// Example: _Log
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Inheret from this class in order to Observe OS events.
	/// </summary>
	class OSEventObserver
	{
	public:
		virtual ~OSEventObserver() = default;

		/// <summary>
		/// Override this function in order to recieve and handle any OS
		/// events.
		/// </summary>
		/// <param name="evnt">Event to handle.</param>
		virtual void HandleEvent(Event& evnt) = 0;
	};

	/// <summary>
	/// Exelius OS event manager. Events in Exelius are currently blocking,
	/// meaning when an event occurs it immediately gets dispatched and
	/// must be dealt with right then an there. For the future, a better
	/// strategy might be to buffer events in an event bus and process
	/// them during the "event" part of the update stage.
	/// </summary>
	class OSEventMessenger
	{
		std::list<OSEventObserver*> m_observers;
	public:

		/// <summary>
		/// Notify all OS event observers of an event.
		/// </summary>
		/// <param name="evnt">Event to handle.</param>
		void NotifyObservers(Event& evnt);

		/// <summary>
		/// Add an observer to the list of notifiable observers.
		/// </summary>
		/// <param name="observerToAdd">Observer to add.</param>
		void AddObserver(OSEventObserver& observerToAdd);
		
		/// <summary>
		/// Remove an observer from the list of notifiable observers.
		/// </summary>
		/// <param name="observerToRemove">observer to remove.</param>
		void RemoveObserver(OSEventObserver& observerToRemove);
	};
}