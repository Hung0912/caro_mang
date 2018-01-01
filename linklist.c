// A complete working C program to demonstrate deletion in singly
// linked list
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 256

// A linked list node
struct Node
{
  int data;
  int room;
  char username[30];
  struct Node *next;
};

/* Given a reference (pointer to pointer) to the head of a list
   and an int, inserts a new node on the front of the list. */
void push(struct Node** head_ref, int new_data,int new_room, char new_string_data[])
{
  struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));
  new_node->data = new_data;
  new_node->room = new_room;
  strcpy(new_node->username,new_string_data);
  new_node->next = (*head_ref);
  (*head_ref)    = new_node;
}

/* Given a reference (pointer to pointer) to the head of a list
   and a key, deletes the first occurrence of key in linked list */
void deleteNode(struct Node **head_ref, int key)
{
  // Store head node
  struct Node* temp = *head_ref, *prev;
  
  // If head node itself holds the key to be deleted
  if (temp != NULL && temp->data == key)
    {
      *head_ref = temp->next;   // Changed head
      free(temp);               // free old head
      return;
    }
  
  // Search for the key to be deleted, keep track of the
  // previous node as we need to change 'prev->next'
  while (temp != NULL && temp->data != key)
    {
      prev = temp;
      temp = temp->next;
    }
  
  // If key was not present in linked list
  if (temp == NULL) return;
  
  // Unlink the node from linked list
  prev->next = temp->next;
  
  free(temp);  // Free memory
}

// This function prints contents of linked list starting from 
// the given node
char* getList(struct Node *node)
{
  static char list[MAXLINE];
  strcpy(list,"Danh sach:\n");
  while (node != NULL)
    {
      strcat(list,node->username);
      strcat(list,"\n");
      //printf(" %d ", node->data);
      node = node->next;
    }
  return list;
}

int getFD(struct Node *node,char* name)
{
  while (node != NULL) 
    {
      if (strcmp(name,node->username) == 0) 
	return node->data;
      node = node->next;
    }
  return -1;
}

char* getUsername(struct Node *node,int fd)
{
  while (node != NULL) 
    {
      if (node->data == fd) 
	return node->username;
      node = node->next;
    }
  return NULL;
}

void generateRoom(struct Node *node,int fd,int room)
{
 
  while (node != NULL) 
    {
      if (node->data == fd) {
	 node->room = room;
      }	
      node = node->next;
    }
}
int getRoomByFD(struct Node *node,int fd) {
   while (node != NULL) 
    {
      if (node->data == fd) {
	 return node->room;
      }	
      node = node->next;
    }
}

int getOtherFDByRoom(struct Node *node,int room,int fd) {
   while (node != NULL) 
    {
      if (node->room == room) {
	 if (node->data == fd) {
	    node = node->next;
	    continue;
	 }
	 return node->data;
      }	
      node = node->next;
    }
}

int availableRoom(struct Node *node)
{
  int i=0,j=0,room=1;
  int* not_available;
  not_available = (int*)malloc(sizeof(int));
  while (node != NULL)
  {
	if (node->room != 0) {
	  not_available[i] = node->room;
	  i++;
	}
	node = node->next;
  }

  while(1) 
  {
	for (j=0;j<=i;j++) 
	{
	   if (room != not_available[j]) {
		if (j == i) {
			free(not_available);
	   		return room;
		}
		else continue;
	   }
		
	   else {
		room++;
		break;
	   }
	   
	}
  }
}
/* Drier program to test above functions*/
/*int main()
{
 
  struct Node* head = NULL;
  
  push(&head, 7,"seven");
  push(&head, 1,"one");
  push(&head, 3,"three");
  push(&head, 2,"two");
  
  puts("Created Linked List: ");
  //printList(head);
  //deleteNode(&head, 1);
  puts("\nLinked List after Deletion of 1: ");
  puts(getList(head));
  printf("Three is %d\n",getFD(head,"three"));
  return 0;
}
*/
