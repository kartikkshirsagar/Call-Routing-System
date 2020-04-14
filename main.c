//ESSENTIALS
/*
1)Lets assume a 2D world and each mobile and the base have their respective locations identified by
  their co-ordinates.
2)Every base station has a coordinate, mobile also has co ordinates.
3) The data in index node would be the base station data struct.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define kary 4
#define capacity 4
#define min_capacity ceil(capacity / 2)
#define INITSIZE 4

typedef enum
{
	FAILURE,
	SUCCESS
} status_code;
typedef enum
{
	FALSE,
	TRUE
} boolean;
// typedef struct pair{
//     int x;
//     int y;
// }coordinate;
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

typedef struct
{
	mobile *array;
	size_t used;
	size_t size;
} vector;

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

void initVector(vector *a)
{
	a->array = (mobile *)malloc(INITSIZE * sizeof(mobile));
	a->used = 0;
	a->size = INITSIZE;
}

// int distance(coordinate a,coordinate b)
// {
//     float retval = (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y);
//     retval=sqrt(retval);
//     return (int)retval;
// }
void push_back(vector *a, mobile element)
{
	// a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
	// Therefore a->used can go up to a->size
	if (a->used == a->size)
	{
		a->size *= 2;
		a->array = (mobile *)realloc(a->array, a->size * sizeof(mobile));
	}
	a->array[a->used] = element;
	a->used++;
}
void removeAt(vector *a, int i)
{
	if (i > a->used || i < 0)
	{
		return;
	}
	int j = i;
	while (j < a->used)
	{
		a->array[j] = a->array[j + 1];
		j++;
	}
	a->used--;
	if (a->used <= (a->size / 2))
	{
		a->size = a->size / 2;
		a->array = (mobile *)realloc(a->array, a->size * sizeof(mobile));
	}
}
void clear(vector *a)
{
	free(a->array);
	a->array = NULL;
	a->used = a->size = 0;
}
// void printVector(vector *a)
// {
// 	int i;
// 	for (i = 0; i < a->used; i++)
// 	{
// 		printf("%d ", a->array[i]);
// 	}
// 	//printf(" --%ld-- ",a->size);
// 	printf("\n");
// }

// coordinate shiftPoint(coordinate newOrigin,coordinate oldOrigin,coordinate point)
// {
//   int xtemp=newOrigin.x-oldOrigin.x;
//   int ytemp=newOrigin.y-oldOrigin.y;

//   coordinate new;
//   new.x = point.x - xtemp;
//   new.y = point.y - ytemp;
//   return new;
// }

mobile MakeMobile(int id, int x)
{
	mobile m;
	m.id = id;
	m.x = x;
	return m;
}

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

treeNode *MakeTree()
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

	return root;
}
boolean isDataNodeFull(treeNode *nodeptr)
{
	boolean retval = FALSE;
	if (nodeptr->type.dataNode->used == capacity)
	{
		retval = TRUE;
	}
	return retval;
}

treeNode *SearchNearest(mobile mobile, treeNode *root, int *datanodeNumber)
{
	treeNode *retval;
	if (root->type.indexNode->first->tag == 'd')
	{
		retval = root;
		//we know its one of the 4 base stations where we have to insert.
		if (mobile.x < root->type.indexNode->stations[0].x)
		{
			*datanodeNumber = 1;
		}
		if (mobile.x > root->type.indexNode->stations[0].x && mobile.x < root->type.indexNode->stations[1].x)
		{
			*datanodeNumber = 2;
		}
		if (mobile.x > root->type.indexNode->stations[1].x && mobile.x < root->type.indexNode->stations[2].x)
		{
			*datanodeNumber = 3;
		}
		if (mobile.x > root->type.indexNode->stations[2].x)
		{
			*datanodeNumber = 4;
		}
	}
	else
	{
		if (mobile.x < root->type.indexNode->stations[0].x)
		{
			retval = SearchNearest(mobile, root->type.indexNode->first, datanodeNumber);
		}
		if (mobile.x > root->type.indexNode->stations[0].x && mobile.x < root->type.indexNode->stations[1].x)
		{
			retval = SearchNearest(mobile, root->type.indexNode->second, datanodeNumber);
		}
		if (mobile.x > root->type.indexNode->stations[1].x && mobile.x < root->type.indexNode->stations[2].x)
		{
			retval = SearchNearest(mobile, root->type.indexNode->third, datanodeNumber);
		}
		if (mobile.x > root->type.indexNode->stations[2].x)
		{
			retval = SearchNearest(mobile, root->type.indexNode->fourth, datanodeNumber);
		}
	}
	return retval;
}

status_code InsertMobile(mobile mobile, treeNode *root)
{
	status_code retval = SUCCESS;
	int num;
	treeNode *nearest;
	nearest = SearchNearest(mobile, root, &num);

	switch (num)
	{
	case 1:
		/* code */
		if (isDataNodeFull(nearest->type.indexNode->first))
		{
			retval = FAILURE;
			printf("NO SPACE LEFT TO INSERT");
		}
		else
		{
			nearest->type.indexNode->first->type.dataNode->data[nearest->type.indexNode->first->type.dataNode->used++] = mobile;
		}

		break;
	case 2:
		if (isDataNodeFull(nearest->type.indexNode->second))
		{
			retval = FAILURE;
		}
		else
		{
			nearest->type.indexNode->second->type.dataNode->data[nearest->type.indexNode->second->type.dataNode->used++] = mobile;
		}
		break;
	case 3:
		if (isDataNodeFull(nearest->type.indexNode->third))
		{
			retval = FAILURE;
		}
		else
		{
			nearest->type.indexNode->third->type.dataNode->data[nearest->type.indexNode->third->type.dataNode->used++] = mobile;
		}
		break;
	case 4:
		if (isDataNodeFull(nearest->type.indexNode->fourth))
		{
			retval = FAILURE;
		}
		else
		{
			nearest->type.indexNode->fourth->type.dataNode->data[nearest->type.indexNode->fourth->type.dataNode->used++] = mobile;
		}
		break;
	default:
		break;
	}

	return retval;
}
status_code DeleteinData(mobile m, treeNode *dataNode)
{
	int i, ind;
	status_code retval = FAILURE;
	mobile *mobiles = dataNode->type.dataNode->data;
	for (i = 0; i < capacity; i++)
	{
		if (m.id == mobiles[i].id)
		{
			ind = i;
			retval = SUCCESS;
		}
	}
	if (retval == SUCCESS)
	{
		int j = ind;
		while (j < capacity - 1)
		{
			mobiles[j + 1] = mobiles[j];
		}
		dataNode->type.dataNode->used--;
	}
	return retval;
}
status_code Delete_Mobile(mobile m, treeNode *root)
{
	status_code retval = SUCCESS;
	int num;
	treeNode *nearest = SearchNearest(m, root, &num);

	switch (num)
	{
	case 1:
		/* code */
		if (nearest->type.indexNode->first->type.dataNode->used == 0)
		{
			retval = FAILURE;
		}
		else
		{
			retval = DeleteinData(m, nearest->type.indexNode->first);
		}

		break;
	case 2:
		if (nearest->type.indexNode->second->type.dataNode->used == 0)
		{
			retval = FAILURE;
		}
		else
		{
			retval = DeleteinData(m, nearest->type.indexNode->second);
		}
		break;
	case 3:
		if (nearest->type.indexNode->third->type.dataNode->used == 0)
		{
			retval = FAILURE;
		}
		else
		{
			retval = DeleteinData(m, nearest->type.indexNode->third);
		}
		break;
	case 4:
		if (nearest->type.indexNode->fourth->type.dataNode->used == 0)
		{
			retval = FAILURE;
		}
		else
		{
			retval = DeleteinData(m, nearest->type.indexNode->fourth);
		}
		break;
	default:
		break;
	}

	return retval;
}

