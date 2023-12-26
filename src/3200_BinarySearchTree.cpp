#include <iostream>
#include <iomanip>
#include <stdint.h>
#include "include/util.h"
#include "include/tracer.h"

using namespace std;

template <class type>
class BSTree
{
private:
    class BSTNode
    {
    public:
        BSTNode* left;
        BSTNode* right;
        type data; 
		BSTNode():left(NULL),right(NULL) {}
        BSTNode(type a_data):left(NULL),right(NULL),data(a_data) {}
    };
    typedef BSTNode* bp;
    bp m_root;
    
public:
    BSTree():m_root(NULL) {}
    ~BSTree() {deleteNode(m_root);}
    bool isEmpty() const {return m_root == NULL;}
    bool find(const type& a_data) const;
    void insert(const type& a_data) {insertAux(m_root,a_data);}
    void remove(const type& a_data);
    void inorder(ostream& out) const {inorderAux(out, m_root);}
    void graph(ostream& out) const {graphAux(out, 0, m_root);}
    
protected:
    void deleteNode(bp a_node);
    void insertAux(bp& a_subRoot, const type& a_data);
    void inorderAux(ostream& out, bp a_subRoot) const;
    void graphAux(ostream& out, int a_indent, bp a_subRoot) const;
    void find2(const type& a_data, bool& found, bp& a_locPtr, bp& a_parent) const;
};

template <class type>
inline void BSTree<type>::deleteNode(bp a_node)
{
    if (a_node->left != NULL)
        deleteNode(a_node->left);
    else if (a_node->right != NULL)
        deleteNode(a_node->right);
    else if (a_node != NULL)
    {
        delete a_node;
        a_node = NULL;
    }
}

template <class type>
inline void BSTree<type>::insertAux(bp& a_subRoot, const type& a_data)
{
    if (a_subRoot == NULL) {
        a_subRoot = new BSTree<type>::BSTNode(a_data);
	}
    else {
		if (a_data%256 == a_subRoot->data%256) Tracer::I()->meet(a_subRoot->data);
		if (a_data < a_subRoot->data)
			insertAux(a_subRoot->left,a_data);
		else if (a_subRoot->data < a_data)
			insertAux(a_subRoot->right,a_data);
		else
			std::cerr << "a_data already in the tree!\n";
	}
}

template <class type>
inline void BSTree<type>::inorderAux(ostream& out, BSTree<type>::bp a_subRoot) const
{
    if (a_subRoot != NULL)
    {
        inorderAux(out, a_subRoot->left);//L
        out << a_subRoot->data << " ";//V
        inorderAux(out, a_subRoot->right);//R
    }
}

template <class type>
inline void BSTree<type>::graphAux(ostream& out, int a_indent, bp a_subRoot) const
{
    if (a_subRoot != NULL)
    {
        graphAux(out, a_indent+8, a_subRoot->right);                //R
        out << setw(a_indent) << " " << a_subRoot->data << endl;    //V
        graphAux(out, a_indent+8, a_subRoot->left);                    //L
    }
}

template <class type>
inline bool BSTree<type>::find(const type& a_data) const
{
    bp locPtr = m_root;
    bool found = false;
    while (!found && locPtr != NULL)
    {
        if (a_data < locPtr->data)
        {
            locPtr = locPtr->left;
        }
        else if (locPtr->data < a_data)
        {
            locPtr = locPtr->right;
        }
        else
        {
            found = true;
        }
    }
    return found;
}

template <class type>
inline void BSTree<type>::find2(const type& a_data, bool& found,
                                        bp& a_locPtr,
                                        bp& a_parent) const
{
    a_locPtr = m_root;
    a_parent = NULL;
    found = false;
    while (!found && a_locPtr != NULL)
    {
        if (a_data < a_locPtr->data)
        {
            a_parent = a_locPtr;
            a_locPtr = a_locPtr->left;
        }
        else if (a_locPtr->data < a_data)
        {
            a_parent = a_locPtr;
            a_locPtr = a_locPtr->right;
        }
        else
        {
            found = true;
        }
    }
}

template <class type>
inline void BSTree<type>::remove(const type& a_data)
{
    bool found = false;
    bp x; //被删除的节点
    bp parent;
    find2(a_data,found,x,parent);
    if (!found)
    {
        std::cerr << "a_data is not in the tree!\n";
        return;
    }
    
    if (x->left != NULL && x->right != NULL)//节点有两个子�?
    {
        //查找x的中续后继节点及其双亲节�?
        bp xSucc = x->right;
        parent = x;
        while (xSucc->left != NULL)
        {
            parent = xSucc;
            xSucc = xSucc->left;
        }
        x->data = xSucc->data;
        x = xSucc;
    }
    bp subTree = x->left;
    if (subTree == NULL)
    {
        subTree = x->right;
    }
    if (parent == NULL)
    {
        m_root = subTree;
    }
    else if (parent->left == x)
    {
        parent->left = subTree;
    }
    else
    {
        parent->right = subTree;
    }
    delete x;
}

//int main()
//{
//    BSTree<int> intBST;
//    cout << "Constructing empty BST\n";
//    cout << "BST " << (intBST.isEmpty()?"is":"is not") << "empty\n";
//    
//    int number;
//    for (;;)
//    {
//        cout << "Item to insert (-999 to stop):";
//        cin >> number;
//        if (number == -999) break;
//        intBST.insert(number);
//    }
//    intBST.inorder(cout);
//    cout << endl;
//    intBST.graph(cout);
//    
//    //����find
//    for (;;)
//    {
//        cout << "Item to find (-999 to stop):";
//        cin >> number;
//        if (number == -999) break;
//        bool found = intBST.find(number);
//        cout << boolalpha << found << endl;
//    }
//    
//    //����remove
//    for (;;)
//    {
//        cout << "Item to remove (-999 to stop):";
//        cin >> number;
//        if (number == -999) break;
//        intBST.remove(number);
//        cout << endl;
//        intBST.graph(cout);
//        cout << endl;
//    }
//    intBST.inorder(cout);
//    return 0;
//}

void run_BinarySearchTree(uint8_t* seedIn, int seedSize) {
	const int Count=7*1024*1024;
	Pair* tmp=new Pair[Count/4];
	fillPairArray(seedIn, seedSize, tmp, Count/4);
	int16_t* arr=(int16_t*)tmp;
	BSTree<int16_t> intBST;
	for(int i=0; i<Count; i+=4) {
		if(intBST.find(arr[i])) {
			//cout<<"found "<<arr[i]<<endl;
			intBST.remove(arr[i]);
		}
		else {
			intBST.insert(arr[i]);
		}
	}
	delete[] tmp;
}

#ifdef SELF_TEST
int main() {
	char hello[100]="ae90..i..8f--r39inviiiii,5j46354q3499@#%@#$%^&$&ADGSGWREF";
	int len=strlen(hello);
	uint64_t firstRes[4];
	uint64_t otherRes[4];
	for(int i=0; i<50; i++) {
		Tracer::I()->clear();
		run_BinarySearchTree((uint8_t*)hello,len);
		if(i==0) {
			Tracer::I()->final_result((unsigned char*)firstRes);
		} else {
			Tracer::I()->final_result((unsigned char*)otherRes);
			for(int i=0; i<4; i++) {
				//printf("H %016llx %016llx\n", firstRes[i], otherRes[i]);
				assert(firstRes[i]==otherRes[i]);
			}
		}
	}
	return 0;
}
#endif
