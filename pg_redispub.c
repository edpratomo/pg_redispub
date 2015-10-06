#include "postgres.h"
#include "fmgr.h"
#include <hiredis/hiredis.h>
#include "utils/builtins.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(redispub);

Datum redispub(PG_FUNCTION_ARGS) {
  redisReply *reply = NULL;
  struct timeval timeout = { 0, 500000 }; /* 0.5 minutes */
  char *channel = text_to_cstring(PG_GETARG_TEXT_P(0));
  char *msg = text_to_cstring(PG_GETARG_TEXT_P(1));

  redisContext *ctx = redisConnectWithTimeout("127.0.0.1", 6379, timeout);
  if (ctx == NULL || ctx->err) {
    if (ctx) {
      ereport(WARNING, (errcode(ERRCODE_WARNING), errmsg("failed to connect to redis: %s", ctx->errstr)));
      redisFree(ctx);
    } else {
      ereport(WARNING, (errcode(ERRCODE_WARNING), errmsg("failed to connect to redis: can't allocate redis context")));
    }
    PG_RETURN_BOOL(false);
  }

  reply = redisCommand(ctx, "PUBLISH %s %s", channel, msg);
  freeReplyObject(reply);
  if (ctx->err) {
    ereport(WARNING, (errcode(ERRCODE_WARNING), errmsg("failed to PUBLISH to redis: %s", ctx->errstr)));
    redisFree(ctx);
    PG_RETURN_BOOL(false);
  } else {
    redisFree(ctx);
    PG_RETURN_BOOL(true);
  }
}
