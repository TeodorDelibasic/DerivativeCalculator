#include "treeNode.h"

#include <iostream>
#include <unordered_map>
#include <functional>
#include <cmath>

std::unordered_map<char, std::string> TreeNode::print_map = TreeNode::createPrintMap();

std::unordered_map<char, std::string> TreeNode::createPrintMap() {
	std::unordered_map<char, std::string> print_map;
	
	print_map.insert({ 'l', "ln" });
	print_map.insert({ 't', "tg" });
	print_map.insert({ 'c', "cos" });
	print_map.insert({ 's', "sin" });
	print_map.insert({ '~', "-" });

	return print_map;
}

std::unordered_map<char, std::function<double(double a, double b)>> TreeNode::expression_map = TreeNode::createExpressionMap();

std::unordered_map<char, std::function<double(double a, double b)>> TreeNode::createExpressionMap() {
	std::unordered_map<char, std::function<double(double a, double b)>> expression_map;

	expression_map.insert({ '+', [](double a, double b) { return a + b; } });
	expression_map.insert({ '-', [](double a, double b) { return a - b; } });
	expression_map.insert({ '*', [](double a, double b) { return a * b; } });
	expression_map.insert({ '/', [](double a, double b) { return a / b; } });
	expression_map.insert({ '^', [](double a, double b) { return std::pow(a, b); } });
	expression_map.insert({ 'l', [](double a, double b) { return std::log(a); } });
	expression_map.insert({ '~', [](double a, double b) { return -a; } });
	expression_map.insert({ 't', [](double a, double b) { return std::tan(a); } });
	expression_map.insert({ 'c', [](double a, double b) { return std::cos(a); } });
	expression_map.insert({ 's', [](double a, double b) { return std::sin(a); } });

	return expression_map;
}

std::unordered_map<char, std::function<TreeNode*(const TreeNode*, char)>> TreeNode::derivative_map = TreeNode::createDerivativeMap();

std::unordered_map<char, std::function<TreeNode*(const TreeNode*, char)>> TreeNode::createDerivativeMap() {
	std::unordered_map<char, std::function<TreeNode*(const TreeNode*, char)>> derivative_map;

	derivative_map.insert({ '+', [](const TreeNode* root, char var) { return addition(root, var); } });
	derivative_map.insert({ '-', [](const TreeNode* root, char var) { return substraction(root, var); } });
	derivative_map.insert({ '*', [](const TreeNode* root, char var) { return multiplication(root, var); } });
	derivative_map.insert({ '/', [](const TreeNode* root, char var) { return division(root, var); } });
	derivative_map.insert({ '^', [](const TreeNode* root, char var) { return power(root, var); } });
	derivative_map.insert({ 'l', [](const TreeNode* root, char var) { return naturalLogarithm(root, var); } });
	derivative_map.insert({ '~', [](const TreeNode* root, char var) { return negation(root, var); } });
	derivative_map.insert({ 't', [](const TreeNode* root, char var) { return tangent(root, var); } });
	derivative_map.insert({ 'c', [](const TreeNode* root, char var) { return cosine(root, var); } });
	derivative_map.insert({ 's', [](const TreeNode* root, char var) { return sine(root, var); } });

	return derivative_map;
}

void TreeNode::printTreeNode(std::ostream& os, int level) const {
	for (int i = 0; i < level; ++i)
		os << "---";

	if (TreeNode::print_map.count(this->data))
		os << "(" << TreeNode::print_map[this->data] << ")";
	else
		os << "(" << this->data << ")";

	os << std::endl;

	if (this->left) this->left->printTreeNode(os, level + 1);
	if (this->right) this->right->printTreeNode(os, level + 1);
}

void TreeNode::printTreeNodePostfix(std::ostream& os) const {
	if (this->left) this->left->printTreeNodePostfix(os);
	if (this->right) this->right->printTreeNodePostfix(os);

	if (TreeNode::print_map.count(this->data))
		os << TreeNode::print_map[this->data];
	else
		os << this->data;

	os << " ";
}

