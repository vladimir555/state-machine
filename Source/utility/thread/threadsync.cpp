#include	"threadsync.h"
#include	"thread.h"
#include	<sys/time.h>
#include	<iostream>

namespace utility {

using	namespace std;

// -------------- class SyncMonitor ---------------
class	SyncMonitor
{
	friend class Synchronizable;
	friend class Synchronized;
	public:
		SyncMonitor()
		{
			pthread_mutex_init(&mutex, &mutexAttributies);
			pthread_cond_init(&condition, NULL);
		}
		
		~SyncMonitor()
		{
			pthread_cond_destroy(&condition);
			pthread_mutex_destroy(&mutex);
		}

	private:	
		pthread_mutex_t mutex;
		pthread_cond_t condition;
		static pthread_mutexattr_t mutexAttributies;
		class SyncMonitorInitializer
		{
		public:
			SyncMonitorInitializer()
			{
				pthread_mutexattr_init(&SyncMonitor::mutexAttributies);
				pthread_mutexattr_setpshared(&SyncMonitor::mutexAttributies, PTHREAD_PROCESS_PRIVATE);
				pthread_mutexattr_settype(&SyncMonitor::mutexAttributies, PTHREAD_MUTEX_ERRORCHECK);
			}
			~SyncMonitorInitializer()
			{
				pthread_mutexattr_destroy(&mutexAttributies);
			}
		};
		static SyncMonitorInitializer initializer;
};

pthread_mutexattr_t SyncMonitor::mutexAttributies;
SyncMonitor::SyncMonitorInitializer SyncMonitor::initializer;

// -------------- class Synchronizable ---------------
Synchronizable::Synchronizable()
	: syncMonitor(NULL)
{
	syncMonitor = new SyncMonitor();
}
Synchronizable::~Synchronizable()
{
	delete syncMonitor;
	syncMonitor = NULL;
}
void	Synchronizable::notify() const 
{
	pthread_cond_signal(&syncMonitor->condition);
}
void	Synchronizable::notifyAll() const 
{
	pthread_cond_broadcast(&syncMonitor->condition);
}
void	Synchronizable::wait() const 
{
	pthread_cond_wait(&syncMonitor->condition, &syncMonitor->mutex);
}
int	Synchronizable::wait(unsigned long mills) const 
{
	timeval currTime;
	if (0 != gettimeofday(&currTime, NULL))
		return -1;
	timespec waitingTime;
	waitingTime.tv_sec = currTime.tv_sec + mills / 1000;
	waitingTime.tv_nsec = (currTime.tv_usec + (mills % 1000) * 1000) * 1000;
	return pthread_cond_timedwait(&syncMonitor->condition, &syncMonitor->mutex, &waitingTime);
}
bool	Synchronizable::checkIsInterrupted()
{
	if (!Thread::getCurrentThread()->isInterrupted())
		return false;
	onInterrupt();
	return true;
}
// -------------- class Synchronized --------------- 
Synchronized::Synchronized(Synchronizable *syncObj)
	: object(syncObj)
{
	if (NULL != object)
		pthread_mutex_lock(&object->syncMonitor->mutex);
}

Synchronized::~Synchronized()
{
	if (NULL != object)
		pthread_mutex_unlock(&object->syncMonitor->mutex);
}

} // namespace qp

