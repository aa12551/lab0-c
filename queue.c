#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (!head)
        return NULL;
    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (!l)
        return;
    element_t *entry = NULL, *safe = NULL;
    list_for_each_entry_safe (entry, safe, l, list)
        q_release_element(entry);
    free(l);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    element_t *newnode = malloc(sizeof(element_t));
    if (!newnode || head == NULL)
        return false;
    newnode->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (!newnode->value) {
        free(newnode);
        return false;
    }
    memcpy(newnode->value, s, strlen(s) + 1);
    list_add(&newnode->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    element_t *newnode = malloc(sizeof(element_t));
    if (!newnode || head == NULL)
        return false;
    newnode->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (!newnode->value) {
        free(newnode);
        return false;
    }
    memcpy(newnode->value, s, strlen(s) + 1);
    list_add_tail(&newnode->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || head->next == head)
        return NULL;
    element_t *removeNode = list_entry(head->next, element_t, list);
    memcpy(sp, removeNode->value, bufsize);
    list_del(head->next);
    return removeNode;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || head->prev == head)
        return NULL;
    element_t *removeNode = list_entry(head->prev, element_t, list);
    memcpy(sp, removeNode->value, bufsize);
    list_del(head->prev);
    return removeNode;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    struct list_head *iterator = NULL;
    if (!head)
        return 0;
    int count = 0;
    list_for_each (iterator, head)
        count++;
    return count;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    if (!head)
        return false;
    struct list_head *slow = head->next, *fast = head->next;
    while (fast != head && fast->next != head) {
        fast = fast->next->next;
        slow = slow->next;
    }
    list_del(slow);
    q_release_element(list_entry(slow, element_t, list));
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

void q_reverse_pointer(struct list_head *head)
{
    struct list_head *temp = head->next;
    head->next = head->prev;
    head->prev = temp;
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head)
        return;
    struct list_head *iterator = head->prev;
    while (iterator != head) {
        q_reverse_pointer(iterator);
        iterator = iterator->next;
    }
    q_reverse_pointer(head);
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    if (!head)
        return;
    struct list_head *iterator = head->next;
    for (int i = 0; i < k; i++) {
        q_reverse_pointer(iterator);
        iterator = iterator->prev;
    }
    head->next->next = iterator;
    iterator->prev->prev = head;
    struct list_head *temp = iterator->prev;
    iterator->prev = head->next;
    head->next = temp;
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head) {}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
