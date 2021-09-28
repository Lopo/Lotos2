#ifndef LOTOSPP_DATABASE_QUERY_H
#define	LOTOSPP_DATABASE_QUERY_H

#include <boost/thread/recursive_mutex.hpp>
#include <sstream>


namespace LotosPP::Database {

/**
 * Thread locking hack
 *
 * By using this class for your queries you lock and unlock database for threads
 */
class Query
	: public std::ostringstream
{
	friend class Driver;

public:
	Query();
	~Query();

	void reset()
	{};

protected:
	static inline boost::recursive_mutex database_lock;
};

	}

#endif
