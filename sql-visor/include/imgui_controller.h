
#ifndef __IMGUI_CONTROLLER_H__
#define __IMGUI_CONTROLLER_H__ 1

#include <stdio.h>
#include <stdlib.h>
#include "imgui.h"
#include "sql_controller.h"
#include "table_controller.h"
#include "nfd.h"

void ImGuiSQLVisor(SQLController& sc);
void SQLTableLayout(SQLController& sc, Table* table);
void QueryPrompt(SQLController& sc);
void HelpMarker(const char* name, const char* desc);

void AddColumnModal(SQLController& sc,const char* table_name);
void AddRowModal(SQLController& sc, Table* table);
void DeleteColumnModal(SQLController& sc, Table* table);
void ChangeValueModal(SQLController& sc, Table* table, int column, int row);

void CreateTable(SQLController& sc, const char* name);
void DeleteTable(SQLController& sc, const char* table);
void AddColumn(SQLController&sc, const char* table_name, const char* colum, const char* datatype, const char* value);
void AddRow(SQLController& sc, const char* table_name, const char* column, const char* value);
void DeleteColumn(SQLController& sc, const char* table, const char* column);
void DeleteRow(SQLController& sc, const char* table_name, const char* column, const char* row);


#endif // __IMGUI_CONTROLLER_H__