#ifndef _treeNode_h_
#define _treeNode_h_

#include <iostream>
#include <unordered_map>
#include <functional>

int arity(char c);

class Tree;

class TreeNode {

	friend class Tree;
	friend std::ostream& operator<<(std::ostream&, const Tree&);

	TreeNode(char data, TreeNode* left = nullptr, TreeNode* right = nullptr) : data(data), left(left), right(right) {

	}

	TreeNode(const TreeNode& root) {
		TreeNode* copyRoot = root.copyTreeNode();

		this->data = copyRoot->data;
		this->left = copyRoot->left;
		this->right = copyRoot->right;
	}

	~TreeNode() {
		delete this->left;
		delete this->right;
	}

	char data;

	TreeNode* left;
	TreeNode* right;

	TreeNode* copyTreeNode() const {
		TreeNode* leftRoot = nullptr;
		TreeNode* rightRoot = nullptr;

		if (this->left) leftRoot = this->left->copyTreeNode();
		if (this->right) rightRoot = this->right->copyTreeNode();

		return new TreeNode(this->data, leftRoot, rightRoot);
	}

	static std::unordered_map<char, std::string> print_map;
	static std::unordered_map<char, std::string> createPrintMap();

	static std::unordered_map<char, std::function<double(double a, double b)>> expression_map;
	static std::unordered_map<char, std::function<double(double a, double b)>> createExpressionMap();

	static std::unordered_map<char, std::function<TreeNode*(const TreeNode*, char)>> derivative_map;
	static std::unordered_map<char, std::function<TreeNode*(const TreeNode*, char)>> createDerivativeMap();

	void printTreeNode(std::ostream&, int) const;
	void printTreeNodePostfix(std::ostream&) const;
	void printTreeNodeInfix(std::ostream&) const;

	double evaluateTreeNode(const std::vector<double>&) const ; 

	bool variableInSubtree(char) const;

	TreeNode* formDerivative(char) const;

	static TreeNode* addition(const TreeNode*, char);
	static TreeNode* substraction(const TreeNode*, char);
	static TreeNode* multiplication(const TreeNode*, char);
	static TreeNode* division(const TreeNode*, char);
	static TreeNode* naturalLogarithm(const TreeNode*, char);
	static TreeNode* tangent(const TreeNode*, char);
	static TreeNode* cosine(const TreeNode*, char);
	static TreeNode* sine(const TreeNode*, char);
	static TreeNode* power(const TreeNode*, char);
	static TreeNode* negation(const TreeNode*, char);

	TreeNode* formatTreeNode();
};

#endif