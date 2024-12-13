#include <sstream>
#include <fstream>
#include <string>
#include <print>
#include <vector>
#include <utility>
#include <algorithm>
#include <ranges>
#include <tuple>
#include <set>
#include <random>


std::stringstream test_input{
"47 | 53\n"
"97 | 13\n"
"97 | 61\n"
"97 | 47\n"
"75 | 29\n"
"61 | 13\n"
"75 | 53\n"
"29 | 13\n"
"97 | 29\n"
"53 | 29\n"
"61 | 53\n"
"97 | 53\n"
"61 | 29\n"
"47 | 13\n"
"75 | 47\n"
"97 | 75\n"
"47 | 61\n"
"75 | 61\n"
"47 | 29\n"
"75 | 13\n"
"53 | 13\n"
"\n"
"75,47,61,53,29\n"
"97,61,53,29,13\n"
"75,29,13\n"
"75,97,47,61,53\n"
"61,13,29\n"
"97,13,75,29,47\n"
};

class Rule {
	int left, right;
	static std::vector<Rule> rules;
public:
	Rule(const std::tuple<int, int>& args) : left(std::get<0>(args)),right(std::get<1>(args))
	{ rules.push_back(*this); }
	void print() const { std::println("rule: {} | {}", left, right); }
	static void print_all() { std::ranges::for_each(rules, [](auto& r) { r.print(); }); }
	const int& get_left() const { return left; } const int& get_right() const { return right; }

	static std::set<const Rule*> test(std::vector<int>& sequence) {
		std::set<const Rule*> rules_broken;

		for (std::vector<int>::iterator element = sequence.begin(); element != sequence.end(); element++) {

			auto rule_subset = rules_that_apply(*element);
			for (const Rule* rule : rule_subset) {
				// check if right_rule appears and is after element
				if (auto element_right_in_rule = std::ranges::find(sequence, rule->get_right());
					*element == rule->get_left() and
					element_right_in_rule != sequence.end() and
					!(element_right_in_rule > element)) {

					//std::println("sequence: {}, rule: {}|{} failed", sequence, rule.get_left(), rule.get_right());
					rules_broken.insert(rule);
				}
				if (auto element_left_in_rule = std::ranges::find(sequence, rule->get_left());
					*element == rule->get_right() and
					element_left_in_rule != sequence.end() and
					!(element_left_in_rule < element)) {

					//std::println("sequence: {}, rule: {}|{} failed", sequence, rule.get_left(), rule.get_right());
					rules_broken.insert(rule);
				}
			}
		}
		return rules_broken;
	}

	static void fix(std::vector<int>& sequence, const Rule* rule) {

		std::println("\n --- \n fixing: {}, rule", sequence);
		rule->print();
		auto left = std::ranges::find(sequence, rule->get_left());
		auto right = std::ranges::find(sequence, rule->get_right());
		int temp = *left;
		*left = *right;
		*right = temp;
		std::println(" fixed: {} \n --- \n", sequence);

	}

	static std::vector<Rule*> rules_that_apply(const int& element) {
		return rules | std::views::filter(
			[&element](auto& r) { return r.left == element || r.right == element; })
			| std::views::transform([](Rule& rule) { return &rule; }) | std::ranges::to<std::vector<Rule*>>();
	}
};
std::vector<Rule> Rule::rules = std::vector<Rule>();
std::vector<std::vector<int>> sequences;

void parse_rule(const std::string& line) {
	std::stringstream s{ line };
	std::tuple<int, int> rule_args;
	std::string token;
	std::getline(s, token, '|');
	std::get<0>(rule_args) = std::stoi(token);
	std::getline(s, token, '|');
	std::get<1>(rule_args) = std::stoi(token);
	Rule rule(rule_args);
}
void parse_sequence(const std::string& line) {
	std::stringstream s{ line };
	std::vector<int> sequence;
	for (std::string token; std::getline(s, token, ',');) {
		sequence.push_back(std::stoi(token));
	}
	sequences.push_back(std::move(sequence));
}

int main() {
	std::ifstream puzzle_input;
	puzzle_input.open("puzzle-inputs/day5.txt", std::ifstream::in);
	for (std::string line; std::getline(puzzle_input, line);) {
		if (line.contains("|"))	parse_rule(line);
		if (line.contains(",")) parse_sequence(line);
	}
	int result = 0, fixed = 0;
	for (auto& sequence : sequences) {

		std::set<const Rule*> rules_broken = Rule::test(sequence);

		if (rules_broken.empty()) {
			std::println("sequence: {} accepted", sequence);
			result += *(sequence.begin() + (sequence.size() / 2));
		} else {
			
			for (; !rules_broken.empty();) {
				std::random_device rd;
				std::mt19937 g(rd());
				auto shuffled_rules = rules_broken | std::ranges::to<std::vector>();
				std::ranges::shuffle(shuffled_rules, g);
				std::ranges::for_each(rules_broken, [&sequence](auto r) { Rule::fix(sequence, r); });
				rules_broken = Rule::test(sequence);
			}

			std::println("sequence: {} fixed", sequence);
			fixed += *(sequence.begin() + (sequence.size() / 2));
		}
	}
	std::println("result: {}, fixed: {}", result, fixed);
}