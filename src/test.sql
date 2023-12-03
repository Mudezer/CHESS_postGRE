-- load this file with psql -f test.sql
-- TODO : the 4th insert in chessgame_table is too long
-- TODO : correction of the GetFirstMoves return, not exactly matched ?

DROP EXTENSION IF EXISTS chess CASCADE;
DROP TABLE IF EXISTS favorite_game;
DROP TABLE IF EXISTS chessgame_table;
DROP TABLE IF EXISTS chessboard_table;


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


-- INSERTION -- NB : input on multiple lines does not work properly

INSERT INTO chessgame_table(p_chessgame) VALUES
    ('1.e4 e5 2.f4 exf4 3.Nf3 g5 4.Bc4 g4 5.Ne5 Qh4+ 6.Kf1 f3 7.g3 Qh3+ 8.Kf2 Qg2+ 9.Ke3 Bh6+ 10.Kd3 d5 11.Bxd5 Na6 12.c3 c6 13.Bxf7+ Ke7 14.Bb3 Nc5+ 15.Kc2 Nxe4 16.Qf1 Bf5 17.Qxg2 Nf2+ 18.d3 fxg2 19.Rg1 Rd8 20.Bxh6 Nxh6 21.Rxg2 Nxd3 22.Nxd3 Bxd3+ 23.Kc1 Rhf8 24.Nd2 Nf5 25.Bd1 Ne3 26.Rg1 Bf1 27.b3 Rf2 28.Rxf1 Nxf1 29.Nxf1 Rxd1+ 30.Kxd1 Rxf1+  0-1'),
    ('1.e4 e5 2.d4 exd4 3.Bc4 c5 4.Nf3 d6 5.c3 dxc3 6.O-O Be6 7.Bxe6 fxe6 8.Qb3 Qd7 9.Ng5 d5 10.exd5 exd5 11.Re1+ Be7 12.Ne6 Kf7 13.Qxc3 Nf6 14.h4 Re8 15.Bf4 Bd6 16.Qg3 Nh5 17.Qg4  1-0'),
    ('1.e4 e5 2.f4 exf4 3.Nf3 g5 4.h4 g4 5.Ng5 Nc6 6.Bc4 Ne5 7.Bb3 h6 8.d4 hxg5 9.dxe5 Rxh4 10.Rf1 g3 11.Nc3 Bb4 12.Qd5 Bxc3+ 13.bxc3 Qe7 14.Ba3 d6 15.exd6 Qe6 16.Qd4 Qf6 17.e5 Qf5 18.dxc7 Be6 19.Ba4+ Bd7 20.O-O-O  1-0'),
    ('1. f3 e5 2. g4 Qh4'),
    --('1. Nf3 Nf6 2. c4 g6 3. Nc3 Bg7 4. d4 O-O 5. Bf4 d5 6. Qb3 dxc4 7.Qxc4 c6 8. e4 Nbd7 9. Rd1 Nb6 10. Qc5 Bg4 11. Bg5 Na4 12. Qa3 Nxc3 13. bxc3 Nxe4 14. Bxe7 Qb6 15. Bc4 Nxc3 16. Bc5 Rfe8+ 17. Kf1 Be6 18. Bxb6 Bxc4+ 19. Kg1 Ne2+ 20. Kf1 Nxd4+ 21. Kg1 Ne2+ 22. Kf1 Nc3+ 23. Kg1 axb6 24. Qb4 Ra4 25. Qxb6 Nxd1 26. h3 Rxa2 27. Kh2 Nxf2 28. Re1 Rxe1 29. Qd8+ Bf8 30. Nxe1 Bd5 31. Nf3 Ne4 32. Qb8 b5 33. h4 h5 34. Ne5 Kg7 35. Kg1 Bc5+ 36. Kf1 Ng3+ 37. Ke1 Bb4+ 38. Kd1 Bb3+ 39. Kc1 Ne2+ 40. Kb1 Nc3+ 41. Kc1 Rc2'),
    ('1. e4 d5 2. exd5 Qxd5 3. Nc3 Qd8 4. Bc4 Nf6 5. Nf3 Bg4 6. h3 Bxf3 7. Qxf3 e6 8. Qxb7 Nbd7 9. Nb5 Rc8 10. Nxa7 Nb6 11. Nxc8 Nxc8 12. d4 Nd6 13. Bb5+ Nxb5 14. Qxb5+ Nd7 15. d5 exd5 16. Be3 Bd6 17. Rd1 Qf6 18. Rxd5 Qg6 19. Bf4 Bxf4 20. Qxd7+ Kf8 21. Qd8'),
    ('1.e4 e5 2.Nf3 Nc6 3.Bc4 Bc5 4.c3 d6 5.d4 exd4 6.cxd4 Bb6 7.d5 Ne5 8.Nxe5 dxe5 9.Nc3 Nf6 10.Bg5 O-O 11.Qf3 Qd6 12.Bxf6 Qxf6 13.Qxf6 gxf6 14.g4 Kg7 15.Ne2 Rh8 16.Rg1 Kf8 17.Rg2 Ke7 18.O-O-O h5 19.g5 f5 20.Nc3 Bc5 21.g6 Bd6 22.gxf7 Kxf7 23.f4 exf4 24.Rdg1 Kf8 25.Rg6 f3 26.exf5 Be5 27.d6 cxd6 28.Rg8+ Rxg8 29.Rxg8+ Ke7 30.Nd5+ Kd7 31.Bb5+  1-0'),
    ('1.e4 c5 2.f4 e6 3.Nf3 Nc6 4.c3 d5 5.e5 f6 6.Be2 Be7 7.Na3 Qb6 8.Nc2 Nh6 9.d4 cxd4 10.cxd4 Bd7 11.Bd3 Nb4 12.Nxb4 Bxb4+ 13.Kf2 O-O 14.Rf1 fxe5 15.fxe5 Nf5 16.Bxf5 Rxf5 17.Kg1 Rc8 18.g4 Rf7 19.a4 Rcf8 20.Be3 Be7 21.Qe2 Qb3 22.Bg5 Bb4 23.Kg2 Rxf3 24.Rxf3 Rxf3 25.Rc1 Rf8 26.Rc7 Bc6 27.Be3 Qc4 28.Qd1 Ba5 29.Re7 Bxa4  0-1'),
    ('1.d4 d5 2.c4 dxc4 3.Nc3 f5 4.e3 e6 5.Bxc4 c6 6.Nf3 Bd6 7.e4 b5 8.Bb3 a5 9.exf5 exf5 10.O-O a4 11.Bxg8 Rxg8 12.Bg5 Qc7 13.Qe2+ Kf8 14.Rfe1 Kf7 15.Rac1 Qb7 16.d5 h6 17.dxc6 Qa6 18.Nxb5 hxg5 19.Nxd6+ Kg6 20.Ne5+ Kf6 21.Qh5 g6 22.Qh7 Be6 23.Nxg6 Nxc6 24.Rxc6 Qd3 25.Qe7+ Kxg6 26.Rxe6+ Kh5 27.Qh7+ Kg4 28.Rc4+ f4 29.h3+ Qxh3 30.Qxh3+  1-0'),
    ('1.e4 e5 2.f4 exf4 3.Bc4 Qh4+ 4.Kf1 g5 5.Nc3 Bg7 6.d4 d6 7.Be2 Nc6 8.e5 Nge7 9.Nb5 O-O 10.Nxc7 Rb8 11.Nf3 Qh6 12.exd6 Nf5 13.c3 Ng3+ 14.hxg3 Qxh1+ 15.Kf2 fxg3+ 16.Kxg3 Qxd1 17.Bxd1 h6 18.b3 b5 19.Be3 f5 20.d5 f4+ 21.Kh2 fxe3 22.dxc6 g4 23.Nd4 Be5+ 24.Kg1 Bxd6 25.Ncxb5 Bc5 26.b4 Bb6 27.Nd6 Bxd4 28.cxd4 Rxb4 29.Nxc8 Rxc8 30.d5 Kf7 31.Bb3 Ke7 32.Kf1 Re4 33.Ke2 Rf8 34.Kd3 Re5 35.Re1 Kd6 36.Rxe3 Rxe3+ 37.Kxe3 h5 38.Ke4 h4 39.Bd1 h3 40.gxh3 gxh3 41.Bf3 h2 42.Bg2 Rf1  0-1'),
    ('1.d4 d5 2.c4 dxc4 3.e3 e5 4.Bxc4 exd4 5.exd4 Nf6 6.Nf3 Bd6 7.O-O O-O 8.Bg5 h6 9.Bh4 g5 10.Bg3 Bg4 11.Nc3 Nc6 12.Qd3 Kg7 13.Ne5 Bxe5 14.dxe5 Nh5 15.Nd5 Nxg3 16.Qxg3 Bh5 17.f4 Na5 18.b3 Nxc4 19.bxc4 c6 20.Nf6 Qd4+ 21.Kh1 Bg6 22.Rad1 Qxc4 23.f5 Bh7 24.Nd7 Rfd8 25.e6 f6 26.Qc7 Rdc8 27.Qxb7 Qb5 28.Rb1 Qxb7 29.Rxb7 Kh8 30.Nxf6 Bg8 31.Rd1 Rd8 32.Rdd7 Rxd7 33.Rxd7 g4 34.Kg1 a5 35.e7  1-0'),
    ('1.e4 c5 2.f4 e6 3.Nf3 d5 4.e5 Nc6 5.c3 f6 6.Na3 Nh6 7.Nc2 Qb6 8.d4 Bd7 9.Ne3 cxd4 10.cxd4 Bb4+ 11.Kf2 O-O 12.Kg3 fxe5 13.fxe5 Be8 14.Kh3 Bh5 15.g4 Bg6 16.Bg2 Be4 17.g5 Nf5 18.Nxf5 Rxf5 19.Be3 Bxf3 20.Bxf3 Nxe5 21.Bg4 Nxg4 22.Qxg4 Raf8 23.Rhg1 Bd6 24.Bc1 Rf3+ 25.Kh4 R8f4  0-1'),
    ('1.d4 d5 2.c4 dxc4 3.e3 e5 4.Bxc4 exd4 5.exd4 Nf6 6.Nc3 Be7 7.Nf3 O-O 8.h3 c6 9.Be3 Bf5 10.g4 Bg6 11.Ne5 Nbd7 12.Nxg6 hxg6 13.h4 Nb6 14.Bb3 Nfd5 15.h5 Nxe3 16.fxe3 Bh4+ 17.Kd2 gxh5 18.Qf3 Bg5 19.Raf1 Qxd4+ 20.Kc2 Qf6 21.Rxh5 Qg6+ 22.e4 Nd5 23.Rfh1 Bh6 24.g5 f5 25.Nxd5 cxd5 26.Bxd5+ Kh7 27.Rxh6+ gxh6 28.Rxh6+ Qxh6 29.gxh6  1-0'),
    ('1.e4 c5 2.f4 e6 3.Nf3 Nc6 4.c3 d5 5.e5 f6 6.Na3 Nh6 7.Nc2 Qb6 8.d4 Bd7 9.Ne3 cxd4 10.cxd4 Bb4+ 11.Kf2 O-O 12.Kg3 fxe5 13.fxe5 Rac8 14.h4 Rxf3+ 15.gxf3 Nxd4 16.Bd3 Rf8 17.f4 Bc5 18.Rf1 Bb5 19.Bxb5 Qxb5 20.Kh3 Ne2 21.Ng2 Nf5 22.Kh2 Neg3 23.Rf3 Ne4 24.Qf1 Qe8 25.b4 Bd4 26.Rb1 Qh5 27.Rbb3 Rc8 28.Be3 Rc2 29.Kg1 Nxe3 30.Rfxe3 Nd2 31.Qd3 Rc1+ 32.Kh2 Nf1+ 33.Kh3 Nxe3 34.Nxe3 Qf3+ 35.Kh2 Rh1+  0-1'),
    ('1.d4 d5 2.c4 dxc4 3.e3 e5 4.Bxc4 exd4 5.exd4 Nf6 6.Nc3 Be7 7.Nf3 O-O 8.Be3 c6 9.h3 Nbd7 10.Bb3 Nb6 11.O-O Nfd5 12.a4 a5 13.Ne5 Be6 14.Bc2 f5 15.Qe2 f4 16.Bd2 Qe8 17.Rae1 Bf7 18.Qe4 g6 19.Bxf4 Nxf4 20.Qxf4 Bc4 21.Qh6 Bxf1 22.Bxg6 hxg6 23.Nxg6 Nc8 24.Qh8+ Kf7 25.Qh7+ Kf6 26.Nf4 Bd3 27.Re6+ Kg5 28.Qh6+ Kf5 29.g4+  1-0'),
    ('1.e4 e5 2.f4 exf4 3.Bc4 Qh4+ 4.Kf1 g5 5.Nc3 Bg7 6.d4 Nc6 7.e5 Nge7 8.Nf3 Qh5 9.Ne4 h6 10.Nf6+ Bxf6 11.exf6 d5 12.Bd3 Nf5 13.Qe1+ Kd8 14.Ne5 Nfxd4 15.c3 Nxe5 16.Qxe5 Nc6 17.Qxd5+ Ke8 18.Bb5 Be6 19.Bxc6+ Kf8 20.Qc5+ Kg8 21.Bf3 Qg6 22.Qd4 c5 23.Qe5 Re8 24.Be2 f3 25.Kf2 fxe2 26.Be3 b6 27.h4 Bd7 28.Qd5 Qxf6+ 29.Kxe2 Bg4+ 30.Kd2 Rd8  0-1'),
    ('1.e4 e5 2.f4 exf4 3.Bc4 Qh4+ 4.Kf1 d6 5.d4 Bg4 6.Qd3 Nc6 7.Bxf7+ Kxf7 8.Qb3+ Kg6 9.Qxb7 Nxd4 10.Qxa8 Nf6 11.Na3 f3 12.g3 Bh3+ 13.Ke1 Qg4 14.Be3 d5 15.Qxa7 Nc6 16.Qxc7 d4 17.Bd2 Qxe4+ 18.Kd1 f2 19.Nxh3 Qf3+ 20.Kc1 Qxh1+  0-1')     
    ; -- 400 deLaBoudonnais, mais skip certaines parties de plus de 40 moves

