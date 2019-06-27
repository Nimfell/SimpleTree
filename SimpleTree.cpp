
//#include "stdafx.h"
//------------------------------------------------------------------------
#include <stdio.h>

const int MAX_CHILD = 10;
const int MAX_LIST = 100;
//========================================================================
class SimpleTreeNode
{
public:
  int NodeValue;
  SimpleTreeNode* Parent;
  SimpleTreeNode* Children[MAX_CHILD];

  SimpleTreeNode(int val, SimpleTreeNode* parent)
  {
    NodeValue = val;
    Parent = parent;
    for (int i = 0; i<MAX_CHILD; i++) Children[i] = NULL;
  }
};
//=========================================================================
class SimpleTree
{
public:
  SimpleTreeNode* Root; // may be NULL
  SimpleTreeNode* list[MAX_LIST];
  int list_count; // list count 

  SimpleTree(SimpleTreeNode* root)
  {
    Root = root;
    list_count = 0;
    for (int i = 0; i<MAX_LIST; i++) 
      list[i] = NULL;
  }

  void AddChild(SimpleTreeNode* ParentNode, SimpleTreeNode* NewChild)
  {
    if (ParentNode == NULL) 
    {
       delete NewChild;
       return;
    }
    for (int i = 0; i < MAX_CHILD; i++)
    {
      if (ParentNode->Children[i] == NULL)
      {
        ParentNode->Children[i] = NewChild; // link to array
        NewChild->Parent = ParentNode;
        return;
      }
    }
  }

  SimpleTreeNode** GetAllNodes()
  {
    reset_list();
    tree_walk(Root);
    return list;
  }

  SimpleTreeNode** FindNodesByValue(int val)
  {
    reset_list();
    if (Root->NodeValue == val)
    {
      list[0] = Root;
      list_count++;
    }
    find_values(Root, val);
    return list;
  }

  void DeleteNode(SimpleTreeNode* NodeToDelete)
  {
    if (NodeToDelete == NULL) return;
    for (int i = 0; i < MAX_CHILD; i++)
      if (NodeToDelete->Parent->Children[i] == NodeToDelete)                            // if we reached the finded node in the children list  
        for (; i < MAX_CHILD; i++) 
            NodeToDelete->Parent->Children[i] = NodeToDelete->Parent->Children[i + 1];  // rewrite children list, deleting the finded node
            
    NodeToDelete->Parent->Children[MAX_CHILD-1] = NULL;                                 // fill the last cell             
  }
  
  void MoveNode(SimpleTreeNode* OriginalNode, SimpleTreeNode* NewParent)
  {
    for (int i = 0; i < MAX_CHILD; i++)                                               // remove node from old location
      if (OriginalNode->Parent->Children[i] == OriginalNode)                          // if we reached the finded node in the children list   
        for (; i < MAX_CHILD; i++)
          OriginalNode->Parent->Children[i] = OriginalNode->Parent->Children[i + 1];  // rewrite children list, deleting the finded node  
    OriginalNode->Parent->Children[MAX_CHILD-1] = NULL;                               // fill the last cell
    AddChild(NewParent, OriginalNode);
  }

  int Count()
  {
    return count_all(Root);
  }

  int LeafCount()
  {
    return leaf_count(Root);
  }

  SimpleTreeNode** EvenTrees()
  {
    // возвращает массив узлов,
    // который завершается NULL-ом
    reset_list();    
    if (even_trees(Root)%2 != 0)
       return NULL;    
    list[list_count++] = NULL; // NULL in the end
    return list;        
  }

private:
  int count_all(SimpleTreeNode* child)
  {
    int s = 1;
    for (int i = 0; (child->Children[i] != NULL) && (i < MAX_CHILD); i++)
      s += count_all(child->Children[i]);
    return s;
  }

  int leaf_count(SimpleTreeNode* child)
  {
    int s;
    if (child->Children[0] == NULL)
      s = 1; // leaf
    else
      s = 0; // branch
    for (int i = 0; (child->Children[i] != NULL) && (i < MAX_CHILD); i++)
      s += leaf_count(child->Children[i]);
    return s;
  }

  int branches_count(SimpleTreeNode* child)
  {
    int s = 1;
    for (int i = 0; (child->Children[i] != NULL) && (i < MAX_CHILD); i++)
      if (child->Children[i]->Children[0] != NULL)
        s += branches_count(child->Children[i]);
    return s;
  }

  void reset_list() //reset
  {
    for (; list_count >= 0; --list_count)
      list[list_count] = NULL;
    list_count = 0;     
  }

  void tree_walk(SimpleTreeNode* child)
  {
    list[list_count++] = child;
    if (child == NULL)
      return;
    for (int i = 0; (child->Children[i] != NULL) && (i < MAX_CHILD); i++)  // by the list to NULL 
      tree_walk(child->Children[i]);                                       // deepening            
  }

