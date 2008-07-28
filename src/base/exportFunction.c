
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "main.h"
#include "list.h"
#include "exportFunction.h"

typedef struct export_fce_item_struct
{
	char *name;
	void *function;
}  export_fce_item_t;

static list_t *listExportFce;

static export_fce_item_t* newExportFceItem(char *name, void *function)
{
	export_fce_item_t *new;

	new = malloc( sizeof(export_fce_item_t) );
	new->name = strdup(name);
	new->function = function;

	return new;
}

static void destroyExportFce(export_fce_item_t *p)
{
	free(p->name);
	free(p);
}

void initExortFunction()
{
	listExportFce = newList();
}
void addToExportFce(char *name, void *function)
{
	printf("add to export function %s\n", name);

	addList(listExportFce, newExportFceItem(name, function) );
}

void* getExportFce(char *name)
{
	int i;

	for( i = 0 ; i < listExportFce->count ; i++ )
	{
		export_fce_item_t *this;

		this = (export_fce_item_t *)listExportFce->list[i];

		if( strcmp(this->name, name) == 0 )
		{
			return this->function;
		}
	}

	return NULL;
}

void quitExportFunction()
{
	destroyListItem(listExportFce, destroyExportFce);
}


 
