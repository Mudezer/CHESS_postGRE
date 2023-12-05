-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION chess" to load this file. \quit


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


CREATE OR REPLACE FUNCTION chessgame_in(cstring)
  RETURNS chessgame
  AS 'MODULE_PATHNAME'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION chessgame_out(chessgame)
  RETURNS cstring
  AS 'MODULE_PATHNAME'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;


CREATE TYPE chessboard (
  internallength =   100, -- variable length :^)
  input          =   chessboard_in,
  output         =   chessboard_out
);
COMMENT ON TYPE chessboard IS 'chessboard datatype for PostgreSQL';


CREATE TYPE chessgame(
  internallength = 256, -- variable length
  input          = chessgame_in,
  output         = chessgame_out
);
COMMENT ON TYPE chessgame IS 'chessgame datatype for PostgreSQL';

-- pas besoin du rcv ou send car ce sont des exemples d'I/O binaire voir: https://www.postgresql.org/docs/current/xtypes.html

/******************************************************************************
 * Constructors
 ******************************************************************************/

 


/******************************************************************************
 * Accessing values --- TODO return text
 ******************************************************************************/

CREATE FUNCTION getBoard(chessgame, int)
  RETURNS chessboard
  AS 'MODULE_PATHNAME', 'getBoard2'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION getFirstMoves(chessgame, int)
  RETURNS chessgame
  AS 'MODULE_PATHNAME', 'getFirstMoves2'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;


/*
*
*/

-- CREATE OR REPLACE FUNCTION get_first_moves(chessgame, INT)
-- RETURNS chessgame
-- AS '$libdir/chess', 'get_first_moves'
-- LANGUAGE C IMMUTABLE STRICT;



-- CREATE FUNCTION len(chessgame) -- mourir??
-- RETURNS INT 
-- -- AS 'SELECT array_length(string_to_array($1, '' ''), 1)' -- on aurait pu flex mais à mon avis ça fonctionne pas
-- AS '$libdir/chess', 'len'
-- LANGUAGE C IMMUTABLE STRICT;

-- CREATE FUNCTION has_opening(game1 chessgame, game2 chessgame)
-- RETURNS BOOLEAN
-- AS 'SELECT get_first_moves(game1, 1) = get_first_moves(game2, 1)'
-- LANGUAGE SQL IMMUTABLE STRICT;

-- CREATE OR REPLACE FUNCTION has_opening(game1 chessgame, game2 chessgame)
-- RETURNS BOOLEAN
-- AS 'SELECT get_first_moves(game1, len(game2)) = game2'
-- LANGUAGE SQL IMMUTABLE STRICT;

-- CREATE OR REPLACE FUNCTION hasBoard(game chessgame, board chessboard, value int4)
-- RETURNS BOOLEAN
-- AS 'SELECT getBoard(game, value) = board'
-- LANGUAGE SQL IMMUTABLE STRICT;
-- CREATE OR REPLACE FUNCTION hasBoard(game chessgame, board chessboard, value int4)
-- RETURNS BOOLEAN
-- AS 'SELECT getBoard(game, value) = board'
-- LANGUAGE SQL IMMUTABLE STRICT;

/******************************************************************************
 * Other functions to implement --- TODO add cstring for hasOpening
 ******************************************************************************/

CREATE FUNCTION hasOpening(chessgame, chessgame)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'hasOpening2'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;




CREATE FUNCTION hasBoard(chessgame, chessboard, int)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'hasBoard2'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;




/******************************************************************************
 * Operators & functions associated to it
 ******************************************************************************/

CREATE OR REPLACE FUNCTION chessgame_eq(chessgame, chessgame)
RETURNS BOOLEAN
AS 'MODULE_PATHNAME', 'chessgame_eq'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION chessgame_ne(chessgame, chessgame)
RETURNS BOOLEAN
AS 'MODULE_PATHNAME', 'chessgame_ne'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION chessgame_lt(chessgame, chessgame)
RETURNS BOOLEAN
AS 'MODULE_PATHNAME', 'chessgame_lt'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION chessgame_le(chessgame, chessgame)
RETURNS BOOLEAN
AS 'MODULE_PATHNAME', 'chessgame_le'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION chessgame_gt(chessgame, chessgame)
RETURNS BOOLEAN
AS 'MODULE_PATHNAME', 'chessgame_gt'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION chessgame_ge(chessgame, chessgame)
RETURNS BOOLEAN
AS 'MODULE_PATHNAME', 'chessgame_ge'
LANGUAGE C IMMUTABLE STRICT;


CREATE OPERATOR = (
  LEFTARG = chessgame,
  RIGHTARG = chessgame,
  PROCEDURE = chessgame_eq,
  COMMUTATOR = =,
  NEGATOR = <>,
  RESTRICT = eqsel,
  JOIN = eqjoinsel
);
COMMENT ON OPERATOR =(chessgame, chessgame) IS 'equals?';

