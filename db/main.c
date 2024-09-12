#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    char **row;
} Row;

typedef struct {
    Row *rows;
    int count;
} Result;


int result_callback(void *resultVar, int argc, char **argv, char **azColName) {
    Result *result = (Result *)resultVar;

    // TODO: understand this block better
    result->rows = realloc(result->rows, sizeof(Row) * (result->count + 1));
    result->rows[result->count].row = malloc(sizeof(char *) * argc);

    // TODO: what is strdup?
    for (int i = 0; i < argc; i++) {
        result->rows[result->count].row[i] = argv[i] ? strdup(argv[i]) : strdup("NULL");
    }

    result->count++;
    return 0;
}


static int callback(void *NotUsed, int argc, 
                    char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}


void create_table(sqlite3 *db) {
  char *zErrMsg = 0;
  int rc;

  char *sql = "CREATE TABLE IF NOT EXISTS TASKS(" \
       "ID INTEGER PRIMARY KEY," \
       "DESCRIPTION TEXT NOT NULL);";

  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Table Failed to Create: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }
}


void create_task(sqlite3 *db, char *description) {
  char *zErrMsg = 0;
  int rc;

  char sql[256] = "";
  strcat(sql, "INSERT INTO TASKS (DESCRIPTION) VALUES ('");
  strcat(sql, description);
  strcat(sql, "');");

  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

  if (rc != SQLITE_OK) {
    fprintf(stdout, "Failled to write task to TASKS: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }
}

void list_all_tasks(sqlite3 *db) {
  char *zErrMsg = 0;
  int rc;

  Result result = {0, 0};

  // TODO: option to limit
  char *sql = "SELECT * FROM TASKS;";

  rc = sqlite3_exec(db, sql, result_callback, (void *)&result, &zErrMsg);

  if (rc != SQLITE_OK) {
    fprintf(stdout, "Failed to list TASKS: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }

  printf("ID    Description\n");
  printf("---   -----------\n");
  for (int i = 0; i < result.count; i++) {
    printf("%s     ", result.rows[i].row[0] ? result.rows[i].row[0] : "NULL");
    printf("%s", result.rows[i].row[1] ? result.rows[i].row[1] : "NULL");
    printf("\n");
  }

  for (int i = 0; i < result.count; i++) {
    for (int j = 0; j < 2; j++) {  // We know there are only 2 columns
      free(result.rows[i].row[j]);
    }
    free(result.rows[i].row);
  }
  free(result.rows);
}


int main(int argc, char** argv) {
  if (argc < 2) {
    printf("Missing argument for input file\n");
    return 1;
  }


  sqlite3 *db;
  int rc;

  rc = sqlite3_open("test.db", &db);

  if (rc) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    return 1;
  }

  create_table(db);

  if (strcmp(argv[1],"add") == 0) {
    char description[256] = "";
    for (int i = 2; i < argc; i++) {
      strcat(description, argv[i]);
      strcat(description, " ");
    }
    printf("adding task=%s\n", description);
    create_task(db, description);
    return 0;
  }

  if (strcmp(argv[1], "list") == 0) {
    list_all_tasks(db);
    return 0;
  }

  // TODO: add delete by ID
  if (strcmp(argv[1], "delete") == 0) {
    printf("TODO: NOT YET IMPLEMENTED\n");
  }

  // TODO: add update by ID
  if (strcmp(argv[1], "update") == 0) {
    printf("TODO: NOT YET IMPLEMENTED\n");
  }

  sqlite3_close(db);

  return 0;
}
