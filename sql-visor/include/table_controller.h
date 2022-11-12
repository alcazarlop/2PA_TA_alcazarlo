
#ifndef __TABLE_CONTROLLER_H__
#define __TABLE_CONTROLLER_H__ 1

#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"

struct Table {
	char* name_;
	char** value_;
	char** colname_;
<<<<<<< HEAD
	bool* is_selected_;
	int rows_, cols_, index_;
=======
	char** datatype_;
	int rows_, cols_, index_, type_, col_index_;
>>>>>>> 33792b3f856cdae46e4879647c326654c09d9084
};

void InitTable(Table* table);
void ReleaseTable(Table* table);

//Callbacks
int get_row_info(void* notused, int argc, char** argv, char** azcolname);
int get_column_names(void* notused, int argc, char** argv, char** azcolname);
int get_columns_callback(void* notused, int argc, char** argv, char** azcolname);
int get_rows_callback(void* notused, int argc, char** argv, char** azcolname);
int get_datatype_callback(void* notused, int argc, char** argv, char** azcolname);

#endif // __TABLE_CONTROLLER_H__