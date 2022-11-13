
#ifndef __TABLE_CONTROLLER_H__
#define __TABLE_CONTROLLER_H__ 1

#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"

struct Table {
	char* name_;
	char** value_;
	char** colname_;
	char** datatype_;
	int rows_, cols_, index_, type_, col_index_;
};

/** @brief Initialized the values of the structure Table
 * @param table A reference to the table to initialize
 */ 
void InitTable(Table* table);

/** @brief Release the memory of the table
 * @param table A reference to the table that will be freed 
 */ 
void ReleaseTable(Table* table);

//Callbacks

/** @brief A callback to get the information of the rows of the database for each table
 * 
 * It used the params necesary for sqlite, see sqlite3.h for more information
 *  
 * @param notused A generic pointer to retrieve the information given by the sqlite callback
 * @param argc The number of columns for each row
 * @param argv The information stored in the rows
 * @param azcolname The names of the columns
 */
int get_row_info(void* notused, int argc, char** argv, char** azcolname);

/** @brief A callback to get the name of the columns of the database for each table
 * 
 * It used the params necesary for sqlite, see sqlite3.h for more information
 *  
 * @param notused A generic pointer to retrieve the information given by the sqlite callback
 * @param argc The number of columns for each row
 * @param argv The information stored in the rows
 * @param azcolname The names of the columns
 */
int get_column_names(void* notused, int argc, char** argv, char** azcolname);

/** @brief A callback to get the number of columns of the database for each table
 * 
 * It used the params necesary for sqlite, see sqlite3.h for more information
 *  
 * @param notused A generic pointer to retrieve the information given by the sqlite callback
 * @param argc The number of columns for each row
 * @param argv The information stored in the rows
 * @param azcolname The names of the columns
 */
int get_columns_callback(void* notused, int argc, char** argv, char** azcolname);

/** @brief A callback to get the number of rows of the database for each table
 * 
 * It used the params necesary for sqlite, see sqlite3.h for more information
 *  
 * @param notused A generic pointer to retrieve the information given by the sqlite callback
 * @param argc The number of columns for each row
 * @param argv The information stored in the rows
 * @param azcolname The names of the columns
 */
int get_rows_callback(void* notused, int argc, char** argv, char** azcolname);

/** @brief A callback to get the datatype of the column of the database for each table
 * 
 * It used the params necesary for sqlite, see sqlite3.h for more information
 *  
 * @param notused A generic pointer to retrieve the information given by the sqlite callback
 * @param argc The number of columns for each row
 * @param argv The information stored in the rows
 * @param azcolname The names of the columns
 */
int get_datatype_callback(void* notused, int argc, char** argv, char** azcolname);

#endif // __TABLE_CONTROLLER_H__