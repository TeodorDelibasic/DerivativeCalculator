#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <stack>

#include "tree.h"

const int VAR_COUNT = 26;

int arity(char c) {
	if (c == '~' || c == 't' || c == 'l' || c == 'c' || c == 's')
		return 1;
	else
		return 2;
}

static std::unordered_map<char, int> formInputPrio() {
	std::unordered_map<char, int> input_prio;

	input_prio.insert({ ')', 1 });
	input_prio.insert({ '+', 2 });
	input_prio.insert({ '-', 2 });
	input_prio.insert({ '~', 3 });
	input_prio.insert({ '*', 4 });
	input_prio.insert({ '/', 4 });
	input_prio.insert({ '^', 6 });
	input_prio.insert({ 'l', 7 });
	input_prio.insert({ 't', 7 });
	input_prio.insert({ 's', 7 });
	input_prio.insert({ 'c', 7 });
	input_prio.insert({ '(', 8 });

	return input_prio;
}

static std::unordered_map<char, int> input_prio = formInputPrio();

static std::unordered_map<char, int> formStackPrio() {
	std::unordered_map<char, int> stack_prio;

	stack_prio.insert({ ')', -1 });
	stack_prio.insert({ '(', 0 });
	stack_prio.insert({ '+', 2 });
	stack_prio.insert({ '-', 2 });
	stack_prio.insert({ '~', 3 });
	stack_prio.insert({ '*', 4 });
	stack_prio.insert({ '/', 4 });
	stack_prio.insert({ '^', 5 });
	stack_prio.insert({ 'l', 7 });
	stack_prio.insert({ 't', 7 });
	stack_prio.insert({ 's', 7 });
	stack_prio.insert({ 'c', 7 });

	return stack_prio;
}

static std::unordered_map<char, int> stack_prio = formStackPrio();

static std::string infixToPostfix(std::string infix) {
    std::stack<char> s;
    std::string postfix;

    unsigned size = 0;
	char c;

    for (unsigned i = 0; i < infix.length(); ++i) {
		c = infix[i];

        if ((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
			postfix.push_back(infix[i]);
        else {

			if (c == 'l' || c == 't') i = i + 1;
			else if (c == 's' || c == 'c') i = i + 2;
            else if (c == '-' && infix[i - 1] == '(') c = '~';

            while (!s.empty() && input_prio[c] <= stack_prio[s.top()]) {
				postfix.push_back(s.top());
				s.pop();
            }

            if (c != ')') s.push(c);
            else s.pop();
        }
    }

    while (!s.empty()) {
		postfix.push_back(s.top());
		s.pop();
    }

	return postfix;
}

int main() {
	std::string infix = "ln(A)+B+5*cos(A)+tg(A)*sin(A)+A^2";
	std::string postfix = infixToPostfix(infix);

	Tree* tree = new Tree(postfix);

	std::vector<double> values(VAR_COUNT);
	values['A' - 'A'] = 5;
	values['B' - 'A'] = 8;

	double value = tree->evaluate(values);

	Tree* derivative = tree->formDerivative('A');
	double derivative_value = derivative->evaluate(values);

	Tree* derivative_of_derivative = derivative->formDerivative('A');

	// ---------------------------------------------------------

	std::cout << infix << std::endl;

	std::cout << std::endl;

	std::cout << "Expression tree:" << std::endl;
	std::cout << *tree;
	tree->printInfixTree(std::cout) << std::endl;

	std::cout << std::endl;

	std::cout << "Derivative tree for variable A:" << std::endl;
	std::cout << *derivative;
	derivative->printInfixTree(std::cout) << std::endl;

	std::cout << "Second order derivative tree for variable A:" << std::endl;
	std::cout << *derivative_of_derivative;
	derivative_of_derivative->printInfixTree(std::cout) << std::endl;

	std::cout << std::endl;

	std::cout << "Values of the variables: " << std::endl;
	std::cout << "A: " << values['A' - 'A'] << std::endl;
	std::cout << "B: " << values['B' - 'A'] << std::endl;

	std::cout << std::endl;

	std::cout << "Value of the expression: " << value << std::endl;
	std::cout << "Value of the derivative: " << derivative_value << std::endl;
}