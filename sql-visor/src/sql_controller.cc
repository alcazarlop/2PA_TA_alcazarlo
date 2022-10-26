
#include "sql_controller.h"

int get_tables_count_callback(void* notused, int argc, char** argv, char** azcolname);
int read_tables_callback(void* notused, int argc, char** argv, char** azcolname);
int other_callback(void* notused, int argc, char** argv, char** azcolname);

SQLController::SQLController(){
	database_ = NULL;
	path_ = (char*)calloc(kStringSize, sizeof(char));
	err_msg_ = (char*)calloc(kStringSize, sizeof(char));
	sql_query_ = (char*)calloc(kStringSize, sizeof(char));
	rc_ = 0;
}

SQLController::SQLController(const SQLController& other){
	database_ = other.database_;
	rc_ = other.rc_;
	memcpy(sql_query_, other.sql_query_, kStringSize);
	memcpy(err_msg_, other.err_msg_, kStringSize);
	memcpy(path_, other.path_, kStringSize);

}

SQLController::~SQLController(){
	free(sql_query_);
	free(err_msg_);
	free(path_);
	close();
}

void SQLController::init(const char* path){
	tables_.value_.clear();
	tables_.colname_.clear();

	rc_ = sqlite3_open(path, &database_);

	if(rc_ != SQLITE_OK) {
		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(database_));
		sqlite3_close(database_);
	}

	// execute_read("SELECT name FROM sqlite_master WHERE type ='table'", &tables_, read_tables_callback);
	execute_read("SELECT COUNT (*) FROM sqlite_master WHERE type ='table'", &test_, get_tables_count_callback);
	execute_read("SELECT name FROM sqlite_master WHERE type ='table'", &test_, other_callback);

	// printf("cadena: %s\n",other_[0]->next_->value_);

	table_info_ = new Info[tables_.value_.size()];	
	for(unsigned int i = 0; i < tables_.value_.size(); ++i){
		char ptr[64] = {'\0'};
		sprintf(ptr, "SELECT * FROM %s", tables_.value_[i]);
		execute_read(ptr, &table_info_[i], read_tables_callback);
	}

}

void SQLController::open(const char* path){
	rc_ = sqlite3_open(path, &database_);
	if (rc_ != SQLITE_OK) {
		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(database_));
		sqlite3_close(database_);
	}
}

void SQLController::close(){

	// for(unsigned i = 0; i < tables_.value_.size(); ++i){
	// 	for(unsigned j = 0; j < table_info_[i].value_.size(); ++j){
	// 		free(table_info_[i].value_.data()[j]);
	// 		table_info_[i].value_.data()[j] = NULL;
	// 	}
	// 	table_info_[i].value_.clear();

	// 	for(unsigned int k = 0; k < table_info_[i].colname_.size(); ++k){
	// 		free(table_info_[i].colname_.data()[k]);
	// 		table_info_[i].colname_.data()[k] = NULL;
	// 	}
	// 	table_info_[i].colname_.clear();
	// }
	// delete[] table_info_;

	// for(unsigned int i = 0; i < tables_.value_.size(); ++i){
	// 	free(tables_.value_.data()[i]);
	// 	tables_.value_.data()[i] = NULL;
	// }

	// for(unsigned int i = 0; i < tables_.colname_.size(); ++i){
	// 	free(tables_.colname_.data()[i]);
	// 	tables_.colname_.data()[i] = NULL;
	// }

	// tables_.value_.clear();
	// tables_.colname_.clear();

	sqlite3_close(database_);
}

void SQLController::execute_read(const char* query, void* data_t, int (*sqlite3_callback)(void*,int,char**, char**)){
	rc_ = sqlite3_exec(database_, query, sqlite3_callback, (void*)data_t, &err_msg_);
	if (rc_ != SQLITE_OK ) {
		fprintf(stderr, "Failed to select data\n");
		fprintf(stderr, "SQL error: %s\n", err_msg_);
		sqlite3_free(err_msg_);
	}
}

void SQLController::execute_write(const char* query){
	sql_query_ = sqlite3_mprintf("%s", query);
	rc_ = sqlite3_exec(database_, sql_query_, NULL, NULL, &err_msg_);
	if (rc_ != SQLITE_OK ) {
		fprintf(stderr, "Failed to select data\n");
		fprintf(stderr, "SQL error: %s\n", err_msg_);
		sqlite3_free(err_msg_);
	}
}

void SQLController::set_query(const char* query){
	memcpy(path_, query, kStringSize);
}

const char* SQLController::path() const {
	return path_;
} 

int read_tables_callback(void* notused, int argc, char** argv, char** azcolname){
	SQLController::Info* temp = (SQLController::Info*) notused;

	for(int i = 0; i < argc; ++i){
		char* val = (char*)calloc(kStringSize, sizeof(char));
		char* colum = (char*)calloc(kStringSize, sizeof(char));

		if(argv[i] == '\0')
			memcpy(val, "NULL", kStringSize);
		else
			memcpy(val, argv[i], kStringSize);

		temp->value_.push_back(val);

		if(temp->colname_.size() >= (unsigned int)argc){
			if(strcmp(azcolname[i], temp->colname_[i]) != 0){
				memcpy(colum, azcolname[i], kStringSize);
				temp->colname_.push_back(colum);
			}
		}
		else {
			memcpy(colum, azcolname[i], kStringSize);
			temp->colname_.push_back(colum);
		}
	}
	return 0;
}

int other_callback(void* notused, int argc, char** argv, char** azcolname){
	SQLController::Table* temp = (SQLController::Table*)notused;

	temp->value_ = (char*)calloc(kStringSize, sizeof(char));
	temp->colname_ = (char*)calloc(kStringSize, sizeof(char));

	memcpy(temp->value_, *argv, kStringSize);
	memcpy(temp->colname_, *azcolname, kStringSize);

	return 0;
}

int get_tables_count_callback(void* notused, int argc, char** argv, char** azcolname){
	SQLController::Table* temp = (SQLController::Table*)notused;
	temp->size_ = atoi(*argv);
	return 0;
}

void SQLController::execute(const char* query, TInfo** data_t){
	rc_ = sqlite3_exec(database_, query, other_callback, data_t, &err_msg_);
	if (rc_ != SQLITE_OK ) {
		fprintf(stderr, "Failed to select data\n");
		fprintf(stderr, "SQL error: %s\n", err_msg_);
		sqlite3_free(err_msg_);
	}
}