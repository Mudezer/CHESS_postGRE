DROP TABLE IF EXISTS favorite_game;
DROP TABLE IF EXISTS movement;
DROP TABLE IF EXISTS game;
DROP TABLE IF EXISTS chessboard;

-- CREATE EXTENSION  
CREATE EXTENSION IF NOT EXISTS chess; -- ensures extension is created

-- CREATE TABLE

CREATE TABLE game(
    game_id int PRIMARY KEY,
    white_player VARCHAR(255),
    black_player VARCHAR(255),
    result VARCHAR(50),
    ECO VARCHAR(20)
);

CREATE TABLE movement(
    move_id int PRIMARY KEY,
    game_id int REFERENCES game(game_id),
    movement_SAN VARCHAR(255), -- ici on changera par SAN qu'on créera dans l'extension même! 
    chessboard_id int REFERENCES chessboard(chessboard_id)
);

CREATE TABLE chessboard(
    chessboard_id int PRIMARY KEY,
    chessboard_fen TEXT, -- ici on changera TEXT par FEN quand on créera le datatype dans l'extension même!
    halfmove_clock int, 
    turn VARCHAR(20),
    fullmove int, 
    en_passant VARCHAR(20),
    roque VARCHAR(10),
);

CREATE TABLE favorite_game (
    fav_game_id serial PRIMARY KEY,
    game_id int REFERENCES game(game_id)
);


-- INSERTION

INSERT INTO game(game_id,white_player,black_player,result,ECO) 
    VALUES(1, "De la Bourdonnais, Louis", "Pelling", "1-0","C21");
INSERT INTO chessboard(chessboard_id, chessboard_fen, halfmove_clock, turn, fullmove, en_passant, roque) 
    VALUES(1,"nbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",0,"White",0,"-","KQkq");
INSERT INTO movement(move_id,game_id,movement_SAN,chessboard_id)
    VALUES(0,1,"0.",1);
INSERT INTO chessboard(chessboard_id, chessboard_fen, halfmove_clock, turn, fullmove, en_passant, roque)
    VALUES(1,"rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq e6 0 2",2,"White",1,"e6","KQkq");
INSERT INTO movement(move_id,game_id,movement_SAN,chessboard_id)
    VALUES(1,1,"1.e4 e5 ",);

-- petit soucis niveau encodage du chessboard_id 
-- -> on aura un id pour chacune des nouvelles games du coup ça va overwrite pas opti du tou