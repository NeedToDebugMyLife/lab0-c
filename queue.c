#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

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
void q_free(struct list_head *head)
{
    if (!head)
        return;

    struct list_head *cur = head->next;

    while (cur != head) {
        struct list_head *tmp;
        tmp = cur;
        cur = cur->next;

        list_del(tmp);
        q_release_element(list_entry(tmp, element_t, list));
    }

    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;

    char *str = strdup(s);
    strcat(str, "\0");

    element_t *node = malloc(sizeof(element_t));

    // malloc failure handle
    if (!str) {
        free(node);
        return false;
    }

    if (!node) {
        free(str);
        return false;
    }

    node->value = str;
    list_add(&node->list, head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;

    char *str = strdup(s);
    strcat(str, "\0");

    element_t *node = malloc(sizeof(element_t));

    // malloc failure handle
    if (!str) {
        free(node);
        return false;
    }

    if (!node) {
        free(str);
        return false;
    }

    node->value = str;
    list_add_tail(&node->list, head);

    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *node = list_entry(head->next, element_t, list);
    char *str = strndup(node->value, bufsize - 1);

    strcat(str, "\0");
    strncpy(sp, str, bufsize);

    list_del(&node->list);

    return node;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *node = list_entry(head->prev, element_t, list);
    char *str = strndup(node->value, bufsize - 1);

    strcat(str, "\0");
    strncpy(sp, str, bufsize);

    list_del(&node->list);

    return node;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    int size = 0;
    struct list_head *cur = head->next;

    while (cur != head) {
        size++;
        cur = cur->next;
    }

    return size;
}

/* Delete the middle node in queue */
// https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
bool q_delete_mid(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;

    int size = q_size(head) / 2;
    struct list_head *tmp = head->next;

    for (int i = 0; i < size; i++)
        tmp = tmp->next;

    list_del(tmp);
    q_release_element(list_entry(tmp, element_t, list));

    return true;
}

/* Delete all nodes that have duplicate string */
// https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
bool q_delete_dup(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;

    char *str = "\0";
    struct list_head *tmp = head->next;
    struct list_head *cur = head->next;

    while (cur->next != head) {
        tmp = cur;
        cur = cur->next;

        if (strcmp(list_entry(tmp, element_t, list)->value, str) == 0 ||
            strcmp(list_entry(tmp, element_t, list)->value,
                   list_entry(cur, element_t, list)->value) == 0) {
            if (strcmp(str, "\0") != 0)
                free(str);

            str = strdup(list_entry(tmp, element_t, list)->value);

            list_del(tmp);
            q_release_element(list_entry(tmp, element_t, list));
        }
    }

    if (strcmp(list_entry(cur, element_t, list)->value, str) == 0) {
        list_del(cur);
        q_release_element(list_entry(cur, element_t, list));
    }

    free(str);
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
// https://leetcode.com/problems/remove-nodes-from-linked-list/
int q_ascend(struct list_head *head)
{
    if (!head || list_empty(head))
        return 0;

    struct list_head *tmp;
    struct list_head *cur = head->prev;

    char *min = strdup(list_entry(cur, element_t, list)->value);
    strcat(min, "\0");

    while (cur != head) {
        tmp = cur;
        cur = cur->prev;

        if (strcmp(list_entry(tmp, element_t, list)->value, min) > 0) {
            list_del(tmp);
            q_release_element(list_entry(tmp, element_t, list));
        } else {
            free(min);
            min = strdup(list_entry(tmp, element_t, list)->value);
        }
    }
    free(min);

    return q_size(head);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
// https://leetcode.com/problems/remove-nodes-from-linked-list/
int q_descend(struct list_head *head)
{
    if (!head || list_empty(head))
        return 0;

    struct list_head *tmp;
    struct list_head *cur = head->prev;

    char *max = strdup(list_entry(cur, element_t, list)->value);
    strcat(max, "\0");

    while (cur != head) {
        tmp = cur;
        cur = cur->prev;

        if (strcmp(list_entry(tmp, element_t, list)->value, max) < 0) {
            list_del(tmp);
            q_release_element(list_entry(tmp, element_t, list));
        } else {
            free(max);
            max = strdup(list_entry(tmp, element_t, list)->value);
        }
    }
    free(max);

    return q_size(head);
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}