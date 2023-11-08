-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION 
chessboard" to load this file. \quit


/******************************************************************************
 * Input/Output
 ******************************************************************************/

CREATE OR REPLACE FUNCTION 
chessboard_in(cstring)
  RETURNS chessboard
  AS 'MODULE_PATHNAME'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION 
chessboard_out(chessboard)
  RETURNS cstring
  AS 'MODULE_PATHNAME'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION 
chessboard_recv(internal)
  RETURNS chessboard
  AS 'MODULE_PATHNAME'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION 
chessboard_send(chessboard)
  RETURNS bytea
  AS 'MODULE_PATHNAME'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE TYPE 
chessboard (
  internallength = 100,
  input          =   chessboard_in,
  output         =   chessboard_out,
  receive        =   chessboard_recv,
  send           =   chessboard_send,
  alignment      = double
);

CREATE OR REPLACE FUNCTION 
chessboard(text)
  RETURNS chessboard
  AS 'MODULE_PATHNAME', '
  chessboard_cast_from_text'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION text(chessboard)
  RETURNS text
  AS 'MODULE_PATHNAME', '
  chessboard_cast_to_text'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE CAST (text as chessboard) WITH FUNCTION chessboard(text) AS IMPLICIT;
CREATE CAST (chessboard as text) WITH FUNCTION text(chessboard);

/******************************************************************************
 * Constructor
 ******************************************************************************/

CREATE FUNCTION 
chessboard(double precision, double precision)
  RETURNS 
  chessboard
  AS 'MODULE_PATHNAME', '
  chessboard_constructor'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

/*****************************************************************************
 * Accessing values
 *****************************************************************************/

CREATE FUNCTION re(
    chessboard)
  RETURNS double precision
  AS 'MODULE_PATHNAME', '
  chessboard_re'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION im(
    chessboard)
  RETURNS double precision
  AS 'MODULE_PATHNAME', '
  chessboard_im'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

