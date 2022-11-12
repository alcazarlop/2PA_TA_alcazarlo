
#ifndef __IMGUI_CONTROLLER_H__
#define __IMGUI_CONTROLLER_H__ 1

#include <stdio.h>
#include <stdlib.h>
#include "imgui.h"
#include "sql_controller.h"
#include "table_controller.h"
#include "nfd.h"

void ImGuiSQLVisor(SQLController& sc);
<<<<<<< HEAD
void SQLTableLayout(SQLController& sc, Table* table, const char* id);
void QueryPrompt(SQLController& sc, bool& q);
void HelpMarker(const char* name, const char* desc);
void InsertModal(SQLController& sc,const char* table_name, bool& b);
void CreateTable(SQLController& sc, const char* name);
void DeleteTable(SQLController& sc, const char* table);
void InsertColumn(SQLController&sc, const char* table_name, const char* colum, const char* datatype, const char* value);
=======
void SQLTableLayout(SQLController& sc, Table* table);
void QueryPrompt(SQLController& sc);
void HelpMarker(const char* name, const char* desc);

void AddColumnModal(SQLController& sc,const char* table_name);
void AddRowModal(SQLController& sc, Table* table);
void DeleteColumnModal(SQLController& sc, Table* table);

void CreateTable(SQLController& sc, const char* name);
void DeleteTable(SQLController& sc, const char* table);
void AddColumn(SQLController&sc, const char* table_name, const char* colum, const char* datatype, const char* value);
void AddRow(SQLController& sc, const char* table_name, const char* column, const char* value);
void DeleteColumn(SQLController& sc, const char* table, const char* column);
void DeleteRow(SQLController& sc, const char* table_name, const char* column, const char* row);
void UpdateRowValue(SQLController& sc, const char* table_name, const char* column, const char* value, const char* prev);
>>>>>>> 33792b3f856cdae46e4879647c326654c09d9084

#endif // __IMGUI_CONTROLLER_H__