  void find_values(SimpleTreeNode* child, int value)
  {
    for (int i = 0; child->Children[i] != NULL; i++)
    {
      if (child->Children[i]->NodeValue == value)
        *(list + list_count++) = child->Children[i];
      find_values(child->Children[i], value);
    }
    return;
  }

  int even_trees(SimpleTreeNode* child)
  {
     int count = 1;
     int child_count = 0;

     for (int i = 0; (child->Children[i] != NULL) && (i < MAX_CHILD); i++)
     {
        child_count = even_trees(child->Children[i]);
    
        if ( (child_count)%2 == 0 ) // if the number of children is even
        {
          child_count = 0;
          list[list_count++] = child;              // paretn 
          list[list_count++] = child->Children[i]; // child
        }
        count += child_count;
     }     
  return count;
  }  
};
//=========================================================================
void test()
{  
  int test = 0;
  SimpleTree tree(new SimpleTreeNode(0, NULL));

  tree.AddChild(tree.Root, new SimpleTreeNode(1, NULL));
  tree.AddChild(tree.Root, new SimpleTreeNode(2, NULL));
  tree.AddChild(tree.Root, new SimpleTreeNode(3, NULL));

  tree.AddChild(tree.Root->Children[0], new SimpleTreeNode(4, NULL));
  tree.AddChild(tree.Root->Children[0], new SimpleTreeNode(5, NULL));
  tree.AddChild(tree.Root->Children[0]->Children[1], new SimpleTreeNode(5, NULL));
  tree.AddChild(tree.Root->Children[0], new SimpleTreeNode(6, NULL));
  tree.AddChild(tree.Root->Children[2], new SimpleTreeNode(0, NULL));

  tree.AddChild(tree.Root->Children[1], new SimpleTreeNode(7, NULL));
  tree.AddChild(tree.Root->Children[1]->Children[0], new SimpleTreeNode(8, NULL));
  tree.AddChild(tree.Root->Children[1]->Children[0], new SimpleTreeNode(0, NULL));

  int count;
  count = tree.Count(); // 12
  count = tree.LeafCount(); // 6

  SimpleTreeNode** all_nodes = tree.GetAllNodes();
  SimpleTreeNode** val_nodes = tree.FindNodesByValue(0);
  
  SimpleTreeNode seven(7, tree.Root);
  SimpleTreeNode* _seven = &seven;
  
  tree.AddChild(tree.Root->Children[0], _seven);
  tree.AddChild(tree.Root->Children[0], new SimpleTreeNode(8, NULL));
  tree.AddChild(tree.Root->Children[0], new SimpleTreeNode(9, NULL));
  tree.AddChild(tree.Root->Children[0], new SimpleTreeNode(10, NULL));
  tree.AddChild(tree.Root->Children[0], new SimpleTreeNode(11, NULL));
  tree.AddChild(tree.Root->Children[0], new SimpleTreeNode(12, NULL));
  tree.AddChild(tree.Root->Children[0], new SimpleTreeNode(13, NULL)); // full

  tree.AddChild(tree.Root->Children[0], new SimpleTreeNode(14, NULL)); // no place for item
  count = tree.Count(); // 12+7 = 19
  count = tree.LeafCount(); // 6+7 = 13
  all_nodes = tree.GetAllNodes();

  tree.DeleteNode(tree.Root->Children[0]);
  all_nodes = tree.GetAllNodes();

  if (tree.Root->Children[0]->Children[0]->NodeValue != 7) test++; 

  tree.MoveNode(tree.Root->Children[0]->Children[0], tree.Root->Children[1]->Children[0]);
  if (tree.Root->Children[0]->Children[0] != NULL) test++;
  tree.MoveNode(tree.Root->Children[0], tree.Root->Children[1]);

  all_nodes = tree.GetAllNodes();

  tree.DeleteNode(tree.Root->Children[0]);
  all_nodes = tree.GetAllNodes();
  tree.DeleteNode(tree.Root->Children[0]);
  all_nodes = tree.GetAllNodes();

  //empty
  tree.AddChild(tree.Root, new SimpleTreeNode(1, NULL));
  tree.AddChild(tree.Root, new SimpleTreeNode(2, NULL));
  tree.AddChild(tree.Root, new SimpleTreeNode(3, NULL));
  all_nodes = tree.GetAllNodes();

  tree.AddChild(tree.Root->Children[1], new SimpleTreeNode(4, NULL));
  tree.AddChild(tree.Root->Children[1], new SimpleTreeNode(5, NULL));
  all_nodes = tree.GetAllNodes();

  int j;
  for (int i = 0; i < MAX_LIST; i++)  {
    if (all_nodes[i] == NULL)  {
      j = i;
      break;  } }

  tree.AddChild(tree.Root->Children[1], new SimpleTreeNode(6, NULL));
  tree.AddChild(tree.Root->Children[2], new SimpleTreeNode(7, NULL));
  tree.AddChild(tree.Root->Children[3], new SimpleTreeNode(8, NULL)); // все доступа
  all_nodes = tree.GetAllNodes();
  tree.EvenTrees();
}