INSERT INTO chessboard_table(p_chessboard) VALUES 
    ('3r1r1k/pq3Ppp/3p1R2/8/3Bn1Q1/Pbp5/1PP3PP/1K1R4 w - - 0 24'),
    ('rnbqkbnr/pp2pppp/3p4/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 0 3')
    ;


-- OUPUT

SELECT * FROM chessgame_table;
SELECT * FROM chessboard_table;


-- Functions to implement
 
select getFirstMoves('1. e4 e5 2. Bc4 Nf6', 3); -- 1. e4 e5 2. Bc4

select hasOpening('1. e4 c5 2. Nf3 d6 3. d4 cxd4', '1. e4 c5 2. Nf3 d6'); -- true
select hasOpening('1. e4 c5 2. Nf3 d6 3. d4 cxd4', '1. e4 c3'); -- false
select hasOpening('1. e4 c5 2. Nf3 d6 3. d4 cxd4', getFirstMoves('1. e4 e5 2. Bc4 Nf6', 1)); -- true


select hasBoard('1. e4 c5 2. Nf3 d6 3. d4 cxd4 4. Nxd4 Nf6 5. Nc3 Nc6 6. Bc4 e6 7. Be3 Be7 8. Bb3 O-O 9. Qe2 Qa5 10. O-O-O Nxd4 11. Bxd4 Bd7 12. Kb1 Bc6 13. f4 Rad8 14. Rhf1 b5 15. f5 b4 16. fxe6 bxc3 17. exf7+ Kh8 18. Rf5 Qb4 19. Qf1 Nxe4 20. a3 Qb7 21. Qf4 Ba4 22. Qg4 Bf6 23. Rxf6 Bxb3', '3r1r1k/pq3Ppp/3p1R2/8/3Bn1Q1/Pbp5/1PP3PP/1K1R4 w - - 0 24', 46); -- True
select hasBoard('1. e4 c5 2. Nf3 d6 3. d4 cxd4 4. Nxd4 Nf6 5. Nc3 Nc6 6. Bc4 e6 7. Be3 Be7 8. Bb3 O-O 9. Qe2 Qa5 10. O-O-O Nxd4 11. Bxd4 Bd7 12. Kb1 Bc6 13. f4 Rad8 14. Rhf1 b5 15. f5 b4 16. fxe6 bxc3 17. exf7+ Kh8 18. Rf5 Qb4 19. Qf1 Nxe4 20. a3 Qb7 21. Qf4 Ba4 22. Qg4 Bf6 23. Rxf6 Bxb3', '3r1r1k/pq3Ppp/3p1R2/8/3Bn1Q1/Pbp5/1PP3PP/1K1R4 w - - 0 24', 45); -- false

