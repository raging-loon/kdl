#include "conditional/ConditionalTree.h"

using kdl::CNode;
using kdl::CTokenPtr;
using kdl::ConditionalTree;
using kdl::token_t;


ConditionalTree::ConditionalTree(int pLevel)
	: m_head{nullptr}, m_p_level(pLevel)
{
	if (pLevel == 0)
		m_nodes = new std::vector<CNode*>;
}

ConditionalTree::ConditionalTree(std::vector<CNode*>* sharedNodes, int pLevel)
	: m_head{ nullptr }, m_p_level { pLevel }
{
	printf("New subtree made. %d owned by %d\n",pLevel, pLevel-1);
	m_nodes = sharedNodes;

}
ConditionalTree::~ConditionalTree()
{
	// only clean up nodes if we are plevel 0 i.e we are the top of the tree list
	if (m_p_level == 0)
	{
		for (int i = 0; i < m_nodes->size(); i++)
			delete m_nodes->at(i);

		m_nodes->clear();
		delete m_nodes;
	}

	if (m_subTree)
		delete m_subTree;
}

bool ConditionalTree::addSubCondition(CTokenPtr op, CTokenPtr left, CTokenPtr right, int pLevel)
{
	if (!op || !left || !right)
		return false;


	auto* nnode = new CNode(op);

	nnode->left = new CNode(left);


	nnode->right = new CNode(right);

	m_nodes->push_back(nnode);
	m_nodes->push_back(nnode->left);
	m_nodes->push_back(nnode->right);

	if (pLevel != m_p_level)
		return forwardSubCondition(op, left, right, pLevel);
	

	if (!m_head)
		m_head = nnode;
	else
		m_head->right = nnode;



	return true;
}

bool ConditionalTree::addJunction(CTokenPtr cmpOP, int pLevel)
{
	if (!cmpOP || !m_head) return false;

	auto* nnode = new CNode(cmpOP);
	m_nodes->push_back(nnode);

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

bool ConditionalTree::merge(int pLevel)
{
	if (!m_subTree) return true;
	
	if(pLevel != m_p_level)
		m_subTree->merge(pLevel);

	if (pLevel >= m_p_level)
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





	return true;
}

void ConditionalTree::dumpTree()
{
	_int_dumpTree(m_head);
	putchar('\n');

}


bool ConditionalTree::forwardSubCondition(CTokenPtr op, CTokenPtr left, CTokenPtr right, int pLevel)
{
	if (!m_subTree)
		m_subTree = new ConditionalTree(m_nodes, m_p_level + 1);

	return m_subTree->addSubCondition(op, left, right, pLevel);

}

bool ConditionalTree::forwardJunction(CTokenPtr cmpOP, int pLevel)
{
	if (!m_subTree)
		m_subTree = new ConditionalTree(m_nodes, m_p_level + 1);

	return m_subTree->addJunction(cmpOP, pLevel);
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
