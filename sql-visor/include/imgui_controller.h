
#ifndef __IMGUI_CONTROLLER_H__
#define __IMGUI_CONTROLLER_H__ 1

#include <stdio.h>
#include <stdlib.h>
#include "imgui.h"
#include "sql_controller.h"
#include "nfd.h"

void ImGuiSQLVisor(SQLController* sc);
void SQLTableLayout(SQLController::Info* table, const char* id);
void QueryPrompt(SQLController* sc, bool& q);
void HelpMarker(const char* name, const char* desc);

#endif // __IMGUI_CONTROLLER_H__