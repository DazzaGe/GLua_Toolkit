/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

*/

#pragma once




namespace GKit
{
	typedef void (*CallbackFunc) (void* infoStruct, void* arg);


	struct Callback
	{
		CallbackFunc	func;
		void*			var;
	};

	struct EventStruct
	{
		unsigned int	numCallback;
		Callback*		callbacks;
	};


	class Event
	{
		public:
								Event(unsigned int _numEvents, unsigned int _maxEvents);
								~Event();

			void				HookEvent(int event, CallbackFunc callback, void* callbackVar);

			void				DoEvents(int event, void* infoStruct);

		private:
			void				ExecuteEvents(int event, void* infoStruct);


		private:
			EventStruct*		p_eventCallbacks;
			unsigned int		numEvents;
			unsigned int		maxEvents;
	};
}