
#include "table_controller.h"

void InitTable(Table* table){
  table->name_ = NULL;
  table->value_ = NULL;
  table->colname_ = NULL;
<<<<<<< HEAD
  table->is_selected_ = NULL;
=======
  table->datatype_ = NULL;
>>>>>>> 33792b3f856cdae46e4879647c326654c09d9084
  table->rows_ = 0;
  table->cols_ = 0;
  table->index_ = 0;
  table->type_ = 0;
  table->col_index_ = 0;
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
  table->type_ = 0;
  table->col_index_ = 0;
}

int get_row_info(void* notused, int argc, char** argv, char** azcolname){
  Table* out = (Table*)notused;
  azcolname = NULL;
  for(int i = 0; i < argc; ++i){
    if(argv[i] != '\0'){
      out->value_[out->index_] = (char*)calloc((strlen(argv[i]) + 1), sizeof(char));
      memcpy(out->value_[out->index_], argv[i], (strlen(argv[i]) + 1));
    }
    else{
      out->value_[out->index_] = (char*)calloc(5, sizeof(char));
      memcpy(out->value_[out->index_], "NULL\0", 5);
    }
    out->is_selected_[out->index_] = false;
    out->index_++;
  }
  return 0;
}

int get_column_names(void* notused, int argc, char** argv, char** azcolname){
  Table* out = (Table*)notused;
  azcolname = NULL;
  for(int i = 0; i < argc; ++i){
    out->colname_[out->col_index_] = (char*)calloc(strlen(argv[i]) + 1, sizeof(char));
    memcpy(out->colname_[out->col_index_], argv[i], strlen(argv[i]) + 1);
  }
  out->col_index_++;
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
  argc = 0;
  out->datatype_[out->type_] = (char*)calloc((strlen(*argv) + 1), sizeof(char));
  memcpy(out->datatype_[out->type_], *argv, (strlen(*argv) + 1));
  out->type_++;
  return 0;
}
