#ifndef LOTOSPP_SINGLETON_H
#define	LOTOSPP_SINGLETON_H


#include "config.h"

#include <boost/noncopyable.hpp>
#include <boost/thread/once.hpp>
#include <boost/scoped_ptr.hpp>


namespace lotospp {

/**
 * The Singleton class is a simple class that can be used to initialize once a
 * resource. The resource being hold in it is initialized only when
 * "getInstance()" is called for the first time, avoiding initialization fiasco.
 */
template<typename T>
class Singleton
	: boost::noncopyable
{
	/// Initialize a resource of type T
	static void initialize()
	{
		m_pointer.reset(new T);
	};

public:
	/**
	 * Initialize the internal instance if still not initialized and returns it
	 *
	 * @return A pointer to the current instance
	 * @throw Any exception the resource can throw during construction or any exception during calling the "new" operator
	 */
	static T* get()
	{
		boost::call_once(initialize, m_flag);
		return m_pointer.get();
	};

private:
	/// A scoped pointer holding the actual resource.
	static boost::scoped_ptr<T> m_pointer;

	/// One-time initialization flag
	static boost::once_flag m_flag;
};

template<typename T>
boost::scoped_ptr<T> Singleton<T>::m_pointer;

template<typename T>
boost::once_flag Singleton<T>::m_flag=BOOST_ONCE_INIT;

} // namespace lotospp

#endif // LOTOSPP_SINGLETON_H
