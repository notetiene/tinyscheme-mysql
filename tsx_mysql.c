/* 
 * MySQL extension for TinyScheme 
 *
 * Copyright (C) 2011   A. Carl Douglas  (carl.douglas@gmail.com)
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <time.h>

#include <mysql.h>
#include "scheme.h"
#include "scheme-private.h"

/* the cons define collides with scheme interface */
#undef _cons
#undef cons

static MYSQL *conn = NULL;

static pointer foreign_connect(scheme *sc, pointer args)
{
  char *host;
  char *user;
  char *pass;
  char *db;

  pointer sc_arg[4];

  int i;

  if(args == sc->NIL)
    return sc->F;

  for (i=0; i < 4; i++) {
    sc_arg[i] = sc->vptr->pair_car(args);
    if (!sc->vptr->is_string(sc_arg[i])) return sc->F;
    args = sc->vptr->pair_cdr(args);
  }
  host = sc->vptr->string_value(sc_arg[0]);
  user = sc->vptr->string_value(sc_arg[1]);
  pass = sc->vptr->string_value(sc_arg[2]);
  db   = sc->vptr->string_value(sc_arg[3]);

  conn = mysql_init(NULL);
  if (conn == NULL)
    return sc->F;
  mysql_real_connect(conn, host, user, pass, db, 0, NULL, 0);
  return sc->T;
}

static pointer foreign_query(scheme *sc, pointer args)
{ 
  char *sql;
  int rc;

  pointer arg;
  
  pointer value;

  MYSQL_RES *result;

  if (args==sc->NIL || conn == NULL)
    return sc->F;

  value = sc->NIL;
  arg = sc->vptr->pair_car(args);
  if (!sc->vptr->is_string(arg)) 
    return sc->F;
  sql = sc->vptr->string_value(arg);

  rc = mysql_query(conn, sql);
  if (rc != 0) 
    return sc->F;

  result = mysql_store_result(conn);
  if (result) {
    MYSQL_ROW row;
    int num_fields = 0;
    int num_rows = 0;
    int i = 0;
    int row_index = 0;

    num_fields = mysql_num_fields(result);
    num_rows = mysql_num_rows(result);

    value = sc->vptr->mk_vector(sc, num_rows);

    for (row_index = 0; (row = mysql_fetch_row(result)) ; row_index++) {
      pointer sc_row = sc->NIL;
      for (i = (num_fields-1); i >= 0; i--) {
        pointer sc_field;
        sc_field = sc->vptr->mk_string(sc, row[i]);
        sc_row   = sc->vptr->cons(sc, sc_field, sc_row);
      }
      sc->vptr->set_vector_elem(value, row_index, sc_row);
    }
    mysql_free_result(result);
  } else {
    if(mysql_field_count(conn) == 0) {
      unsigned long num_rows;
      char buffer[255];
      num_rows = mysql_affected_rows(conn);
      snprintf(buffer, 255, "%lu rows affected.", num_rows);
      value = sc->vptr->mk_string(sc, buffer);
    }
  }

  return value;
}

static pointer foreign_disconnect(scheme *sc, pointer args)
{
  mysql_close(conn);
  conn = NULL;
  return sc->T;
}

static pointer foreign_error(scheme *sc, pointer args)
{
  const char *myerr;
  pointer result;

  myerr = mysql_error(conn);
  if (myerr) 
    result = sc->vptr->mk_string(sc, myerr);
  else
    result = sc->F;

  return result; 
}

void init_tsx_mysql(scheme *sc)
{
  sc->vptr->scheme_define(sc,sc->global_env,
    sc->vptr->mk_symbol(sc,"mysql-connect"),
    sc->vptr->mk_foreign_func(sc, foreign_connect)); 
  sc->vptr->scheme_define(sc,sc->global_env,
    sc->vptr->mk_symbol(sc,"mysql-disconnect"),
    sc->vptr->mk_foreign_func(sc, foreign_disconnect)); 
  sc->vptr->scheme_define(sc,sc->global_env,
    sc->vptr->mk_symbol(sc,"mysql-error"),
    sc->vptr->mk_foreign_func(sc, foreign_error)); 
  sc->vptr->scheme_define(sc,sc->global_env,
    sc->vptr->mk_symbol(sc,"mysql-query"),
    sc->vptr->mk_foreign_func(sc, foreign_query)); 
}

/* EOF */

