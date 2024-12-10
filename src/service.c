#include "microdevtools.h"

int HelloWorldController(mdt_service_t *s) {
  mdt_http_response_hd_set(s->response, "Content-Type", "application/json");
  const char *msg = mdt_json_encode(s->pool, "Hello, World!", MDT_JSON_T_STRING);
  mdt_printf(s, "{\"err\":%s,\"log\":%s,\"out\":%s}", "false", "null", msg);
  return 200;
}

void mdt_handler(mdt_service_t *s) {
  mdt_route(s, "GET", "/api/helloworld", HelloWorldController);
}