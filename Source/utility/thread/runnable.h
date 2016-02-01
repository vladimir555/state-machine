#ifndef RUNNABLE_H_
#define RUNNABLE_H_

namespace utility {

/*! \class Runnable
 * \brief Класс, от которого должны наследоваться все класса, методы объектов которого могут запускаться в отдельном потоке.
 */ 
class	Runnable
{
	public:
		/*! \fn Runnable()
		 * \brief Конструктор.
		 */
		Runnable()
		{
		};
		/*! \fn ~Runnable()
		 * \brief Деструктор.
		 */		
		virtual	~Runnable()
		{
		};
		/*! \fn virtual	void	run()
		 * \brief Методы, который можно выполнить в отдельном потоке.
		 */
		virtual	void	run()=0;
}; // class Runnable

} // namespace qp

#endif /*RUNNABLE_H_*/
