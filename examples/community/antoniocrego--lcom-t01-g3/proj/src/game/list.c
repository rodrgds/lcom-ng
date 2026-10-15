#include <lcom/lcf.h>

#include<stdlib.h>
#include <stdio.h>
#include "list.h"

node *head;
enemy_node *enemy_list_head;

void createList(invis_hitbox a){

    head=(node *)malloc(sizeof(node));

    if(head == NULL){
        printf("Error");
        return;
    }
    
    head->node=a;
    head->next=NULL;   
}

void addNode(invis_hitbox a){
    node *ptr, *temp;

    ptr=head;

    temp =(node *)malloc(sizeof(node));

    temp->node=a;
    temp->next=NULL;

    if(temp==NULL){
        printf("Error");
        return;
    }
    else{
        ptr=head;
        while(ptr->next!=NULL){
             ptr=ptr->next;
        }
    ptr->next=temp;
    }
}

void deleteNode(){
    node *ptr, *temp;

    if(head==NULL){
        printf("Empty List");
        return;
    }

    if(head->next==NULL){
        ptr=head;
        head=NULL;
        free(ptr);
    }
    else{
        ptr=head;
        while(ptr->next!=NULL){
            temp=ptr;
            ptr=ptr->next;
        }
        temp->next=NULL;
        free(ptr);
    }
}

void deleteList(){
    while(head!=NULL) deleteNode();
}

void createEnemyList(enemy_character a){

    enemy_list_head=(enemy_node *)malloc(sizeof(enemy_node));

    if(enemy_list_head == NULL){
        printf("Error");
        return;
    }
    
    enemy_list_head->node=a;
    enemy_list_head->next = NULL; 
}

void addEnemyNode(enemy_character a){
    enemy_node *ptr, *temp;

    ptr=enemy_list_head;

    temp =(enemy_node *)malloc(sizeof(enemy_node));

    temp->node=a;
    temp->next=NULL;

    if(temp==NULL){
        printf("Error");
        return;
    }
    else{
        ptr=enemy_list_head;
        while(ptr->next!=NULL){
             ptr=ptr->next;
        }
    ptr->next=temp;
    }
}

void deleteLastEnemyNode(){
    enemy_node *ptr, *temp;

    if(enemy_list_head==NULL){
        printf("Empty List");
        return;
    }

    if(enemy_list_head->next==NULL){
        ptr=enemy_list_head;
        enemy_list_head=NULL;
        free(ptr);
    }
    else{
        ptr=enemy_list_head;
        while(ptr->next!=NULL){
            temp=ptr;
            ptr=ptr->next;
        }
        temp->next=NULL;
        free(ptr);
    }
}

void deleteEnemyNode(enemy_node *a){
    enemy_node *ptr, *temp;
    if(enemy_list_head == NULL){
        printf("Empty List");
        return;
    }
    if(enemy_list_head == a){
        ptr = enemy_list_head;
        enemy_list_head = enemy_list_head->next;
        free(ptr);
    }
    else{
        ptr = enemy_list_head;
        while (ptr->next != NULL)
        {
            if(ptr->next == a){
                temp = ptr->next;
                ptr->next = ptr->next->next;
                free(temp);
                return;
            }

            ptr = ptr->next;
        }
        
    }
}

void deleteEnemyList(){
    while(enemy_list_head!=NULL) deleteLastEnemyNode();
}
