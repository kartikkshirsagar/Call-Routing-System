#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct mobile_phone
{
    int id;
    int x;
} mobile;
typedef struct base_station
{
    int x;
    int id;
} base_station;

typedef enum
{
    FALSE,
    TRUE
} boolean;
typedef struct Index_node_type
{
    base_station stations[3];
    struct BPlus_Tree_Node *first;
    struct BPlus_Tree_Node *second;
    struct BPlus_Tree_Node *third;
    struct BPlus_Tree_Node *fourth;
    int size;
    int *set;

} indexNode;
typedef struct data_node_type
{
    mobile data[4];
    int used;
    struct BPlus_Tree_Node *next;
    struct BPlus_Tree_Node *prev;
} dataNode;

typedef struct BPlus_Tree_Node
{
    char tag;
    union {
        struct data_node_type *dataNode;
        struct Index_node_type *indexNode;
    } type;

} treeNode;

base_station Makebs(int id, int x)
{
    base_station bs;
    bs.id = id;
    bs.x = x;
    return bs;
}
treeNode *makeIndexNode(int x1, int x2, int x3)
{
    base_station b1, b2, b3;
    b1 = Makebs(rand() % 1000, x1);
    b2 = Makebs(rand() % 1000, x2);
    b3 = Makebs(rand() % 1000, x3);
    treeNode *node = (treeNode *)malloc(sizeof(treeNode));
    indexNode *iptr = (indexNode *)malloc(sizeof(indexNode));
    iptr->stations[0] = b1;
    iptr->stations[1] = b2;
    iptr->stations[2] = b3;
    node->tag = 'i';
    node->type.indexNode = iptr;
    node->type.indexNode->first = NULL;
    node->type.indexNode->second = NULL;
    node->type.indexNode->third = NULL;
    node->type.indexNode->fourth = NULL;
    return node;
}
treeNode *makeDataNode()
{
    treeNode *nptr;
    nptr = (treeNode *)malloc(sizeof(treeNode));
    dataNode *dptr = (dataNode *)malloc(sizeof(dataNode));
    dptr->used = 0;
    nptr->tag = 'd';
    nptr->type.dataNode = dptr;
    return nptr;
}

struct Queue
{
    int front, rear, size;
    unsigned capacity;
    treeNode **array;
};

struct Queue *createQueue(unsigned capacity)
{
    struct Queue *queue = (struct Queue *)malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;
    queue->array = (treeNode **)malloc(queue->capacity * sizeof(treeNode *));
    return queue;
}

int isFull(struct Queue *queue)
{
    return (queue->size == queue->capacity);
}

boolean isEmpty(struct Queue *queue)
{
    return (queue->size == 0) ? TRUE : FALSE;
}

void pushQ(struct Queue *queue, treeNode *item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}

treeNode *popQ(struct Queue *queue)
{
    if (isEmpty(queue))
        return NULL;
    treeNode *item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

// int front(struct Queue* queue)
// {
// 	if (isEmpty(queue))
// 		return INT_MIN;
// 	return queue->array[queue->front];
// }

// int rear(struct Queue* queue)
// {
// 	if (isEmpty(queue))
// 		return INT_MIN;
// 	return queue->array[queue->rear];
// }

void PrintBase(base_station b)
{
    printf("|---------------|\n");
    printf("|Base ID is %d|\n", b.id);
    printf("|Base Coordinate is %d|\n", b.x);
    printf("|---------------|\n");
}

void bfsPrint(treeNode *root, int spaces)
{
    if (root == NULL)
    {
        printf("Empty Tree\n");
        return;
    }
    struct Queue* Q=createQueue(16);
    pushQ(Q, root);
    while (isEmpty(Q) != TRUE)
    {
        int size = Q->size;
        treeNode *node = popQ(Q);
        if (node != NULL)
        {
            for (int i = 0; i < spaces; i++)
            {
                printf("\t");
            }
            for (int i = 0; i < size; i++)
            {
                int j = 0;
                while (j < 3)
                {
                    PrintBase(node->type.indexNode->stations[i]);
                    j++;
                }
                for (int i = 0; i < spaces; i++)
                {
                    printf("\t");
                }
            }
            printf("\n\n\n\n");
            pushQ(Q, node->type.indexNode->first);
            pushQ(Q, node->type.indexNode->second);
            pushQ(Q, node->type.indexNode->third);
            pushQ(Q, node->type.indexNode->fourth);
            spaces=spaces/2;
        }
        else
        {
            popQ(Q);
        }
        
    }
}

int main()
{
    treeNode *root, *first, *second, *third, *fourth;
    root = makeIndexNode(20, 50, 80);
    first = root->type.indexNode->first = makeIndexNode(6, 12, 18);
    second = root->type.indexNode->second = makeIndexNode(25, 35, 45);
    third = root->type.indexNode->third = makeIndexNode(55, 65, 75);
    fourth = root->type.indexNode->fourth = makeIndexNode(90, 100, 120);

    first->type.indexNode->first = makeDataNode();
    first->type.indexNode->second = makeDataNode();
    first->type.indexNode->third = makeDataNode();
    first->type.indexNode->fourth = makeDataNode();

    second->type.indexNode->first = makeDataNode();
    second->type.indexNode->second = makeDataNode();
    second->type.indexNode->third = makeDataNode();
    second->type.indexNode->fourth = makeDataNode();

    third->type.indexNode->first = makeDataNode();
    third->type.indexNode->second = makeDataNode();
    third->type.indexNode->third = makeDataNode();
    third->type.indexNode->fourth = makeDataNode();

    fourth->type.indexNode->first = makeDataNode();
    fourth->type.indexNode->second = makeDataNode();
    fourth->type.indexNode->third = makeDataNode();
    fourth->type.indexNode->fourth = makeDataNode();

    bfsPrint(root, 4);
}
