/**
 * Event Schedular library for scheduling network events.
 * Chris Costello and Austin Longo
 **/
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <map>
#include <queue>
#include <mutex>
#include <ctime>
#include <chrono>
#include <vector>
#include <sys/select.h>
#include "ThreadPool.h"

using namespace std;
/** 
 * Making this global to be accessable by all classes
 * Still looking for a better solution.
 **/
struct EventTime
{
	int eventID;
	long execTime; //delete this later.
	
};
	
/** Making new class for priority queue compatability */
class CompareEventTime {
public:
	/**
	 * Used by priority queue to order based on exec time.
	 * Orders earliest first.
	 **/
	bool operator()(EventTime &e1, EventTime &e2) {
		if (e2.execTime < e1.execTime) {
			return true;
		}
		return false;
	}
};

class EventScheduler {
public:
	EventScheduler(size_t maxEvents);
	~EventScheduler();
	int eventSchedule(void evFunction(void *), void* arg, int timeout);
	void eventCancel(int eventID);
	
private:
	int max_events;
	int event_num;
	// map of eventID to function to execute
	// schedule execution with priority queue where
	// the smallest delay has highest priority

	struct FunctionInfo
	{
		void (*function)(void*);
		void* arg;
		timeval go_time;
		bool isCancelled;
	};
	
	typedef map<int, FunctionInfo> eventFunctionMap;
	typedef priority_queue<EventTime, vector<EventTime>, CompareEventTime> functionQueue;
	eventFunctionMap efMap;
	functionQueue fQueue;
	
	ThreadPool *t_pool;
	mutex data_mutex;

	long getExecTime(int timeout);
	static void executeEvents(void* arg);
	long getNow();
	
};


EventScheduler::EventScheduler(size_t maxEvents) {
	// cout << "EventScheduler created with max " << maxEvents << " events." << endl;
	max_events = maxEvents;
	event_num = 0;
	t_pool = new ThreadPool(max_events);
	// tp->dispatch_thread(executeEvents, NULL);
}

EventScheduler::~EventScheduler() {
	while(!fQueue.empty()) {
		usleep(700);
	}
	delete t_pool;	
}

int 
EventScheduler::eventSchedule(void evFunction(void *), void* arg, int timeout) {
	int thisEvent = event_num;

	// Set timeval times
	int secs = 0;
	int msecs = timeout * 1000;
	while (msecs >= 1000000) {
		msecs -= 1000000;
		secs++;
	}
	// Record the function to call plus the arg
	FunctionInfo f; 
	f.function = evFunction;
	f.arg = arg;
	f.go_time = {secs, msecs};
	f.isCancelled = false;

	// Record the eventID with the time to execute.
	EventTime e;
	e.eventID = thisEvent;
	e.execTime = getExecTime(timeout);

	// Store info in map and priority queue
	data_mutex.lock();
	efMap.insert(eventFunctionMap::value_type(thisEvent, f));
	fQueue.push(e);
	data_mutex.unlock();

	// cout << "Scheduled function with timeout " << timeout << " event id = " << event_num << endl;
	t_pool->dispatch_thread(executeEvents, this);
	
	event_num++;
	return thisEvent;
}

void
EventScheduler::eventCancel(int eventID) {
	/* This will not remove events from the queue,
	   only the map.
	   When the eventID is not found in the map,
	   that item will simply get popped and the 
	   event executer will move on. */
	
	// Remove entry from map
	data_mutex.lock();
	efMap.find(eventID)->second.isCancelled = true;
	data_mutex.unlock();
}


long
EventScheduler::getExecTime(int timeout) {
	// NOTE: Assuming timeout is in milliseconds.
	using namespace std::chrono;
	long now = getNow();
	long execTime = now + milliseconds(timeout).count();
	return execTime;
}

long
EventScheduler::getNow() {
	using namespace std::chrono;
	return duration_cast<milliseconds>(
		high_resolution_clock::now().time_since_epoch()).count();
}

void 
EventScheduler::executeEvents(void *arg) {
	EventScheduler* es = (EventScheduler*) arg;
	
	es->data_mutex.lock();	
	// grab top event
	int id = es->fQueue.top().eventID;
	eventFunctionMap::iterator it = es->efMap.find(id);
	es->data_mutex.unlock();
	
	select(0, NULL, NULL, NULL, &it->second.go_time);


	if(it->second.isCancelled) {
		//Cancelled
		es->data_mutex.lock();
		// cout << "Event confirmed cancelled" << endl;
		es->fQueue.pop();
		es->efMap.erase(id);
		es->data_mutex.unlock();
	} else {
		// cout << "Dispached thead for event " << id << endl;
		// execute the function
		(*(it->second.function))(it->second.arg);
		
		es->data_mutex.lock();
		es->fQueue.pop();
		es->efMap.erase(id);
		es->data_mutex.unlock();
	}
}

