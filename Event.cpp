/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

	Event - Provides event baseclass

*/

#include "Event.h"


GKit::Event::Event(unsigned int _numEvents, unsigned int _maxEvents)
{
	numEvents = _numEvents;
	maxEvents = _maxEvents;

	p_eventCallbacks = new EventStruct[numEvents];
	for (unsigned int i = 0; i < numEvents; i++)
	{
		p_eventCallbacks[i].numCallback = 0;
		p_eventCallbacks[i].callbacks = new Callback[maxEvents];
	}
}

GKit::Event::~Event()
{
	for (unsigned int i = 0; i < numEvents; i++)
	{
		delete[] p_eventCallbacks[i].callbacks;
	}

	delete[] p_eventCallbacks;
}


void GKit::Event::HookEvent(int event, CallbackFunc callback, void* callbackVar)
{
	if (p_eventCallbacks[event].numCallback >= maxEvents)
		return;

	p_eventCallbacks[event].callbacks[p_eventCallbacks[event].numCallback].func = callback;
	p_eventCallbacks[event].callbacks[p_eventCallbacks[event].numCallback].var = callbackVar;

	p_eventCallbacks[event].numCallback++;
}


void GKit::Event::DoEvents(int event, void* infoStruct)
{
	ExecuteEvents(event, infoStruct);
}



void GKit::Event::ExecuteEvents(int event, void* infoStruct)
{
	for (unsigned int i = 0; i < p_eventCallbacks[event].numCallback; i++)
	{
		p_eventCallbacks[event].callbacks[i].func(infoStruct, p_eventCallbacks[event].callbacks[i].var);
	}
}