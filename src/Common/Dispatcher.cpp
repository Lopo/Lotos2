#include "Dispatcher.h"

#ifdef __DEBUG_SCHEDULER__
#	include <iostream>
#endif

#include <boost/bind.hpp>

#include "network/OutputMessage.h"
#include "Task.h"
#include "ExceptionHandler.h"


using namespace lotospp;


Dispatcher::Dispatcher()
{
	m_taskList.clear();
	m_threadState=STATE_TERMINATED;
}

void Dispatcher::shutdownAndWait()
{
	shutdown();
	m_thread.join();
}

void Dispatcher::start()
{
	assert(m_threadState==STATE_TERMINATED);
	m_threadState=STATE_RUNNING;
	m_thread=boost::thread(boost::bind(&Dispatcher::dispatcherThread, (void*)this));
}

void Dispatcher::dispatcherThread(void* p)
{
	Dispatcher* dispatcher=(Dispatcher*)p;
#ifdef __EXCEPTION_TRACER__
	ExceptionHandler dispatcherExceptionHandler;
	dispatcherExceptionHandler.InstallHandler();
#endif
#ifdef __DEBUG_SCHEDULER__
	std::cout << "Starting Dispatcher" << std::endl;
#endif

	network::OutputMessagePool* outputPool;

	// NOTE: second argument defer_lock is to prevent from immediate locking
	boost::unique_lock<boost::mutex> taskLockUnique(dispatcher->m_taskLock, boost::defer_lock);

	while (dispatcher->m_threadState!=STATE_TERMINATED) {
		Task* task=nullptr;

		// check if there are tasks waiting
		taskLockUnique.lock(); //getDispatcher().m_taskLock.lock();

		if (dispatcher->m_taskList.empty()) {
			//if the list is empty wait for signal
#ifdef __DEBUG_SCHEDULER__
			std::cout << "Dispatcher: Waiting for task" << std::endl;
#endif
			dispatcher->m_taskSignal.wait(taskLockUnique);
			}

#ifdef __DEBUG_SCHEDULER__
		std::cout << "Dispatcher: Signalled" << std::endl;
#endif

		if (!dispatcher->m_taskList.empty() && (dispatcher->m_threadState!=STATE_TERMINATED)) {
			// take the first task
			task=dispatcher->m_taskList.front();
			dispatcher->m_taskList.pop_front();
			}

		taskLockUnique.unlock();

		// finally execute the task...
		if (task) {
			if (!task->hasExpired()) {
				network::OutputMessagePool::getInstance()->startExecutionFrame();
				(*task)();

				outputPool=network::OutputMessagePool::getInstance();
				if (outputPool) {
					outputPool->sendAll();
					}
				}

			delete task;

#ifdef __DEBUG_SCHEDULER__
			std::cout << "Dispatcher: Executing task" << std::endl;
#endif
			}
		}
#ifdef __EXCEPTION_TRACER__
	dispatcherExceptionHandler.RemoveHandler();
#endif
}

void Dispatcher::addTask(Task* task, bool push_front /*= false*/)
{
	bool do_signal=false;
	m_taskLock.lock();
	if (m_threadState==STATE_RUNNING) {
		do_signal=m_taskList.empty();
		if (push_front) {
			m_taskList.push_front(task);
			}
		else {
			m_taskList.push_back(task);
			}

#ifdef __DEBUG_SCHEDULER__
		std::cout << "Dispatcher: Added task" << std::endl;
#endif
		}
#ifdef __DEBUG_SCHEDULER__
	else {
		std::cout << "Error: [Dispatcher::addTask] Dispatcher thread is terminated." << std::endl;
		}
#endif

	m_taskLock.unlock();

	// send a signal if the list was empty
	if (do_signal) {
		m_taskSignal.notify_one();
		}
}

void Dispatcher::flush()
{
	Task* task=nullptr;
	while (!m_taskList.empty()) {
		task=m_taskList.front();
		m_taskList.pop_front();
		(*task)();
		delete task;
		network::OutputMessagePool* outputPool=network::OutputMessagePool::getInstance();
		if (outputPool) {
			outputPool->sendAll();
			}
		}
#ifdef __DEBUG_SCHEDULER__
	std::cout << "Flushing Dispatcher" << std::endl;
#endif
}

void Dispatcher::stop()
{
	m_taskLock.lock();
	m_threadState=STATE_CLOSING;
	m_taskLock.unlock();
#ifdef __DEBUG_SCHEDULER__
	std::cout << "Stopping Dispatcher" << std::endl;
#endif
}

void Dispatcher::shutdown()
{
	m_taskLock.lock();
	m_threadState=STATE_TERMINATED;
	flush();
	m_taskLock.unlock();
#ifdef __DEBUG_SCHEDULER__
	std::cout << "Shutdown Dispatcher" << std::endl;
#endif
}
