#include <sstream>
#include <fstream>
#include <string>
#include <print>
#include <vector>
#include <functional>
#include <algorithm>
#include <ranges>

std::stringstream test_input{
	"7 6 4 2 1\n"
	"1 2 7 8 9\n"
	"9 7 6 2 1\n"
	"1 3 2 4 5\n"
	"8 6 4 4 1\n"
	"1 3 6 7 9\n"
};

void parse_ints(std::vector<int>& int_sequence, const std::string& line) {
	std::stringstream ss{ line };
	std::string substring;
	while (std::getline(ss, substring, ' '))
		int_sequence.push_back(std::stoi(substring));
}

bool is_report_safe(const std::vector<int>& report) {

	std::vector<bool> decreasing, increasing, distance;
	for (const auto& [left, right] : report | std::views::adjacent<2>) {		
		decreasing.push_back(std::greater<int>{}(left, right));
		increasing.push_back(std::less<int>{}(left, right));
		distance.push_back([](const int& l, const int& r) { return std::abs(l - r) <= 3; }(left, right));
	}

	return std::ranges::all_of(distance, [](bool e) { return e; }) and
		(std::ranges::all_of(decreasing, [](bool e) { return e; }) or
		 std::ranges::all_of(increasing, [](bool e) { return e; }));
}

bool is_report_safe_without_one(const std::vector<int>& report) {
	for (auto i : std::views::iota(std::size_t(0), report.size())) {
		std::vector without_one(report);
		without_one.erase(without_one.begin() + i);
		if (is_report_safe(without_one)) return true;
	}
	return false;
}

int main() {
	std::println("start");
	
	std::ifstream puzzle_input;
	puzzle_input.open("puzzle-inputs/day2.txt", std::ifstream::in);

	int safe_reports = 0;
	std::vector<int> report;
	for (std::string line; std::getline(puzzle_input, line);) {
		parse_ints(report, line);
		if (is_report_safe(report) or is_report_safe_without_one(report))
			safe_reports++;

		report.clear();
	}
	
	std::println("safe reports: {}\n", safe_reports);
}