#include "run.h"

#include "postgres.h"
#include "utils/builtins.h"

#include "funcapi.h"
#include <stdbool.h>

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(run_bench);
Datum run_bench(PG_FUNCTION_ARGS) {
  typedef struct Context {
    int index;
  } Context;

  typedef struct Item {
    const char *name;
    long long (*pg)(void);
    long long (*cpp)(void);
  } Item;

#define add(s)                                                                 \
  { .name = #s, .pg = pg_##s, .cpp = cpp_##s }
  static const Item is[] = {
      add(bitmapset_compare),
      add(bitmapset_is_subset),
      add(bitmapset_add_member),
  };
#undef add

  FuncCallContext *funcctx;

  if (SRF_IS_FIRSTCALL()) {
    funcctx = SRF_FIRSTCALL_INIT();

    MemoryContext oldcontext =
        MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);
    {
      TupleDesc tupdesc = CreateTemplateTupleDesc(3);
      TupleDescInitEntry(tupdesc, 1, "name", TEXTOID, -1, 0);
      TupleDescInitEntry(tupdesc, 2, "pg time", INT8OID, -1, 0);
      TupleDescInitEntry(tupdesc, 3, "cpp time", INT8OID, -1, 0);
      funcctx->tuple_desc = BlessTupleDesc(tupdesc);
      Context *context = (Context *)palloc(sizeof(Context));
      context->index = 0;
      funcctx->user_fctx = context;
    }
    MemoryContextSwitchTo(oldcontext);
  }

  funcctx = SRF_PERCALL_SETUP();
  Context *context = (Context *)funcctx->user_fctx;

  if (context->index >= sizeof(is) / sizeof(Item)) {
    SRF_RETURN_DONE(funcctx);
  }

  bool nulls[3] = {false, false, false};
  Datum v[3] = {
      DirectFunctionCall1(textin, CStringGetDatum(is[context->index].name)),
      Int64GetDatum(is[context->index].pg()),
      Int64GetDatum(is[context->index].cpp()),
  };
  ReturnSetInfo *rsi = (ReturnSetInfo *)fcinfo->resultinfo;
  HeapTuple tuple = heap_form_tuple(rsi->expectedDesc, v, nulls);

  context->index++;
  SRF_RETURN_NEXT(funcctx, HeapTupleGetDatum(tuple));
  PG_RETURN_VOID();
}
