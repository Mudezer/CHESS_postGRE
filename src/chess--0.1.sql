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
  internallength =   100,
  input          =   chessboard_in,
  output         =   chessboard_out
);
COMMENT ON TYPE chessboard IS 'chessboard datatype for PostgreSQL';


CREATE TYPE chessgame(
  internallength = 1024, -- peut etre 512
  input          = chessgame_in,
  output         = chessgame_out
);
COMMENT ON TYPE chessgame IS 'chessgame datatype for PostgreSQL';

/******************************************************************************
 * GETTERS
 ******************************************************************************/

CREATE FUNCTION getBoard(chessgame, INTEGER)
  RETURNS chessboard
  AS 'MODULE_PATHNAME', 'getBoard2'
  LANGUAGE C IMMUTABLE;

CREATE FUNCTION getFirstMoves(chessgame, INTEGER)
  RETURNS chessgame
  AS 'MODULE_PATHNAME', 'getFirstMoves2'
  LANGUAGE C IMMUTABLE;

/******************************************************************************
 * Predicates
 ******************************************************************************/

CREATE OR REPLACE FUNCTION hasOpening(chessgame, chessgame)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'hasOpening2'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION hasBoard(chessgame, chessboard, int)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'hasBoard2'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

/******************************************************************************
 * B-Tree Operators
 ******************************************************************************/

CREATE OR REPLACE FUNCTION chessgame_eq(chessgame, chessgame)
RETURNS BOOLEAN
AS 'MODULE_PATHNAME', 'chessgame_eq'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION chessgame_le(chessgame, chessgame)
RETURNS BOOLEAN
AS 'MODULE_PATHNAME', 'chessgame_le'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION chessgame_lt(chessgame, chessgame)
RETURNS BOOLEAN
AS 'MODULE_PATHNAME', 'chessgame_lt'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION chessgame_ge(chessgame, chessgame)
RETURNS BOOLEAN
AS 'MODULE_PATHNAME', 'chessgame_ge'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION chessgame_gt(chessgame, chessgame)
RETURNS BOOLEAN
AS 'MODULE_PATHNAME', 'chessgame_gt'
LANGUAGE C IMMUTABLE STRICT;


CREATE OPERATOR = (
  LEFTARG = chessgame,
  RIGHTARG = chessgame,
  PROCEDURE = chessgame_eq,
  COMMUTATOR = =,
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
* GIN Operators
******************************************************************************/

CREATE OR REPLACE FUNCTION chessboard_overlap(chessgame, chessgame)
RETURNS BOOLEAN
AS 'MODULE_PATHNAME', 'chessboard_overlap'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION chessboard_contains(chessgame, chessboard)
RETURNS BOOLEAN
AS 'MODULE_PATHNAME', 'chessboard_contains'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION chessboard_contained(chessboard, chessgame)
RETURNS BOOLEAN
AS 'MODULE_PATHNAME', 'chessboard_contained'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION chessboard_eq(chessgame, chessboard)
RETURNS BOOLEAN
AS 'MODULE_PATHNAME', 'chessboard_eq'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR &&(
  LEFTARG = chessgame,
  RIGHTARG = chessgame,
  PROCEDURE = chessboard_overlap,
  COMMUTATOR = &&
);

CREATE OPERATOR @>(
  LEFTARG = chessgame,
  RIGHTARG = chessboard,
  PROCEDURE = chessboard_contains,
  COMMUTATOR = '<@'
  -- RESTRICT = contsel,
	-- JOIN = contjoinsel
);

CREATE OPERATOR <@(
  LEFTARG = chessboard,
  RIGHTARG = chessgame,
  PROCEDURE = chessboard_contained,
  COMMUTATOR = '@>'
  -- RESTRICT = contsel,
	-- JOIN = contjoinsel
);

CREATE OPERATOR =(
  LEFTARG = chessgame,
  RIGHTARG = chessboard,
  PROCEDURE = chessboard_eq,
  COMMUTATOR = =
);

/******************************************************************************
 * GIN Support Functions
 ******************************************************************************/

CREATE OR REPLACE FUNCTION chessboard_cmp(chessboard, chessboard)
RETURNS INTEGER
AS 'MODULE_PATHNAME', 'chessboard_cmp'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION chessboard_extractValue(chessgame, internal)
RETURNS internal
AS 'MODULE_PATHNAME', 'chessboard_extractValue'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION chessboard_extractQuery(chessboard, internal, int2, internal, internal)
RETURNS internal
AS 'MODULE_PATHNAME', 'chessboard_extractQuery'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION chessboard_consistent(internal, int2, chessboard, int4, internal, internal)
RETURNS BOOLEAN
AS 'MODULE_PATHNAME', 'chessboard_consistent'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR CLASS gin_chessboard_ops  
DEFAULT FOR TYPE chessgame USING gin
AS
    OPERATOR 1 && (chessgame, chessgame), -- overlapping elements
    OPERATOR 2 @> (chessgame, chessboard), -- contains
    OPERATOR 3 <@ (chessboard, chessgame), -- is contained by, will never be used
    OPERATOR 4 = (chessgame, chessboard), -- equals
    FUNCTION 1 chessboard_cmp(chessboard, chessboard), -- support function
    FUNCTION 2 chessboard_extractValue(chessgame, internal),
    FUNCTION 3 chessboard_extractQuery(chessboard, internal, int2, internal, internal),
    FUNCTION 4 chessboard_consistent(internal, int2, chessboard, int4, internal, internal),
    STORAGE chessboard;


