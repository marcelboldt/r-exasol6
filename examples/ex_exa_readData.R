\dontrun{
    library(exasol6)

    con <- exa.connect("192.168.137.8", 8563, "R", "sys", "exasol")
    exa.readData(con, "SELECT CURRENT_TIMESTAMP")

    df <- exa.readData(con, "SELECT * FROM R.FLIGHTS LIMIT 100",
                       row.names = c(1:100),
                       stringsAsFactors=FALSE)
}
