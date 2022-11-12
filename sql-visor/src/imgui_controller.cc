
#include "imgui_controller.h"

void ImGuiSQLVisor(SQLController& sc){
  static bool open = false;
  static bool close = true;

  char delete_id[kStringSize] = {'\0'};
  static char table_name[kStringSize] = {'\0'};

  static int currentNode = -1;
  static int nodeOpen = -1;

  if(ImGui::BeginMainMenuBar()){
    if(ImGui::BeginMenu("File")){
      if(ImGui::MenuItem("Open database")){
        nfdchar_t *outPath = NULL;
        nfdresult_t result = NFD_OpenDialog("db", NULL, &outPath);
        if ( result == NFD_OKAY ) {
          if(open){
            sc.close();
          }
          sc.init(outPath);
          sc.set_path(outPath);
          free(outPath);
          open = true;
        }
        else {
          printf("Error: %s\n", NFD_GetError());
        }
      }
      if(ImGui::MenuItem("Close database", nullptr, false, open)){
        sc.close();
        open = false;
      }
      ImGui::EndMenu();
    }
    if(ImGui::MenuItem("Query", nullptr, false, open)){
      ImGui::OpenPopup("Command Promt"); 
    }
    QueryPrompt(sc);  
    ImGui::EndMainMenuBar();
  }
  if(open){
	 ImGui::Begin("SQL Database", nullptr);
    if(ImGui::Button("Create")){
      CreateTable(sc, table_name);
      memset(table_name, '\0', kStringSize);
    }
    ImGui::SameLine();
    ImGui::InputTextMultiline("##001", table_name, (kStringSize >> 3), ImVec2(ImGui::GetWindowWidth(), 20.0f));
    ImGui::Separator(); 
    for(int i = 0; i < sc.tables_.cols_; ++i){
      sprintf(delete_id, "Delete##%d", i);
      if(ImGui::Button(delete_id)){
        DeleteTable(sc, sc.tables_.value_[i]);
        break;
      }
      ImGui::SameLine();
      if(nodeOpen == i){
        ImGui::SetNextItemOpen(false, ImGuiCond_Always);
      }
      if(ImGui::CollapsingHeader(sc.table_info_[i].name_)){
        if(currentNode == i){
          SQLTableLayout(sc, &sc.table_info_[i]);
        }
        else {
          nodeOpen = currentNode;
          currentNode = i;
        }
      }
    }
    ImGui::End();
  }
}

void SQLTableLayout(SQLController& sc, Table* table){
  int padding = 0;
  static char value_buffer[(kStringSize >> 3)] = {'\0'};
  static int int_value = 0;
  static float float_value = 0;
  static char popup_id[kStringSize >> 4];
  static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY 
                                 | ImGuiTableFlags_RowBg 
                                 | ImGuiTableFlags_BordersOuter 
                                 | ImGuiTableFlags_BordersV 
                                 | ImGuiTableFlags_Resizable 
                                 | ImGuiTableFlags_Reorderable 
                                 | ImGuiTableFlags_Hideable;
  if(table->cols_ > 0){
    if(ImGui::Button("Add New Column")){
      ImGui::OpenPopup("Add Column"); 
    }
    ImGui::SameLine();
    if(ImGui::Button("Add New Row")){
      ImGui::OpenPopup("Add Row");
    }
    ImGui::SameLine();
    if(ImGui::Button("Delete Column")){
      ImGui::OpenPopup("Delete Column");
    }
    ImGui::SameLine();
    if(ImGui::Button("Rename Column")){
      ImGui::OpenPopup("Rename Column");
    }
    HelpMarker("Right click on the rows for more options", nullptr);
    if(ImGui::BeginTable(table->name_, table->cols_, flags)){
      for(int col = 0; col < table->cols_; ++col){
        ImGui::TableSetupColumn(table->colname_[col]);
      }
      ImGui::TableHeadersRow();
      for(int rows = 0; rows < table->index_; ++rows){
        ImGui::TableNextColumn();
        ImGui::MenuItem(table->value_[rows]);
        sprintf(popup_id,"##%d%d", rows, padding % table->cols_);
        if(ImGui::BeginPopupContextItem(popup_id)){
          if(ImGui::BeginMenu("Change")){
            switch(*_strupr(table->datatype_[padding % table->cols_])){
              case 'N': ImGui::InputText("##600", value_buffer, (kStringSize >> 3) - 1); break;
              case 'I': ImGui::InputInt("##600", &int_value);
                        int_value = std::clamp(int_value, 0, 9999);
                        sprintf(value_buffer, "%d", int_value); 
                        break;
              case 'R': ImGui::InputFloat("##600", &float_value); 
                        float_value = std::clamp(float_value, 0.0f, 9999.0f);
                        sprintf(value_buffer, "%f", float_value); 
                        break;
              case 'T': ImGui::InputText("##600", value_buffer, (kStringSize >> 3) - 1); break;
            }
            ImGui::SameLine();
            if(ImGui::Button("Change")){
              UpdateRowValue(sc, table->name_, table->colname_[padding % table->cols_], value_buffer);
            }
            ImGui::EndMenu();
          }
          if(ImGui::MenuItem("Delete")){
            DeleteRow(sc, table->name_, table->colname_[padding % table->cols_], table->value_[rows]);
          }
          ImGui::EndPopup();
        }
        padding++;
        if((padding % table->cols_) == 0 && padding != 0){
          ImGui::TableNextRow();
        }
      }
      ImGui::EndTable();
    }
  }

  AddColumnModal(sc, table->name_);
  AddRowModal(sc, table);
  DeleteColumnModal(sc, table);
  RenameColumnModal(sc, table);
}

