#include "microdevtools.h"

mdt_dbd_pool_t *dbd_pool;
volatile sig_atomic_t server_run = 1;

void mdt_signal_exit(int signum) {
  if (signum == SIGTERM || signum == SIGINT) {
    server_run = 0;
  }
}

int main(int argc, char **argv) {
  struct sigaction sig_action;
  mdt_signal_handler(&sig_action, mdt_signal_exit);
  apr_status_t rv = apr_initialize();
  if (rv != APR_SUCCESS) exit(EXIT_FAILURE);
  apr_pool_t *mp;
  rv = apr_pool_create(&mp, NULL);
  if (rv != APR_SUCCESS) exit(EXIT_FAILURE);
  mdt_server_t *s = mdt_server_alloc(mp);
  if (s == NULL) exit(EXIT_FAILURE);
  char *er_msg;
  if (!mdt_server_init(mp, &s, argc, argv, &(er_msg))) exit(EXIT_FAILURE);
  dbd_pool = NULL;
  if (s->dbd_driver != NULL) {
    if (s->dbd_conn_s != NULL) {
      rv = apr_dbd_init(mp);
      if (rv == APR_SUCCESS) {
        if (!mdt_dbd_pool_alloc(mp)) exit(EXIT_FAILURE);
        if (!mdt_dbd_pool_init(mp, s->dbd_driver, s->dbd_conn_s)) {
          exit(EXIT_FAILURE);
        }
      }
    }
  }
  if (DAEMON) mdt_daemonize();
  if (MONGOOSE) {
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);
    if (TLS && s->addr_s) {
      mg_http_listen(&mgr, s->addr, mdt_http_request_handler, NULL);
      mg_http_listen(&mgr, s->addr_s, mdt_http_request_handler, (void*)s);
    } else {
      mg_http_listen(&mgr, s->addr, mdt_http_request_handler, (void*)s);
    }
    while (server_run) mg_mgr_poll(&mgr, 1000);
    mg_mgr_free(&mgr);
  }
  if(dbd_pool) mdt_dbd_pool_destroy();
  mdt_server_destroy(s);
  apr_pool_destroy(mp);
  apr_terminate();
  return 0;
}