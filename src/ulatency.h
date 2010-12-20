#ifndef __ulatency__

#include <glib.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "proc/procps.h"
#include "proc/readproc.h"


#define VERSION 0.1

#define OPENPROC_FLAGS PROC_FILLMEM | \
  PROC_FILLUSR | PROC_FILLGRP | PROC_FILLSTATUS | PROC_FILLSTAT | \
  PROC_FILLWCHAN | PROC_FILLCGROUP | PROC_FILLSUPGRP

extern GMainLoop *main_loop;
extern GList *filter_list;

enum FILTER_TYPES {
  FILTER_LUA,
  FILTER_C
};

enum FILTER_SKIP {
  FILTER_STOP=-1,
};


struct lua_callback {
  lua_State *lua_state;
  int lua_func;
  int lua_data;
};

struct lua_filter {
  lua_State *lua_state;
  int lua_func;
  int lua_data;
  GRegex *regexp_cmdline;
  GRegex *regexp_basename;
};

struct filter_block {
  unsigned int pid;
  GTime timeout;
  gboolean skip;
};


typedef struct _filter {
  enum FILTER_TYPES type;
  char *name;
  int (*check)(struct proc_t *proc, struct _filter *filter);
  int (*callback)(struct proc_t *proc, struct _filter *filter);
  void *data;
  GHashTable *skip_filter;
} filter;




filter *filter_new();
void filter_free(filter *filter);

void filter_register(filter *filter);
void filter_unregister(filter *filter);

void filter_run_for_proc(gpointer data, gpointer user_data);
int l_filter_run_for_proc(struct proc_t *proc, filter *flt);
void cp_proc_t(const struct proc_t *src, struct proc_t *dst);

#endif