void QueryPrompt(SQLController& sc){
  static char buffer[kStringSize] = {'\0'};
  ImVec2 button_size = ImVec2(100.0f, 20.0f);
  if(ImGui::BeginPopupModal("Command Promt", nullptr, ImGuiWindowFlags_AlwaysAutoResize)){
    ImGui::InputText("Query##001", buffer, (kStringSize - 1));
    if(ImGui::Button("Submit", button_size)){
      sc.execute_write(buffer);
      memset(buffer, '\0', kStringSize);
      sc.close();
      sc.init(sc.path());
    }
    ImGui::SameLine();
    if(ImGui::Button("Cancel", button_size))
      ImGui::CloseCurrentPopup();
    ImGui::SameLine();
    HelpMarker("(?)", "SELECT <expression> FROM <tables> [WHERE <condition>]\nCREATE TABLE contacts (<name> <type>)\nDROP TABLE <table>\nINSERT INTO <table> (<colum>) VALUES (<value>)\nUPDATE <table> SET <colum> = <value> [WHERE <condition>]\nALTER TABLE <table> ADD <new colum> <dataype> <definition>\nMax query size: 255");
    ImGui::EndPopup();
  }
} 

void HelpMarker(const char* name, const char* desc){
  ImGui::TextDisabled(name);
  if (ImGui::IsItemHovered() && desc != nullptr){
    ImGui::BeginTooltip();
    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
    ImGui::TextUnformatted(desc);
    ImGui::PopTextWrapPos();
    ImGui::EndTooltip();
  }
}