select getBoard('1. e4 c5 2. Nf3 d6 3. d4 cxd4 4. Nxd4 Nf6 5. Nc3 Nc6 6. Bc4 e6 7. Be3 Be7 8. Bb3 O-O 9. Qe2 Qa5 10. O-O-O Nxd4 11. Bxd4 Bd7 12. Kb1 Bc6 13. f4 Rad8 14. Rhf1 b5 15. f5 b4 16. fxe6 bxc3 17. exf7+ Kh8 18. Rf5 Qb4 19. Qf1 Nxe4 20. a3 Qb7 21. Qf4 Ba4 22. Qg4 Bf6 23. Rxf6 Bxb3', 4); -- rnbqkbnr/pp2pppp/3p4/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 0 3


-- OPERATORS

select ('1. e4 e5' = '1. e4 e5'); -- true
select ('1. e4 e5' = '1. e4 c5'); -- false

select ('1. e4 e5' != '1. e4 c5'); -- true
select ('1. e4 e5' != '1. e4 e5'); -- false

select ('1. e4 e5' <= '1. e4 e5'); -- true
select ('1. e4 e5' <= '1. e4 e5 2. Bc4 Nf6'); -- true
select ('1. e4 e5' <= '1. e4 c5 2. Bc4 Nf6'); -- false

