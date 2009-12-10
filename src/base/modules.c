#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "main.h"
#include "list.h"
#include "tux.h"
#include "shot.h"
#include "modules.h"
#include "arena.h"
#include "arenaFile.h"
#include "proto.h"
#include "shareFunction.h"

#ifndef PUBLIC_SERVER
#include "interface.h"
#include "image.h"
#include "layer.h"
#include "configFile.h"
#endif /* PUBLIC_SERVER */

static export_fce_t export_fce = {
#ifndef PUBLIC_SERVER
	.fce_addLayer = addLayer,
	.fce_image_get = image_get,
#endif /* PUBLIC_SERVER */
	.fce_module_load_dep = module_load_dep,
	.fce_share_function_add = share_function_add,
	.fce_share_function_get = share_function_get,

	.fce_getValue = arena_file_get_value,
	.fce_net_multiplayer_get_game_type = net_multiplayer_get_game_type,
	.fce_tux_get_proportion = tux_get_proportion,
	.fce_tux_set_proportion = tux_set_proportion,
	.fce_tux_action = tux_action,

	.fce_timer_get_current_time = timer_get_current_time,

	.fce_arena_get_current = arena_get_current,
	.fce_arena_conflict_space = arena_conflict_space,
	.fce_arena_is_free_space = arena_is_free_space,
	.fce_arena_find_free_space = arena_find_free_space,

	.fce_proto_send_module_server = proto_send_module_server,
	.fce_proto_send_del_server = proto_send_del_server,
#ifndef PUBLIC_SERVER
	.fce_proto_send_module_client = proto_send_module_client,
#endif /* PUBLIC_SERVER */
	.fce_shot_destroy = shot_destroy,
	.fce_shot_bound_bombBall = shot_bound_bombBall,
	.fce_shot_transform_lasser = shot_transform_lasser
};

static list_t *listModule;
static mod_reg_t mod_reglist;

/* register module as valid */
static mod_reg_t *mod_register (char *name, mod_sym_t *sym)
{
	/* alloc and init context */
	mod_reg_t *mod = (mod_reg_t *) malloc (sizeof (mod_reg_t));
	
	if (!mod)
		return 0;

	mod->name = strdup (name);
	mod->sym = sym;
		
	/* add entry into list */
	mod->next = &mod_reglist;
	mod->prev = mod_reglist.prev;
	mod->prev->next = mod;
	mod->next->prev = mod;

	return mod;
}

static mod_reg_t *mod_find (char *name)
{
	mod_reg_t *mod;
	for (mod = mod_reglist.next; mod != &mod_reglist; mod = mod->next) {
		if (!strcmp (mod->name, name))
			return mod;
	}

	return 0;
}

static void setModulePath(char *name, char *path)
{
	sprintf(path, PATH_MODULES "%s" MODULE_TYPE, name);
}

static module_t *newModule(char *name)
{
	char path[STR_PATH_SIZE];

	module_t *ret;

	assert(name != NULL);
	
	mod_reg_t *mod = mod_find (name);
	
	if (!mod) {
		error ("Module [%s] is unavailable", name);
		return 0;
	}
		
	setModulePath(name, path);

	ret = malloc (sizeof (module_t));
	
	if (!ret)
		return 0;
	
	ret->name = strdup (name);

	ret->fce_init = mod->sym->init;
#ifndef PUBLIC_SERVER
	ret->fce_draw = mod->sym->draw;
#endif /* PUBLIC_SERVER */
	ret->fce_event = mod->sym->event;
	ret->fce_destroy = mod->sym->destroy;
	ret->fce_isConflict = mod->sym->isConflict;
	ret->fce_cmd = mod->sym->cmdArena;
	ret->fce_recvMsg = mod->sym->recvMsg;

	debug("Loading module [%s]", name);

	if (ret->fce_init(&export_fce) != 0) {
		error("Unable to load module [%s]", name);
		free(ret->name);
		free(ret);
		return NULL;
	}

	return ret;
}

static int destroyModule(module_t *p)
{
	assert(p != NULL);

	debug("Unloading module [%s]", p->name);

	p->fce_destroy();
	free(p->name);
	free(p);

	return 0;
}

void module_init()
{
	listModule = list_new();
	share_function_init();

	mod_reglist.next = &mod_reglist;
	mod_reglist.prev = &mod_reglist;
	
	mod_register ("libmodAI", &modai_sym);
	mod_register ("libmodWall", &modwall_sym);
	mod_register ("libmodPipe", &modpipe_sym);
	mod_register ("libmodMove", &modmove_sym);
	mod_register ("libmodBasic", &modbasic_sym);
	mod_register ("libmodTeleport", &modteleport_sym);
}

int isModuleLoaded(char *name)
{
	int i;

	for (i = 0; i < listModule->count; i++) {
		module_t *this;

		this = (module_t *) listModule->list[i];

		if (strcmp(this->name, name) == 0) {
			return 1;
		}
	}

	return 0;
}

int module_load(char *name)
{
	module_t *module;

	if (isModuleLoaded(name)) {
		error("Unable to load already loaded module [%s]", name);
		return -1;
	}

	module = newModule(name);

	if (module == NULL) {
		error("Unable to load module [%s]", name);
		return -1;
	}

	list_add(listModule, module);

	return 0;
}

int module_load_dep(char *name)
{
	if (isModuleLoaded(name)) {
		return 0;
	}

	return module_load(name);
}

void module_cmd(char *s)
{
	int i;

	for (i = 0; i < listModule->count; i++) {
		module_t *this;

		this = (module_t *) listModule->list[i];
		this->fce_cmd(s);
	}
}

#ifndef PUBLIC_SERVER
void module_draw(int x, int y, int w, int h)
{
	int i;

	for (i = 0; i < listModule->count; i++) {
		module_t *this;

		this = (module_t *) listModule->list[i];
		this->fce_draw(x, y, w, h);
	}
}
#endif /* PUBLIC_SERVER */

void module_event()
{
	int i;

	for (i = 0; i < listModule->count; i++) {
		module_t *this;

		this = (module_t *) listModule->list[i];
		this->fce_event();
	}
}

int module_is_conflict(int x, int y, int w, int h)
{
	int i;

	for (i = 0; i < listModule->count; i++) {
		module_t *this;

		this = (module_t *) listModule->list[i];

		if (this->fce_isConflict(x, y, w, h) == 1) {
			return 1;
		}
	}

	return 0;
}

int module_recv_msg(char *msg)
{
	int i;

	for (i = 0; i < listModule->count; i++) {
		module_t *this;

		this = (module_t *) listModule->list[i];
		this->fce_recvMsg(msg);
	}

	return 0;
}

void module_quit()
{
	list_destroy_item(listModule, destroyModule);
	share_function_quit();
}
