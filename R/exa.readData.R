#' Low level method to send a query to EXASOL and retrieve a result set.
#'
#' @description The function sends an SQL statement via an open connection to an EXASOL DB
#' and retrieves a result set. The result set is at C++ level fetched via JSON over Websockets,
#' parsed and handed over to R as a list of vectors, that is then converted to a data.frame.
#'
#' @param con The host's IP or DNS name to connect to.
#' @param sql The EXASOL user password; default: 'exasol'.
#' @param ... Further options to be passed to `as.data.frame()`.
#'
#' @return A connection object.
#'
#' @author Marcel Boldt <marcel.boldt@@exasol.com>
#'
#' @example examples/ex_exa_readData.R
#' @useDynLib exasol6
#' @export
exa.readData <- function(con, sql, ...) {
    as.data.frame(
        con$exec_sql(sql),
        ...
        )
}
