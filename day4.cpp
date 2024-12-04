#include <sstream> 
#include <string>
#include <print>
#include <array>
#include <vector>
#include <utility>
#include <ranges>
#include <fstream>
#include <set>


std::stringstream test_input{
"MMMSXXMASM\n"
"MSAMXMSMSA\n"
"AMXSXMAAMM\n"
"MSAMASMSMX\n"
"XMASAMXAMM\n"
"XXAMMXXAMA\n"
"SMSMSASXSS\n"
"SAXAMASAAA\n"
"MAMMMXMMMM\n"
"MXMXAXMASX\n"
};

struct Position { 
	int x, y;
};
class Data
{
	std::vector<std::vector<char>> data;
public:
	Data() {}
	void move_line_into_data(std::vector<char>&& line) { data.push_back(std::move(line)); }
	const char& operator()(const Position& p) const { return data[p.y][p.x]; }
	std::size_t width() const { return data[0].size(); }
	std::size_t height() const { return data.size(); }
};
constexpr std::array<char, 4> xmas = { 'X', 'M', 'A', 'S' };
constexpr std::array<Position, 8> directions{
		Position(  0, -1 ), Position( 0,  1 ),  // Up, down
		Position( -1,  0 ), Position( 1,  0 ),  // Left, right
		Position( -1, -1 ), Position( 1,  1 ),  // Top left, bottom right
		Position( -1,  1 ), Position( 1, -1 )   // Bottom left, top right
};
constexpr std::array<std::tuple<Position, Position>, 2> diagonals{
		std::tuple(Position(-1, -1), Position(1,  1)),  // Top left, bottom right
		std::tuple(Position(-1,  1), Position(1, -1))   // Bottom left, top right
};


bool is_position_valid(const Position& position, const Data& data) {
	return (position.x >= 0 and position.x < data.width()) and 
		(position.y >= 0 and position.y < data.height());
}

int search_for_xmas(const Data& data, const Position& position) {
	if (data(position) != xmas[0]) return 0;
	int found = 0;

	for (const Position& d : directions) {
		std::size_t letter_index = 1;
		Position next{ position.x + d.x, position.y + d.y };

		for (; letter_index < xmas.size()
			and is_position_valid(next, data)
			and data(next) == xmas[letter_index];
			letter_index++, next.x += d.x, next.y += d.y);

		if (letter_index == xmas.size()) found++;
	}
	return found;
}

bool search_for_mas(const Data& data, const Position& position) {
	if (data(position) != 'A') return false;
	int found = 0;
	for (const auto& [d1, d2] : diagonals) {

		Position next_d1{ position.x + d1.x, position.y + d1.y },
				 next_d2{ position.x + d2.x, position.y + d2.y };

		if (!is_position_valid(next_d1, data) or !is_position_valid(next_d2, data)) continue;
		
		std::set<char> ms = { 'M', 'S' };
		char c1 = data(next_d1), c2 = data(next_d2);
		found += std::erase_if(ms, [&c1](const char& e) { return e == c1; });
		found += std::erase_if(ms, [&c2](const char& e) { return e == c2; });	
	}
	return found == 4;
}

int main() {
	std::ifstream puzzle_input;
	puzzle_input.open("puzzle-inputs/day4.txt");

	Data data;
	for (std::string line; std::getline(puzzle_input, line);) {
		std::vector<char> v(line.c_str(), line.c_str() + line.length());
		data.move_line_into_data(std::move(v));
	}

	int result_xmas = 0, result_mas = 0;
	for (int row = 0; row < data.height(); row++) {
		for (int column = 0; column < data.width(); column++) {		
			result_xmas += search_for_xmas(data, Position{ column, row });
			result_mas += search_for_mas(data, Position{ column, row });
		}
	}

	std::print("{}, {}", result_xmas, result_mas);
}