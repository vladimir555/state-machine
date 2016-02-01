#ifndef THREAD_H_
#define THREAD_H_

#include	<pthread.h>
#include	"runnable.h"
#include	"threadsync.h"
#include	<ext/hash_map>
#include	<memory>


namespace utility {

using __gnu_cxx::hash_map;

/*! \class Thread
 * \brief Класс, который служит для представления потоков исполнения.
 */

class	Thread: public Runnable {
	
	friend void	*pthreadStart(void *param);
	
	public:
		/*! \fn Thread(Runnable *_target = NULL, const std::string &_name = "")
		 * \brief Конструктор.
		 * \param _target Объект метод run(), которого запускается на выполнение в отдельном потоке.
		 * \param _name Имя потока.
		 * 
		 * Если _target не указан, то поток запускает на выполнение свой метод run(), в котором ничего не делается.
		 * Если не указано имя потока, то оно будет сгенерировано автоматически в виде "Thread-#". При этом имя потока не уникально.
		 */ 
		Thread(Runnable *_target = NULL, const std::string &_name = "");
		/*! fn virtual	~Thread()
		 * \brief Деструктор.
		 * 
		 * В деструкторе не ожидается завершение потока. Если он будет вызван в момент, когда поток ещё выполненияется, то
		 * может случиться необъяснимое. Если необходимо в деструкторе также удаляется информация и потоке из списка потоков. 
		 */
		virtual	~Thread();
		/*! \fn void	run()
		 * \brief Метод, запускаемый в отдельном потоке.
		 */
		void	run(){ return; }
		/*! \fn bool	start()
		 * \brief Метод запускает метод run() указанного в конструкторе target в отдельном потоке.
		 */
		bool	start();
		/*! \fn bool	join()
		 * \brief Метод блокирует поток, вызвавший данный метод, в ожидании завершения работы данного потока.
		 */
		bool	join();
		/*! \fn bool	join(unsigned long mills)
		 * \brief Метод блокирует поток, вызвавший данный метод, в ожидании завершения работы данного потока в течении указанного времени.
		 * \param mills Время ожидания завершения выполнения потока указанное в миллисекундах.
		 */		
		bool	join(unsigned long mills);
		/*! \fn void	interrupt()
		 * \brief Метод посылает сигнал target на остановку выполнения метода run().
		 * 
		 * Обработка этого сигнала полностью ложиться на объект target. В любом случае после вызова данного
		 * метода метод isInterrupted() будет возвращать true.
		 */
		void	interrupt();
		/*! \fn bool	isAlive()
		 * \brief Метод возвращает выполняется ли поток или уже нет.
		 * 
		 * \return true - в случае, когда поток был запущен на выполнение и ещё не завершил выполнение, false - иначе.
		 */ 
		bool	isAlive() const { return alive; }
		/*! \fn bool	isInterrupt()
		 * \brief Метод возвращает было ли прервано выполнение потока.
		 * 
		 * \return true - в случае, когда выполнение потока было прервано вызовом метода interrupt(), false - иначе.
		 * 
		 * interrupted устанавливается в false при успешном запуске выполнения потока.
		 */
		bool	isInterrupted() const { return interrupted;	}
		/*! \fn unsigned long getID() const
		 * \brief Метод возвращает идентификатор потока.
		 */
		unsigned long getID() const { return static_cast<unsigned long>(pthread); }
		/*! \fn static Thread	*getCurrentThread()
		 * \brief Метод возвращает указатель на поток в котором был вызван данный метод.
		 */
		static Thread	*getCurrentThread();
		/*! \fn std::string	getName() const 
		 * \brief Метод возвращает имя потока.
		 */
		std::string	getName() const { return name; }
		/*! \fn sleep(unsigned long seconds) const
		 * \brief Метод приостанавливает выполнение потока, в котором вызван данный метод на указанное время.
		 * \param seconds Время в секундах.
		 */		
		void	sleep(unsigned long mills) const;
		/*! \var int error
		 * \brief Атрибут, в который сохраняется последняя возникшая ошибка. 
		 */
		int		error;
		
	protected:
		/*! \fn virtual void	beforeTargetRun()
		 *\brief Метод, выполняется непосредственно перед запуском потока на исполнение.
		 */
		virtual void	beforeTargetRun();
		/*! \fn virtual void	afterTargetRun()
		 *\brief Метод, выполняется непосредственно после завершения работы потока исполнения.
		 */		
		virtual void	afterTargetRun();

	private:
		/*! \fn Thread(const Thread &pthread)
		 * \bried Конструктор копирования. Не определен.
		 */
		Thread(const Thread &pthread);
		/*! \fn Thread	operator=(const Thread &pthread)
		 * \bried Оператор присваивания. Не определен.
		 */
		Thread&	operator=(const Thread &pthread);
		/*! \fn void	initializeThread()
		 * \bried Метод, который выполняет некоторую инициализацию при создании объекта этого класса.
		 */		
		void	initializeThread();
		
	protected:
		Runnable *target;
	private:
		std::string name;
		
		pthread_t pthread;

		volatile bool alive;
		volatile bool interrupted;
		
		/*! \fn static pthread_t getCurrentThreadID()
		 * \bried Метод возвращает идентификатор текущего потока (потока, в котором был вызван данный метод).
		 */
		static pthread_t getCurrentThreadID();
		/*! \fn static Thread *getCurrentThread(pthread_t current)
		 * \bried Метод, который по указанному идентификатору потока находит в поток в списке всех зарегистрированных потоков.
		 */
		static Thread *getCurrentThread(pthread_t current);
		/*! \fn static	void	unregisterThread(Thread *thread)
		 * \bried Метод, который удаляет указанный поток из списока всех потоков.
		 */
		static	void	unregisterThread(Thread *thread);
		/*! \fn static	void	registerThread(Thread *thread)
		 * \bried Метод, который заносит указанный поток в список всех потоков.
		 */
		static	void	registerThread(Thread *thread);
		
		static hash_map<pthread_t, Thread*> threads;
		static Synchronizable threadsLock;
		static Thread mainThread;
}; //class Thread

} // namespace qp

#endif /*THREAD_H_*/