treeNode *lowestRouter(treeNode *root, mobile m1, mobile m2)
{
	int flag = 0;
	while (root->tag != 'd' && flag == 0)
	{
		if (m1.x < root->type.indexNode->stations[0].x && m2.x < root->type.indexNode->stations[0].x)
		{
			root = root->type.indexNode->first;
		}
		else if ((m1.x > root->type.indexNode->stations[0].x && m1.x < root->type.indexNode->stations[1].x) && (m2.x > root->type.indexNode->stations[0].x && m2.x < root->type.indexNode->stations[1].x))
		{
			root = root->type.indexNode->second;
		}
		else if ((m1.x > root->type.indexNode->stations[1].x && m1.x < root->type.indexNode->stations[2].x) && (m2.x > root->type.indexNode->stations[1].x && m2.x < root->type.indexNode->stations[2].x))
		{
			root = root->type.indexNode->third;
		}
		else if (m1.x > root->type.indexNode->stations[2].x && m2.x > root->type.indexNode->stations[2].x)
		{
			root = root->type.indexNode->fourth;
		}

		else
			flag = 1;
	}
	return root;
}
boolean SearchIndata(treeNode *dataNode, mobile m)
{
	int i;
	boolean retval = FALSE;
	for (i = 0; i < dataNode->type.dataNode->used; i++)
	{
		if (dataNode->type.dataNode->data[i].id == m.id)
		{
			retval = TRUE;
		}
	}
	return retval;
}
base_station *findPhone(mobile m, treeNode *root)
{
	treeNode *nearest;
	int num;
	base_station *retval;
	nearest = SearchNearest(m, root, &num);
	switch (num)
	{
	case 1:
		if (SearchIndata(nearest->type.indexNode->first, m))
		{
			retval = &nearest->type.indexNode->stations[0];
		}
		break;
	case 2:
		if (SearchIndata(nearest->type.indexNode->second, m))
		{
			retval = &nearest->type.indexNode->stations[1];
		}
		break;
	case 3:
		if (SearchIndata(nearest->type.indexNode->third, m))
		{
			retval = &nearest->type.indexNode->stations[2];
		}
		break;
	case 4:
		if (SearchIndata(nearest->type.indexNode->fourth, m))
		{
			retval = &nearest->type.indexNode->stations[2];
		}
		break;
	}
	return retval;
}

