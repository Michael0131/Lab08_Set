/***********************************************************************
 * Header:
 *    BST
 * Summary:
 *    Our custom implementation of a BST for set and for map
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        BST                 : A class that represents a binary search tree
 *        BST::iterator       : An iterator through BST
 * Author
 *    Michael, James, Brayden
 ************************************************************************/


#pragma once

#ifdef DEBUG
#define debug(x) x
#else // !DEBUG
#define debug(x)
#endif // !DEBUG

#include <cassert>
#include <utility>
#include <memory>           // for std::allocator
#include <functional>       // for std::less
#include <utility>          // for std::pair
#include <initializer_list>

class TestBST; // forward declaration for unit tests
class TestMap;
class TestSet;

namespace custom
{

    template <class TT>
    class set;

    template <class KK, class VV>
    class map;

    /*****************************************************************
     * BINARY SEARCH TREE
     * Create a Binary Search Tree
     *****************************************************************/
    template <typename T>
    class BST
    {
        friend class ::TestBST; // give unit tests access to the privates
        friend class ::TestMap;
        friend class ::TestSet;

        template <class KK, class VV>
        friend class map;

        template <class TT>
        friend class set;

        template <class KK, class VV>
        friend void swap(map<KK, VV>& lhs, map<KK, VV>& rhs);

    public:
        //
        // Construct
        //
        BST();
        BST(const BST& rhs);
        BST(BST&& rhs);
        BST(const std::initializer_list<T>& il);
        ~BST();

        //
        // Assign
        //
        BST& operator = (const BST& rhs);
        BST& operator = (BST&& rhs);
        BST& operator = (const std::initializer_list<T>& il);
        void swap(BST& rhs);

        //
        // Iterator
        //
        class iterator;
        iterator begin() const noexcept;
        iterator end()   const noexcept { return iterator(nullptr); }

        //
        // Access
        //
        iterator find(const T& t);

        //
        // Insert
        //
        std::pair<iterator, bool> insert(const T& t, bool keepUnique = false);
        std::pair<iterator, bool> insert(T&& t, bool keepUnique = false);

        //
        // Remove
        //
        iterator erase(iterator& it);
        void clear() noexcept;

        //
        // Status
        //
        bool empty() const noexcept { return numElements == 0; }
        size_t size() const noexcept { return numElements; }

    private:
        class BNode;
        BNode* root;              // root node of the binary search tree
        size_t numElements;        // number of elements currently in the tree

        void deleteBinaryTree(BNode*& p) noexcept;
    };

    /*****************************************************************
     * BINARY NODE
     * A single node in a binary tree. Note that the node does not know
     * anything about the properties of the tree so no validation can be done.
     *****************************************************************/
    template <typename T>
    class BST<T>::BNode
    {
    public:
        //
        // Construct
        //
        BNode() :
            data(),
            pLeft(nullptr),
            pRight(nullptr),
            pParent(nullptr),
            isRed(false)
        {
        }

        BNode(const T& t) :
            data(t),
            pLeft(nullptr),
            pRight(nullptr),
            pParent(nullptr),
            isRed(false)
        {
        }

        BNode(T&& t) :
            data(std::move(t)),
            pLeft(nullptr),
            pRight(nullptr),
            pParent(nullptr),
            isRed(false)
        {
        }

        //
        // Insert
        //
        void addLeft(BNode* pNode);
        void addRight(BNode* pNode);
        void addLeft(const T& t);
        void addRight(const T& t);
        void addLeft(T&& t);
        void addRight(T&& t);

        //
        // Status
        //
        bool isRightChild(BNode* pNode) const
        {
            return pNode && pNode->pParent == this && pRight == pNode;
        }

        bool isLeftChild(BNode* pNode) const
        {
            return pNode && pNode->pParent == this && pLeft == pNode;
        }

        //
        // Data
        //
        T data;                  // Actual data stored in the BNode
        BNode* pLeft;            // Left child - smaller
        BNode* pRight;           // Right child - larger
        BNode* pParent;          // Parent
        bool isRed;              // Red-black balancing stuff
    };

