#ifndef _tree_h_
#define _tree_h_

#include <iostream>
#include <vector>
#include <unordered_map>

struct TreeNode;

class Tree {

public:
	Tree(TreeNode*);
	Tree(const std::string&);
	~Tree();

	friend std::ostream& operator<<(std::ostream&, const Tree&);

	std::ostream& printPostfixTree(std::ostream&) const;
	std::ostream& printInfixTree(std::ostream&) const;

	double evaluate(const std::vector<double>&) const;

	Tree* formDerivative(char) const;

private:
	TreeNode* root;

	void formatTree();
};

#endif