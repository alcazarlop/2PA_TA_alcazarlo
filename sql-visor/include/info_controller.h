
#ifndef __INFO_CONTROLLER_H__
#define __INFO_CONTROLLER_H__ 1

#include <stdlib.h>
#include <string.h>

struct TInfo {
	char* value_;
	char* colname_;
	TInfo* next_;
};

void InsertList(TInfo** head, char* argv, char* azcolname);
void DeleteList(TInfo** head, TInfo* del_node);

#endif // __INFO_CONTROLLER_H__