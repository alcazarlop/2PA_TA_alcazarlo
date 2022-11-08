
#include "table_controller.h"
#include <stdio.h>

void InitTable(Table* table){
  table->name_ = NULL;
  table->value_ = NULL;
  table->colname_ = NULL;
  table->datatype_ = NULL;
  table->rows_ = 0;
  table->cols_ = 0;
  table->index_ = 0;
}

void ReleaseTable(Table* table){
  for(int i = 0; i < table->index_; ++i){
    if(NULL != table->value_[i]){
      free(table->value_[i]); 
      table->value_[i] = NULL;
    }
  }
  for(int i = 0; i < table->cols_; ++i){
    if(NULL != table->colname_[i]){
     free(table->colname_[i]); 
     table->colname_[i] = NULL;
    }
    if(NULL != table->datatype_){
      free(table->datatype_[i]); 
      table->datatype_[i] = NULL;
    }
  }
  if(NULL != table->value_){
    free(table->value_); 
    table->value_ = NULL;
   }
  if(NULL != table->colname_){
    free(table->colname_); 
    table->colname_ = NULL;
  }
  if(NULL != table->datatype_){
    free(table->datatype_); 
    table->datatype_ = NULL;
  }
  if(NULL != table->name_){
    free(table->name_);
    table->name_ = NULL;
  }
  table->index_ = 0;
  table->rows_ = 0;
  table->cols_ = 0;
}

int read_tables_callback(void* notused, int argc, char** argv, char** azcolname){
  Table* out = (Table*)notused;
  for(int i = 0; i < argc; ++i){
    if(out->index_ < argc){
      out->colname_[i] = (char*)calloc((strlen(azcolname[i]) + 1), sizeof(char));
      memcpy(out->colname_[i], azcolname[i], (strlen(azcolname[i]) + 1));
    }
    if(argv[i] != '\0'){
      out->value_[out->index_] = (char*)calloc((strlen(argv[i]) + 1), sizeof(char));
      memcpy(out->value_[out->index_], argv[i], (strlen(argv[i]) + 1));
    }
    else{
      out->value_[out->index_] = (char*)calloc(4, sizeof(char));
      memcpy(out->value_[out->index_], "NULL\0", 4);
    }
    out->index_++;
  }
  return 0;
}

int get_columns_callback(void* notused, int argc, char** argv, char** azcolname){
  Table* out = (Table*)notused;
  out->cols_ = atoi(*argv);
  azcolname = NULL;
  argc = 0;
  return 0;
}

int get_rows_callback(void* notused, int argc, char** argv, char** azcolname){
  Table* out = (Table*)notused;
  out->rows_ = atoi(*argv);
  azcolname = NULL;
  argc = 0;
  return 0;
}

int get_datatype_callback(void* notused, int argc, char** argv, char** azcolname){
  Table* out = (Table*)notused;
  azcolname = NULL;
  for(int i = 0; i < argc; ++i){
    out->datatype_[i] = (char*)calloc((strlen(argv[i]) + 1), sizeof(char));
    memcpy(out->datatype_[i], argv[i], (strlen(argv[i]) + 1));
  }
  return 0;
}