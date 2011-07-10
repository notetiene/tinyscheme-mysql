<beginning of page>

tsx_mysql(3)  tsx_mysql(3)
NAME 
 tsx_mysql.so <-> a mysql extension for tiny scheme
SYNOPSIS 
 (load-extension "tsx_mysql")
 Load the MySQL extension into the tiny scheme environment.
 (mysql-connect hostname username password database)
 Connect to a MySQL database. This function takes four string
 arguments: hostname, username, password, database.
 (mysql-disconnect)
 Disconnect from a MySQL database.
 (mysql-query sql)
 Query a MySQL database. This function takes a string containing
 SQL statements. The query results will be returned as a list of
 strings.
 (mysql-error)
 Returns the current MySQL error string.
FILES 
 tsx_mysql.so
ENVIRONMENT 
AUTHOR 
  tsx_mysql(3)
