
#include "imgui_controller.h"

void ImGuiSQLVisor(SQLController& sc){
  static bool open = false;
  static bool query = false;
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
      query = true;
    }
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
      if(ImGui::CollapsingHeader(sc.tables_.value_[i])){
        SQLTableLayout(sc, &sc.table_info_[i], sc.tables_.value_[i]);
      }
    }
    ImGui::End();
  }

  if(query) {
    ImGui::OpenPopup("Command Promt"); 
    QueryPrompt(sc, query);
  }
}

void SQLTableLayout(SQLController& sc, Table* table, const char* id){
  int padding = 0;
  static bool insert = false;
  static bool selected = false;
  static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY 
                                 | ImGuiTableFlags_RowBg 
                                 | ImGuiTableFlags_BordersOuter 
                                 | ImGuiTableFlags_BordersV 
                                 | ImGuiTableFlags_Resizable 
                                 | ImGuiTableFlags_Reorderable 
                                 | ImGuiTableFlags_Hideable;
  if(table->cols_ > 0){
    if(ImGui::Button("Add New Column")){
      insert = true;
    }
    if(ImGui::BeginTable(id, table->cols_, flags)){
      for(int col = 0; col < table->cols_; ++col){
        ImGui::TableSetupColumn(table->colname_[col]);
      }
      ImGui::TableHeadersRow();
      for(int rows = 0; rows < table->index_; ++rows){
        ImGui::TableNextColumn();
        if(ImGui::Selectable(table->value_[rows], &table->is_selected_[rows])){
          InsertModal(sc, id, &table->is_selected_[rows]);
        }
        padding++;
        if((padding % table->cols_) == 0 && padding != 0){
          ImGui::TableNextRow();
        }
      }
      ImGui::EndTable();
    }
  }

  if(insert) {
    ImGui::OpenPopup("Add Column"); 
    InsertModal(sc, id, insert);
  } 
  if(selected) {
    ImGui::OpenPopup("Add Column"); 
  } 

}

void QueryPrompt(SQLController& sc, bool& q){
  static char buffer[kStringSize] = {'\0'};
  if(ImGui::BeginPopupModal("Command Promt", &q, ImGuiWindowFlags_AlwaysAutoResize)){
    ImGui::Text("Query"); 
    ImGui::SameLine();
    ImGui::InputText("##002", buffer, (kStringSize - 1));
    ImGui::SameLine();
    if(ImGui::Button("Submit")){
      sc.execute_write(buffer);
      memset(buffer, '\0', kStringSize);
      sc.close();
      sc.init(sc.path());
    }
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

void InsertModal(SQLController& sc, const char* table_name, bool& b){
  static char column_buffer[(kStringSize >> 3)] = {'\0'};
  static char value_buffer[(kStringSize >> 3)] = {'\0'};
  ImVec2 button_size = ImVec2(100.0f, 20.0f);

  static int index = 0;
  char* datatypes[] = {"TEXT", "INTEGER", "DECIMAL"};
  char* current = datatypes[index];

  if(ImGui::BeginPopupModal("Add Column", &b, ImGuiWindowFlags_AlwaysAutoResize)){
    ImGui::InputText("Column", column_buffer,((kStringSize >> 3) - 1));
    ImGui::InputText("Value", value_buffer, ((kStringSize >> 3) - 1));
    if(ImGui::BeginCombo("Datatype", current)){
      for(int i = 0; i < IM_ARRAYSIZE(datatypes); ++i){
        if(ImGui::Selectable(datatypes[i]))
          index = i;        
      }
      ImGui::EndCombo();
    }
    if(ImGui::Button("Ok", button_size)){
      InsertColumn(sc, table_name, column_buffer, current, value_buffer);
      memset(column_buffer, '\0', (kStringSize >> 3));
      memset(value_buffer, '\0', (kStringSize >> 3));
      b = false;
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if(ImGui::Button("Cancel", button_size)){
      memset(column_buffer, '\0', (kStringSize >> 3));
      memset(value_buffer, '\0', (kStringSize >> 3));
      b = false;
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

void InsertColumn(SQLController& sc, const char* table_name, const char* colum, const char* datatype, const char* value){
  char insert_buffer[kStringSize] = {'\0'};
  sprintf(insert_buffer, "ALTER TABLE %s ADD %s %s DEFAULT %s", table_name, colum, datatype, value);
  sc.execute_write(insert_buffer);
  sc.close();
  sc.init(sc.path());
}
