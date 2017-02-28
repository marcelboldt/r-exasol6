library(Rcpp)

con <- exa.connect("192.168.137.8", 8563, "R",  "sys", "exasol")
df <- as.data.frame(con$exec_sql("SELECT * FROM PUB1092.FLIGHTS LIMIT 10"))