    /**********************************************************
     * BINARY SEARCH TREE ITERATOR
     * Forward and reverse iterator through a BST
     *********************************************************/
    template <typename T>
    class BST<T>::iterator
    {
        friend class ::TestBST; // give unit tests access to the privates
        friend class ::TestMap;
        friend class ::TestSet;

        template <class KK, class VV>
        friend class map;

        template <class TT>
        friend class set;

    public:
        // constructors and assignment
        iterator(BNode* p = nullptr) :
            pNode(p)
        {
        }

        iterator(const iterator& rhs) :
            pNode(rhs.pNode)
        {
        }

        iterator& operator = (const iterator& rhs)
        {
            if (this != &rhs)
                pNode = rhs.pNode;
            return *this;
        }

        // compare
        bool operator == (const iterator& rhs) const
        {
            return pNode == rhs.pNode;
        }

        bool operator != (const iterator& rhs) const
        {
            return pNode != rhs.pNode;
        }

        // de-reference. Cannot change because it will invalidate the BST
        const T& operator * () const
        {
            assert(pNode != nullptr);
            return pNode->data;
        }

        // increment and decrement
        iterator& operator ++ ();
        iterator operator ++ (int postfix)
        {
            iterator temp(*this);
            ++(*this);
            return temp;
        }

        iterator& operator -- ();
        iterator operator -- (int postfix)
        {
            iterator temp(*this);
            --(*this);
            return temp;
        }

        // must give friend status to remove so it can call getNode() from it
        friend typename BST<T>::iterator BST<T>::erase(iterator& it);

    private:
        // the node
        BNode* pNode;
    };

    /*********************************************
     *********************************************
     *********************************************
     ******************** BST ********************
     *********************************************
     *********************************************
     *********************************************/

     /*********************************************
      * BST :: DEFAULT CONSTRUCTOR
      ********************************************/
    template <typename T>
    BST<T>::BST() :
        root(nullptr),
        numElements(0)
    {
    }

    /*********************************************
     * BST :: COPY CONSTRUCTOR
     * Copy one tree to another
     ********************************************/
    template <typename T>
    BST<T>::BST(const BST<T>& rhs) :
        root(nullptr),
        numElements(0)
    {
        *this = rhs;
    }

    /*********************************************
     * BST :: MOVE CONSTRUCTOR
     * Move one tree to another
     ********************************************/
    template <typename T>
    BST<T>::BST(BST<T>&& rhs) :
        root(rhs.root),
        numElements(rhs.numElements)
    {
        rhs.root = nullptr;
        rhs.numElements = 0;
    }

    /*********************************************
     * BST :: INITIALIZER LIST CONSTRUCTOR
     * Create a BST from an initializer list
     ********************************************/
    template <typename T>
    BST<T>::BST(const std::initializer_list<T>& il) :
        root(nullptr),
        numElements(0)
    {
        *this = il;
    }

    /*********************************************
     * BST :: DESTRUCTOR
     ********************************************/
    template <typename T>
    BST<T>::~BST()
    {
        clear();
    }

    /*********************************************
     * BST :: ASSIGNMENT OPERATOR
     * Copy one tree to another
     ********************************************/
    template <typename T>
    BST<T>& BST<T>::operator = (const BST<T>& rhs)
    {
        if (this == &rhs)
            return *this;

        // Define the recursive assign logic locally to reuse nodes
        auto recursiveAssign =
            [&](auto& self, BNode*& pDest, const BNode* pSrc, BNode* pParent) -> void
            {
                // 1. Source is empty: delete whatever is left in destination
                if (pSrc == nullptr)
                {
                    deleteBinaryTree(pDest);
                    return;
                }

                // 2. Destination is empty: create a new node (Copy Constructor)
                if (pDest == nullptr)
                {
                    pDest = new BNode(pSrc->data);
                    pDest->pParent = pParent;
                }
                // 3. Both exist: reuse the existing node (Assignment Operator)
                else
                {
                    pDest->data = pSrc->data;
                }

                // Recursively synchronize the children
                self(self, pDest->pLeft, pSrc->pLeft, pDest);
                self(self, pDest->pRight, pSrc->pRight, pDest);
            };

        // Synchronize the trees starting from the root
        recursiveAssign(recursiveAssign, this->root, rhs.root, nullptr);

        this->numElements = rhs.numElements;
        return *this;
    }