void TreeNode::printTreeNodeInfix(std::ostream& os) const {
	if (TreeNode::print_map.count(this->data)) {
		os << TreeNode::print_map[this->data];

		os << "(";
		this->left->printTreeNodeInfix(os);
		os << ")";
	}
	else {
		if (this->left) {
			std::cout << "(";
			this->left->printTreeNodeInfix(os);
			std::cout << ")";
		}

		if (TreeNode::print_map.count(this->data))
			os << TreeNode::print_map[this->data];
		else
			os << this->data;

		if (this->right) {
			std::cout << "(";
			this->right->printTreeNodeInfix(os);
			std::cout << ")";
		}
	}
}

double TreeNode::evaluateTreeNode(const std::vector<double>& values) const {
	double left_value = (this->left ? this->left->evaluateTreeNode(values) : 0);
	double right_value = (this->right ? this->right->evaluateTreeNode(values) : 0);

	if (this->data >= 'A' && this->data <= 'Z')
		return values[this->data - 'A'];

	if (this->data >= '0' && this->data <= '9')
		return this->data - '0';

	return TreeNode::expression_map[this->data](left_value, right_value);
}

bool TreeNode::variableInSubtree(char var) const {
	if (this->data == var)
		return true;

	bool check = false;

	if (this->left && this->left->variableInSubtree(var)) return true;
	if (this->right && this->right->variableInSubtree(var)) return true;

	return false;
}

TreeNode* TreeNode::formDerivative(char var) const {
	if (this->data == var)
		return new TreeNode('1');

	if ((this->data >= 'A' && this->data <= 'Z') || (this->data >= '0' && this->data <= '9'))
		return new TreeNode('0');

	return TreeNode::derivative_map[this->data](this, var);
}

TreeNode* TreeNode::addition(const TreeNode* root, char var) {
	TreeNode* left_derivative = root->left->formDerivative(var);
	TreeNode* right_derivative = root->right->formDerivative(var);

	return new TreeNode('+', left_derivative, right_derivative);
}

TreeNode* TreeNode::substraction(const TreeNode* root, char var) {
	TreeNode* left_derivative = root->left->formDerivative(var);
	TreeNode* right_derivative = root->right->formDerivative(var);

	return new TreeNode('-', left_derivative, right_derivative);
}

TreeNode* TreeNode::multiplication(const TreeNode* root, char var) {
	TreeNode* left_derivative = root->left->formDerivative(var);
	TreeNode* right_derivative = root->right->formDerivative(var);

	TreeNode* temp1 = new TreeNode('*', left_derivative, root->right->copyTreeNode());
	TreeNode* temp2 = new TreeNode('*', right_derivative, root->left->copyTreeNode());

	return new TreeNode('+', temp1, temp2);
}

TreeNode* TreeNode::division(const TreeNode* root, char var) {
	TreeNode* left_derivative = root->left->formDerivative(var);
	TreeNode* right_derivative = root->right->formDerivative(var);

	TreeNode* left_copy = root->left->copyTreeNode();
	TreeNode* right_copy = root->right->copyTreeNode();

	TreeNode* summand1 = new TreeNode('*', left_derivative, right_copy);
	TreeNode* summand2 = new TreeNode('*', right_derivative, left_copy);

	return new TreeNode('/', new TreeNode('-', summand1, summand2), new TreeNode('*', root->right->copyTreeNode(), root->right->copyTreeNode()));
}

TreeNode* TreeNode::naturalLogarithm(const TreeNode* root, char var) { // ln(X)
	TreeNode* left_derivative = root->left->formDerivative(var); // X

	TreeNode* dividend = new TreeNode('1'); // 1
	TreeNode* divisor = root->left->copyTreeNode(); // X

	TreeNode* temp1 = new TreeNode('/', dividend, divisor); // 1/X

	return new TreeNode('*', temp1, left_derivative); // 1/X * X'
}

TreeNode* TreeNode::tangent(const TreeNode* root, char var) { // tg(X)
	TreeNode* left_derivative = root->left->formDerivative(var); // X'

	TreeNode* left_copy = root->left->copyTreeNode(); // X

	TreeNode* dividend = new TreeNode('1'); // 1
	TreeNode* divisor = new TreeNode('^', new TreeNode('c', left_copy, nullptr), new TreeNode('2')); // cos(X)^2

	TreeNode* temp1 = new TreeNode('/', dividend, divisor); // 1/cos(X)^2

	return new TreeNode('*', temp1, left_derivative); // 1/cos(X)^2 * X'
}