void printBase(treeNode *root, mobile mobile)
{
	printf("\n");
	printf("------------------------------------------------------\n");

	if (mobile.x < root->type.indexNode->stations[0].x)
	{
		printf("Base Station ID-> %d\n", root->type.indexNode->stations[0].id);
		printf("Base Station Coordinate-> %d\n", root->type.indexNode->stations[0].x);
	}
	else if (mobile.x > root->type.indexNode->stations[0].x && mobile.x < root->type.indexNode->stations[1].x)
	{
		printf("Base Station ID-> %d\n", root->type.indexNode->stations[1].id);
		printf("Base Station Coordinate-> %d\n", root->type.indexNode->stations[1].x);
	}
	else if (mobile.x > root->type.indexNode->stations[1].x && mobile.x < root->type.indexNode->stations[2].x)
	{
		printf("Base Station ID-> %d\n", root->type.indexNode->stations[2].id);
		printf("Base Station Coordinate-> %d\n", root->type.indexNode->stations[2].x);
	}
	else if (mobile.x > root->type.indexNode->stations[2].x)
	{
		printf("Base Station ID-> %d\n", root->type.indexNode->stations[2].id);
		printf("Base Station Coordinate-> %d\n", root->type.indexNode->stations[2].x);
	}

	printf("------------------------------------------------------\n");
}

void PrintMobile(mobile m)
{
	printf("--------------------\n");
	printf("Mobile ID is %d\n", m.id);
	printf("Mobile Coordinate is %d\n", m.x);
	printf("--------------------\n");
}

void PrintData(treeNode *dataNode)
{
	int i;
	int used = dataNode->type.dataNode->used;
	for (i = 0; i < used; i++)
	{
		PrintMobile(dataNode->type.dataNode->data[i]);
	}
}

void getData(treeNode *dataNode, vector *set)
{
	int i;
	int used = dataNode->type.dataNode->used;
	for (i = 0; i < used; i++)
	{
		push_back(set, dataNode->type.dataNode->data[i]);
	}
}

void TraverseAllDataAndPrint(treeNode *root)
{
	if (root->tag == 'd')
	{
		PrintData(root);
	}
	else
	{
		TraverseAllDataAndPrint(root->type.indexNode->first);
		TraverseAllDataAndPrint(root->type.indexNode->second);
		TraverseAllDataAndPrint(root->type.indexNode->third);
		TraverseAllDataAndPrint(root->type.indexNode->fourth);
	}
}

void TraverseAllDataAndGet(treeNode *root, vector *v)
{
	if (root->tag == 'd')
	{
		getData(root, v);
	}
	else
	{
		TraverseAllDataAndGet(root->type.indexNode->first, v);
		TraverseAllDataAndGet(root->type.indexNode->second, v);
		TraverseAllDataAndGet(root->type.indexNode->third, v);
		TraverseAllDataAndGet(root->type.indexNode->fourth, v);
	}
}

treeNode *SearchBase(int x, treeNode *root)
{
	if (root->tag != 'd')
	{
		int i;
		if (root->tag == 'i')
		{
			for (i = 0; i < capacity - 1; i++)
			{
				if (root->type.indexNode->stations[i].x == x)
				{
					return root;
				}
			}
		}
		SearchBase(x, root->type.indexNode->first);
		SearchBase(x, root->type.indexNode->second);
		SearchBase(x, root->type.indexNode->third);
		SearchBase(x, root->type.indexNode->fourth);
	}
}

