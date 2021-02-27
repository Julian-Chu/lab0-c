#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

#ifndef strlcpy
#define strlcpy(dst, src, sz) snprintf((dst), (sz), "%s", (src))
#endif

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q)
        return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;
    /* Free queue structure */
    list_ele_t *node = q->head;
    while (node) {
        free(node->value);
        list_ele_t *tmp = node;
        node = node->next;
        free(tmp);
    }

    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Retu:n false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    if (!q)
        return false;

    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;

    newh->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (!newh->value) {
        free(newh);
        return false;
    }
    newh->next = NULL;

    strlcpy(newh->value, s, strlen(s) + 1);
    newh->next = q->head;
    if (!q->head) {
        q->tail = newh;
    }
    q->head = newh;
    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (q == NULL)
        return false;

    list_ele_t *newt;
    if (!(newt = malloc(sizeof(list_ele_t))))
        return false;
    if (!(newt->value = malloc(sizeof(char) * (strlen(s) + 1)))) {
        free(newt);
        return false;
    }
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    strlcpy(newt->value, s, strlen(s) + 1);
    newt->next = NULL;
    if (!q->tail) {
        q->tail = newt;
        q->head = newt;
    } else {
        q->tail->next = newt;
        q->tail = q->tail->next;
    }
    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head)
        return false;

    list_ele_t *node = q->head;
    if (sp) {
        strncpy(sp, node->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    q->head = q->head->next;
    if (!q->head) {
        q->tail = NULL;
    }
    free(node->value);
    free(node);
    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    /* Remember: It should operate in O(1) time */
    if (!q)
        return 0;
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || q->size <= 1)
        return;

    list_ele_t *curr, *next;
    curr = q->head;

    while (curr->next) {
        next = curr->next;
        curr->next = next->next;
        next->next = q->head;
        q->head = next;
    }
    q->tail = curr;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
    if (!q || !q->head)
        return;
    q->head = merge_sort(q->head);
    while (q->tail->next) {
        q->tail = q->tail->next;
    }
}

list_ele_t *merge_sort(list_ele_t *node)
{
    if (!node)
        return NULL;
    if (!node->next)
        return node;
    list_ele_t *slow = node, *fast = node->next;
    while (fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    list_ele_t *mid = slow->next;
    slow->next = NULL;

    list_ele_t *left, *right;
    left = merge_sort(node);
    right = merge_sort(mid);

    return merge_sort_two_nodes(left, right);
}

list_ele_t *merge_sort_two_nodes(list_ele_t *a, list_ele_t *b)
{
    if (!b)
        return a;
    if (!a)
        return b;

    list_ele_t *head, *curr;

    if (strcmp(a->value, b->value) < 0) {
        head = a;
        curr = a;
        a = a->next;
    } else {
        head = b;
        curr = b;
        b = b->next;
    }

    while (a && b) {
        if (strcmp(a->value, b->value) < 0) {
            curr->next = a;
            a = a->next;
        } else {
            curr->next = b;
            b = b->next;
        }
        curr = curr->next;
    }

    if (a) {
        curr->next = a;
    }

    if (b) {
        curr->next = b;
    }
    return head;
}
