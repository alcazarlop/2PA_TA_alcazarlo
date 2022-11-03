
#include "sql_controller.h"

SQLController::SQLController(){
	database_ = NULL;
	rc_ = 0;
	err_msg_ = (char*)calloc(kStringSize, sizeof(char));
	path_ = (char*)calloc(kStringSize, sizeof(char));
}

SQLController::SQLController(const SQLController& other){
	database_ = other.database_;
	rc_ = other.rc_;
	memcpy(err_msg_, other.err_msg_, kStringSize);
	memcpy(path_, other.path_, kStringSize);
}

SQLController::~SQLController(){
	free(err_msg_);
	free(path_);
	close();
}

void SQLController::init(const char* path){
	char buffer[kStringSize] = {'\0'};

	rc_ = sqlite3_open(path, &database_);
	if(rc_ != SQLITE_OK) {
		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(database_));
		sqlite3_close(database_);
	}

	//Read Tables Structure
	InitTable(&tables_);
	execute_read("SELECT COUNT (*) FROM sqlite_master WHERE type ='table'", &tables_, get_columns_callback);
	tables_.colname_ = (char**)calloc(tables_.cols_, sizeof(char*));
	tables_.value_ = (char**)calloc(tables_.cols_, sizeof(char*));
	execute_read("SELECT name FROM sqlite_master WHERE type ='table'", &tables_, read_tables_callback);

	//Read Tables Info
	table_info_ = (Table*)calloc(tables_.cols_, sizeof(Table));
	for(int i = 0; i < tables_.cols_; ++i){
		sprintf(buffer, "SELECT COUNT (*) FROM %s", tables_.value_[i]);
		execute_read(buffer, &table_info_[i], get_rows_callback);

		sprintf(buffer, "SELECT COUNT (*) FROM pragma_table_info('%s')", tables_.value_[i]);
		execute_read(buffer, &table_info_[i], get_columns_callback);

		table_info_[i].value_ = (char**)calloc((table_info_[i].cols_ * table_info_[i].rows_), sizeof(char*));
		table_info_[i].colname_ = (char**)calloc(table_info_[i].cols_, sizeof(char*));

		sprintf(buffer, "SELECT * FROM %s", tables_.value_[i]);
		execute_read(buffer, &table_info_[i], read_tables_callback);
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
	for(int i = 0; i < tables_.cols_; ++i)
		ReleaseTable(&table_info_[i]);

	free(table_info_);
	
	ReleaseTable(&tables_);
	sqlite3_close(database_);
}

void SQLController::execute_read(const char* query, void* data_t, int (*sqlite3_callback)(void*,int,char**, char**)){
	rc_ = sqlite3_exec(database_, query, sqlite3_callback, data_t, &err_msg_);
	if (rc_ != SQLITE_OK ) {
		fprintf(stderr, "Failed to select data\n");
		fprintf(stderr, "SQL error: %s\n", err_msg_);
		sqlite3_free(err_msg_);
	}
}

void SQLController::execute_write(const char* query){
	query = sqlite3_mprintf("%s", query);
	rc_ = sqlite3_exec(database_, query, NULL, NULL, &err_msg_);
	if (rc_ != SQLITE_OK ) {
		fprintf(stderr, "Failed to select data\n");
		fprintf(stderr, "SQL error: %s\n", err_msg_);
		sqlite3_free(err_msg_);
	}
}

void SQLController::set_path(const char* path){
	memcpy(path_, path, kStringSize);
}

const char* SQLController::path() const {
	return path_;
}
