
#include "info_controller.h"

void InsertList(TInfo** head, char* argv, char* azcolname){
	TInfo* node = (TInfo*)calloc(1, sizeof(TInfo));
	node->value_ = (char*)calloc(strlen(argv), sizeof(char)); // Mirar + 1 or '\0'
	node->colname_ = (char*)calloc(strlen(azcolname), sizeof(char));

	node->next_ = *head;
	memcpy(node->value_, argv, strlen(node->value_));
	memcpy(node->colname_, azcolname, strlen(node->colname_));
	*head = node;
}

void DeleteList(TInfo** head, TInfo* del_node){
	TInfo* node = *head;
	if(node == del_node){
		*head = (*head)->next_;
		free(node);
		node = NULL;
	}
	while(node->next_ != NULL && node->next_ != del_node){
		node = node->next_;
	}
	if(node->next_ == del_node){
		TInfo* destoy_node = node->next_;
		node->next_ = node->next_->next_;
		free(destoy_node);
		destoy_node = NULL;
	}
}