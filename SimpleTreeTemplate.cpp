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
      for(int i=0; i<MAX_CHILD; i++) Children[i] = NULL;      
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
    }

    void AddChild(SimpleTreeNode* ParentNode, SimpleTreeNode* NewChild)
    {
      for(int i = 0; i < MAX_CHILD; i++) 
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
      for(int i = 0; i < MAX_CHILD; i++)       
        if (NodeToDelete->Parent->Children[i] == NodeToDelete)                        //if we reached the finded node in the children list  
          for( ; i < MAX_CHILD; i++)                       
            NodeToDelete->Parent->Children[i] = NodeToDelete->Parent->Children[i+1];  // rewrite children list, deleting the finded node       
      //-- memory release --
      NodeToDelete->Parent = NULL;                                                    //chop off
      reset_list();  
      tree_walk(NodeToDelete);                                                        // create node list of NodeToDelete         
      for(int i = 0; ((i < MAX_LIST) && (*(list + i) != NULL)) ; i++)                 // while all list is not passed or the value NULL is not reached 
      {
        delete *(list + i);                                                           // delete node by node in the list
      }
      return;
    } 
   
    void MoveNode(SimpleTreeNode* OriginalNode, SimpleTreeNode* NewParent)
    {    
      for(int i = 0; i < MAX_CHILD; i++)       
          if (OriginalNode->Parent->Children[i] == OriginalNode)                        //if we reached the finded node in the children list   
            for( ; i < MAX_CHILD; i++)                       
              OriginalNode->Parent->Children[i] = OriginalNode->Parent->Children[i+1];  // rewrite children list, deleting the finded node  
            
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

protected:
    int count_all(SimpleTreeNode* child)
    { 
      int s = 1;
      for(int i = 0; (child->Children[i] != NULL) && (i < MAX_CHILD); i++) 
        s += count_all(child->Children[i]) ;
      return s;    
    }

    int leaf_count(SimpleTreeNode* child)
    { 
      int s;
      if (child->Children[0] == NULL) 
        s = 1; // leaf
      else 
        s = 0; // branch
      for(int i = 0; (child->Children[i] != NULL) && (i < MAX_CHILD); i++)         
        s += leaf_count(child->Children[i]);
      return s;    
    }

    int branches_count(SimpleTreeNode* child)
    { 
      int s = 1;
      for(int i = 0; (child->Children[i] != NULL) && (i < MAX_CHILD); i++) 
        if (child->Children[i]->Children[0] != NULL)
          s += branches_count(child->Children[i]);
      return s;    
    }
  
    void reset_list() //reset
    {
      for(; list_count >= 0; list_count--) 
        list[list_count] = NULL;
      list_count = 0;
    }

    void tree_walk(SimpleTreeNode* child) 
    { 
      *(list + list_count++) = child;                                       // write down to list
      if(child == NULL)
        return;
      for(int i = 0; (child->Children[i] != NULL) && (i < MAX_CHILD); i++)  // by the list to NULL 
        tree_walk(child->Children[i]);                                      // deepening            
    } 

    void find_values(SimpleTreeNode* child, int value) 
    {
      for(int i = 0; child->Children[i] != NULL; i++)   
      { 
        if (child->Children[i]->NodeValue == value)        
          *(list + list_count++) = child->Children[i]; 
        find_values(child->Children[i], value);        
      }
      return;
    }

	// it's not working!!! May be later...
    SimpleTreeNode** recurs10( SimpleTreeNode** children ) 
    {
      SimpleTreeNode* child;
      if ( *children == NULL) 
      {       
        return children; 
        // после выхода из рекурсии
        child = *(children);
        if (*children == child->Parent->Children[0]) // выходим до начала списка потомков
        {        
          return children; // выход на уровень выше         
          return recurs10(children + 1); // продвижение на списку на уровне выше
        }
        else 
          return children; // выход из списка до нулевого элемента
      }
      else // углубление в рекурсию
      {
        child = *(children); // берём прямой указатель на потомка
        if ( *(child->Children) == NULL)
        {
          *(list + list_count++) = *children;
          return recurs10(children + 1); // переход по уровню
        }
        else
        {
          *(list + list_count++) = *children;
          return recurs10( child->Children );  // переход к кроне
        }
      }     
    } 
};

//=========================================================================
void test()
{
  int test = 0;
  SimpleTree tree(new SimpleTreeNode(0,NULL));  
 
  tree.AddChild(tree.Root, new SimpleTreeNode(1, tree.Root));
  tree.AddChild(tree.Root, new SimpleTreeNode(2, tree.Root));
  tree.AddChild(tree.Root, new SimpleTreeNode(3, tree.Root));

  tree.AddChild(tree.Root->Children[0], new SimpleTreeNode(4, tree.Root));
  tree.AddChild(tree.Root->Children[0], new SimpleTreeNode(5, tree.Root));
  tree.AddChild(tree.Root->Children[0]->Children[1], new SimpleTreeNode(5, tree.Root));
  tree.AddChild(tree.Root->Children[0], new SimpleTreeNode(6, tree.Root));
  tree.AddChild(tree.Root->Children[2], new SimpleTreeNode(0, tree.Root));

  tree.AddChild(tree.Root->Children[1], new SimpleTreeNode(7, tree.Root));
  tree.AddChild(tree.Root->Children[1]->Children[0], new SimpleTreeNode(8, tree.Root));
  tree.AddChild(tree.Root->Children[1]->Children[0], new SimpleTreeNode(0, tree.Root));
  
  int count; 
  count = tree.Count();
  count = tree.LeafCount();

  SimpleTreeNode** all_nodes = tree.GetAllNodes();
  SimpleTreeNode** val_nodes = tree.FindNodesByValue(0);

  tree.AddChild(tree.Root->Children[0], new SimpleTreeNode(7, tree.Root));
  tree.AddChild(tree.Root->Children[0], new SimpleTreeNode(8, tree.Root));
  tree.AddChild(tree.Root->Children[0], new SimpleTreeNode(9, tree.Root));
  tree.AddChild(tree.Root->Children[0], new SimpleTreeNode(10, tree.Root));
  tree.AddChild(tree.Root->Children[0], new SimpleTreeNode(11, tree.Root));
  tree.AddChild(tree.Root->Children[0], new SimpleTreeNode(12, tree.Root));
  tree.AddChild(tree.Root->Children[0], new SimpleTreeNode(13, tree.Root));
  tree.AddChild(tree.Root->Children[0], new SimpleTreeNode(14, tree.Root));

  all_nodes = tree.GetAllNodes();
  tree.DeleteNode(tree.Root->Children[0]);
  all_nodes = tree.GetAllNodes();
  tree.MoveNode(tree.Root->Children[0]->Children[0],tree.Root->Children[1]->Children[0]); // переместить 7(8,0) в 0->3->0->
  all_nodes = tree.GetAllNodes();

  return;
}



