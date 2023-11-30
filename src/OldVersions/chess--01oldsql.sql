
/*****************************************************************************
 * Accessing values
 *****************************************************************************/

-- GETTERS

CREATE OR REPLACE FUNCTION getBoard(chessboard) -- spécifier le nom de la fonction et son argument (la fonction peut avoir un nom différent du fichier C)
RETURNS TEXT -- on indique son type de retour
-- la prochaine ligne permet de spécifier le chemin vers la fonction en C
-- ne pas oublier d'indiquer la fonction en C
AS '$libdir/chess', 'getBoard'
LANGUAGE C IMMUTABLE STRICT; -- on indique le langage, le fait que la fonction ne modifie pas la base de données

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
DEFAULT FOR TYPE chessboard USING btree
AS
    OPERATOR 1 < ,
    OPERATOR 2 <= ,
    OPERATOR 3 = ,
    OPERATOR 4 >= ,
    OPERATOR 5 > ;
    -- TODO : add comparison functions


