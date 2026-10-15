#include "bullet_node.h"

bullet_node *createBulletNode(bullet *shot) {
    bullet_node *node = (bullet_node *)malloc(sizeof(bullet_node));
    if (node == NULL) {
        return NULL;
    }
    node->shot = shot;
    node->next = NULL;
    return node;
}

void destroyBulletNode(bullet_node *node) {
    if (node != NULL) {
        destroyBullets(node->shot); 
        free(node);
    }
}

void destroyAllBullets(bullet_node *head) {
    bullet_node *current = head;
    bullet_node *next;
    while (current != NULL) {
        next = current->next;
        destroyBulletNode(current);
        current = next;
    }
}

void addBullet(bullet_node **head, bullet *shot) {
    bullet_node *new_node = createBulletNode(shot);
    new_node->next = *head;
    *head = new_node;
}

void removeBullet(bullet_node **head, bullet_node *node_to_remove) {
    if (*head == node_to_remove) {
        *head = node_to_remove->next;
    } else {
        bullet_node *current = *head;
        while (current != NULL && current->next != node_to_remove) {
            current = current->next;
        }
        if (current != NULL) {
            current->next = node_to_remove->next;
        }
    }
    destroyBulletNode(node_to_remove);
}
