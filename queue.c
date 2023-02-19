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
    list_for_each_entry_safe (entry, safe, l, list) {
        list_del(&entry->list);
        q_release_element(entry);
    }
    free(l);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *newnode = malloc(sizeof(element_t));
    if (!newnode)
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
    if (!head)
        return false;
    element_t *newnode = malloc(sizeof(element_t));
    if (!newnode)
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
    strncpy(sp, removeNode->value, bufsize);
    list_del(head->next);
    return removeNode;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || head->prev == head)
        return NULL;
    element_t *removeNode = list_entry(head->prev, element_t, list);
    strncpy(sp, removeNode->value, bufsize);
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
    if (!head || list_empty(head))
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
    if (!head || list_empty(head) || list_is_singular(head))
        return false;

    struct list_head *curr = head->next, *next = curr->next;
    bool key = false;

    while (curr != head && next != head) {
        element_t *curr_entry = list_entry(curr, element_t, list);
        element_t *next_entry = list_entry(next, element_t, list);

        while (next != head && !strcmp(curr_entry->value, next_entry->value)) {
            list_del(next);
            q_release_element(next_entry);
            next = curr->next;
            next_entry = list_entry(next, element_t, list);
            key = true;
        }

        if (key) {
            list_del(curr);
            q_release_element(curr_entry);
            key = false;
        }

        curr = next;
        next = next->next;
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    if (!head)
        return;
    struct list_head *iterator = head->next;
    while (iterator != head && iterator->next != head) {
        char *temp = list_entry(iterator, element_t, list)->value;
        list_entry(iterator, element_t, list)->value =
            list_entry(iterator->next, element_t, list)->value;
        list_entry(iterator->next, element_t, list)->value = temp;
        iterator = iterator->next->next;
    }
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
    printf("enter\n");
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

/* Function to split the doubly linked list into two halves */
void splitList(struct list_head *source,
               struct list_head **frontRef,
               struct list_head **backRef)
{
    struct list_head *slowPtr = source;
    struct list_head *fastPtr = source->next;

    /* Move fastPtr by two and slowPtr by one */
    while (fastPtr != NULL) {
        fastPtr = fastPtr->next;
        if (fastPtr != NULL) {
            slowPtr = slowPtr->next;
            fastPtr = fastPtr->next;
        }
    }

    /* Set the front and back halves of the list */
    *frontRef = source;
    *backRef = slowPtr->next;
    (*backRef)->prev = NULL;
    slowPtr->next = NULL;
}

/* Function to merge two sorted doubly linked lists */
struct list_head *mergeLists(struct list_head *a, struct list_head *b)
{
    struct list_head *result = NULL;

    /* Base case */
    if (a == NULL) {
        return b;
    } else if (b == NULL) {
        return a;
    }
    char *s1 = list_entry(a, element_t, list)->value;
    char *s2 = list_entry(b, element_t, list)->value;
    /* Recursively merge the lists */
    if (strcmp(s1, s2) < 0) {
        result = a;
        result->next = mergeLists(a->next, b);
        result->next->prev = result;
    } else {
        result = b;
        result->next = mergeLists(a, b->next);
        result->next->prev = result;
    }

    return result;
}

/* Function to perform merge sort on a doubly linked list */
void sort(struct list_head **headRef)
{
    struct list_head *head = *headRef;
    struct list_head *a = NULL;
    struct list_head *b = NULL;

    /* Base case: if the list is empty or has only one element */
    if (head == NULL || head->next == NULL) {
        return;
    }

    /* Split the list into two halves */
    splitList(head, &a, &b);

    /* Recursively sort the two halves */
    sort(&a);
    sort(&b);

    /* Merge the sorted halves */
    *headRef = mergeLists(a, b);
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head)
{
    head->prev->next = NULL;
    sort(&(head->next));
    struct list_head *iter = head;
    while (iter->next != NULL) {
        iter = iter->next;
    }
    head->next->prev = head;
    head->prev = iter;
    iter->next = head;
}


/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || head->next == head)
        return 0;
    element_t *entry = list_entry(head->prev, element_t, list);
    element_t *safe = list_entry(entry->list.prev, element_t, list);
    char *max = " ";
    int count = 0;
    while (&entry->list != (head)) {
        if (strcmp(max, entry->value) > 0 && count != 0) {
            list_del(&entry->list);
            q_release_element(entry);
        } else {
            max = entry->value;
            count++;
        }
        entry = safe;
        safe = list_entry(safe->list.prev, element_t, list);
    }
    return count;
}

void mergetwolists(struct list_head **l1, struct list_head **l2)
{
    struct list_head *l1_iter = (*l1)->next;
    struct list_head *l2_iter = (*l2)->next;
    struct list_head *iter = *l1;
    while (l1_iter != *l1 && l2_iter != *l2) {
        char *s1 = list_entry(l1_iter, element_t, list)->value;
        char *s2 = list_entry(l2_iter, element_t, list)->value;
        if (strcmp(s1, s2) > 0) {
            iter->next = l2_iter;
            l2_iter->prev = iter;
            l2_iter = l2_iter->next;
            iter = iter->next;
        } else {
            iter->next = l1_iter;
            l1_iter->prev = iter;
            l1_iter = l1_iter->next;
            iter = iter->next;
        }
    }
    if (l1_iter == *l1) {
        iter->next = l2_iter;
        l2_iter->prev = iter;
        while (l2_iter->next != *l2)
            l2_iter = l2_iter->next;
        l2_iter->next = *l1;
        (*l1)->prev = l2_iter;
    }

    if (l2_iter == *l2) {
        iter->next = l1_iter;
        l1_iter->prev = iter;
    }
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    if (!head)
        return 0;

    struct list_head *l1 = list_entry(head->next, queue_contex_t, chain)->q;
    struct list_head *iter = head->next;
    while (iter->next != head) {
        struct list_head *l2 = list_entry(iter->next, queue_contex_t, chain)->q;
        mergetwolists(&l1, &l2);
        list_entry(iter->next, queue_contex_t, chain)->q = NULL;
        iter = iter->next;
    }
    return q_size(l1);
}
