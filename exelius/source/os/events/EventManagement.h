#pragma once

#include <EASTL/list.h>

// Events in Exelius are currently blocking, meaning when an event occurs it
// immediately gets dispatched and must be dealt with right then an there.
// For the future, a better strategy might be to buffer events in an event
// bus and process them during the "event" part of the update stage.

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class Event;

	/// <summary>
	/// Inheret from this class in order to Observe OS events.
	/// </summary>
	class OSEventObserver
	{
	public:
		OSEventObserver() = default;
		OSEventObserver(const OSEventObserver&) = delete;
		OSEventObserver(OSEventObserver&&) = delete;
		OSEventObserver& operator=(const OSEventObserver&) = delete;
		OSEventObserver& operator=(OSEventObserver&&) = delete;
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
		eastl::list<OSEventObserver*> m_observers;
	public:
		OSEventMessenger() = default;
		OSEventMessenger(const OSEventMessenger&) = delete;
		OSEventMessenger(OSEventMessenger&&) = delete;
		OSEventMessenger& operator=(const OSEventMessenger&) = delete;
		OSEventMessenger& operator=(OSEventMessenger&&) = delete;
		~OSEventMessenger() = default;

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
		/// <param name="observerToRemove">Observer to remove.</param>
		void RemoveObserver(OSEventObserver& observerToRemove);
	};
}