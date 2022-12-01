#include <iostream>
#include <math.h>
#include <algorithm>

#include "header.hpp"

/*
TODO:
	- drag and drop pieces
	- side pawn capture
	- promote pawns
	- improve sprites loading
	- block moves when king is being attacked
	- end the game when checkmate
*/

int game_step = 0;

int board[N_CELLS][N_CELLS] = {
	{ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK},
	{PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN},
	{0, 	0, 		0, 		0, 		0, 		0, 		0, 		0},
	{0, 	0, 		0, 		0,		0, 		0, 		0, 		0},
	{0, 	0, 		0, 		0, 		0, 		0, 		0, 		0},
	{0, 	0, 		0, 		0, 		0, 		0, 		0, 		0},
	{PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN},
	{ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK}
};

void Player::print_pieces(){
	for(auto i: pieces){
		std::cout << i.r << " ," << i.c << "\n";

	}
}


void Player::update_piece_pos(int i, int r, int c){
	pieces[i].r = r;
	pieces[i].c = c;
	pieces[i].has_moved = 1;

	initialize_pieces();
}

void Player::remove_piece(int r, int c){
	for (int i = 0;i<16;i++)
	{
		if (pieces[i].r == r
			&& pieces[i].c == c)
		{
			pieces[i].type = -1;
			pieces[i].r = -1;
			pieces[i].c = -1;
		}
	}

	initialize_pieces();
}

void Player::initialize_pieces()
{
	int i{};
	int row = team != -1 ? 0 : N_CELLS - 1;
	
	//pawns
	for (i = 0;i<N_CELLS;i++)
	{
		if (it == 0){
			pieces[i].r = row + team;
			pieces[i].c = i;
			pieces[i].type = board[row+team][i];  // type is only loaded at first, then it doesnt change until captured
			
		}
		if (i == N_CELLS - 1)
			it = 1;

		pieces[i].sprite.setPosition(BOARD_START_X + CELL_SIZE*pieces[i].c + OFFSET, BOARD_START_Y + pieces[i].r * CELL_SIZE + OFFSET);
	}

	
	//rooks
	if (game_step == 0){
		pieces[i].r = row;
		pieces[i].c = 0;
		pieces[i].type = board[pieces[i].r][pieces[i].c];  // same, only set type in the first step, then it doesnt change until capture
	}
	pieces[i].sprite.setPosition(BOARD_START_X + CELL_SIZE * (pieces[i].c) + OFFSET, BOARD_START_Y + pieces[i].r * CELL_SIZE + OFFSET);
	i++;
	
	if (game_step == 0){
		pieces[i].r = row;
		pieces[i].c = N_CELLS - 1;
		pieces[i].type = board[pieces[i].r][pieces[i].c];
	}
	pieces[i].sprite.setPosition(BOARD_START_X + CELL_SIZE*(pieces[i].c) + OFFSET, BOARD_START_Y + pieces[i].r * CELL_SIZE + OFFSET);
	i++;
	
	//knights
	if (game_step == 0){
		pieces[i].r = row;
		pieces[i].c = 1;
		pieces[i].type = board[pieces[i].r][pieces[i].c];
	}
	pieces[i].sprite.setPosition(BOARD_START_X + CELL_SIZE*(pieces[i].c) + OFFSET, BOARD_START_Y + pieces[i].r * CELL_SIZE + OFFSET);
	i++;
	
	if (game_step == 0){
		pieces[i].r = row;
		pieces[i].c = N_CELLS - 2;
		pieces[i].type = board[pieces[i].r][pieces[i].c];
	}
	pieces[i].sprite.setPosition(BOARD_START_X + CELL_SIZE*(pieces[i].c) + OFFSET, BOARD_START_Y + pieces[i].r * CELL_SIZE + OFFSET);
	i++;
	
	//bishops
	if (game_step == 0){
		pieces[i].r = row;
		pieces[i].c = 2;
		pieces[i].type = board[pieces[i].r][pieces[i].c];
	}
	pieces[i].sprite.setPosition(BOARD_START_X + CELL_SIZE*(pieces[i].c) + OFFSET, BOARD_START_Y + pieces[i].r * CELL_SIZE + OFFSET);
	i++;

	if (game_step == 0){	
		pieces[i].r = row;
		pieces[i].c = N_CELLS - 3;
		pieces[i].type = board[pieces[i].r][pieces[i].c];
	}
	pieces[i].sprite.setPosition(BOARD_START_X + CELL_SIZE*(pieces[i].c) + OFFSET, BOARD_START_Y + pieces[i].r * CELL_SIZE + OFFSET);
	i++;
	
	//queen
	if (game_step == 0){
		pieces[i].r = row;
		pieces[i].c = 3;
		pieces[i].type = board[pieces[i].r][pieces[i].c];
	}
	pieces[i].sprite.setPosition(BOARD_START_X + CELL_SIZE*(pieces[i].c) + OFFSET, BOARD_START_Y + pieces[i].r * CELL_SIZE + OFFSET);
	i++;
	
	//king
	if (game_step == 0){
		pieces[i].r = row;
		pieces[i].c = 4;
		pieces[i].type = board[pieces[i].r][pieces[i].c];
	}
	pieces[i].sprite.setPosition(BOARD_START_X + CELL_SIZE*(pieces[i].c) + OFFSET, BOARD_START_Y + pieces[i].r * CELL_SIZE + OFFSET);

}


