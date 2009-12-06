#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "main.h"
#include "modules.h"
#include "tux.h"
#include "shot.h"
#include "list.h"
#include "gun.h"
#include "space.h"

#ifndef PUBLIC_SERVER
#include "interface.h"
#include "image.h"
#else
#include "publicServer.h"
#endif

static export_fce_t *export_fce;

int init(export_fce_t *p)
{
	export_fce = p;

	debug("Initializing Basic module");
	return 0;
}

#ifndef PUBLIC_SERVER

int draw(int x, int y, int w, int h)
{
	UNUSED(x);
	UNUSED(y);
	UNUSED(w);
	UNUSED(h);

	debug("Drawing Basic module");
	return 0;
}
#endif

int event()
{
	debug("Basic module catch event");
	return 0;
}

int isConflict(int x, int y, int w, int h)
{
	debug("isConflict(%d, %d, %d, %d) in Basic module", x, y, w, h);
	return 0;
}

static void cmd_basic(char *line)
{
	debug("cmd_basic(%s) in Basic module", line);
}

void cmdArena(char *line)
{
	if (strncmp(line, "basic", 5) == 0)
		cmd_basic(line);
}

void recvMsg(char *msg)
{
	debug("recvMsg(%s) in Basic module", msg);
}

int destroy()
{
	debug("Destroying Basic module");
	return 0;
}
