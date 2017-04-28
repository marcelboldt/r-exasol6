#' Low level method to connect to an EXASOL data base.
#'
#' @description The function connects to an EXASOL database and returns a connection object.
#'
#' A websockets (RFC 6455) connection is initiated to the EXASOL server, over which a JSON element
#' is sent to requests the start of the login sequence. Subsequently, a 1024-bit RSA public key is received
#' from EXASOL DB, to encrypt the user password. The client uses that to build request JSON for login into
#' EXASOL DB, which also contains some meta information (client name, etc.). EXASOL responds either
#' with a confirmation containing a session ID and further meta data, or a rejection.
#'
#' @param host The host's IP or DNS name to connect to.
#' @param port The EXASOL DB port; default: 8563.
#' @param user The EXASOL DB user; default: 'sys'.
#' @param pwd The EXASOL user password; default: 'exasol'.
#'
#' @return Returns a connection object.
#'
#' @author Marcel Boldt <marcel.boldt@@exasol.com>
#'
#' @example examples/ex_exa_connect.R
#' @useDynLib exasol6
#' @export
exa.connect <- function(host, port = 8563, user="sys", pwd="exasol") {

  exa_wscon <- Rcpp::Module("exa_wscon_module", PACKAGE = "exasol6")
  exa_ws_con <- exa_wscon$R_exa_ws_connection

  client = sessionInfo()$R.version$version.string

  con <- new(exa_ws_con, host, port, client, user, pwd)
  con
  }
