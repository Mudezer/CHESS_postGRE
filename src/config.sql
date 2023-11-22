DROP TABLE IF EXISTS favorite_game;
DROP TABLE IF EXISTS movement;
DROP TABLE IF EXISTS game;
DROP TABLE IF EXISTS chessboard;


-- CREATE EXTENSION
CREATE EXTENSION IF NOT EXISTS chess; -- ensures extension is created

-- CREATE TABLE

CREATE TABLE game_table(
    game_id int PRIMARY KEY,
    white_player VARCHAR(255),
    black_player VARCHAR(255),
    result VARCHAR(50),
    ECO VARCHAR(20)
);


CREATE TABLE chessgame_table( -- movement_table 
    move_id int PRIMARY KEY,
    game_id int REFERENCES game(game_id),
    movement_SAN VARCHAR(255), -- on pourrait par exemple changer par un type chessgame_san (qui est en réalité le struct C)
    chessboard_id int REFERENCES chessboard(chessboard_id)
);

CREATE TABLE chessboard_table(
    chessboard_id int PRIMARY KEY,
    p_board chessboard -- ici on utilise le type chessboard qui est en réalité le struct C chessboard
);

CREATE TABLE favorite_game_table(
    fav_game_id serial PRIMARY KEY,
    game_id int REFERENCES game(game_id)
);


-- INSERTION

INSERT INTO game_table(game_id,white_player,black_player,result,ECO) VALUES(1, "De la Bourdonnais, Louis", "Pelling", "1-0","C21");
INSERT INTO chessboard_table(chessboard_id, p_board) VALUES(0,'nbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1');
INSERT INTO movement_table(move_id ,game_id ,movement_SAN ,chessboard_id) VALUES(0,1,'0.',0);
INSERT INTO chessboard_table(chessboard_id, p_board) VALUES(1,'rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq e6 0 2');
INSERT INTO movement_table(move_id ,game_id ,movement_SAN ,chessboard_id) VALUES(1,1,'1.e4 e5 ',1);
INSERT INTO chessboard_table(chessboard_id, p_board) VALUES(2, 'rnbqkbnr/pppp1ppp/8/8/3pP3/8/PPP2PPP/RNBQKBNR w KQkq - 0 3');
INSERT INTO movement_table(move_id ,game_id ,movement_SAN ,chessboard_id) VALUES(2,1,'2.d4 exd4 ',2);
INSERT INTO chessboard_table(chessboard_id, p_board) VALUES(3,'rnbqkbnr/pp1p1ppp/8/2p5/2BpP3/8/PPP2PPP/RNBQK1NR w KQkq c6 0 4');
INSERT INTO movement_table(move_id ,game_id ,movement_SAN ,chessboard_id) VALUES(3,1,'3.Bc4 c5 ',3);
INSERT INTO chessboard_table(chessboard_id, p_board) VALUES(4,'rnbqkbnr/pp3ppp/3p4/2p5/2BpP3/5N2/PPP2PPP/RNBQK2R w KQkq - 0 5');
INSERT INTO movement_table(move_id ,game_id ,movement_SAN ,chessboard_id) VALUES(4,1,'4.Nf3 d6 ',4);
INSERT INTO chessboard_table(chessboard_id, p_board) VALUES(5,'rnbqkbnr/pp3ppp/3p4/2p5/2B1P3/2p2N2/PP3PPP/RNBQK2R w KQkq - 0 6');
INSERT INTO movement_table(move_id ,game_id ,movement_SAN ,chessboard_id) VALUES(5,1,'5.c3 dxc3 ',5);
INSERT INTO chessboard_table(chessboard_id, p_board) VALUES(6,'rn1qkbnr/pp3ppp/3pb3/2p5/2B1P3/2p2N2/PP3PPP/RNBQ1RK1 w kq - 2 7');
INSERT INTO movement_table(move_id ,game_id ,movement_SAN ,chessboard_id) VALUES(6,1,'6.O-O Be6 ',6);
INSERT INTO chessboard_table(chessboard_id, p_board) VALUES(7,'rn1qkbnr/pp4pp/3pp3/2p5/4P3/2p2N2/PP3PPP/RNBQ1RK1 w kq - 0 8');
INSERT INTO movement_table(move_id ,game_id ,movement_SAN ,chessboard_id) VALUES(7,1,'7.Bxe6 fxe6 ',7);
INSERT INTO chessboard_table(chessboard_id, p_board) VALUES(8,'rn2kbnr/pp1q2pp/3pp3/2p5/4P3/1Qp2N2/PP3PPP/RNB2RK1 w kq - 2 9');
INSERT INTO movement_table(move_id ,game_id ,movement_SAN ,chessboard_id) VALUES(8,1,'8.Qb3 Qd7 ',8);
INSERT INTO chessboard_table(chessboard_id, p_board) VALUES(9,'rn2kbnr/pp1q2pp/4p3/2pp2N1/4P3/1Qp5/PP3PPP/RNB2RK1 w kq - 0 10');
INSERT INTO movement_table(move_id ,game_id ,movement_SAN ,chessboard_id) VALUES(9,1,'9.Ng5 d5 ',9);



-- OPERATORS

-- CONSTRUCTORS

-- INDEXES

-- GETTERS