CREATE OPERATOR <= (
  LEFTARG = chessgame,
  RIGHTARG = chessgame,
  PROCEDURE = chessgame_le,
  COMMUTATOR = >=,
  NEGATOR = >,
  RESTRICT = scalarltsel,
  JOIN = scalarltjoinsel
);

CREATE OPERATOR < (
  LEFTARG = chessgame,
  RIGHTARG = chessgame,
  PROCEDURE = chessgame_lt,
  COMMUTATOR = >,
  NEGATOR = >=,
  RESTRICT = scalarltsel,
  JOIN = scalarltjoinsel
);

CREATE OPERATOR >= (
  LEFTARG = chessgame,
  RIGHTARG = chessgame,
  PROCEDURE = chessgame_ge,
  COMMUTATOR = <=,
  NEGATOR = <,
  RESTRICT = scalargtsel,
  JOIN = scalargtjoinsel
);

CREATE OPERATOR > (
  LEFTARG = chessgame,
  RIGHTARG = chessgame,
  PROCEDURE = chessgame_gt,
  COMMUTATOR = <,
  NEGATOR = <=,
  RESTRICT = scalargtsel,
  JOIN = scalargtjoinsel
);


/******************************************************************************
 * B-tree support function
 ******************************************************************************/

CREATE OR REPLACE FUNCTION chessgame_cmp(chessgame, chessgame)
RETURNS INTEGER
AS 'MODULE_PATHNAME', 'chessgame_cmp'
LANGUAGE C IMMUTABLE STRICT;


/******************************************************************************
 * B-tree operator class
 ******************************************************************************/

CREATE OPERATOR CLASS btree_chessgame_ops
DEFAULT FOR TYPE chessgame USING btree
AS
    OPERATOR 1 < ,
    OPERATOR 2 <= ,
    OPERATOR 3 = ,
    OPERATOR 4 >= ,
    OPERATOR 5 > ,
    FUNCTION 1 chessgame_cmp(chessgame, chessgame);
    
/******************************************************************************
 * GIN support function
 ******************************************************************************/

-- CREATE OR REPLACE FUNCTION chessboard_overlap(chessboard, chessboard)
-- RETURNS BOOLEAN
-- AS 'MODULE_PATHNAME', 'chessboard_overlap'
-- LANGUAGE C IMMUTABLE STRICT;

-- CREATE OR REPLACE FUNCTION chessboard_contains(chessgame, chessboard)
-- RETURNS BOOLEAN
-- AS 'MODULE_PATHNAME', 'chessboard_contains'
-- LANGUAGE C IMMUTABLE STRICT;

-- CREATE OR REPLACE FUNCTION chessboard_contained(chessgame, chessboard)
-- RETURNS BOOLEAN
-- AS 'MODULE_PATHNAME', 'chessboard_contained'
-- LANGUAGE C IMMUTABLE STRICT;

-- CREATE OR REPLACE FUNCTION chessboard_eq(chessgame, chessboard)
-- RETURNS BOOLEAN
-- AS 'MODULE_PATHNAME', 'chessboard_eq'
-- LANGUAGE C IMMUTABLE STRICT;

-- CREATE OPERATOR (
--   LEFTARG = chessboard,
--   RIGHTARG = chessboard,
--   PROCEDURE = chessboard_overlap,
--   COMMUTATOR = @@
-- );

-- CREATE OPERATOR @>(
--   LEFTARG = chessgame,
--   RIGHTARG = chessboard,
--   PROCEDURE = chessboard_contains
-- );

-- CREATE OPERATOR <@(
--   LEFTARG = chessboard,
--   RIGHTARG = chessgame,
--   PROCEDURE = chessboard_contained
-- );

-- CREATE OPERATOR =(
--   LEFTARG = chessgame,
--   RIGHTARG = chessboard,
--   PROCEDURE = chessboard_eq
-- );

-- CREATE OR REPLACE FUNCTION chessboard_cmp(chessgame, chessboard)
-- RETURNS BOOLEAN
-- AS 'MODULE_PATHNAME', 'chessboard_cmp'
-- LANGUAGE C IMMUTABLE STRICT;

-- CREATE OPERATOR CLASS gin_chessgboard_ops
-- DEFAULT FOR TYPE chessgame USING gin
-- AS
--     OPERATOR 1 &&, -- overlapping elements
--     OPERATOR 2 @>, -- contains
--     OPERATOR 3 <@, -- is contained by
--     OPERATOR 4 =, -- equals
--     FUNCTION 1 chessboard_cmp(chessgame, chessboard) -- support function to compare this shit
--     FUNCTION 2 extractValue



