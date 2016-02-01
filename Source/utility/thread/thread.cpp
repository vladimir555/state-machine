#include	"thread.h"
#include	<errno.h>
#include	<sstream>
#include	<signal.h>
#include	<string>
#include	<sys/time.h>
#include	<iostream>
#include    <bits/time.h>

namespace	utility {

using	namespace std;

void	*pthreadStart(void *param)
{
	Thread *thread = static_cast<Thread*>(param);
	thread->beforeTargetRun();
	thread->target->run();
	thread->afterTargetRun();
	return NULL;
}

//---------- определение статических атрибутов ----------
hash_map<pthread_t, Thread*> Thread::threads;
Synchronizable Thread::threadsLock;
Thread Thread::mainThread(NULL, "mainThread");

void	Thread::sleep(unsigned long mills) const
{
	timespec timer;
	timer.tv_sec = mills / 1000;
	timer.tv_nsec = (mills % 1000) * 1000 * 1000;
	nanosleep(&timer, NULL);
}

//----------- конструкторы, деструкторы и тп ----------
Thread::Thread(Runnable *_target, const std::string &_name)
	: Runnable(), error(0), target(_target), name(_name), pthread(0), alive(false), interrupted(false)//, sleepper(new Sleepper())
{
	Thread *tt = dynamic_cast<Thread*>(target);
	if (NULL != tt)
		throw std::exception();	
	initializeThread();
}
Thread::~Thread()
{
	if (isAlive())
	{
		if (pthread != getCurrentThreadID())
		{
			interrupt();
			cout << "if (pthread != getCurrentThreadID())" << endl;
			join(); // TODO: Подумать как лучше сделать.
		}
		else
		{
			cout << "!!!if (pthread != getCurrentThreadID())" << endl;
			unregisterThread(this);
		}
	}
}
void	Thread::initializeThread()
{
	if (NULL == target)	target = this;
	if (name.empty())
	{
		stringstream ss;
		ss << "Thread - " << static_cast<size_t>(threads.size());
		name = ss.str();
	}
}

//--------- защищенные методы ----------
void	Thread::beforeTargetRun()
{
	alive = true;
	interrupted = false;
	registerThread(this);
}
void	Thread::afterTargetRun()
{
	alive = false;
	unregisterThread(this);
}

//---------- публичные методы для работы с потоками - запуск потока, ожидание завершения работы потока, ----------
//---------- проверка состояния потока, завершение работы потока, получение идентификатора потока       ----------
bool	Thread::start()
{
	if (isAlive())	return false;
	int result = pthread_create(&pthread, NULL, pthreadStart, static_cast<void*>(this));
	if (0 != result)
	{
		error = errno;
		return false;
	}
	return true;
}
bool	Thread::join()
{
//	if (!isAlive())	return false;
	int result = pthread_join(pthread, NULL);
	if (0 != result)
	{
		error = errno;
		return false;
	}
	return true;
}
bool	Thread::join(unsigned long mills)
{
//	if (!isAlive())	return false;
	clock_t finish = clock() + static_cast<unsigned long int>((double(mills) / 1000 * double(CLOCKS_PER_SEC)));
	do 
	{
		if (ESRCH == pthread_kill(pthread, 0))
			return join();
	} while (clock() < finish);
	return false;
//	
//	timeb currTime;
//	ftime(&currTime);
//	timespec waitingTime;
//	cout << "currTime.tv_sec = " << currTime.time << endl;
//	cout << "currTime.tv_nsec = " << currTime.millitm << endl;	
//	waitingTime.tv_sec = currTime.time + mills / 1000;
//	waitingTime.tv_nsec = currTime.millitm + (mills % 1000) * 1000000000;
//	cout << "waitingTime.tv_sec = " << waitingTime.tv_sec << endl;
//	cout << "waitingTime.tv_nsec = " << waitingTime.tv_nsec << endl;
//	int result = pthread_timedjoin_np(pthread, NULL, &waitingTime);
//	if (0 != result)	return false;
//	return true;
}
void	Thread::interrupt()
{
	interrupted = true;// TODO: нужно чтобы был слип который можно было бы прерывать interruptом.
//	pthread_cond_broadcast(&sleepper->condition);
//	notifyAll();
	Synchronizable *syncObj = dynamic_cast<Synchronizable*>(target);
	if (NULL != syncObj)
		syncObj->notifyAll();
}
Thread	*Thread::getCurrentThread()
{
	return getCurrentThread(getCurrentThreadID());
}
//---------- методы для работы со списком потоков ----------
pthread_t Thread::getCurrentThreadID()
{
	pthread_t pid = pthread_self();
	return pid;
}
Thread *Thread::getCurrentThread(pthread_t threadID)
{
	Synchronized sync(&threadsLock);
	hash_map<pthread_t, Thread*>::const_iterator i = threads.find(threadID);
	if (i != threads.end()) 
	{
		return i->second; // ------------>
	}
	//тред в мапе отсутствует, это возможно только если находимся в корневом треде
	// в этом случае создаем для этого треда объект типа Thread, заносим его в список
	// и проставляем ему параметры
	mainThread.alive = true;
	mainThread.pthread = threadID;
	registerThread(&mainThread);
	return &mainThread;
}
void	Thread::unregisterThread(Thread *thread)
{
	Synchronized sync(&threadsLock);
	hash_map<pthread_t, Thread*>::iterator i = threads.find(thread->pthread);
	if (i != threads.end())
		threads.erase(i);
	
}
void	Thread::registerThread(Thread *thread)
{
	Synchronized sync(&threadsLock);
	threads.insert(pair<pthread_t, Thread*>(thread->pthread, thread));
}

} // namespace qp