void AddColumnModal(SQLController& sc, const char* table_name){
  static char column_buffer[(kStringSize >> 3)] = {'\0'};
  ImVec2 button_size = ImVec2(100.0f, 20.0f);

  static int index = 0;
  char* datatypes[] = {"INTEGER", "REAL", "TEXT"};
  char* current = datatypes[index];

  static char value_buffer[(kStringSize >> 3)] = {'\0'};
  static int int_value = 0;
  static float float_value = 0.0f;

  if(ImGui::BeginPopupModal("Add Column", nullptr, ImGuiWindowFlags_AlwaysAutoResize)){
    ImGui::InputText("Column", column_buffer,((kStringSize >> 3) - 1));
    if(ImGui::BeginCombo("Datatype", current)){
      for(int i = 0; i < IM_ARRAYSIZE(datatypes); ++i){
        if(ImGui::Selectable(datatypes[i]))
          index = i;        
      }
      ImGui::EndCombo();
    }
    switch(index){
      case 0: ImGui::InputInt("Value", &int_value);
              int_value = std::clamp(int_value, 0, 9999);
              sprintf(value_buffer, "%d", int_value); 
              break;
      case 1: ImGui::InputFloat("Value", &float_value); 
              float_value = std::clamp(float_value, 0.0f, 9999.0f);
              sprintf(value_buffer, "%f", float_value); 
              break;
      case 2: ImGui::InputText("Value", value_buffer, ((kStringSize >> 3) - 1)); 
              break;
    }
    if(ImGui::Button("Ok", button_size)){
      AddColumn(sc, table_name, column_buffer, current, value_buffer);
      memset(column_buffer, '\0', (kStringSize >> 3));
      memset(value_buffer, '\0', (kStringSize >> 3));
      int_value = 0;
      float_value = 0.0f;
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if(ImGui::Button("Cancel", button_size)){
      memset(column_buffer, '\0', (kStringSize >> 3));
      memset(value_buffer, '\0', (kStringSize >> 3));
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}

void AddRowModal(SQLController& sc, Table* table){
  ImVec2 button_size = ImVec2(100.0f, 20.0f);
  static int index = 0;
  static bool initialize = false;

  static char** value_buffer;
  static int* int_value;
  static float* float_value;

  char concat_buffer[kStringSize] = {'\0'};
  char column_buffer[kStringSize] = {'\0'};

  if(ImGui::BeginPopupModal("Add Row", nullptr, ImGuiWindowFlags_AlwaysAutoResize)){
    if(!initialize){
      value_buffer = (char**)calloc(table->cols_, sizeof(char*));
      int_value = (int*)calloc(table->cols_, sizeof(int));
      float_value = (float*)calloc(table->cols_, sizeof(float));
      for(int i = 0; i < table->cols_; ++i){
        value_buffer[i] = (char*)calloc(kStringSize >> 3, sizeof(char));
        int_value[i] = 0;
        float_value[i] = 0.0f;
      }
      initialize = true;
    }
    for(int i = 0; i < table->cols_; ++i){
      switch(*_strupr(table->datatype_[i])){
        case 'N': ImGui::InputText(table->colname_[i], value_buffer[i], ((kStringSize >> 3) - 1)); 
                  break;
        case 'I': ImGui::InputInt(table->colname_[i], &int_value[i]);
                  int_value[i] = std::clamp(int_value[i], 0, 9999); 
                  sprintf(value_buffer[i], "%d", int_value[i]); 
                  break;
        case 'R': ImGui::InputFloat(table->colname_[i], &float_value[i]); 
                  float_value[i] = std::clamp(float_value[i], 0.0f, 9999.0f);
                  sprintf(value_buffer[i], "%f", float_value[i]); 
                  break;
        case 'T': ImGui::InputText(table->colname_[i], value_buffer[i], ((kStringSize >> 3) - 1)); 
                  break;
      }
    }
    if(ImGui::Button("Ok", button_size)){
      for(int i = 0; i < table->cols_; ++i){
        strcat(column_buffer, table->colname_[i]);
        if(i < table->cols_ - 1) strcat(column_buffer, ", ");
        if(*_strupr(table->datatype_[i]) == 'T'){
          strcat(concat_buffer, "'");
          strcat(concat_buffer, value_buffer[i]);
          strcat(concat_buffer, "'");
        }
        else {
          strcat(concat_buffer, value_buffer[i]);
        }
        if(i < table->cols_ - 1) strcat(concat_buffer,", ");
        free(value_buffer[i]);
      }
      AddRow(sc, table->name_, column_buffer, concat_buffer);
      free(value_buffer);
      free(int_value);
      free(float_value);
      initialize = false;
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if(ImGui::Button("Cancel", button_size)){
      for(int i = 0; i < table->cols_; ++i){
        free(value_buffer[i]);
      }
      free(value_buffer);
      free(int_value);
      free(float_value);
      initialize = false;
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}

void DeleteColumnModal(SQLController& sc, Table* table){
  static int index = 0;
  if(ImGui::BeginPopupModal("Delete Column", nullptr, ImGuiWindowFlags_AlwaysAutoResize)){
    if(ImGui::BeginCombo("Select Column", table->colname_[index])){
      for(int i = 0; i < table->cols_; ++i){
        if(ImGui::Selectable(table->colname_[i])){
          index = i;
        }
      }      
      ImGui::EndCombo();
    }
    if(ImGui::Button("Delete")){
      DeleteColumn(sc, table->name_, table->colname_[index]);      
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if(ImGui::Button("Cancel")){
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}

void RenameColumnModal(SQLController& sc, Table* table){
  ImVec2 button_size = ImVec2(100.0f, 20.0f);
  static char rename_buffer[kStringSize >> 3];
  static int index = 0;

  if(ImGui::BeginPopupModal("Rename Column", nullptr, ImGuiWindowFlags_AlwaysAutoResize)){
    if(ImGui::BeginCombo("Select Column", table->colname_[index])){
      for(int i = 0; i < table->cols_; ++i){
        if(ImGui::Selectable(table->colname_[i])){
          index = i;
        }
      }      
      ImGui::EndCombo();
    }
    ImGui::InputText("New Name", rename_buffer, (kStringSize >> 3) - 1);
    if(ImGui::Button("Ok", button_size)){
      RenameColumnName(sc, table->name_, table->colname_[index], rename_buffer);
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if(ImGui::Button("Cancel", button_size)){
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}

void CreateTable(SQLController& sc, const char* name){
  char create_buffer[kStringSize] = {'\0'};
  sprintf(create_buffer, "CREATE TABLE %s (id INTEGER)",name);
  sc.execute_write(create_buffer);
  sprintf(create_buffer, "INSERT INTO %s (id) VALUES ('0')", name);
  sc.execute_write(create_buffer);
  sc.close();
  sc.init(sc.path());
}

void DeleteTable(SQLController& sc, const char* table){
  char delete_buffer[kStringSize] = {'\0'};
  sprintf(delete_buffer, "DROP TABLE %s", table);
  sc.execute_write(delete_buffer);
  sc.close();
  sc.init(sc.path());
}

void AddColumn(SQLController& sc, const char* table_name, const char* colum, const char* datatype, const char* value){
  char insert_buffer[kStringSize] = {'\0'};
  sprintf(insert_buffer, "ALTER TABLE %s ADD %s %s DEFAULT '%s'", table_name, colum, datatype, value);
  sc.execute_write(insert_buffer);
  sc.close();
  sc.init(sc.path());
}

void AddRow(SQLController& sc, const char* table_name, const char* column, const char* value){
  char add_row_buffer[kStringSize] = {'\0'};
  sprintf(add_row_buffer, "INSERT INTO %s (%s) VALUES (%s)", table_name, column, value);
  sc.execute_write(add_row_buffer);
  sc.close();
  sc.init(sc.path());
}

void DeleteColumn(SQLController& sc, const char* table_name, const char* column){
  char delete_column_buffer[kStringSize] = {'\0'};
  sprintf(delete_column_buffer, "ALTER TABLE %s DROP COLUMN %s", table_name, column);
  sc.execute_write(delete_column_buffer);
  sc.close();
  sc.init(sc.path());
}

void DeleteRow(SQLController& sc, const char* table_name, const char* column, const char* row){
  char delete_row_buffer[kStringSize] = {'\0'};
  sprintf(delete_row_buffer, "DELETE FROM %s WHERE %s = '%s'", table_name, column, row);
  sc.execute_write(delete_row_buffer);
  sc.close();
  sc.init(sc.path());
}

void UpdateRowValue(SQLController& sc, const char* table_name, const char* column, const char* value){
  char update_buffer[kStringSize] = {'\0'};
  sprintf(update_buffer, "UPDATE %s SET %s = '%s' WHERE rowid IN (SELECT rowid FROM %s LIMIT 1)", table_name, column, value, table_name);
  sc.execute_write(update_buffer);
  sc.close();
  sc.init(sc.path());
}

void RenameColumnName(SQLController& sc, const char* table_name, const char* col_name, const char* new_name){
  char update_buffer[kStringSize] = {'\0'};
  sprintf(update_buffer, "ALTER TABLE %s RENAME COLUMN %s TO %s", table_name, col_name, new_name);
  sc.execute_write(update_buffer);
  sc.close();
  sc.init(sc.path());
}
