#include "tree.h"

#include "treeNode.h"

#include <stack>
#include<iostream>

int arity(char c);

Tree::Tree(TreeNode* root) {
	this->root = root;
}

Tree::Tree(const std::string& postfix) {
	std::stack<TreeNode*> s;

	TreeNode* tn1 = nullptr;
	TreeNode* tn2 = nullptr;
	TreeNode* tn3 = nullptr;

	for (unsigned i = 0; i < postfix.length(); ++i) {
		if ((postfix[i] >= 'A' && postfix[i] <= 'Z') || (postfix[i] >= '0' && postfix[i] <= '9')) {
			s.push(new TreeNode(postfix[i]));
		}
		else {
			if (arity(postfix[i]) == 1) {
				tn2 = s.top();
				s.pop();

				tn1 = new TreeNode(postfix[i], tn2, nullptr);
			}
			else {
				tn3 = s.top();
				s.pop();

				tn2 = s.top();
				s.pop();

				tn1 = new TreeNode(postfix[i], tn2, tn3);
			}
			
			s.push(tn1);
		}
	}
	this->root = s.top();
	this->formatTree();
}

Tree::~Tree() {
	delete this->root;
}

std::ostream& operator<<(std::ostream& os, const Tree& tree) {
	tree.root->printTreeNode(os, 0);
	return os;
}

std::ostream& Tree::printPostfixTree(std::ostream& os) const {
	this->root->printTreeNodePostfix(os);
	return os;
}

std::ostream& Tree::printInfixTree(std::ostream& os) const {
	this->root->printTreeNodeInfix(os);
	return os;
}

double Tree::evaluate(const std::vector<double>& values) const {
	return this->root->evaluateTreeNode(values);
}

Tree* Tree::formDerivative(char var) const {
	Tree* derivative = new Tree(this->root->formDerivative(var));
	derivative->formatTree();
	return derivative;
}

void Tree::formatTree() {
	this->root = this->root->formatTreeNode();
}
