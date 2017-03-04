library(Rcpp)
library(exasol6)

con <- exa.connect("192.168.137.8", 8563, "R",  "sys", "exasol")

con$exec_sql("SELECT CURRENT_TIMESTAMP")

df <- as.data.frame((con$exec_sql("SELECT * FROM PUB1092.FLIGHTS limit 1")), strigsAsFactors = FALSE)

