#ifndef SEIZETHREAD_H_
#define SEIZETHREAD_H_

#include	"thread.h"

namespace utility {

/*! \class SeizeThread
 * \brief Класс, который служит для представления потоков исполнения, такой поток становится владельцем объекта target и удаляет его вместе с собой.
 */
class SeizeThread : public Thread
{
	public:
		/*! \fn SeizeThread(Runnable *_target = NULL, const std::string &_name = "")
		 * \brief Конструктор.
		 * \param _target Объект метод run(), которого запускается на выполнение в отдельном потоке.
		 * \param _name Имя потока.
		 * 
		 * Если _target не указан, то поток запускает на выполнение свой метод run(), в котором ничего не делается.
		 * Если не указано имя потока, то оно будет сгенерировано автоматически в виде "Thread-#". При этом имя потока не уникально.
		 */ 
		SeizeThread(Runnable *_target = NULL, const std::string &_name = "");
		/*! fn virtual	~SeizeThread()
		 * \brief Деструктор.
		 * 
		 * В деструкторе не ожидается завершение потока. Если он будет вызван в момент, когда поток ещё выполненияется, то
		 * может случиться необъяснимое. Если необходимо в деструкторе также удаляется информация и потоке из списка потоков. 
		 */
		virtual	~SeizeThread();
	private:
}; // class SeizeThread

} // namespace qp

#endif /*SEIZETHREAD_H_*/
