#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIANE_DB_ENV "DIANE_DB"
#define DIANE_DB_FILENAME ".diane_db"

// TODO support different path separators for different environments
#define PATH_SEP "/"

char *db_file_path(void)
{
    char *db_path = NULL;
    const char *diane_db_env = getenv(DIANE_DB_ENV);
    if (diane_db_env)
    {
        asprintf(&db_path, "%s", diane_db_env);
    }
    else
    {
        // TODO handle home paths that include trailing path separator
        asprintf(&db_path, "%s%s%s", getenv("HOME"), PATH_SEP, DIANE_DB_FILENAME);
    }

    return db_path;
}

int error_from_sqlite_error(char **dest, const char *message, char *sqlite_error)
{
    int rc = asprintf(dest, "%s: %s\n", message, sqlite_error);
    sqlite3_free(sqlite_error);

    return rc;
}

int open_db(const char *db_path, sqlite3 **db, char **error)
{
    int rc = sqlite3_open(db_path, db);
    if (rc != SQLITE_OK)
    {
        asprintf(error, "Cannot open database: %s\n", sqlite3_errmsg(*db));
        return rc;
    }

    const char *sql = "CREATE TABLE IF NOT EXISTS Items(\
        Id INTEGER PRIMARY KEY AUTOINCREMENT\
        ,Content TEXT\
        ,CreatedOn TIMESTAMP DEFAULT CURRENT_TIMESTAMP\
    )";
    char *sql_error = NULL;
    rc = sqlite3_exec(*db, sql, NULL, NULL, &sql_error);
    if (rc != SQLITE_OK)
    {
        error_from_sqlite_error(error, "Cannot create items table", sql_error);
        return rc;
    }

    return 0;
}

int add_item(sqlite3 *db, const char *content, char **error)
{
    char *sql = NULL;
    asprintf(&sql, "INSERT INTO Items (Content) VALUES ('%s')", content);
    char *sql_error = NULL;
    int rc = sqlite3_exec(db, sql, NULL, NULL, &sql_error);
    if (rc != SQLITE_OK)
    {
        error_from_sqlite_error(error, "Cannot insert item", sql_error);
        return rc;
    }

    return 0;
}

void join(char **dest, const int count, const int start, const char **strings, const char *separator)
{
    int sep_len = strlen(separator);
    int length = 0;
    for (int i = start; i < count; ++i)
    {
        length += strlen(strings[i]);
        if (i != count - 1)
        {
            i += sep_len;
        }
    }
    ++length; // null terminator

    *dest = calloc(length, sizeof(char));
    for (int i = start; i < count; ++i)
    {
        strcat(*dest, strings[i]);
        if (i != count - 1)
        {
            strcat(*dest, separator);
        }
    }
}

void on_error(const char *error, sqlite3 *db)
{
    fprintf(stderr, "%s", error);
    sqlite3_close(db);
    exit(EXIT_FAILURE);
}

int main(const int argc, const char *argv[])
{
    char *db_path = db_file_path();
    sqlite3 *db = NULL;
    char *error = NULL;
    int rc = open_db(db_path, &db, &error);
    if (rc != 0)
    {
        on_error(error, db);
    }
    // TODO only show with verbose flag
    printf("Using database %s\n", db_path);

    char *item = NULL;
    join(&item, argc, 1, argv, " ");
    rc = add_item(db, item, &error);
    if (rc != 0)
    {
        on_error(error, db);
    }
    free(item);
    item = NULL;

    sqlite3_close(db);
    free(db_path);
    db_path = NULL;

    return EXIT_SUCCESS;
}