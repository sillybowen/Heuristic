#ifndef MCP_LIST_NODE_HEADER
#define MCP_LIST_NODE_HEADER

#include "lock.hpp"

namespace base {

// This is a linked-list-node class.
// Currently, not thread-safe
// It's a friend of ListBasedSet class
//
class ListNode {
public:

  // Constructor:
  ListNode(const int& it, ListNode* nptr);

  // class "ListNode" is a friend to class "ListBasedSet"
  friend class ListBasedSet;

private:
  int       item;
  ListNode* next;
};

typedef ListNode* pLsNode;

} // namespace base

#endif