select ('1. e4 e5'<'1. e4 e5 2. Bc4 Nf6'); -- true
select ('1. e4 e5'<'1. e4 c5 2. Bc4 Nf6'); -- false

select ('1. e4 e5' >= '1. e4 e5'); -- true
select ('1. e4 e5 2. Bc4 Nf6' >= '1. e4 e5'); -- true
select ('1. e4 c5 2. Bc4 Nf6' >= '1. e4 e5'); -- false

select ('1. e4 e5 2. Bc4 Nf6' > '1. e4 e5'); -- true
select ('1. e4 f5 2. Bc4 Nf6' > '1. e4 e5'); -- true
select ('1. e4 e5 2. Bc4 Nf6' > '1. e4 e5'); -- false


-- Queries, some from the project assignment

explain 
SELECT count(*)
FROM chessgame_table
WHERE hasOpening(p_chessgame,'e4');

explain
SELECT count(*)
FROM chessgame_table
WHERE hasBoard(p_chessgame,'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1', 3);

explain 
SELECT count(*) 
FROM chessgame_table 
WHERE p_chessgame > '1.e4';


-- !!!!! TODO !!!!!
--select getFirstMoves('1. e4 e5 2. Bc4 Nf6', 1); -- 1. e4
--select hasOpening('1. e4 c5 2. Nf3 d6 3. d4 cxd4', getFirstMoves('1. e4 e5 2. Bc4 Nf6', 1)); -- true but return false



-- CONSTRUCTORS


-- INDEXES

create index btree on chessgame_table(p_chessgame);

explain SELECT *
FROM chessgame_table
ORDER BY p_chessgame;

explain SELECT count(*)
FROM chessgame_table
WHERE hasOpening(p_chessgame,'e4');

set session enable_seqscan=false; -- To force the query to use the btree & compare execution time

explain SELECT *
FROM chessgame_table
ORDER BY p_chessgame;

explain SELECT count(*)
FROM chessgame_table
WHERE hasOpening(p_chessgame,'e4');


-- GETTERS

