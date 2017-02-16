
#' @useDynLib exasol6
#' @export
exa.connect <- function(host, port, client, user, pwd) {

  exa_wscon <- Rcpp::Module("exa_wscon_module", PACKAGE = "exasol6")
  exa_ws_con <- exa_wscon$R_exa_ws_connection
  
  con <- new(exa_ws_con, host, port, client, user, pwd)
  con
  }