// if the piece belongs to this player returns index of piece in pieces array, if it doesnt belong return -1
int Player::get_piece_idx(int r_i, int c_i){
	for (int i = 0;i<16;i++)
	{
		if (pieces[i].r == r_i
			&& pieces[i].c == c_i)
		{
			return i;
		}
	}
	return -1;
}

// check if place (r, c) is being attacked by any piece of player Player
bool Player::is_attacked(int r, int c){
	std::vector<sf::Vector2i> avail_moves{};
	for (int i = 0;i<16;i++)
	{
		avail_moves = get_moves(pieces[i].type, pieces[i].r, pieces[i].c, get_team());

		// if the piece if a pawn we need to see if it is blocking castle; get_moves only gives direct moves of pieces
		if (pieces[i].type == PAWN) 
		{

			avail_moves.push_back(sf::Vector2i(pieces[i].r + get_team(), pieces[i].c + 1));
			avail_moves.push_back(sf::Vector2i(pieces[i].r + get_team(), pieces[i].c - 1));

		}

		if (std::find(avail_moves.begin(), avail_moves.end(), sf::Vector2i(r, c)) != avail_moves.end())
		{
			return true;
		}
	}

	return false;
}

// to see if a place is inside the board
bool check_boundaries(int r, int c){
	return ((r < N_CELLS && c < N_CELLS) && (r >= 0 && c >= 0));
}

