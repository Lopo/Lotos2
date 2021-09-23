#ifndef LOTOSPP_DATABASE_QUERY_H
#define	LOTOSPP_DATABASE_QUERY_H


#include "config.h"

#include <sstream>

#include <boost/thread/recursive_mutex.hpp>


namespace lotospp {
	namespace database {

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

	void reset() {};

protected:
	static boost::recursive_mutex database_lock;
};

	} // namespace database
} // namespace lotospp

#endif // LOTOSPP_DATABASE_QUERY_H
