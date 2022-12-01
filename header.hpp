#include <SFML/Graphics.hpp>

#define N_CELLS 8
#define BOARD_SIZE 1000.0f
#define BOARD_START_X 50
#define BOARD_START_Y 50
#define CELL_SIZE ((1000.0f-(2*50)) / 8)
#define PAWN 2
#define KNIGHT 3
#define BISHOP 4
#define ROOK 5
#define QUEEN 6
#define KING 7
#define OFFSET 20

typedef struct Piece{
	int c{}, r{};
	sf::Sprite sprite;
	int type=-1;
	int has_moved = 0;
	int under_attack = 0;
}Piece;

class Player
{
private:
	int team{};  // -1 is bottom team and 1 is upper team

public:
	std::string name{};
	int points{0};
	Piece pieces[16];
	int it = 0;
	
	Player(int t, std::string n){
		team = t;
		name = n;
	}

	void initialize_pieces();
	void add_points(int pts){points+=pts;}
	void update_piece_pos(int i, int r, int c);
	void remove_piece(int r, int c);
	int get_piece_idx(int r_i, int c_i);
	int get_team(){return team;}
	void print_pieces();
	bool is_attacked(int r, int c);
};

void print_board();
bool move_piece(int r_i, int c_i, int r_f, int c_f, Player& player, Player& player2);  
bool is_move_legal(sf::Vector2i i_pos, sf::Vector2i f_pos, int team);
std::vector<sf::Vector2i> get_moves(int piece, int r, int c, int team);
bool check_boundaries(int r, int c);
std::vector<int> map_mouse_to_coord(float x, float y);
