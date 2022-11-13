
#ifndef __IMGUI_CONTROLLER_H__
#define __IMGUI_CONTROLLER_H__ 1

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include "imgui.h"
#include "sql_controller.h"
#include "table_controller.h"
#include "nfd.h"

/** @brief Shows the SQL visor
 * 
 * Calls to imgui backend to show the database
 * 
 * @param sc A reference to the open database necessary for its visualization
 */
void ImGuiSQLVisor(SQLController& sc);

/** @brief Distributes the SQL database in a determinated format
 * 
 * Distributes the database's tables in columns and rows
 * 
 * @param sc A reference to the open database
 * @param table A reference to the current open table
 */
void SQLTableLayout(SQLController& sc, Table* table);

/** @brief Open a command prompt to input a query to the database
 * 
 * A simple modal window to input queries, it does not show the out, but its reflected on the tables
 * 
 * @param sc A reference to the queried database
 */ 
void QueryPrompt(SQLController& sc);

/** @brief A help marker to show extra info for the user
 * 
 * It shows the information once the mouse is hovered over the mark
 * 
 * @param name The name of the marker
 * @param desc The description of the marker when mouse is hovered over it
 */
void HelpMarker(const char* name, const char* desc);

/** @brief Open a imgui modal window to add new columns to the table
 * 
 * Opens a small window with the necessary options to add a new column
 * 
 * @param sc A reference to the opened database
 * @param table_name The name of the table in which the column will be added
 */ 
void AddColumnModal(SQLController& sc,const char* table_name);

/** @brief Open a imgui modal window to add a new row to the table
 * 
 * Opens a small window with the necessary options to add a new row
 * 
 * @param sc A reference to the opened database
 * @param table A reference to the table which the row will be added
 */
void AddRowModal(SQLController& sc, Table* table);

/** @brief Opens a imgui modal window to select the column to delete
 * 
 * @param sc A reference to the opened database
 * @param table A reference to the table which the column will be deleted
 */
void DeleteColumnModal(SQLController& sc, Table* table);

/** @brief Opens a imgui modal to change the column name
 * 
 * @param sc A reference to the opened database
 * @param table A reference to the table in which the column will be renamed
 */
void RenameColumnModal(SQLController& sc, Table* table);

/** @brief A wrapper for SQL query create table to add new tables
 * 
 * The function sets the query and calls the sqlite callback, then the database is re-initiated
 * 
 * @param sc A reference to the opened database
 * @param name The name of the new table
 */
void CreateTable(SQLController& sc, const char* name);

/** @brief A wrapper for SQL query delete table to remove tables
 * 
 * The function sets the query and calls the sqlite callback, then the database is re-initiated
 * 
 * @param sc A reference to the opened database
 * @param name The name of the table to delete
 */
void DeleteTable(SQLController& sc, const char* table);

/** @brief A wrapper for SQL query alter table to add new columns
 * 
 * The function sets the query and calls the sqlite callback, the the database is re-initiaded
 * 
 * @param table_name The name of the table which the column will be added
 * @param column The name of the new column
 * @param datatype The datatype of the column
 * @param value The default value of the column
 */ 
void AddColumn(SQLController&sc, const char* table_name, const char* colum, const char* datatype, const char* value);

/** @brief A wrapper for SQL query inser into to add new rows
 * 
 * The function sets the query and calls the sqlite callback, the the database is re-initiaded
 * 
 * @param table_name The name of the table which the column will be added
 * @param column The name of the column where the row will be added
 * @param value The value of the new row
 */
void AddRow(SQLController& sc, const char* table_name, const char* column, const char* value);

/** @brief A wrapper for SQL query alter table to remove columns
 * 
 * The function sets the query and calls the sqlite callback, the the database is re-initiaded
 * 
 * @param table The name of the table which the column will be deleted
 * @param column The name of the column that will be deleted
 */
void DeleteColumn(SQLController& sc, const char* table, const char* column);

/** @brief A wrapper for SQL query delete to remove rows
 * 
 * The function sets the query and calls the sqlite callback, then the database is re-initiated,
 * it only deletes the selected row
 * 
 * @param sc A reference to the opened database
 * @param table_name The name of the table in which the row will be delete
 */
void DeleteRow(SQLController& sc, const char* table_name);

/** @brief A wrapper for SQL query update to change row values
 * 
 * The function sets the query and calls the sqlite callback, then the database is re-initiated,
 * it only changes the selected row
 * 
 * @param sc A reference to the opened database
 * @param table_name The name of the table in which the row will be change
 * @param colum The name of the column where the row is allocated
 * @param value The new value of the row
 */
void UpdateRowValue(SQLController& sc, const char* table_name, const char* column, const char* value);

/** @brief A wrapper for SQL query rename to change column names
 * 
 * The function sets the query and calls the sqlite callback, then the database is re-initiated
 * 
 * @param sc A reference to the opened database
 * @param table_name The name of the table in which the column is allocated
 * @param col_name The current name of the column
 * @param new_name The new name of the column
 */
void RenameColumnName(SQLController& sc, const char* table_name, const char* col_name, const char* new_name);

#endif // __IMGUI_CONTROLLER_H__