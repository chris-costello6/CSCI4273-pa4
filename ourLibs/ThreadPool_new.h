/*********************/
/* This is a basic implementation for a thread pool library
*/

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <map>
#include <queue>

#define DEFAULT_THREAD_COUNT 10

using namespace std;

class ThreadPool
{
	public:
		ThreadPool( );
	    ThreadPool(size_t threadCount);
	    ~ThreadPool( );
	    int dispatch_thread(void dispatch_function(void*), void *arg);
	    bool thread_avail( );

	private:
		// work structure (function and arg pointer)
    	typedef struct
    	{
	    	void (*dispatch_function)(void*);
	    	void* arg;
		} work_t;


    	size_t 							m_PoolSize;			// the number of threads
    	pthread_t* 						m_ThreadPool;		// array of threads
    	map<pthread_t, pthread_mutex_t> m_ThreadMutexes;	// map of thread ID to associated mutex
    	map<pthread_t, bool>			m_ThreadStatus;		// map of thread ID to availibility boolean
	   	queue<work_t*>					m_WorkQueue;		// queue that holds pending work
	   	pthread_mutex_t 				m_QueueMutex;		// mutex to protect m_WorkQueue
	   	bool 							m_IsWorkDone;		// boolean to signal all threads to exit

    	void initialize_pool( );	
    	void* start_thread();
    	static void* work_helper(void *func) 
    	{
			return ((ThreadPool*)func)->start_thread();
		}

};

ThreadPool::ThreadPool( )
	:m_PoolSize(DEFAULT_THREAD_COUNT)
{
	initialize_pool();
	cout << "ThreadPool created with " << m_PoolSize << " threads." << endl;
}

ThreadPool::ThreadPool(size_t threadCount)
	:m_PoolSize(threadCount)
{
	initialize_pool();
	cout << "ThreadPool created with " << m_PoolSize << " threads." << endl;
}

ThreadPool::~ThreadPool( )
{
	m_IsWorkDone = true;

	for (int i = 0; i < m_PoolSize; i++)
		pthread_join(m_ThreadPool[i], NULL);

    delete[] m_ThreadPool;

    pthread_mutex_destroy(&m_QueueMutex);
}

int 
ThreadPool::dispatch_thread(void dispatch_function(void*), void *arg)
{
	work_t* newWork = new work_t;
	newWork->dispatch_function = dispatch_function;
	newWork->arg = arg;

	m_WorkQueue.push(newWork);

	return 0;
}

bool
ThreadPool::thread_avail( )
{
	for(int i = 0; i < m_PoolSize; i++)	
	{
		if(m_ThreadStatus[m_ThreadPool[i]])
			return true;
	}

	// No threads were found available
	return false;
}


void
ThreadPool::initialize_pool( )
{
	m_IsWorkDone = false;

	m_ThreadPool = new pthread_t[m_PoolSize];

	pthread_mutex_init(&m_QueueMutex, NULL);

	for (int i = 0; i < m_PoolSize; i++)
	{
		// Declare the mutex
		pthread_mutex_t mutex;

		// Initialize each mutex -> unlocked
		if(pthread_mutex_init(&mutex, NULL))
		{
			printf("ERROR: Failed to initialize mutex.\n");
			exit(EXIT_FAILURE);
		}

		// Declare the thread
		pthread_t thread;
		
		// Add the thread to the array
		m_ThreadPool[i] = thread;

		// Add mapping of thread to mutex
		m_ThreadMutexes[thread] = mutex;

		// Add mapping of thread to availability status
		m_ThreadStatus[thread] = true;

		// Create the thread and send it to wait for work
		pthread_create(&thread, NULL, work_helper, this);
	}

}

void*
ThreadPool::start_thread()
{
	while(1)
	{
		// Try to lock the queue mutex
		// This will block if it's already locked and will
		// wakeup once it's available, then execute the else{}
		if (pthread_mutex_lock(&m_QueueMutex)) { }
		else
		{
			m_ThreadStatus[pthread_self()] = false;
			void (*dispatch)(void*);
			work_t *newWork;
			bool foundWork = false;
			
			// Check if there is work to do
			if(m_WorkQueue.size() > 0)
			{
				// Get new work at the front of the queue and remove it
				newWork = m_WorkQueue.front();
				m_WorkQueue.pop();

				// "Pull out" the work function and execute it
				dispatch = newWork->dispatch_function;

				foundWork = true;
			}

			pthread_mutex_unlock(&m_QueueMutex);

			if (foundWork)
			{
				// Execute it
				dispatch(newWork->arg);

				// Delete the struct now that we're done
				delete newWork;
			}
		}

		

		if (m_IsWorkDone && m_WorkQueue.empty())
			pthread_exit(NULL);


		m_ThreadStatus[pthread_self()] = true;
	}

	return NULL;
}