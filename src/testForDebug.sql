-- load this file with psql -d chess -f testForDebug.sql
-- TODO : the 4th insert in chessgame_table is too long
-- TODO : correction of the GetFirstMoves return, not exactly matched ?

DROP EXTENSION IF EXISTS chess CASCADE;
DROP TABLE IF EXISTS favorite_game;
DROP TABLE IF EXISTS chessgame_table;
DROP TABLE IF EXISTS chessboard_table;

set enable_seqscan=true;

-- CREATE EXTENSION

CREATE EXTENSION IF NOT EXISTS chess; -- ensures extension is created


-- CREATE TABLE

CREATE TABLE chessgame_table(
    id SERIAL PRIMARY KEY NOT NULL,
    p_chessgame chessgame
);

CREATE TABLE chessboard_table(
    id SERIAL PRIMARY KEY NOT NULL,
    p_chessboard chessboard
);


-- INSERTION

INSERT INTO chessgame_table(p_chessgame) VALUES
    ('1.e4 e5 2.f4 exf4 '),
    ('1.g1 g5 2.d4 exd4 '),
    ('1.f3 e5 2.d4 exd4 '),
    ('1.f5 e5 2.d4 exd4 '),
    ('1.f1 e5 2.d4 exd4 '),
    ('1.e3 e5 2.d4 exd4 '),
    ('1.a3 e5 2.d4 exd4 '),
    ('1.b3 e5 2.d4 exd4 '),
    ('1.a2 e5 2.d4 exd4 '),
    ('1.e4 e5 2.d4 exd4 ')
    ;

INSERT INTO chessboard_table(p_chessboard) VALUES 
    ('3r1r1k/pq3Ppp/3p1R2/8/3Bn1Q1/Pbp5/1PP3PP/1K1R4 w - - 0 24'),
    ('rnbqkbnr/pp2pppp/3p4/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 0 3')
    ;


-- OUPUT

--SELECT * FROM chessgame_table;
--SELECT * FROM chessboard_table;

--create index btree on chessgame_table(p_chessgame);

explain SELECT count(*) FROM chessgame_table WHERE p_chessgame > '1.e4';

--explain select hasBoard('1. e4 c5 2. Nf3 d6 3. d4 cxd4 4. Nxd4 Nf6 5. Nc3 Nc6 6. Bc4 e6 7. Be3 Be7 8. Bb3 O-O 9. Qe2 Qa5 10. O-O-O Nxd4 11. Bxd4 Bd7 12. Kb1 Bc6 13. f4 Rad8 14. Rhf1 b5 15. f5 b4 16. fxe6 bxc3 17. exf7+ Kh8 18. Rf5 Qb4 19. Qf1 Nxe4 20. a3 Qb7 21. Qf4 Ba4 22. Qg4 Bf6 23. Rxf6 Bxb3', '3r1r1k/pq3Ppp/3p1R2/8/3Bn1Q1/Pbp5/1PP3PP/1K1R4 w - - 0 24', 46); -- True

----explain analyse select * from chessgame_table;

--explain analyse SELECT p_chessgame 
---FROM chessgame_table
--WHERE hasOpening(p_chessgame,'e4');
--where hasBoard(p_chessgame,'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1', 3);

--select getBoard('1.e4 e5 2.f4 exf4 ', 1);
--select getFirstMoves('1.e4 e5 2.f4 exf4 ', 1);
--explain select hasBoard('1.e4 e5 2.f4 exf4 ', 'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1', 1);

--explain select ('1. e4 e5' = '1. e4 e5'); -- true