// gets all the possible places where the piece can go
std::vector<sf::Vector2i> get_moves(int piece, int r, int c, int team){
	std::vector<sf::Vector2i> avail_moves{};
	int blocked[] = {0, 0, 0, 0, 0, 0, 0, 0};  // to not jump over pieces when checking available places to move

	if (piece == KNIGHT)
	{
		if (check_boundaries(r + 2, c + 1)) { avail_moves.push_back(sf::Vector2i(r + 2, c + 1)); }
		if (check_boundaries(r + 2, c - 1)) { avail_moves.push_back(sf::Vector2i(r + 2, c - 1)); }
		if (check_boundaries(r + 1, c + 2)) { avail_moves.push_back(sf::Vector2i(r + 1, c + 2)); }
		if (check_boundaries(r - 1, c + 2)) { avail_moves.push_back(sf::Vector2i(r - 1, c + 2)); }
		if (check_boundaries(r - 2, c + 1)) { avail_moves.push_back(sf::Vector2i(r - 2, c + 1)); }
		if (check_boundaries(r - 2, c - 1)) { avail_moves.push_back(sf::Vector2i(r - 2, c - 1)); }
		if (check_boundaries(r + 1, c - 2)) { avail_moves.push_back(sf::Vector2i(r + 1, c - 2)); }
		if (check_boundaries(r - 1, c - 2)) { avail_moves.push_back(sf::Vector2i(r - 1, c - 2)); }
	}
	else if (piece == BISHOP)
	{
		for (int i = 1; i < N_CELLS; i++)
		{
			if (check_boundaries(r + i, c + i) 
				&& !blocked[0]) { avail_moves.push_back(sf::Vector2i(r + i, c + i)); }
			if (check_boundaries(r - i, c - i) 
				&& !blocked[1]) { avail_moves.push_back(sf::Vector2i(r - i, c - i)); }
			if (check_boundaries(r - i, c + i) 
				&& !blocked[2]) { avail_moves.push_back(sf::Vector2i(r - i, c + i)); }
			if (check_boundaries(r + i, c - i) 
				&& !blocked[3]) { avail_moves.push_back(sf::Vector2i(r + i, c - i)); }
			
			// blocks moves when a piece is found; only knights can jump over other pieces
			if (board[r + i][c + i] != 0){ blocked[0] = 1; }
			if (board[r - i][c - i] != 0){ blocked[1] = 1; }
			if (board[r - i][c + i] != 0){ blocked[2] = 1; }
			if (board[r + i][c - i] != 0){ blocked[3] = 1; }
		}
	}
	else if (piece == ROOK)
	{
		for (int i = 1; i < N_CELLS; i++)
		{
			if (check_boundaries(r + i, c) 
				&& !blocked[0]) { avail_moves.push_back(sf::Vector2i(r + i, c)); }
			if (check_boundaries(r - i, c) 
				&& !blocked[1]) { avail_moves.push_back(sf::Vector2i(r - i, c)); }
			if (check_boundaries(r, c + i) 
				&& !blocked[2]) { avail_moves.push_back(sf::Vector2i(r, c + i)); }
			if (check_boundaries(r, c - i) 
				&& !blocked[3]) { avail_moves.push_back(sf::Vector2i(r, c - i)); }

			// blocks moves when a piece is found; only knights can jump over other pieces
			if (board[r + i][c] != 0){ blocked[0] = 1; }
			if (board[r - i][c] != 0){ blocked[1] = 1; }
			if (board[r][c + i] != 0){ blocked[2] = 1; }
			if (board[r][c - i] != 0){ blocked[3] = 1; }
		}

	}
	else if (piece == QUEEN)
	{
		for (int i = 1; i < N_CELLS; i++)
		{
			if (check_boundaries(r + i, c + i) 
				&& !blocked[0]) { avail_moves.push_back(sf::Vector2i(r + i, c + i)); }
			if (check_boundaries(r - i, c - i) 
				&& !blocked[1]) { avail_moves.push_back(sf::Vector2i(r - i, c - i)); }
			if (check_boundaries(r - i, c + i) 
				&& !blocked[2]) { avail_moves.push_back(sf::Vector2i(r - i, c + i)); }
			if (check_boundaries(r + i, c - i) 
				&& !blocked[3]) { avail_moves.push_back(sf::Vector2i(r + i, c - i)); }

			// blocks moves when a piece is found; only knights can jump over other pieces
			if (board[r + i][c + i] != 0){ blocked[0] = 1; }
			if (board[r - i][c - i] != 0){ blocked[1] = 1; }
			if (board[r - i][c + i] != 0){ blocked[2] = 1; }
			if (board[r + i][c - i] != 0){ blocked[3] = 1; }
		}

		for (int i = 1; i < N_CELLS; i++)
		{
			if (check_boundaries(r + i, c) 
				&& !blocked[4]) { avail_moves.push_back(sf::Vector2i(r + i, c)); }
			if (check_boundaries(r - i, c) 
				&& !blocked[5]) { avail_moves.push_back(sf::Vector2i(r - i, c)); }
			if (check_boundaries(r, c + i) 
				&& !blocked[6]) { avail_moves.push_back(sf::Vector2i(r, c + i)); }
			if (check_boundaries(r, c - i) 
				&& !blocked[7]) { avail_moves.push_back(sf::Vector2i(r, c - i)); }

			// blocks moves when a piece is found; only knights can jump over other pieces
			if (board[r + i][c] != 0){ blocked[4] = 1; }
			if (board[r - i][c] != 0){ blocked[5] = 1; }
			if (board[r][c + i] != 0){ blocked[6] = 1; }
			if (board[r][c - i] != 0){ blocked[7] = 1; }
		}
	}
	else if (piece == KING)
	{
		// normal positions
		if (check_boundaries(r + 1, c + 1)) { avail_moves.push_back(sf::Vector2i(r + 1, c + 1)); }
		if (check_boundaries(r - 1, c - 1)) { avail_moves.push_back(sf::Vector2i(r - 1, c - 1)); }
		if (check_boundaries(r - 1, c + 1)) { avail_moves.push_back(sf::Vector2i(r - 1, c + 1)); }
		if (check_boundaries(r + 1, c - 1)) { avail_moves.push_back(sf::Vector2i(r + 1, c - 1)); }
		if (check_boundaries(r + 1, c)) { avail_moves.push_back(sf::Vector2i(r + 1, c)); }
		if (check_boundaries(r - 1, c)) { avail_moves.push_back(sf::Vector2i(r - 1, c)); }
		if (check_boundaries(r, c + 1)) { avail_moves.push_back(sf::Vector2i(r, c + 1)); }
		if (check_boundaries(r, c - 1)) { avail_moves.push_back(sf::Vector2i(r, c - 1)); }

		
		// add moves for castling
		if (check_boundaries(r, c + 2)) { avail_moves.push_back(sf::Vector2i(r, c + 2)); }
		if (check_boundaries(r, c - 2)) { avail_moves.push_back(sf::Vector2i(r, c - 2)); }
	}
	else if (piece == PAWN)
	{
		if (board[r + team][c + team] != 0 || board[r + team][c - team] != 0)
		{
			if (check_boundaries(r + team, c + team)) { avail_moves.push_back(sf::Vector2i(r + team, c + team)); }
			if (check_boundaries(r + team, c - team)) { avail_moves.push_back(sf::Vector2i(r + team, c - team)); }
		}
		if (check_boundaries(r + team, c)) { avail_moves.push_back(sf::Vector2i(r + team, c)); }
		if (check_boundaries(r + 2 * team, c)) { avail_moves.push_back(sf::Vector2i(r + 2 * team, c)); }
	}

	return avail_moves;
}

