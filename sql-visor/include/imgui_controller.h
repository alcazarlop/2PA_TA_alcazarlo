
#ifndef __IMGUI_CONTROLLER_H__
#define __IMGUI_CONTROLLER_H__ 1

#include <stdio.h>
#include <stdlib.h>
#include "imgui.h"
#include "sql_controller.h"
#include "table_controller.h"
#include "nfd.h"

void ImGuiSQLVisor(SQLController& sc);
void SQLTableLayout(SQLController& sc, Table* table, const char* id);
void QueryPrompt(SQLController& sc, bool& q);
void HelpMarker(const char* name, const char* desc);
void InsertModal(SQLController& sc,const char* table_name, bool& b);
void CreateTable(SQLController& sc, const char* name);
void DeleteTable(SQLController& sc, const char* table);
void InsertColumn(SQLController&sc, const char* table_name, const char* colum, const char* datatype, const char* value);

#endif // __IMGUI_CONTROLLER_H__