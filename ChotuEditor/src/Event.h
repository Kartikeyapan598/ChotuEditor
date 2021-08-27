#pragma once

namespace CE
{
	class Event
	{
	public :
		virtual ~Event() = default;

	};

	class EventDispactcher : public Event
	{
	public :
		EventDispactcher(Event& event)
			:m_event(event)
		{
		}

	private :
		Event m_event;
	};
}