    /*********************************************
     * BST :: ASSIGNMENT OPERATOR with INITIALIZATION LIST
     * Copy nodes onto a BTree
     ********************************************/
    template <typename T>
    BST<T>& BST<T>::operator = (const std::initializer_list<T>& il)
    {
        clear();
        for (const T& t : il)
            insert(t);
        return *this;
    }

    /*********************************************
     * BST :: ASSIGN-MOVE OPERATOR
     * Move one tree to another
     ********************************************/
    template <typename T>
    BST<T>& BST<T>::operator = (BST<T>&& rhs)
    {
        clear();
        swap(rhs);
        return *this;
    }

    /*********************************************
     * BST :: SWAP
     * Swap two trees
     ********************************************/
    template <typename T>
    void BST<T>::swap(BST<T>& rhs)
    {
        BNode* tempRoot = rhs.root;
        rhs.root = root;
        root = tempRoot;

        size_t tempElements = rhs.numElements;
        rhs.numElements = numElements;
        numElements = tempElements;
    }

    /*****************************************************
     * BST :: INSERT
     * Insert a node at a given location in the tree
     ****************************************************/
    template <typename T>
    std::pair<typename BST<T>::iterator, bool>
        BST<T>::insert(const T& t, bool keepUnique)
    {
        // empty tree
        if (root == nullptr)
        {
            root = new BNode(t);
            numElements = 1;
            return std::pair<iterator, bool>(iterator(root), true);
        }

        BNode* pCur = root;
        BNode* pPar = nullptr;
        bool wentLeft = false;  // remember last direction

        if (keepUnique)
        {
            while (pCur != nullptr)
            {
                pPar = pCur;

                // must check equality first for keepUnique tests
                if (t == pCur->data)
                    return std::pair<iterator, bool>(iterator(pCur), false);

                wentLeft = (t < pCur->data);  // ONE < per level
                pCur = wentLeft ? pCur->pLeft : pCur->pRight;
            }
        }
        else
        {
            while (pCur != nullptr)
            {
                pPar = pCur;

                wentLeft = (t < pCur->data);  // ONE < per level
                pCur = wentLeft ? pCur->pLeft : pCur->pRight; // duplicates go right
            }
        }

        BNode* pNew = new BNode(t);
        pNew->pParent = pPar;

        // attach WITHOUT another <
        if (wentLeft)
            pPar->pLeft = pNew;
        else
            pPar->pRight = pNew;

        ++numElements;
        return std::pair<iterator, bool>(iterator(pNew), true);
    }

    template <typename T>
    std::pair<typename BST<T>::iterator, bool>
        BST<T>::insert(T&& t, bool keepUnique)
    {
        // empty tree
        if (root == nullptr)
        {
            root = new BNode(std::move(t));
            numElements = 1;
            return std::pair<iterator, bool>(iterator(root), true);
        }

        BNode* pCur = root;
        BNode* pPar = nullptr;
        bool wentLeft = false;  // remember last direction

        if (keepUnique)
        {
            while (pCur != nullptr)
            {
                pPar = pCur;

                if (t == pCur->data)
                    return std::pair<iterator, bool>(iterator(pCur), false);

                wentLeft = (t < pCur->data);  // ONE < per level
                pCur = wentLeft ? pCur->pLeft : pCur->pRight;
            }
        }
        else
        {
            while (pCur != nullptr)
            {
                pPar = pCur;

                wentLeft = (t < pCur->data);  // ONE < per level
                pCur = wentLeft ? pCur->pLeft : pCur->pRight;
            }
        }

        BNode* pNew = new BNode(std::move(t));
        pNew->pParent = pPar;

        // attach WITHOUT another <
        if (wentLeft)
            pPar->pLeft = pNew;
        else
            pPar->pRight = pNew;

        ++numElements;
        return std::pair<iterator, bool>(iterator(pNew), true);
    }

