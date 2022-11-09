
#include "imgui_controller.h"

void ImGuiSQLVisor(SQLController& sc){
  static bool open = false;
  static bool close = true;

  char delete_id[kStringSize] = {'\0'};
  static char table_name[kStringSize] = {'\0'};

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
      if(ImGui::CollapsingHeader(sc.table_info_[i].name_)){
        SQLTableLayout(sc, &sc.table_info_[i]);
      }
    }
    ImGui::End();
  }
}

void SQLTableLayout(SQLController& sc, Table* table){
  int padding = 0;
  static int column = 0;
  static int row = 0;
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
    if(ImGui::BeginTable(table->name_, table->cols_, flags)){
      for(int col = 0; col < table->cols_; ++col){
        ImGui::TableSetupColumn(table->colname_[col]);
      }
      ImGui::TableHeadersRow();
      for(int rows = 0; rows < table->index_; ++rows){
        ImGui::TableNextColumn();
        ImGui::MenuItem(table->value_[rows]);
        if(ImGui::BeginPopupContextItem()){
          if(ImGui::MenuItem("Change"));
          if(ImGui::MenuItem("Delete")){
            // DeleteRow(sc, table->name_, table->colname_[padding % rows], table->value_[rows]);
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
  if (ImGui::IsItemHovered()){
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
      case 0: ImGui::InputInt("Value", &int_value); sprintf(value_buffer, "%d", int_value); break;
      case 1: ImGui::InputFloat("Value", &float_value); sprintf(value_buffer, "%f", float_value); break;
      case 2: ImGui::InputText("Value", value_buffer, ((kStringSize >> 3) - 1)); break;
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

  static char value_buffer[(kStringSize >> 3)];
  static int int_value = 0;
  static float float_value = 0.0f;

  if(ImGui::BeginPopupModal("Add Row", nullptr, ImGuiWindowFlags_AlwaysAutoResize)){
    if(ImGui::BeginCombo("Select column", table->colname_[index])){
      for(int i = 0; i < table->cols_; ++i){
        if(ImGui::Selectable(table->colname_[i])){
          index = i;
        }
      }
      ImGui::EndCombo();
    }
    switch(*table->datatype_[index]){
      case 'n': ImGui::InputText("Value", value_buffer, ((kStringSize >> 3) - 1)); break;
      case 'i': ImGui::InputInt("Value", &int_value); sprintf(value_buffer, "%d", int_value); break;
      case 'r': ImGui::InputFloat("Value", &float_value); sprintf(value_buffer, "%f", float_value); break;
      case 't': ImGui::InputText("Value", value_buffer, ((kStringSize >> 3) - 1)); break;
    }
    if(ImGui::Button("Ok", button_size)){
      AddRow(sc, table->name_, table->colname_[index], value_buffer);
      int_value = 0;
      float_value = 0.0f;
      memset(value_buffer, '\0', (kStringSize >> 3));
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if(ImGui::Button("Cancel", button_size)){
      int_value = 0;
      float_value = 0.0f;
      memset(value_buffer, '\0', (kStringSize >> 3));
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

void ChangeValueModal(SQLController& sc, Table* table, int column, int row){
  if(ImGui::BeginPopupModal(table->value_[row])){
    ImGui::Button("Submit");
    if(ImGui::Button("Delete")){
      DeleteRow(sc, table->name_, table->colname_[column], table->value_[row]);
      ImGui::CloseCurrentPopup();
    }
    if(ImGui::Button("Close")){
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}

void CreateTable(SQLController& sc, const char* name){
  char create_buffer[kStringSize] = {'\0'};
  sprintf(create_buffer, "CREATE TABLE %s (id INTEGER)",name);
  sc.execute_write(create_buffer);
  sprintf(create_buffer, "INSERT INTO %s (id) VALUES (1)", name);
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
  sprintf(add_row_buffer, "INSERT INTO %s (%s) VALUES ('%s')", table_name, column, value);
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