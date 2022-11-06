
#ifndef __TABLE_CONTROLLER_H__
#define __TABLE_CONTROLLER_H__ 1

#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"

struct Table {
	char** value_;
	char** colname_;
	bool* is_selected_;
	int rows_, cols_, index_;
};

void InitTable(Table* table);
void ReleaseTable(Table* table);

//Callbacks
int read_tables_callback(void* notused, int argc, char** argv, char** azcolname);
int get_columns_callback(void* notused, int argc, char** argv, char** azcolname);
int get_rows_callback(void* notused, int argc, char** argv, char** azcolname);

#endif // __TABLE_CONTROLLER_H__