/***********************************************************************
* Header:
*    Set
* Summary:
*      __       ____       ____         __
*     /  |    .'    '.   .'    '.   _  / /
*     `| |   |  .--.  | |  .--.  | (_)/ /
*      | |   | |    | | | |    | |   / / _
*     _| |_  |  `--'  | |  `--'  |  / / (_)
*    |_____|  '.____.'   '.____.'  /_/
*
*    This will contain the class definition of:
*        set                 : A class that represents a Set
*        set::iterator       : An iterator through Set
* Author
*    Michael, James, Brayden
************************************************************************/


#pragma once

#include <cassert>
#include <iostream>
#include "bst.h"
#include <memory>     // for std::allocator
#include <functional> // for std::less

class TestSet;        // forward declaration for unit tests

namespace custom
{

/************************************************
 * SET
 * A class that represents a Set
 ***********************************************/
template <typename T>
class set
{
   friend class ::TestSet; // give unit tests access to the privates
public:
   
   // 
   // Construct
   //
   set() 
   { 
      // ---------- (Michael Code to Complete) ----------
   }
   set(const set &  rhs)
   { 
      // ---------- (Michael Code to Complete) ----------
      bst = rhs.bst;
   }
   set(set && rhs) 
   { 
      // ---------- (Michael Code to Complete) ----------
      bst = std::move(rhs.bst);
   }
   set(const std::initializer_list <T> & il) 
   {
      // ---------- (Michael Code to Complete) ----------
      insert(il);
   }
   template <class Iterator>
   set(Iterator first, Iterator last) 
   {
      // ---------- (Michael Code to Complete) ----------
      insert(first, last);
   }
  ~set() { }

   //
   // Assign
   //

   set & operator = (const set & rhs)
   {
      // ---------- (Michael Code to Complete) ----------
      if (this != &rhs)
         bst = rhs.bst;
      return *this;
   }
   set & operator = (set && rhs)
   {
      // ---------- (Michael Code to Complete) ----------
      if (this != &rhs)
         bst = std::move(rhs.bst);
      return *this;
   }
   set & operator = (const std::initializer_list <T> & il)
   {
      // ---------- (Michael Code to Complete) ----------
      clear();
      insert(il);
      return *this;
   }
   void swap(set& rhs) noexcept
   {
      // ---------- (Michael Code to Complete) ----------
      bst.swap(rhs.bst);
   }

   //
   // Iterator
   //

   class iterator;
   iterator begin() const noexcept 
   { 
      // ---------- (Brayden Code to Complete) ----------
      return iterator(); 
   }
   iterator end() const noexcept 
   { 
      // ---------- (Brayden Code to Complete) ----------
      return iterator(); 
   }

   //
   // Access
   //
   iterator find(const T& t) 
   { 
      // ---------- (James Code to Complete) ----------
      return iterator(bst.find(t)); 
   }

   //
   // Status
   //
   bool   empty() const noexcept 
   { 
      // ---------- (James Code to Complete) ----------
      return bst.empty();    
   }
   size_t size() const noexcept 
   { 
      // ---------- (James Code to Complete) ----------
      return bst.size();
   }

   //
   // Insert
   //
   std::pair<iterator, bool> insert(const T& t)
   {
      // ---------- (James Code to Complete) ----------
      std::pair<iterator, bool> p(iterator(), true);
      return p;
   }
   std::pair<iterator, bool> insert(T&& t)
   {
      // ---------- (James Code to Complete) ----------
      std::pair<iterator, bool> p(iterator(), true);
      return p;
   }
   void insert(const std::initializer_list <T>& il)
   {
      // ---------- (James Code to Complete) ----------
   }
   template <class Iterator>
   void insert(Iterator first, Iterator last)
   {
      // ---------- (James Code to Complete) ----------
   }


   //
   // Remove
   //
   void clear() noexcept 
   { 
      // ---------- (Michael Code to Complete) ----------
      bst.clear();
   }
   iterator erase(iterator &it)
   { 
      // ---------- (Michael Code to Complete) ----------
      if (it.it.pNode == nullptr)
         return end();

      auto itBST = it.it;
      auto itNextBST = bst.erase(itBST);
      return iterator(itNextBST);
   }
   size_t erase(const T & t) 
   {
      // ---------- (Michael Code to Complete) ----------
      iterator it = find(t);
      if (it == end())
         return 0;

      erase(it);
      return 1;
   }
   iterator erase(iterator &itBegin, iterator &itEnd)
   {
      // ---------- (Brayden Code to Complete) ----------
      return iterator();
   }

private:
   
   custom::BST <T> bst;
};


/**************************************************
 * SET ITERATOR
 * An iterator through Set
 *************************************************/
template <typename T>
class set <T> :: iterator
{
   friend class ::TestSet; // give unit tests access to the privates
   friend class custom::set<T>;

public:
   // constructors, destructors, and assignment operator
   iterator() 
   { 
      // ---------- (Michael Code to Complete) ----------
      it = typename custom::BST<T>::iterator(nullptr);
   }
   iterator(const typename custom::BST<T>::iterator& itRHS) 
   {  
      // ---------- (Michael Code to Complete) ----------
      it = itRHS;
   }
   iterator(const iterator & rhs) 
   { 
      // ---------- (Brayden Code to Complete) ----------
   }
   iterator & operator = (const iterator & rhs)
   {
      // ---------- (Brayden Code to Complete) ----------
      return *this;
   }

   // equals, not equals operator
   bool operator != (const iterator & rhs) const 
   { 
      // ---------- (Brayden Code to Complete) ----------
      return true;
   }
   bool operator == (const iterator & rhs) const 
   { 
      // ---------- (Brayden Code to Complete) ----------
      return true; 
   }

   // dereference operator: by-reference so we can modify the Set
   const T & operator * () const 
   { 
      // ---------- (Brayden Code to Complete) ----------
      return *(new T); 
   }

   // prefix increment
   iterator & operator ++ ()
   {
      // ---------- (Brayden Code to Complete) ----------
      return *this;
   }

   // postfix increment
   iterator operator++ (int postfix)
   {
      // ---------- (Brayden Code to Complete) ----------
      return *this;
   }
   
   // prefix decrement
   iterator & operator -- ()
   {
      // ---------- (Brayden Code to Complete) ----------
      return *this;
   }
   
   // postfix decrement
   iterator operator-- (int postfix)
   {
      // ---------- (Brayden Code to Complete) ----------
      return *this;
   }
   
private:

   typename custom::BST<T>::iterator it;
};


}; // namespace custom