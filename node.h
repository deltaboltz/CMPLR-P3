
#ifndef NODE_H
#define NODE_H
#include <set>
#include <vector>
#include "token.h"

template <class T>
class node {
  private:
      std::vector<node<T>> children_;
      std::vector<token> tokens_;
      T key_;
      void preHelper(std::ostream& out, std::string indent);

  public:
      // AST functionality
      void insert(node<T> child);
      void insert(token childToken);
      node<T> (T key);
      // traversal functionality
      void preOrder(std::ostream& out);
      void preOrder();
      std::string toString();
};

template <class T>
node<T>::node(T key)
{
  key_ = key; // Initialize the node name
}

template <class T>
void node<T>::insert(node<T> child)
{
  children_.push_back(child);
}

template <class T>
void node<T>::insert(token childToken)
{
  tokens_.push_back(childToken);
}

template <class T>
std::string node<T>::toString()
{
if (tokens_.size() > 0)
  {
    std::string tokenStr("");
    int i = 0;
    while (i < tokens_.size()-1)
    {
      tokenStr += tokens_[i].toString() + ", ";
      i++;
    }
    tokenStr += tokens_[i].toString() + "\n";
    return key_ + "  Tokens: " + tokenStr;
  }
  return key_ + "\n";
}

template <class T>
void node<T>::preHelper(std::ostream& out, std::string indent)
{
  // helper for preOrder
  std::string newdent("  ");
  newdent += indent;
  out << indent << toString();
  for (int i = 0; i < children_.size(); i++)
  {
      children_[i].preHelper(out, newdent);
  }
}

// traversal methods with variable ostream
template <class T>
void node<T>::preOrder(std::ostream& out)
{
  preHelper(out, "");
}

// traversal methods with default std::cout stream
template <class T>
void node<T>::preOrder()
{
  preHelper(std::cout, "");
}

#endif
