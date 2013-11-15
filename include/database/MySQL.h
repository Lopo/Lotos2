#ifndef LOTOS2_DATABASE_MYSQL_H
#define LOTOS2_DATABASE_MYSQL_H

#include <mysql/mysql.h>

#include "database/Driver.h"


class DatabaseMySQL
	: public DatabaseDriver
{
public:
	DatabaseMySQL();
	virtual ~DatabaseMySQL();

	virtual bool getParam(DBParam_t param);

	virtual bool beginTransaction();
	virtual bool rollback();
	virtual bool commit();

	virtual uint64_t getLastInsertedRowID();

	virtual std::string escapeString(const std::string &s);
	virtual std::string escapeBlob(const char* s, uint32_t length);

protected:
	virtual bool internalQuery(const std::string &query);
	virtual DBResult_ptr internalSelectQuery(const std::string &query);
	virtual void freeResult(DBResult *res);

	MYSQL m_handle;
};


class MySQLResult
	: public DBResult
{
	friend class DatabaseMySQL;

public:
	virtual int32_t getDataInt(const std::string &s);
	virtual uint32_t getDataUInt(const std::string &s);
	virtual int64_t getDataLong(const std::string &s);
	virtual std::string getDataString(const std::string &s);
	virtual const char* getDataStream(const std::string &s, unsigned long &size);

	virtual DBResult_ptr advance();
	virtual bool empty();

protected:
	MySQLResult(MYSQL_RES* res);
	virtual ~MySQLResult();

	typedef std::map<const std::string, uint32_t> listNames_t;
	listNames_t m_listNames;

	MYSQL_RES* m_handle;
	MYSQL_ROW m_row;
};

#endif /* LOTOS2_DATABASE_MYSQL_H */