    /*************************************************
     * BST :: ERASE
     * Remove a given node as specified by the iterator
     ************************************************/
    template <typename T>
    typename BST<T>::iterator BST<T>::erase(iterator& it)
    {
        if (!it.pNode)
            return end();

        iterator itNext = it;
        itNext++;

        // Case 1: No children
        if (it.pNode->pLeft == nullptr && it.pNode->pRight == nullptr)
        {
            if (it.pNode->pParent == nullptr)
                root = nullptr;
            else if (it.pNode->pParent->pLeft == it.pNode)
                it.pNode->pParent->pLeft = nullptr;
            else
                it.pNode->pParent->pRight = nullptr;

            delete it.pNode;
        }
        // Case 2: Left child only
        else if (it.pNode->pRight == nullptr)
        {
            if (it.pNode->pParent == nullptr)
                root = it.pNode->pLeft;
            else if (it.pNode->pParent->pLeft == it.pNode)
                it.pNode->pParent->pLeft = it.pNode->pLeft;
            else
                it.pNode->pParent->pRight = it.pNode->pLeft;

            it.pNode->pLeft->pParent = it.pNode->pParent;
            delete it.pNode;
        }
        // Case 3: Right child only
        else if (it.pNode->pLeft == nullptr)
        {
            if (it.pNode->pParent == nullptr)
                root = it.pNode->pRight;
            else if (it.pNode->pParent->pLeft == it.pNode)
                it.pNode->pParent->pLeft = it.pNode->pRight;
            else
                it.pNode->pParent->pRight = it.pNode->pRight;

            it.pNode->pRight->pParent = it.pNode->pParent;
            delete it.pNode;
        }
        // Case 4: Two children
        else
        {
            // Unlink Successor from its current position
            if (itNext.pNode->pParent->pLeft == itNext.pNode)
                itNext.pNode->pParent->pLeft = itNext.pNode->pRight;
            else
                itNext.pNode->pParent->pRight = itNext.pNode->pRight;

            if (itNext.pNode->pRight)
                itNext.pNode->pRight->pParent = itNext.pNode->pParent;

            // Point Successor to it.pNode's old children
            itNext.pNode->pLeft = it.pNode->pLeft;
            if (it.pNode->pLeft)
                it.pNode->pLeft->pParent = itNext.pNode;

            itNext.pNode->pRight = it.pNode->pRight;
            if (it.pNode->pRight)
                it.pNode->pRight->pParent = itNext.pNode;

            // Point Successor to it.pNode's old parent
            itNext.pNode->pParent = it.pNode->pParent;
            if (it.pNode->pParent == nullptr)
                root = itNext.pNode;
            else if (it.pNode->pParent->pLeft == it.pNode)
                it.pNode->pParent->pLeft = itNext.pNode;
            else
                it.pNode->pParent->pRight = itNext.pNode;

            // Clean up
            delete it.pNode;
        }

        numElements--;
        return itNext;
    }

    /*****************************************************
     * BST :: CLEAR
     * Removes all the BNodes from a tree
     ****************************************************/
    template <typename T>
    void BST<T>::clear() noexcept
    {
        deleteBinaryTree(root);
        numElements = 0;
    }

    /*****************************************************
     * BST :: DELETE BINARY TREE
     * Core, recursive helper for CLEAR
     ****************************************************/
    template <typename T>
    void BST<T>::deleteBinaryTree(BNode*& pNode) noexcept
    {
        if (pNode == nullptr)
            return;

        deleteBinaryTree(pNode->pLeft);
        deleteBinaryTree(pNode->pRight);

        delete pNode;
        pNode = nullptr;
    }

    /*****************************************************
     * BST :: BEGIN
     * Return the first node (left-most) in a binary search tree
     ****************************************************/
    template <typename T>
    typename BST<T>::iterator BST<T>::begin() const noexcept
    {
        // Empty tree
        if (root == nullptr)
            return end();

        // Start at root, walk pLeft till null
        BNode* pCur = root;
        while (pCur->pLeft != nullptr)
            pCur = pCur->pLeft;

        return iterator(pCur);
    }

