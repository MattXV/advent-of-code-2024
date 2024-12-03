#include <print>
#include <sstream>
#include <fstream>
#include <regex>
#include <string>
#include <numeric>
#include <functional>

std::stringstream test_input{
	"xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))"
	"xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))"
};

int process_mul_substring(const std::string& substring) {
	std::stringstream token(std::regex_replace(substring, std::regex("mul\\(|\\)"), ""));
	std::vector<int> ints;
	for (std::string int_str; std::getline(token, int_str, ',');)
		ints.push_back(std::stoi(int_str));
	return std::accumulate(ints.begin(), ints.end(), 1, std::multiplies<int>());
}

int main() {
	std::ifstream puzzle_input;
	puzzle_input.open("puzzle-inputs/day3.txt", std::ifstream::in);

	std::regex mul("mul\\(\\d+,\\d+\\)", std::regex_constants::ECMAScript);
	std::regex do_dont("do\\(\\)|don't\\(\\)", std::regex_constants::ECMAScript);

	std::string data;
	for (std::string line; std::getline(puzzle_input, line);) data += line;

	std::sregex_iterator dos(data.begin(), data.end(), do_dont);
	std::sregex_iterator next(dos);
	const std::sregex_iterator end = std::sregex_iterator();
	
	int next_position = (*(++next)).position(), result = 0;
	for (std::sregex_iterator i(data.begin(), data.end(), mul); i != end; ++i) {
		if ((*i).position() > next_position) {
			dos++;
			next_position = ++next != end ? (*next).position() : data.size();
		}
		if (!(*dos).str().compare("don't()") and (*i).position() > (*dos).position()) continue;

		std::string token = (*i).str();
		int p = process_mul_substring(token);
		result += p;
	}

	std::println("result: {}", result);
}