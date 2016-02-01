#ifndef THREADSYNC_H_
#define THREADSYNC_H_

namespace utility {

class SyncMonitor;

/*! \class Synchronizable
 * \brief Класс, от которого должны наследоваться все классы, доступ к объектам которых должен быть синхронизированным.
 */ 
class Synchronizable 
{
	public:
		/*! \fn Synchronizable()
		 * \brief Конструктор.
		 */
		Synchronizable();
		/*! \fn ~Synchronizable()
		 * \brief Деструктор.
		 */		
		virtual ~Synchronizable();
		/*! fn void	notify() const 
		 * \brief Метод, который устанавливает переменную состояния объекта.
		 * 
		 * После вызова этого метода, разблокируется поток ожидающий переменную состояния. 
		 */
		void	notify() const ;
		/*! fn void	notifyAll() const 
		 * \brief Метод, который устанавливает переменную состояния объекта.
		 * 
		 * После вызова этого метода, разблокируются все потоки ожидающие переменную состояния. Но только один из них сможет захватить мьютекс.
		 */
		void	notifyAll() const ;
		/*! fn void wait() const 
		 * \brief Метод, который освобождает мьютекс объекта и блокирует поток, вызвавший этот метод, в ожидании переменной состояния объекта.
		 * 
		 * \attention Не совсем понятно, когда когда вновь происходит захват мьютекса.
		 */
		void	wait() const;
		/*! fn void wait(unsigned long mills) const
		 * \brief Метод, который освобождает мьютекс объекта и блокирует поток, вызвавший этот метод, в ожидании переменной состояния объекта.
		 * \param mills Время ожидания в секундах.
		 * 
		 * \attention Не совсем понятно, когда вновь происходит захват мьютекса. Совсем не понятно почему учитывается только время в секундах.
		 */
		int	wait(unsigned long mills) const;
		/*! \fn virtual void	onInterrupt()
		 * \brief Метод, который должен выполнять освобождение русурсов и, по-идеи, должен вызываться при остоновке потока, в котором выполняется метод данного Synchronizable.
		 */			
		virtual void	onInterrupt()	{;	};
		/*! \fn bool checkIsInterrupted()
		 * \brief Метод выполняет проверку был ли остановлен поток, в котором выполняется объект Synchronizable, и, если это произошло, вызывает метод onInterrupt() для освобождения ресурсов.
		 * \return true - если поток был остановлен, false - иначе.
		 */
		bool	checkIsInterrupted();
	private:
		friend class Synchronized;
		SyncMonitor *syncMonitor;
}; // class Synchronizable

/*! \class Synchronized
 * \brief Класс с помощью которого можно обеспечивать эксклюзивный доступ к объекту типа Synchronizable.
 */
class Synchronized 
{
	public:
		/* \fn Synchronized(Synchronizable *syncObj)
		 * \brief Конструктор.
		 * \param syncObj Объект, доступ к которому будет синхронизироваться. 
		 * 
		 * В конструкторе происходит попытка захвата мьютекса объекта типа Synchronizable.
		 */
		Synchronized(Synchronizable *syncObj);
		/* \fn ~Synchronized()
		 * \brief Деструктор.
		 * 
		 * В конструкторе происходит освобождение мьютекса объекта типа Synchronizable, захваченного в конструкторе.
		 */		
		~Synchronized();
	private:
		Synchronized(const Synchronized &);
		Synchronized& operator=(const Synchronized &);
		Synchronizable *object;
}; // class Synchronized

} // namespace qp


#endif /*THREADSYNC_H_*/
