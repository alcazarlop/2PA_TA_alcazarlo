
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

 	/** @brief Initialize and allocates the memory necesary to visualize the database
 	 * 
 	 * The function allocates all the memory using the information extracted from 
 	 * the sqlite callbacks, and fill the tables to show the information
 	 * 
 	 * @param path The path to the database that will be opened
 	 */ 
 	void init(const char* path);

 	/** @brief Close the database and deallocates the memory
 	 */
 	void close();

 	/** @brief A wrapper for sqlite3_exec() function
 	 * 
 	 * The function calls the sqlite3_exec() with the desired callback, 
 	 * it checks that the query and the callback are properly setted
 	 * but not the data_t. 
 	 * It does not return error when the query or the callback fails
 	 * 
 	 * @param query The query that will be executed by the callback
 	 * @param data_t The data where the information retrieve for the callback will be stored
 	 * @param sqlite3_callback The sqlite3 callback that will be executed
 	 */ 
 	void execute_read(const char* query, void* data_t, int (*sqlite3_callback)(void*,int,char**, char**));

 	/** @brief A wrapper for sqlite3_exec() function
 	 * 
 	 * The function calls the sqlite3_exec() func with no callback,
 	 * and no data_t to retrieve information. 
 	 * It just actualize the database.
 	 * 
 	 * @param query The query to execute
 	 */ 
 	void execute_write(const char* query);

 	/** @brief A setter to store the path of the current opened database
 	 */ 
 	void set_path(const char* path);

 	/** @brief Return a pointer to the store path of the current opened database
 	 * @return A pointer to path
 	 */ 
 	const char* path() const;

 	Table tables_;
 	Table* table_info_;

 private:
	sqlite3* database_;
	int rc_;
	char* err_msg_;
	char* path_;

};

#endif // __SQL_CONTROLLER_H__