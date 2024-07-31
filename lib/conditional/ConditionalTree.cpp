#include "conditional/ConditionalTree.h"
#include "conditional/ConditionalSemanticChecker.h"

#include <cstdlib>

using kdl::CNode;
using kdl::CTokenPtr;
using kdl::ConditionalTree;
using kdl::ConditionalSemanticChecker;
using kdl::token_t;
using kdl::CNodeList;

ConditionalTree::ConditionalTree(int pLevel)
	: m_head{nullptr}, m_p_level(pLevel)
{
	if (pLevel == 0)
		m_nodesPtr = &m_nodes;
}

ConditionalTree::ConditionalTree(CNodeList* sharedNodes, int pLevel)
	: m_head{ nullptr }, m_p_level { pLevel }
{
	printf("New subtree made. %d owned by %d\n",pLevel, pLevel-1);
	m_nodesPtr = sharedNodes;

}
ConditionalTree::~ConditionalTree()
{
	// only clean up nodes if we are plevel 0 i.e we are the top of the tree list
	if (m_p_level == 0)
	{
		//printf("I am being destroyed\n");
		for (int i = 0; i < m_nodes.size(); i++)
		{
			delete m_nodes.at(i);
				
		}

	}

	if (m_subTree)
		delete m_subTree;
}



bool ConditionalTree::addSubCondition(CTokenPtr op, CTokenPtr left, CTokenPtr right, int pLevel, bool leftIsMV, bool rightIsMv)
{
	if (!op || !left || !right)
		return false;

	

	auto* nnode = new CNode(op);

	nnode->left = new CNode(left);


	nnode->right = new CNode(right);
	if (left->t == token_t::MULTI_VAR_IDENTIFIER)
		nnode->left->isMultiVar = true;
	if (right->t == token_t::MULTI_VAR_IDENTIFIER)
		nnode->right->isMultiVar = true;

	m_nodesPtr->push_back(nnode);
	m_nodesPtr->push_back(nnode->left);
	m_nodesPtr->push_back(nnode->right);

	if (pLevel != m_p_level)
		return forwardSubCondition(op, left, right, pLevel);
	

	if (!m_head)
		m_head = nnode;
	else
		m_head->right = nnode;

	

	return ConditionalSemanticChecker::isValidSubCondition(op, left, right);
}

bool ConditionalTree::addJunction(CTokenPtr cmpOP, int pLevel)
{
	if (!cmpOP)
		return false;
	auto* nnode = new CNode(cmpOP);
	m_nodesPtr->push_back(nnode);

	if (pLevel == m_p_level)
	{
		CNode* prevHead = m_head;
		m_head = nnode;
		m_head->left = prevHead;
	}
	else
	{
		if (m_head->right)
			nnode->left = m_head->right;
		m_head->right = nnode;

		return forwardJunction(cmpOP, pLevel);
	}

	return true;
}

bool ConditionalTree::addVariableReference(CTokenPtr var, int pLevel, bool isMultiVar)
{
	auto* nvar = new CNode(var);
	nvar->value = var;
	nvar->isMultiVar = isMultiVar;

	m_nodesPtr->push_back(nvar);
	if (pLevel != m_p_level)
		return forwardVariableReference(var, pLevel, isMultiVar);

	if (!m_head)
		m_head = nvar;
	else
		m_head->right = nvar;


	return true;
}



bool ConditionalTree::merge(int &pLevel)
{
	if (!m_subTree) return true;
	

	if (pLevel - 1== m_p_level)
	{

		printf("[%d] Merging sub tree (%d).\n", m_p_level, pLevel);
	
		// If our head is null, our conditional
		// might start with parenthesis, thus we would
		// not have added any subconditions.
		if (!m_head)
			m_head = m_subTree->m_head;
		else
			m_head->right = m_subTree->m_head;

		delete m_subTree;
		m_subTree = nullptr;

	}
	else
		m_subTree->merge(pLevel);


	return true;
}


void ConditionalTree::dumpTree()
{
	_int_dumpTree(m_head);
	putchar('\n');

}

void ConditionalTree::dumpTree2()
{
	_int_dumpTree2(m_head, false, "");
}


bool ConditionalTree::forwardSubCondition(CTokenPtr op, CTokenPtr left, CTokenPtr right, int pLevel)
{
	createSubTree();
	return m_subTree->addSubCondition(op, left, right, pLevel);

}

bool ConditionalTree::forwardJunction(CTokenPtr cmpOP, int pLevel)
{
	createSubTree();
	return m_subTree->addJunction(cmpOP, pLevel);
}

bool ConditionalTree::forwardVariableReference(CTokenPtr var, int pLevel, bool isMultiVar)
{
	createSubTree();

	return m_subTree->addVariableReference(var, pLevel, isMultiVar);
}


void ConditionalTree::_int_dumpTree(const CNode* head)
{
	if (head)
	{
		putchar('(');
		printf(" %s ", kdl::getTokenName(head->value->t));
		_int_dumpTree(head->left);
		_int_dumpTree(head->right);
		putchar(')');

	}
}

void kdl::ConditionalTree::_int_dumpTree2(const CNode* head, bool isLeftNode, const std::string& prefix)
{
	if (head)
	{

		printf("%s", prefix.c_str());

		printf("%s", isLeftNode ? "|--" : "\\--");

		if (!head->left && !head->right && head->value->t == token_t::IDENTIFIER)
		{
			if (head->isMultiVar)
				printf("MultiVar ");
			else
				printf("SingleVar ");
			printf("%s\n", getTokenName(head->value->t));

		}
		else
		{
			printf("%s\n", getTokenName(head->value->t));

			_int_dumpTree2(head->left, true, prefix + (isLeftNode ? "|   " : "    "));
			_int_dumpTree2(head->right, false, prefix + (isLeftNode ? "|   " : "    "));


		}
	}
}

constexpr void kdl::ConditionalTree::createSubTree()
{
	if (!m_subTree)
		m_subTree = new ConditionalTree(m_nodesPtr, m_p_level + 1);
}
