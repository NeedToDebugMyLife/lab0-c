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

    char *str = strdup("");
    struct list_head *cur = head->next;

    while (cur->next != head) {
        struct list_head *tmp;
        tmp = cur;
        cur = cur->next;

        if (strcmp(list_entry(tmp, element_t, list)->value, str) == 0 ||
            strcmp(list_entry(tmp, element_t, list)->value,
                   list_entry(cur, element_t, list)->value) == 0) {
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
// https://leetcode.com/problems/swap-nodes-in-pairs/
void q_swap(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *cur = head->next;
    struct list_head *next = cur->next;

    while (cur != head && next != head) {
        list_del(cur);
        list_add(cur, next);
        cur = cur->next;
        next = cur->next;
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *cur = head->prev;

    while (cur != head) {
        struct list_head *tmp;
        tmp = cur;
        cur = cur->prev;
        list_move_tail(tmp, head);
    }
}

/* Reverse the nodes of the list k at a time */
// https://leetcode.com/problems/reverse-nodes-in-k-group/
void q_reverseK(struct list_head *head, int k)
{
    if (!head || list_empty(head))
        return;

    int size = q_size(head);
    if (k > size || k < 2)
        return;

    if (k == 2) {
        q_swap(head);
        return;
    }

    struct list_head *tmp = head->next;
    struct list_head *cur = head->next;
    struct list_head *reverse_h = head->next;
    struct list_head *reverse_t = head->next;

    for (int i = 0; i < size; i++) {
        tmp = cur;
        cur = cur->next;

        if (i % k == k - 1) {
            tmp->next = head;
            head->prev = tmp;
            q_reverse(head);

            reverse_t->next = head->next;
            head->next->prev = reverse_t;

            if (i == k - 1)
                reverse_h = head->next;

            reverse_t = head->prev;

            head->next = cur;
            cur->prev = head;
        }
    }

    if (head->next != head) {
        reverse_t->next = head->next;
        head->next->prev = reverse_t;

        reverse_t = tmp;
    }

    head->prev = reverse_t;
    head->next = reverse_h;

    reverse_h->prev = head;
    reverse_t->next = head;
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    int counter = 0;
    int mid = q_size(head) / 2 - 1;

    struct list_head *cut = head->next;

    while (counter != mid) {
        cut = cut->next;
        counter++;
    }

    struct list_head head1;
    struct list_head head2;

    INIT_LIST_HEAD(&head1);
    INIT_LIST_HEAD(&head2);

    list_splice_tail(head, &head2);
    list_cut_position(&head1, &head2, cut);
    INIT_LIST_HEAD(head);

    q_sort(&head1, descend);
    q_sort(&head2, descend);

    const char *str1 = NULL;
    const char *str2 = NULL;

    struct list_head *tmp;
    struct list_head *cur1 = (&head1)->next;
    struct list_head *cur2 = (&head2)->next;

    while (cur1 != &head1 && cur2 != &head2) {
        str1 = list_entry(cur1, element_t, list)->value;
        str2 = list_entry(cur2, element_t, list)->value;

        if (descend) {
            if (strcmp(str1, str2) >= 0) {
                tmp = cur1;
                cur1 = cur1->next;
            } else {
                tmp = cur2;
                cur2 = cur2->next;
            }
        } else {
            if (strcmp(str1, str2) <= 0) {
                tmp = cur1;
                cur1 = cur1->next;
            } else {
                tmp = cur2;
                cur2 = cur2->next;
            }
        }
        list_del(tmp);
        list_add_tail(tmp, head);
    }

    list_splice_tail(&head1, head);
    list_splice_tail(&head2, head);
}

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
// https://leetcode.com/problems/merge-k-sorted-lists/
int q_merge(struct list_head *head, bool descend)
{
    if (!head || list_empty(head))
        return 0;

    if (list_is_singular(head))
        return q_size(list_entry(head->next, queue_contex_t, chain)->q);

    int total = 0;
    struct list_head result;
    struct list_head *cur = head->next;

    INIT_LIST_HEAD(&result);

    while (cur != head) {
        total += list_entry(cur, queue_contex_t, chain)->size;
        cur = cur->next;
    }

    for (int i = 0; i < total; i++) {
        cur = head->next;

        while (list_empty(list_entry(cur, queue_contex_t, chain)->q) &&
               cur != head)
            cur = cur->next;

        queue_contex_t *context_cur = list_entry(cur, queue_contex_t, chain);
        queue_contex_t *context_max = list_entry(cur, queue_contex_t, chain);
        queue_contex_t *context_min = list_entry(cur, queue_contex_t, chain);

        const element_t *element_cur;
        const element_t *element_max =
            list_entry(context_cur->q->next, element_t, list);
        const element_t *element_min =
            list_entry(context_cur->q->next, element_t, list);

        while (cur != head) {
            context_cur = list_entry(cur, queue_contex_t, chain);

            if (context_cur->size != 0) {
                element_cur = list_entry(context_cur->q->next, element_t, list);

                if (descend) {
                    if (strcmp(element_cur->value, element_max->value) > 0) {
                        element_max = element_cur;
                        context_max = context_cur;
                    }
                } else {
                    if (strcmp(element_cur->value, element_min->value) < 0) {
                        element_min = element_cur;
                        context_min = context_cur;
                    }
                }
            }
            cur = cur->next;
        }

        struct list_head *node;

        if (descend) {
            node = context_max->q->next;
            context_max->size--;
        } else {
            node = context_min->q->next;
            context_min->size--;
        }

        list_del(node);
        list_add_tail(node, &result);
    }

    list_splice_tail_init(&result,
                          list_entry(head->next, queue_contex_t, chain)->q);

    int size = q_size(list_entry(head->next, queue_contex_t, chain)->q);
    list_entry(cur, queue_contex_t, chain)->size = size;

    return size;
}