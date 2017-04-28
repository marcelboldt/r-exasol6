#' @docType package
#' @name exasol6-package
#' @aliases exasol6
#' @import methods Rcpp
#' @exportPattern ^[[:alpha:]]+
#'
#'
#' @title R Interface & SDK for the EXASOL Database
#'
#' @description The EXASOL 6 R SDK offers interface functionality such as connecting to, querying
#' and writing into an EXASOL data base It is optimised for fast
#' reading & writing from and to a multinode cluster, via a Websockets protocol. Implemented are DBI
#' compliant methods for database access, querying and modiifcation. The package integrates with
#' EXASOL's InDB R UDF capabilities, which allows to deploy and execute R code dynamically from
#' an R application running on a client.
#'
#' EXASOL is a high-performance and highly scalable in-memory RDBMS that runs in a shared-nothing MPP cluster.
#' Ranked #1 in the TPC-H benckmark, it is considered the fastest analytical data warehouse available. A free edition
#' can be downloaded from \url{https://www.exasol.com/portal}.
#'
#'
#'
#' @author Marcel Boldt, EXASOL & Community
#'
#' Maintainer: \packageMaintainer{exasol6}
#'
#' @references
#'
#' \enumerate{
#' \item The development version of the package is available on \url{https://github.com/EXASOL/r-exasol}
#' \item Bugs and improvements may be noted on \url{https://github.com/EXASOL/r-exasol/issues}
#' \item Downloads & manuals related to the EXASOL Database are at \url{https://www.exasol.com/portal}
#' \item Q & A: \url{https://www.exasol.com/portal/questions}
#'}
#' @keywords sql
#' @keywords distributed
#' @keywords in-memory
NULL

.onAttach <- function(libname, pkgname) {
  # show startup message
  message <- paste("EXASOL6 SDK", utils::packageVersion("exasol6"), "loaded.")
  packageStartupMessage(message, appendLF = TRUE)
}
