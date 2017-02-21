#' @docType package
#' @name exasol6-package
#' @aliases exasol6
#' @exportPattern ^[[:alpha:]]+
#'
#' @title R Interface & SDK for the EXASOL Database
#'
#' @description The EXASOL R Package offers interface functionality such as connecting to, querying
#' and writing into an EXASOL Database (version 5 onwards). It is optimised for massively parallel
#' reading & writing from and to a multinode cluster. Implemented are DBI compliant methods for database
#' access, querying and modiifcation. The package integrates with EXASOL's InDB R UDF capabilities, which
#' allows to deploy and execute R code dynamically from an R application running on a client.
#'
#' EXASOL is an InMemory RDBMS that runs in a MPP cluster (shared-nothing) environment. Leading the TPC-H
#' benckmark, it is considered the fastest analytical data warehouse available. The community edition
#' can be downloaded for free on \url{https://www.exasol.com/portal}.
#'
#'
#'
#'
#' @author EXASOL AG & Community
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
