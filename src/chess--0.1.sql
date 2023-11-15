-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION 
chessboard" to load this file. \quit


/******************************************************************************
 * Input/Output
 ******************************************************************************/

CREATE OR REPLACE FUNCTION chessboard_in(cstring)
  RETURNS chessboard
  AS 'MODULE_PATHNAME'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION chessboard_out(chessboard)
  RETURNS cstring
  AS 'MODULE_PATHNAME'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

-- CREATE OR REPLACE FUNCTION 
-- chessboard_recv(internal)
--   RETURNS chessboard
--   AS 'MODULE_PATHNAME'
--   LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

-- CREATE OR REPLACE FUNCTION 
-- chessboard_send(chessboard)
--   RETURNS bytea
--   AS 'MODULE_PATHNAME'
--   LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE TYPE chessboard (
  INTERNALLENGTH =   1024, -- 1024 bytes, un peu overkill mais a mon avis on aura jamais plus grand haha
  INPUT          =   chessboard_in,
  OUTPUT         =   chessboard_out
);
COMMENT ON TYPE chessboard IS 'chessboard datatype for PostgreSQL';

CREATE TYPE chessgame(
  INPUT          = chessgame_in,
  OUTPUT         = chessgame_out,
  INTERNALLENGTH = 1024
);
COMMENT ON TYPE chessgame IS 'chessgame datatype for PostgreSQL';

-- pas besoin du rcv ou send car ce sont des exemples d'I/O binaire voir: https://www.postgresql.org/docs/current/xtypes.html

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

-- CREATE FUNCTION re(
--     chessboard)
--   RETURNS double precision
--   AS 'MODULE_PATHNAME', '
--   chessboard_re'
--   LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

-- CREATE FUNCTION im(
--     chessboard)
--   RETURNS double precision
--   AS 'MODULE_PATHNAME', '
--   chessboard_im'
--   LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

-- GETTERS

CREATE OR REPLACE FUNCTION getBoard(chessboard) -- spécifier le nom de la fonction et son argument (la fonction peut avoir un nom différent du fichier C)
RETURNS TEXT -- on indique son type de retour
-- la prochaine ligne permet de spécifier le chemin vers la fonction en C
-- ne pas oublier d'indiquer la fonction en C
AS '$libdir/chess', 'getBoard'
LANGUAGE C IMMUTABLE STRICT; -- on indique le langage, le fait que la fonction ne modifie pas la base de données

CREATE OR REPLACE FUNCTION getTurn(chessboard)
RETURNS TEXT
AS '$libdir/chess', 'getTurn'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION getCastling(chessboard)
RETURNS TEXT
AS '$libdir/chess', 'getCastling'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION getEnPassant(chessboard)
RETURNS TEXT
AS '$libdir/chess', 'getEnPassant'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION getHalfMoveClock(chessboard)
RETURNS INTEGER
AS '$libdir/chess', 'getHalfMoveClock'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION getFullMoveNumber(chessboard)
RETURNS INTEGER 
AS '$libdir/chess', 'getFullMoveNumber'
LANGUAGE C IMMUTABLE STRICT;


-- B-tree COMPARISON INDEX

-- TODO: create btreee comparison functions

-- GIN COMPARISON INDEX

CREATE OPERATOR = (
  LEFTARG = chessboard,
  RIGHTARG = chessboard,
  PROCEDURE = chessboard_eq,
  COMMUTATOR = =,
  NEGATOR = <>,
  RESTRICT = eqsel,
  JOIN = eqjoinsel
);
COMMENT ON OPERATOR =(chessboard, chessboard) IS 'equals?';

CREATE OPERATOR <> (
  LEFTARG = chessboard,
  RIGHTARG = chessboard,
  PROCEDURE = chessboard_ne,
  COMMUTATOR = <>,
  NEGATOR = =,
  RESTRICT = neqsel,
  JOIN = neqjoinsel
);

CREATE OPERATOR < (
  LEFTARG = chessboard,
  RIGHTARG = chessboard,
  PROCEDURE = chessboard_lt,
  COMMUTATOR = >,
  NEGATOR = >=,
  RESTRICT = scalarltsel,
  JOIN = scalarltjoinsel
);

CREATE OPERATOR <= (
  LEFTARG = chessboard,
  RIGHTARG = chessboard,
  PROCEDURE = chessboard_le,
  COMMUTATOR = >=,
  NEGATOR = >,
  RESTRICT = scalarltsel,
  JOIN = scalarltjoinsel
);

CREATE OPERATOR > (
  LEFTARG = chessboard,
  RIGHTARG = chessboard,
  PROCEDURE = chessboard_gt,
  COMMUTATOR = <,
  NEGATOR = <=,
  RESTRICT = scalargtsel,
  JOIN = scalargtjoinsel
);

CREATE OPERATOR >= (
  LEFTARG = chessboard,
  RIGHTARG = chessboard,
  PROCEDURE = chessboard_ge,
  COMMUTATOR = <=,
  NEGATOR = <,
  RESTRICT = scalargtsel,
  JOIN = scalargtjoinsel
);

CREATE OPERATOR CLASS gin_chessboard_ops
DEFAULT FOR TYPE chessboard USING gin
AS
    OPERATOR 1 < ,
    OPERATOR 2 <= ,
    OPERATOR 3 = ,
    OPERATOR 4 >= ,
    OPERATOR 5 > ;
    -- TODO : add comparison functions


