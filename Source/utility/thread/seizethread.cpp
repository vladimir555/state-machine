#include	"seizethread.h"
#include	<iostream>

namespace utility {


using	namespace std;


SeizeThread::SeizeThread(Runnable *_target, const std::string &_name)
	: Thread(_target, _name)
{
}


SeizeThread::~SeizeThread()
{
	delete target;
	target = NULL;
}


} // namespace qp