void isSetEmpty(treeNode *node)
{
	vector v;
	initVector(&v);
	TraverseAllDataAndGet(node, &v);
	if (v.used == 0)
	{
		printf("It's Empty");
	}
	else
	{
		printf("Not Empty");
	}
}

void PrintChildToParent(treeNode *root, treeNode *last, mobile m, int num)
{
	if (last != root)
	{
		if (m.x < root->type.indexNode->stations[0].x)
		{
			PrintChildToParent(root->type.indexNode->first, last, m, 1);
		}
		else if (m.x > root->type.indexNode->stations[0].x && m.x < root->type.indexNode->stations[1].x)
		{
			PrintChildToParent(root->type.indexNode->second, last, m, 2);
		}
		else if (m.x > root->type.indexNode->stations[1].x && m.x < root->type.indexNode->stations[2].x)
		{
			PrintChildToParent(root->type.indexNode->third, last, m, 3);
		}
		else
		{
			PrintChildToParent(root->type.indexNode->fourth, last, m, 4);
		}
	}
	printBase(root, m);
}

void PrintParentToChild(treeNode *root, treeNode *last, mobile m)
{
	static int i = 0;
	i++;
	if (root->tag != 'd')
	{
		if (m.x < root->type.indexNode->stations[0].x)
		{
			if (i != 0)
			{
				printBase(root, m);
			}
			PrintParentToChild(root->type.indexNode->first, last, m);
		}
		else if (m.x > root->type.indexNode->stations[0].x && m.x < root->type.indexNode->stations[1].x)
		{
			if (i != 0)
			{
				printBase(root, m);
			}
			PrintParentToChild(root->type.indexNode->second, last, m);
		}
		else if (m.x > root->type.indexNode->stations[1].x && m.x < root->type.indexNode->stations[2].x)
		{
			if (i != 0)
			{
				printBase(root, m);
			}
			PrintParentToChild(root->type.indexNode->third, last, m);
		}
		else
		{
			if (i != 0)
			{
				printBase(root, m);
			}
			PrintParentToChild(root->type.indexNode->fourth, last, m);
		}
	}
}

status_code CallRoute(treeNode *root, mobile m1, mobile m2)
{
	int num;
	PrintChildToParent(root, SearchNearest(m1, root, &num), m1, 1);
	PrintParentToChild(lowestRouter(root, m1, m2), SearchNearest(m2, root, &num), m2);
}

status_code moveMobile(mobile m, base_station b, treeNode *root)
{
	Delete_Mobile(m, root);
	//We would have to change the existing mobile's coordinates so as to maintain the internal sorting order in the tree.
	m.x = b.x - 1;
	InsertMobile(m, root);
}

int main()
{
	treeNode *root;
	int num;
	root = MakeTree();
	mobile m = MakeMobile(100, 10);
	mobile m2 = MakeMobile(101, 60);
	InsertMobile(m, root);
	InsertMobile(m2, root);
	base_station *bs = findPhone(m, root);
	printf("%d\n", bs->x);
	//TraverseAllData(root);
	CallRoute(root, m, m2);
	isSetEmpty(root->type.indexNode->first);

	int ans;
	while (1)
	{
		printf("Enter the operation you want to do\n");
		printf("1.Insert Mobile\n2.Delete Mobile\n3.Call Route Trace\n4.Find out if a set is empty\n5.Find out a mobile's base station\n6.Move mobile to another base station\n");
		scanf("%d", &ans);
		switch (ans)
		{
		case 1:
			int mobileid, mobilex;
			printf("Please enter id and x-coordinate\n");
			scanf("%d %d", &mobileid, &mobilex);
			mobile m = MakeMobile(mobileid, mobilex);
			InsertMobile(m, root);
			break;
		case 2:
			int mobileid, mobilex;
			printf("Please enter id and x-coordinate\n");
			scanf("%d %d", &mobileid, &mobilex);
			mobile m = MakeMobile(mobileid, mobilex);
			Delete_Mobile(m, root);
			break;

		case 3:
			printf("Please enter id and x-coordinate of both phones(id1,x1,id2,x2)");
			int id1, id2, x1, x2;
			scanf("%d %d %d %d", &id1, &x1, &id2, &x2);
			mobile m = MakeMobile(id1, x1);
			mobile m2 = MakeMobile(id2, x2);
			CallRoute(root, m, m2);
			break;

		case 4:
			printf() break;

		case 5:
			break;

		case 6:
			break;
		}
	}
}