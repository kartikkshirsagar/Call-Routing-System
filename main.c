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
#define COVERAGE_RADIUS 10

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
	int *array;
	size_t used;
	size_t size;
} vector;

typedef struct Index_node_type
{
	base_station bs1;
	base_station bs2;
	base_station bs3;
	struct BPlus_Tree_Node *first;
	struct BPlus_Tree_Node *second;
	struct BPlus_Tree_Node *third;
	struct BPlus_Tree_Node *fourth;
	int size;
	int* set;

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
	a->array = (int *)malloc(INITSIZE * sizeof(int));
	a->used = 0;
	a->size = INITSIZE;
}

// int distance(coordinate a,coordinate b)
// {
//     float retval = (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y);
//     retval=sqrt(retval);
//     return (int)retval;
// }
void push_back(vector *a, int element)
{
	// a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
	// Therefore a->used can go up to a->size
	if (a->used == a->size)
	{
		a->size *= 2;
		a->array = (int *)realloc(a->array, a->size * sizeof(int));
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
		a->array = (int *)realloc(a->array, a->size * sizeof(int));
	}
}
void clear(vector *a)
{
	free(a->array);
	a->array = NULL;
	a->used = a->size = 0;
}
void printVector(vector *a)
{
	int i;
	for (i = 0; i < a->used; i++)
	{
		printf("%d ", a->array[i]);
	}
	//printf(" --%ld-- ",a->size);
	printf("\n");
}

// coordinate shiftPoint(coordinate newOrigin,coordinate oldOrigin,coordinate point)
// {
//   int xtemp=newOrigin.x-oldOrigin.x;
//   int ytemp=newOrigin.y-oldOrigin.y;

//   coordinate new;
//   new.x = point.x - xtemp;
//   new.y = point.y - ytemp;
//   return new;
// }
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
		if (mobile.x < root->type.indexNode->bs1.x)
		{
			*datanodeNumber = 1;
		}
		if (mobile.x > root->type.indexNode->bs1.x && mobile.x < root->type.indexNode->bs2.x)
		{
			*datanodeNumber = 2;
		}
		if (mobile.x > root->type.indexNode->bs2.x && mobile.x < root->type.indexNode->bs3.x)
		{
			*datanodeNumber = 3;
		}
		if (mobile.x > root->type.indexNode->bs3.x)
		{
			*datanodeNumber = 4;
		}
	}
	else
	{
		if (mobile.x < root->type.indexNode->bs1.x)
		{
			retval=SearchNearest(mobile, root->type.indexNode->first,datanodeNumber);
		}
		if (mobile.x > root->type.indexNode->bs1.x && mobile.x < root->type.indexNode->bs2.x)
		{
			retval=SearchNearest(mobile, root->type.indexNode->second,datanodeNumber);
		}
		if (mobile.x > root->type.indexNode->bs2.x && mobile.x < root->type.indexNode->bs3.x)
		{
			retval=SearchNearest(mobile, root->type.indexNode->third,datanodeNumber);
		}
		if (mobile.x > root->type.indexNode->bs3.x)
		{
			retval=SearchNearest(mobile, root->type.indexNode->fourth,datanodeNumber);
		}
	}
	return retval;
}

status_code InsertMobile(mobile mobile, treeNode *root)
{
	status_code retval = SUCCESS;
	int num;
	treeNode* nearest;
	nearest=SearchNearest(mobile,root,&num);
	
	switch (num)
	{
	case 1:
		/* code */
		if(nearest->type.indexNode->first->type.dataNode->used==capacity)
		{
			retval= FAILURE;
		}
		else{
			nearest->type.indexNode->first->type.dataNode->data[nearest->type.indexNode->first->type.dataNode->used++]=mobile;
		}
		
		break;
	case 2:
		if(nearest->type.indexNode->second->type.dataNode->used==capacity)
		{
			retval = FAILURE;
		}
		else{
			nearest->type.indexNode->second->type.dataNode->data[nearest->type.indexNode->second->type.dataNode->used++]=mobile;
		}
		break;
	case 3:
		if(nearest->type.indexNode->third->type.dataNode->used==capacity)
		{
			retval=FAILURE;
		}
		else{
			nearest->type.indexNode->third->type.dataNode->data[nearest->type.indexNode->third->type.dataNode->used++]=mobile;
		}
		break;
	case 4:
		if(nearest->type.indexNode->fourth->type.dataNode->used==capacity)
		{
			retval= FAILURE;
		}
		else{
			nearest->type.indexNode->fourth->type.dataNode->data[nearest->type.indexNode->fourth->type.dataNode->used++]=mobile;		
		}
		break;
	default:
		break;
	}

	return retval;
}

int main()
{
}