TreeNode* TreeNode::cosine(const TreeNode* root, char var) { // cos(X)
	TreeNode* left_derivative = root->left->formDerivative(var); // X'

	TreeNode* left_copy = root->left->copyTreeNode(); // X

	TreeNode* temp1 = new TreeNode('*', new TreeNode('s', left_copy, nullptr), left_derivative); // sin(X) * X'

	return new TreeNode('~', temp1, nullptr); // - sin(X) * X'
}

TreeNode* TreeNode::sine(const TreeNode* root, char var) { // sin(X)
	TreeNode* left_derivative = root->left->formDerivative(var); // X'

	TreeNode* left_copy = root->left->copyTreeNode(); // X

	return new TreeNode('*', new TreeNode('c', left_copy, nullptr), left_derivative); // cos(X) * X'
}

TreeNode* TreeNode::power(const TreeNode* root, char var) {
	TreeNode* left_derivative = root->left->formDerivative(var);
	TreeNode* right_derivative = root->right->formDerivative(var);

	TreeNode* left_copy = root->left->copyTreeNode();
	TreeNode* right_copy = root->right->copyTreeNode();

	if (root->left->variableInSubtree(var)) { // X^Y
		TreeNode* right_copy_copy = root->right->copyTreeNode(); // Y

		TreeNode* temp1 = new TreeNode('^', left_copy, new TreeNode('-', right_copy, new TreeNode('1'))); // X^(Y-1)
		TreeNode* temp2 = new TreeNode('*', right_copy_copy, temp1); // Y * X^(Y-1)

		return new TreeNode('*', temp2, left_derivative); // Y * X^(Y-1) * X'
	}
	else { // Y^X
		TreeNode* copy = root->copyTreeNode(); // Y^X

		TreeNode* temp1 = new TreeNode('*', new TreeNode('l', left_copy, nullptr), copy); // ln(Y) * Y^X

		return new TreeNode('*', temp1, right_derivative); // ln(Y) * Y^X * X'
	}
}

TreeNode* TreeNode::negation(const TreeNode* root, char var) { // -X
	TreeNode* left_derivative = root->left->formDerivative(var); // X'

	return new TreeNode('~', left_derivative, nullptr); // -X'
}

TreeNode* TreeNode::formatTreeNode() {
	TreeNode* left_formatted = (this->left ? this->left->formatTreeNode() : nullptr);
	TreeNode* right_formatted = (this->right ? this->right->formatTreeNode() : nullptr);

	if (this->data == '+') {
		if (left_formatted->data == '0')
			return right_formatted;
		if (right_formatted->data == '0')
			return left_formatted;
	}
	else if (this->data == '-') {
		if (left_formatted->data == '0')
			return new TreeNode('~', right_formatted, nullptr);
		if (right_formatted->data == '0')
			return left_formatted;
	}
	else if (this->data == '*') {
		if (left_formatted->data == '0' || right_formatted->data == '0')
			return new TreeNode('0', nullptr, nullptr);
		if (left_formatted->data == '1')
			return right_formatted;
		if (right_formatted->data == '1')
			return left_formatted;
	}
	else if (this->data == '/') {
		if (left_formatted->data == '0')
			return new TreeNode('0', nullptr, nullptr);
		if (right_formatted->data == '1')
			return left_formatted;
	}
	else if (this->data == '~') {
		if (left_formatted->data == '0')
			return new TreeNode('0', nullptr, nullptr);
	}
	else if (this->data == 'l') {
		if (left_formatted->data == '1')
			return new TreeNode('0', nullptr, nullptr);
	}
	else if (this->data == 't') {
		if (left_formatted->data == '0')
			return new TreeNode('0', nullptr, nullptr);
	}
	else if (this->data == 'c') {
		if (left_formatted->data == '0')
			return new TreeNode('1', nullptr, nullptr);
	}
	else if (this->data == 's') {
		if (left_formatted->data == '0')
			return new TreeNode('0', nullptr, nullptr);
	}
	else if (this->data == '^') {
		if (left_formatted->data == '1' || right_formatted->data == '0')
			return new TreeNode('1', nullptr, nullptr);
		if (left_formatted->data == '0')
			return new TreeNode('0', nullptr, nullptr);
	}

	this->left = left_formatted;
	this->right = right_formatted;
	
	return this;
}
