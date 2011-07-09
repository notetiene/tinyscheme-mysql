#!/usr/local/bin/scheme

(load-extension "tsx_mysql")

(display "Mysql extension test.")(newline)

(define queries (list 
                  "SELECT VERSION(), CURRENT_DATE" 
                  "SHOW DATABASES" 
                  "SELECT  table_name, table_type, engine FROM information_schema.tables"))

(define run-query 
  (lambda (sql)
    (let ((result (mysql-query sql)))
      (if result
        (display result)
        (display (mysql-error))))))

(if (mysql-connect "" "" "" "")
    (for-each run-query queries)
    (display (mysql-error)))

(mysql-disconnect)
(newline)


