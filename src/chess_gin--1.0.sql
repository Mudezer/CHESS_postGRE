-- Comparison

CREATE OR REPLACE FUNCTION hasBoard(chessgame, chessboard, int64)
RETURNS BOOLEAN
AS '$libdir/chess', 'hasBoard'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OPERATOR CLASS gitn_chess_ops
DEFAULT FOR TYPE chessboard USING GIN 
AS
    OPERATOR 1 <,
    OPERATOR 2 <=,
    OPERATOR 3 =,
    OPERATOR 4 >=,
    OPERATOR 5 >,
    FUNCTION 1 hasBoard(chessgame, chessboard, int64);
    