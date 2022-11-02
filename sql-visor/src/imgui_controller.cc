
#include "imgui_controller.h"

void ImGuiSQLVisor(SQLController* sc){
  static bool open = false;
  static bool query = false;

  if(ImGui::BeginMainMenuBar()){
    if(ImGui::BeginMenu("File")){
      if(ImGui::MenuItem("Open database")){
        nfdchar_t *outPath = NULL;
        nfdresult_t result = NFD_OpenDialog("db", NULL, &outPath);
        if ( result == NFD_OKAY ) {
          if(open){
            sc->close();
          }
          sc->init(outPath);
          sc->set_query(outPath);
          free(outPath);
          open = true;
        }
        else {
          printf("Error: %s\n", NFD_GetError());
        }
      }
      if(ImGui::MenuItem("Close database", nullptr, false, open)){
        sc->close();
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
    for(int i = 0; i < sc->tables_.cols_; ++i){
      if(ImGui::CollapsingHeader(sc->tables_.value_[i])){
        SQLTableLayout(&sc->table_info_[i], sc->tables_.value_[i]);
      }
    }
	 ImGui::End();
  }

  if(query) QueryPrompt(sc, query);
}

void SQLTableLayout(Table* table, const char* id){
  int padding = 0;
  static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY 
                                 | ImGuiTableFlags_RowBg 
                                 | ImGuiTableFlags_BordersOuter 
                                 | ImGuiTableFlags_BordersV 
                                 | ImGuiTableFlags_Resizable 
                                 | ImGuiTableFlags_Reorderable 
                                 | ImGuiTableFlags_Hideable;
  if(table->cols_ > 0){
    if(ImGui::BeginTable(id, table->cols_, flags)){
      for(int col = 0; col < table->cols_; ++col){
        ImGui::TableSetupColumn(table->colname_[col]);
      }
      ImGui::TableHeadersRow();
      for(int rows = 0; rows < table->index_; ++rows){
        ImGui::TableNextColumn();
        ImGui::Text(table->value_[rows]);
        padding++;
        if((padding % table->cols_) == 0 && padding != 0){
          ImGui::TableNextRow();
        }
      }
      ImGui::EndTable();
    }
  }

}

void QueryPrompt(SQLController* sc, bool& q){
  static char buffer[kStringSize];
  ImGui::OpenPopup("Command Promt");
  if(ImGui::BeginPopupModal("Command Promt", NULL, ImGuiWindowFlags_AlwaysAutoResize)){
    ImGui::InputText("Query", buffer, kStringSize);
    ImGui::SameLine();
    if(ImGui::Button("Submit")){
      sc->execute_write(buffer);
      memset(buffer, '\0', kStringSize);
      sc->close();
      sc->init(sc->path());
    }
    if(ImGui::Button("Close")){
      q = false;
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    HelpMarker("Help", "SELECT <expression> FROM <tables> [WHERE <condition>]\nCREATE TABLE contacts (<name> <type>)\nDROP TABLE <table>\nINSERT INTO <table> (<colum>) VALUES (<value>)\nUPDATE <table> SET <colum> = <value> [WHERE <condition>]\nMax query size: 255");
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
