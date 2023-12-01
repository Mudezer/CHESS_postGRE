-- load this file with psql -d chess -f test.sql
-- TODO : the 4th insert in chessgame_table is too long
-- TODO : correction of the GetFirstMoves return, not exactly matched ?


DROP TABLE IF EXISTS favorite_game;
DROP TABLE IF EXISTS chessgame_table;
DROP TABLE IF EXISTS chessboard_table;


-- CREATE EXTENSION

CREATE EXTENSION IF NOT EXISTS chess; -- ensures extension is created


-- CREATE TABLE

CREATE TABLE chessgame_table(
    id int,
    --id INT PRIMARY KEY NOT NULL AUTO_INCREMENT
    p_chessgame chessgame
);

CREATE TABLE chessboard_table(
    id int,
    p_chessboard chessboard
);


-- INSERTION

INSERT INTO chessgame_table(id, p_chessgame) VALUES(1,'1.e4 e5 2.f4 exf4 3.Nf3 g5 4.Bc4 g4 5.Ne5 Qh4+ 6.Kf1 f3 7.g3 Qh3+ 8.Kf2 Qg2+
                                                9.Ke3 Bh6+ 10.Kd3 d5 11.Bxd5 Na6 12.c3 c6 13.Bxf7+ Ke7 14.Bb3 Nc5+ 15.Kc2 Nxe4
                                                16.Qf1 Bf5 17.Qxg2 Nf2+ 18.d3 fxg2 19.Rg1 Rd8 20.Bxh6 Nxh6 21.Rxg2 Nxd3 22.Nxd3 Bxd3+
                                                23.Kc1 Rhf8 24.Nd2 Nf5 25.Bd1 Ne3 26.Rg1 Bf1 27.b3 Rf2 28.Rxf1 Nxf1 29.Nxf1 Rxd1+
                                                30.Kxd1 Rxf1+  0-1');
INSERT INTO chessgame_table(id, p_chessgame) VALUES(2, '1.e4 e5 2.d4 exd4 3.Bc4 c5 4.Nf3 d6 5.c3 dxc3 6.O-O Be6 7.Bxe6 fxe6 8.Qb3 Qd7
                                                9.Ng5 d5 10.exd5 exd5 11.Re1+ Be7 12.Ne6 Kf7 13.Qxc3 Nf6 14.h4 Re8 15.Bf4 Bd6
                                                16.Qg3 Nh5 17.Qg4  1-0');
INSERT INTO chessgame_table(id, p_chessgame) VALUES(3, '1.e4 e5 2.f4 exf4 3.Nf3 g5 4.h4 g4 5.Ng5 Nc6 6.Bc4 Ne5 7.Bb3 h6 8.d4 hxg5
                                                9.dxe5 Rxh4 10.Rf1 g3 11.Nc3 Bb4 12.Qd5 Bxc3+ 13.bxc3 Qe7 14.Ba3 d6 15.exd6 Qe6
                                                16.Qd4 Qf6 17.e5 Qf5 18.dxc7 Be6 19.Ba4+ Bd7 20.O-O-O  1-0');
--INSERT INTO chessgame_table(id, p_chessgame) VALUES(4, '1.e4 e5 2.d4 exd4 3.Nf3 c5 4.Bc4 Nc6 5.c3 Qf6 6.O-O d6 7.cxd4 cxd4 8.Ng5 Nh6
--                                                9.f4 Be7 10.e5 Qg6 11.exd6 Qxd6 12.Na3 O-O 13.Bd3 Bf5 14.Nc4 Qg6 15.Nf3 Bxd3
--                                                16.Nce5 Bc2 17.Nxg6 Bxd1 18.Nxe7+ Nxe7 19.Rxd1 Nhf5 20.g4 Ne3 21.Bxe3 dxe3
--                                                22.Rd7 Rfe8 23.Re1 Ng6 24.f5 Nf4 25.Rd4 Nh3+ 26.Kg2 Nf2 27.Rc4 Rad8 28.h3 h6
--                                                29.Re2 b5 30.Rd4 Rxd4 31.Nxd4 a6 32.Kf3 Nxh3 33.Rxe3 Ng5+ 34.Kf4 Rxe3 35.Kxe3 g6
--                                                36.fxg6 fxg6 37.Nc6 Ne6 38.Ke4 Kf7 39.Ke5 h5 40.gxh5 gxh5 41.Kf5 Nc7 42.b3 Ke8
--                                                43.a4 bxa4 44.bxa4 Nd5 45.Kg5 Ne7 46.Nb8 a5 47.Na6 Ng6 48.Kxh5 Nf4+ 49.Kg5 Ne6+
--                                                50.Kf5 Kd7 51.Ke5 Nd8  1/2-1/2');


INSERT INTO chessboard_table(id, p_chessboard) VALUES (
    (1, '3r1r1k/pq3Ppp/3p1R2/8/3Bn1Q1/Pbp5/1PP3PP/1K1R4 w - - 0 24'),
    (2, 'rnbqkbnr/pp2pppp/3p4/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 0 3')
);


-- OUPUT
SELECT * FROM chessgame_table;
SELECT * FROM chessboard_table;


-- Functions to implement
select getFirstMoves('1. e4 e5 2. Bc4 Nf6', 3); -- 1. e4 e5 2. Bc4

select hasOpening('1. e4 c5 2. Nf3 d6 3. d4 cxd4', '1. e4 c5 2. Nf3 d6'); -- true
select hasOpening('1. e4 c5 2. Nf3 d6 3. d4 cxd4', '1. e4 c3'); -- false

select hasBoard('1. e4 c5 2. Nf3 d6 3. d4 cxd4 4. Nxd4 Nf6 5. Nc3 Nc6 6. Bc4 e6 7. Be3 Be7 8. Bb3 O-O 9. Qe2 Qa5 10. O-O-O Nxd4 11. Bxd4 Bd7 12. Kb1 Bc6 13. f4 Rad8 14. Rhf1 b5 15. f5 b4 16. fxe6 bxc3 17. exf7+ Kh8 18. Rf5 Qb4 19. Qf1 Nxe4 20. a3 Qb7 21. Qf4 Ba4 22. Qg4 Bf6 23. Rxf6 Bxb3', '3r1r1k/pq3Ppp/3p1R2/8/3Bn1Q1/Pbp5/1PP3PP/1K1R4 w - - 0 24', 46); -- True
select hasBoard('1. e4 c5 2. Nf3 d6 3. d4 cxd4 4. Nxd4 Nf6 5. Nc3 Nc6 6. Bc4 e6 7. Be3 Be7 8. Bb3 O-O 9. Qe2 Qa5 10. O-O-O Nxd4 11. Bxd4 Bd7 12. Kb1 Bc6 13. f4 Rad8 14. Rhf1 b5 15. f5 b4 16. fxe6 bxc3 17. exf7+ Kh8 18. Rf5 Qb4 19. Qf1 Nxe4 20. a3 Qb7 21. Qf4 Ba4 22. Qg4 Bf6 23. Rxf6 Bxb3', '3r1r1k/pq3Ppp/3p1R2/8/3Bn1Q1/Pbp5/1PP3PP/1K1R4 w - - 0 24', 45); -- false

select getBoard('1. e4 c5 2. Nf3 d6 3. d4 cxd4 4. Nxd4 Nf6 5. Nc3 Nc6 6. Bc4 e6 7. Be3 Be7 8. Bb3 O-O 9. Qe2 Qa5 10. O-O-O Nxd4 11. Bxd4 Bd7 12. Kb1 Bc6 13. f4 Rad8 14. Rhf1 b5 15. f5 b4 16. fxe6 bxc3 17. exf7+ Kh8 18. Rf5 Qb4 19. Qf1 Nxe4 20. a3 Qb7 21. Qf4 Ba4 22. Qg4 Bf6 23. Rxf6 Bxb3', 4); -- rnbqkbnr/pp2pppp/3p4/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 0 3


-- OPERATORS


-- CONSTRUCTORS


-- INDEXES


-- GETTERS

