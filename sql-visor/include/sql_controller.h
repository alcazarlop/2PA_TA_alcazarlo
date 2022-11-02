
#ifndef __SQL_CONTROLLER_H__
#define __SQL_CONTROLLER_H__ 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include "table_controller.h"

const unsigned int kStringSize = 256;

class SQLController {
 public:
 	SQLController();
 	SQLController(const SQLController& other);
 	~SQLController();

 	void init(const char* path);
 	void open(const char* path);
 	void close();

 	void execute_read(const char* query, void* data_t, int (*sqlite3_callback)(void*,int,char**, char**));
 	void execute_write(const char* query);

 	void set_query(const char* query);
 	const char* path() const;

 	Table tables_;
 	Table* table_info_;

private:
	sqlite3* database_;
	int rc_;
	char* err_msg_;
	char* sql_query_;
	char* path_;

};

#endif // __SQL_CONTROLLER_H__