// to check if the chosen new position is part of the possible moves of the piece
bool is_move_legal(sf::Vector2i i_pos, sf::Vector2i f_pos, Player& player){
	int piece = board[i_pos.x][i_pos.y];
	int team = player.get_team();
	std::vector<sf::Vector2i> avail_moves{};
	int pawn_starting_row = team != -1 ? team : (N_CELLS - 1 + team);

	int piece_idx = player.get_piece_idx(i_pos.x, i_pos.y);

	// handle pawn move if it is the first move of the pawn
	// if the row where the pawn is is not the starting row, then it can only move forward or capture, cannot move 2 places again
	if (piece == PAWN && pawn_starting_row != i_pos.x)
	{
		if (board[f_pos.x][f_pos.y] == 0) { return ((f_pos.x - i_pos.x == team) && (f_pos.y - i_pos.y == 0)); }
		else 
		{
			return ((f_pos.x - i_pos.x == team) 
				&& (f_pos.y - i_pos.y == team || f_pos.y - i_pos.y == -team));
		}
	}
	else
	{
		avail_moves = get_moves(piece, i_pos.x, i_pos.y, team);
		if (piece == KING && player.pieces[piece_idx].has_moved)
		{
			// removes two castling positions since king moved and cannot castle anymore
			avail_moves.pop_back();
			avail_moves.pop_back();
		}
		return std::find(avail_moves.begin(), avail_moves.end(), f_pos) != avail_moves.end();
	}

	return false;
}

// to move the piece and handle captures and handle if the final position is occupied
bool move_piece(int r_i, int c_i, int r_f, int c_f, Player& player, Player& player2){ 
	int type = board[r_i][c_i];

	// if the move is in the possible moves for that piece
	if (is_move_legal(sf::Vector2i(r_i, c_i), sf::Vector2i(r_f, c_f), player))
	{
		int piece_idx = player.get_piece_idx(r_i, c_i);
		if (piece_idx != -1)
		{
			// handle castling
			if (type == KING 
				&& !player.pieces[piece_idx].has_moved)
			{
				int rook_1_idx = player.get_piece_idx(r_f, c_f - 2);
				int rook_2_idx = player.get_piece_idx(r_f, c_f + 1);
				// long left castle
				if (board[r_f][c_f - 2] == ROOK
					&& board[r_i][c_i - 1] == 0
					&& board[r_i][c_i - 2] == 0
					&& rook_1_idx != -1
					&& !player2.is_attacked(r_i, c_i)
					&& !player2.is_attacked(r_i, c_i - 1)
					&& !player2.is_attacked(r_i, c_i - 2))
				{
					// move king
					player.update_piece_pos(piece_idx, r_f, c_f);
					board[r_i][c_i] = 0;
					board[r_f][c_f] = type;
					// move rook
					player.update_piece_pos(player.get_piece_idx(r_f, c_f - 2), r_i, c_i - 1);
					board[r_f][c_f - 2] = 0;
					board[r_i][c_i - 1] = ROOK;

					player.pieces[piece_idx].has_moved = 1;
					return true;	
				}
				// short right castle
				if (board[r_f][c_f + 1] == ROOK
					&& board[r_i][c_i + 1] == 0
					&& board[r_i][c_i + 2] == 0
					&& rook_2_idx != -1
					&& !player2.is_attacked(r_i, c_i)
					&& !player2.is_attacked(r_i, c_i + 1)
					&& !player2.is_attacked(r_i, c_i + 2))
				{
					// move king
					player.update_piece_pos(piece_idx, r_f, c_f);
					board[r_i][c_i] = 0;
					board[r_f][c_f] = type;
					// move rook
					player.update_piece_pos(player.get_piece_idx(r_f, c_f + 1), r_i, c_i + 1);
					board[r_f][c_f + 1] = 0;
					board[r_i][c_i + 1] = ROOK;

					player.pieces[piece_idx].has_moved = 1;
					return true;	
				}

			}
			if (type == KING && (abs(c_i - c_f) > 1 || abs(r_i - r_f) > 1))
			{
				return false;
			}

			int final_place = board[r_f][c_f];

			// capture piece
			if (final_place != 0)
			{ 

				// see if piece belongs to this player; if so return false
				int final_place_team = player2.get_piece_idx(r_f, c_f);
				if (final_place_team == -1) { return false; }

				player.add_points(final_place); 
				player2.remove_piece(r_f, c_f);
			}
			
			player.update_piece_pos(piece_idx, r_f, c_f);
			board[r_i][c_i] = 0;
			board[r_f][c_f] = type;

			return true;	
		}
	}

	return false;
}

