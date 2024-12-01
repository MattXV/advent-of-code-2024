#include <string>
#include <fstream>
#include <sstream>
#include <format>
#include <iostream>
#include <vector>
#include <stack>
#include <ranges>
#include <algorithm>

std::stringstream test_input {

	"3   4\n"
	"4   3\n"
	"2   5\n"
	"1   3\n"
	"3   9\n"
	"3   3\n"
};

std::vector<int> left_column, right_column;

int main() {
	std::ifstream puzzle_input_file;
	puzzle_input_file.open("puzzle-inputs/day1.txt", std::ifstream::in);

	while (!puzzle_input_file.eof()) {
		
		std::string line;
		std::getline(puzzle_input_file, line);
		if (line.empty()) break;

		int left_int = std::stoi(line.substr(0, line.find(' ')));
		line.erase(0, line.find_last_of(' '));
		int right_int = std::stoi(line);

		left_column.push_back(left_int);
		right_column.push_back(right_int);

		//std::cout << std::format("{}, {}\n", left_int, right_int);
	}
	
	std::cout << "---" << std::endl;

	std::ranges::sort(left_column);
	std::ranges::sort(right_column);

	int result = 0;
	int result_similarity = 0;
	for (auto const& [left, right] : std::views::zip(left_column, right_column)) {
		int distance = std::abs(left - right);

		result += distance;

		int multiplier = 0;
		for (const int& i : right_column | std::views::filter([left](const int& element) { return left == element; }))
			multiplier++;
		result_similarity += left * multiplier;
	}


	std::cout << std::format("result: {}, similarity result: {}", result, result_similarity);
}
