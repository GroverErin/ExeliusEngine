#include "EXEPCH.h"
#include "Source/OS/Events/EventManagement.h"
#include "Source/OS/Events/Event.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Notify all OS event observers of an event.
	/// </summary>
	/// <param name="evnt">Event to handle.</param>
	void OSEventMessenger::NotifyObservers(Event& evnt)
	{
		for (auto* pObserver : m_observers)
		{
			assert(pObserver);
			pObserver->HandleEvent(evnt);

			// Not sure how I want to handle event propagation.
			// Should there be a IsHandled check after each observer call?
		}
	}

	/// <summary>
	/// Add an observer to the list of notifiable observers.
	/// </summary>
	/// <param name="observerToAdd">Observer to add.</param>
	void OSEventMessenger::AddObserver(OSEventObserver& observerToAdd)
	{
		// Should probably check to make sure that the
		// observer to be added is not already in the list.
		m_observers.push_back(&observerToAdd);
	}

	/// <summary>
	/// Remove an observer from the list of notifiable observers.
	/// </summary>
	/// <param name="observerToRemove">observer to remove.</param>
	void OSEventMessenger::RemoveObserver(OSEventObserver& observerToRemove)
	{
		// I don't believe this will throw if it is not found.
		m_observers.remove(&observerToRemove);
	}
}