    /****************************************************
     * BST :: FIND
     * Return the node corresponding to a given value
     ****************************************************/
    template <typename T>
    typename BST<T>::iterator BST<T>::find(const T& t)
    {
        BNode* pCur = root;
        while (pCur != nullptr)
        {
            if (t == pCur->data)          // found it
                return iterator(pCur);
            else if (t < pCur->data)      // go left
                pCur = pCur->pLeft;
            else                          // go right
                pCur = pCur->pRight;
        }

        return end();
    }

    /******************************************************
     ******************************************************
     ******************************************************
     *********************** B NODE ***********************
     ******************************************************
     ******************************************************
     ******************************************************/

     /******************************************************
      * BINARY NODE :: ADD LEFT
      * Add a node to the left of the current node
      ******************************************************/
    template <typename T>
    void BST<T>::BNode::addLeft(BNode* pNode)
    {
        pLeft = pNode;
        if (pNode)
            pNode->pParent = this;
    }

    /******************************************************
     * BINARY NODE :: ADD RIGHT
     * Add a node to the right of the current node
     ******************************************************/
    template <typename T>
    void BST<T>::BNode::addRight(BNode* pNode)
    {
        pRight = pNode;
        if (pNode)
            pNode->pParent = this;
    }

    /******************************************************
     * BINARY NODE :: ADD LEFT
     * Add a node to the left of the current node
     ******************************************************/
    template <typename T>
    void BST<T>::BNode::addLeft(const T& t)
    {
        addLeft(new BNode(t));
    }

    /******************************************************
     * BINARY NODE :: ADD LEFT
     * Add a node to the left of the current node
     ******************************************************/
    template <typename T>
    void BST<T>::BNode::addLeft(T&& t)
    {
        addLeft(new BNode(std::move(t)));
    }

    /******************************************************
     * BINARY NODE :: ADD RIGHT
     * Add a node to the right of the current node
     ******************************************************/
    template <typename T>
    void BST<T>::BNode::addRight(const T& t)
    {
        addRight(new BNode(t));
    }

    /******************************************************
     * BINARY NODE :: ADD RIGHT
     * Add a node to the right of the current node
     ******************************************************/
    template <typename T>
    void BST<T>::BNode::addRight(T&& t)
    {
        addRight(new BNode(std::move(t)));
    }

    /*************************************************
     *************************************************
     *************************************************
     ****************** ITERATOR *********************
     *************************************************
     *************************************************
     *************************************************/

     /**************************************************
      * BST ITERATOR :: INCREMENT PREFIX
      * advance by one
      *************************************************/
    template <typename T>
    typename BST<T>::iterator& BST<T>::iterator::operator ++ ()
    {
        if (pNode == nullptr)
            return *this; // ++end() stays end()

        // if there is a right subtree, next is left-most of it
        if (pNode->pRight != nullptr)
        {
            pNode = pNode->pRight;
            while (pNode->pLeft != nullptr)
                pNode = pNode->pLeft;
            return *this;
        }

        // otherwise, climb until we come from a left child
        auto pCur = pNode;
        auto pPar = pNode->pParent;
        while (pPar != nullptr && pCur == pPar->pRight)
        {
            pCur = pPar;
            pPar = pPar->pParent;
        }

        pNode = pPar; // may be nullptr (end)
        return *this;
    }

    /**************************************************
     * BST ITERATOR :: DECREMENT PREFIX
     * advance by one
     *************************************************/
    template <typename T>
    typename BST<T>::iterator& BST<T>::iterator::operator -- ()
    {
        if (pNode == nullptr)
            return *this; // tests expect --end() stays end()

        // if there is a left subtree, prev is right-most of it
        if (pNode->pLeft != nullptr)
        {
            pNode = pNode->pLeft;
            while (pNode->pRight != nullptr)
                pNode = pNode->pRight;
            return *this;
        }

        // otherwise, climb until we come from a right child
        auto pCur = pNode;
        auto pPar = pNode->pParent;
        while (pPar != nullptr && pCur == pPar->pLeft)
        {
            pCur = pPar;
            pPar = pPar->pParent;
        }

        pNode = pPar; // may be nullptr (end)
        return *this;
    }

} // namespace custom