std::vector<int> map_mouse_to_coord(float x, float y)
{
	std::vector<int> board_coords{};
	int board_x = (x - BOARD_START_X) / CELL_SIZE, board_y = (y - BOARD_START_Y) / CELL_SIZE;

	board_coords.push_back(board_x);
	board_coords.push_back(board_y);

	return board_coords;
}

void print_board()
{
	for (int i = 0;i<N_CELLS;i++)
	{
		for (int j = 0;j<N_CELLS;j++)
		{
			std::cout << board[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

int main(){

	int flag = 0;
	int j{};

	Player p1(-1, "player1");
	Player p2(1, "player2");

	sf::Texture t1, t2;
	t1.loadFromFile("resources/chess_board2.png");
	t2.loadFromFile("resources/chess_pieces_9.png");

	float pieces_w = t2.getSize().x / 6;
	float pieces_h = t2.getSize().y / 2;


	//PAWNS
	for (j = 0;j<8;j++){
		p1.pieces[j].sprite.setTexture(t2);
		p1.pieces[j].sprite.setTextureRect(sf::IntRect(t2.getSize().x - t2.getSize().x/6, 0, t2.getSize().x/6, pieces_h));  // (x, y, bb_w, bb_h)
		
		p2.pieces[j].sprite.setTexture(t2);
		p2.pieces[j].sprite.setTextureRect(sf::IntRect(t2.getSize().x - t2.getSize().x/6, pieces_h, t2.getSize().x/6, pieces_h));  // (x, y, bb_w, bb_h)	
	}

	//ROOKS
	p1.pieces[j].sprite.setTexture(t2);
	p1.pieces[j].sprite.setTextureRect(sf::IntRect(4 * pieces_w, 0, t2.getSize().x/6, pieces_h));  // (x, y, bb_w, bb_h)
	
	p2.pieces[j].sprite.setTexture(t2);
	p2.pieces[j].sprite.setTextureRect(sf::IntRect(4 * pieces_w, pieces_h, t2.getSize().x/6, pieces_h));  // (x, y, bb_w, bb_h)
	j++;

	p1.pieces[j].sprite.setTexture(t2);
	p1.pieces[j].sprite.setTextureRect(sf::IntRect(4 * pieces_w, 0, t2.getSize().x/6, pieces_h));  // (x, y, bb_w, bb_h)
	
	p2.pieces[j].sprite.setTexture(t2);
	p2.pieces[j].sprite.setTextureRect(sf::IntRect(4 * pieces_w, pieces_h, t2.getSize().x/6, pieces_h));  // (x, y, bb_w, bb_h)
	j++;

	//KNIGHTS
	p1.pieces[j].sprite.setTexture(t2);
	p1.pieces[j].sprite.setTextureRect(sf::IntRect(3 * pieces_w, 0, t2.getSize().x/6, pieces_h));  // (x, y, bb_w, bb_h)
	
	p2.pieces[j].sprite.setTexture(t2);
	p2.pieces[j].sprite.setTextureRect(sf::IntRect(3 * pieces_w, pieces_h, t2.getSize().x/6, pieces_h));  // (x, y, bb_w, bb_h)
	j++;

	p1.pieces[j].sprite.setTexture(t2);
	p1.pieces[j].sprite.setTextureRect(sf::IntRect(3 * pieces_w, 0, t2.getSize().x/6, pieces_h));  // (x, y, bb_w, bb_h)
	
	p2.pieces[j].sprite.setTexture(t2);
	p2.pieces[j].sprite.setTextureRect(sf::IntRect(3 * pieces_w, pieces_h, t2.getSize().x/6, pieces_h));  // (x, y, bb_w, bb_h)
	j++;

	//BISHOPS
	p1.pieces[j].sprite.setTexture(t2);
	p1.pieces[j].sprite.setTextureRect(sf::IntRect(2 * pieces_w, 0, t2.getSize().x/6, pieces_h));  // (x, y, bb_w, bb_h)
	
	p2.pieces[j].sprite.setTexture(t2);
	p2.pieces[j].sprite.setTextureRect(sf::IntRect(2 * pieces_w, pieces_h, t2.getSize().x/6, pieces_h));  // (x, y, bb_w, bb_h)
	j++;

	p1.pieces[j].sprite.setTexture(t2);
	p1.pieces[j].sprite.setTextureRect(sf::IntRect(2 * pieces_w, 0, t2.getSize().x/6, pieces_h));  // (x, y, bb_w, bb_h)
	
	p2.pieces[j].sprite.setTexture(t2);
	p2.pieces[j].sprite.setTextureRect(sf::IntRect(2 * pieces_w, pieces_h, t2.getSize().x/6, pieces_h));  // (x, y, bb_w, bb_h)
	j++;

	//QUEEN
	p1.pieces[j].sprite.setTexture(t2);
	p1.pieces[j].sprite.setTextureRect(sf::IntRect(1 * pieces_w, 0, t2.getSize().x/6, pieces_h));  // (x, y, bb_w, bb_h)
	
	p2.pieces[j].sprite.setTexture(t2);
	p2.pieces[j].sprite.setTextureRect(sf::IntRect(1 * pieces_w, pieces_h, t2.getSize().x/6, pieces_h));  // (x, y, bb_w, bb_h)
	j++;

	//KING
	p1.pieces[j].sprite.setTexture(t2);
	p1.pieces[j].sprite.setTextureRect(sf::IntRect(0, 0, t2.getSize().x/6, pieces_h));  // (x, y, bb_w, bb_h)
	
	p2.pieces[j].sprite.setTexture(t2);
	p2.pieces[j].sprite.setTextureRect(sf::IntRect(0, pieces_h, t2.getSize().x/6, pieces_h));  // (x, y, bb_w, bb_h)
	j++;


	sf::Sprite s(t1);
	sf::Sprite s2(t2);
	sf::RenderWindow window(sf::VideoMode(1000,1000), "Chess");

	p1.initialize_pieces();
	p2.initialize_pieces();

	game_step = 1;

	int ri, ci, rf, cf;
	int count_click = 0;

	while (window.isOpen())
	{
		sf::Event e;
		while(window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
				window.close();

			if (e.type == sf::Event::MouseButtonPressed)
				if (e.mouseButton.button == sf::Mouse::Left)
				{
					count_click++;

					std::vector<int> board_coords = map_mouse_to_coord(e.mouseButton.x, e.mouseButton.y);

					if (count_click == 1)
					{
						ri = board_coords.at(1);
						ci = board_coords.at(0);
					}
					else if (count_click == 2)
					{
						rf = board_coords.at(1);
						cf = board_coords.at(0);
						count_click = 0;

						// white to play (bottom player)
						if (!flag){
							
							if (move_piece(ri, ci, rf, cf, p1, p2)){
								flag = !flag;

							}
						}
						// black to play (top player)
						else
						{
							if (move_piece(ri, ci, rf, cf, p2, p1)){
								flag = !flag;	

							}
						}
					}


				}
		}


		window.clear();
		window.draw(s);
		for (int i = 0;i<16;i++){ window.draw(p1.pieces[i].sprite); window.draw(p2.pieces[i].sprite); }
		window.display();

	}
	return 0;
}