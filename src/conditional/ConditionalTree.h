#ifndef CONDITIONAL_CONDITIONAL_TREE_H_
#define CONDITIONAL_CONDITIONAL_TREE_H_

#include "parser/parser.h"
#include "parser/tokens.h"
#include <vector>
namespace kdl
{
/* 
TODO:
	See if we can detect the max parenthesis level
	to preallocate the right number of condition trees
*/


struct CNode
{
	CNode* left{ nullptr };
	CNode* right{ nullptr };

	CTokenPtr value{nullptr};
	// Is something like $malfn*
	bool isMultiVar;

	CNode(CTokenPtr val)
		: left(nullptr), right(nullptr), value(val), isMultiVar(false)
	{}

	
};

using CNodeList = std::vector<CNode*>;

///
/// PURPOSE
///		Hold a binary tree representing a set of conditions
///			e.g. filesize > 10MB and $malfn
/// 
///		This works by continually adding subconditions (e.g. filesize > 10MB)
///		And combining them with junctions ( 'and' or 'or' )
/// 
///		
///		Each Condition Tree has a parenthesis level (plevel) that it is responsible for.
///		Whenever a subcondition or junction is added, the we will it's plevel to the one passed.
///		
///		If they do not match, the we will create a sub-condition tree
///		and forward any future requests to that plevel to it. It will do the same.
/// 
///		This sub tree is created via the private constructor. An existing std::vector<CNode*> is passed
///		to save memory and prevent unnecessary copying.
/// 
///		When merge is called, all sub-trees beneath us will also be merged. Then our sub-tree
///		will merge with us.
/// 
///		Merging consists of moving the sub-tree's head to our head's right leaf
/// 

class ConditionalTree 
{
public:
	ConditionalTree(int pLevel = 0);
	~ConditionalTree();
	///
	/// PURPOSE
	///		Adds a subcondition to the tree
	/// 
	///		If the head is null, the new subcondition becomes the head
	/// 
	///		Otherwise, the subcondition becomes the head's right node
	/// 
	bool addSubCondition(CTokenPtr op, CTokenPtr left, CTokenPtr right, int pLevel, bool leftIsMV = false, bool rightIsMv = false);


	/// 
	/// PURPOSE
	///		Adds a junction to the tree
	/// 
	///		If the head is null, an error will be thrown
	/// 
	///		If this is a parenthisized junction, add it as the head's right node
	/// 
	///		Otherwise, the junction becomes the new head and the previous head becomes
	///		the junctions's left node	
	/// 
	bool addJunction(CTokenPtr cmpOP, int pLevel);

	/// 
	/// PURPOSE
	///		Merge any trees below pLevel with the tree at pLevel
	///		After merging, the tree at pLevel will set it's head's 
	///		right leaf to it's sub-tree's head.
	///		It will also copy the contents of it's node list.	
	/// 
	bool merge(int &pLevel);


	const CNode* const getHead() const { return m_head; }

	bool addVariableReference(CTokenPtr var, int pLevel, bool isMultiVar);

	void dumpTree();
	void dumpTree2();

private:
	/// Private constructor to pass plevel 0's nodes to
	ConditionalTree(CNodeList* sharedNodes, int pLevel);

	bool forwardSubCondition(CTokenPtr op, CTokenPtr left, CTokenPtr right, int pLevel);
	bool forwardJunction(CTokenPtr cmpOP, int pLevel);

	bool forwardVariableReference(CTokenPtr var, int pLevel, bool isMultiVar);
	
	void _int_dumpTree(const CNode* head);
	void _int_dumpTree2(const CNode* head, bool isLeftNode, const std::string& prefix);


	constexpr void createSubTree();

private:


	CNode* m_head;

	/// List of pointers to the nodes so they can be destroyed 
	CNodeList	* m_nodesPtr;
	CNodeList m_nodes;
	/// Our parenthesis level
	int m_p_level;

	/// Sub Tree where additions with plevels that don't match ours 
	/// are forwarded to
	ConditionalTree* m_subTree{ nullptr };

};

} // kdl

#endif // CONDITIONAL_CONDITIONAL_TREE_H_