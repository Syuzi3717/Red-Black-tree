#ifndef RED_BLACK_TREE_
#define RED_BLACK_TREE_

#include <iostream>

enum _color { red , black };

template <typename T>
struct rbnode
{
    rbnode* parent;
    rbnode* leftchild;
    rbnode* rightchild;
    _color color;
    T value;
    rbnode() 
        : parent(nullptr)
        , leftchild(nullptr)
        , rightchild(nullptr)
        , color(red)
        , value()
    {}
    rbnode(const T& v) 
        : parent(nullptr)
        , leftchild(nullptr)
        , rightchild(nullptr)
        , color(red)
        , value(v)
    {}
};

template <typename T>
class rbtree 
{
public:
    rbtree() : nil(new rbnode<T>), root(nil) { nil->leftchild = nullptr; nil->parent = nullptr; nil->rightchild = nullptr; nil->color = black; }

public:
    void insert(const T&);
    rbnode<T>* search(const T&);
    void erase(const T&);
    void print();
private:
    void leftrotate(rbnode<T>*);
    void rightrotate(rbnode<T>*);
    void balanceinsert(rbnode<T>*);
    void balancedelete(rbnode<T>*);
    void deletee(rbnode<T>*); 
    rbnode<T>* findmover(rbnode<T>*);
    void printpostorder(rbnode<T>*);
    void printpreorder(rbnode<T>*);
    void printinorder(rbnode<T>*);
private:
    rbnode<T>* nil;
    rbnode<T>* root;
};

template <typename T>
rbnode<T>* rbtree<T>::search(const T& val)
{
    rbnode<T>* tmp = root;
    while(tmp != nil && val != tmp->value)
    {
        if(val < tmp->value)
        {
            tmp = tmp->leftchild;
        }
        else
        {
            tmp = tmp->rightchild;
        }
    }
    return tmp;
}

template <typename T>
void rbtree<T>::erase(const T& val)
{
    rbnode<T>* tmp = search(val);
    if(tmp != nil)
    {
        deletee(tmp);
    }
    else 
    {
        std::cout << "the " << val << " element is missing" << std::endl;
    }
}

template <typename T>
void rbtree<T>::insert(const T& val)
{
    rbnode<T>* ins = new rbnode<T>(val);
    rbnode<T>* tmp = root;
    rbnode<T>* wti = nil;
    while(tmp != nil)
    {
        wti = tmp;
        if(ins->value > tmp->value)
        {
            tmp = tmp->rightchild;
        }
        else 
        {
            tmp = tmp->leftchild;
        }
    }
    ins->parent = wti;
    if(wti == nil)
    {
        root = ins;
    }
    else 
    {
        if(ins->value > wti->value)
        {
            wti->rightchild = ins;
        }
        else wti->leftchild = ins;
    }
    ins->leftchild = nil;
    ins->rightchild = nil;
    ins->color = red;
    balanceinsert(ins);
}

template <typename T>
void rbtree<T>::leftrotate(rbnode<T>* no)
{
    rbnode<T>* tmp = no->rightchild;
    if(no->parent == nil)
    {
        root = tmp;
    }
    else
    {
        if(no == no->parent->leftchild)
        {
            no->parent->leftchild = tmp;
        }
        else 
        {
            no->parent->rightchild = tmp;
        }
    }
    tmp->parent = no->parent;
    no->rightchild = tmp->leftchild;
    tmp->leftchild->parent = no;
    tmp->leftchild = no;
    no->parent = tmp;
}

template <typename T>
void rbtree<T>::rightrotate(rbnode<T>* no)
{
    rbnode<T>* tmp = no->leftchild;
    if(no->parent == nil)
    {
        root = tmp;
    }
    else
    {
        if(no->parent->leftchild == no)
        {
            no->parent->leftchild = tmp;
        }
        else{
            no->parent->rightchild =tmp;
        }
    }
    tmp->parent = no->parent;
    no->leftchild = tmp->rightchild;
    tmp->rightchild->parent = no;
    tmp->rightchild = no;
    no->parent = tmp;
}

template <typename T>
void rbtree<T>::balanceinsert(rbnode<T>* no)
{
    while(no->parent->color == red)
    {
        if(no->parent == no->parent->parent->leftchild)
        {
            rbnode<T>* tmp = no->parent->parent->rightchild;
            if(tmp->color == red)
            {
                no->parent->color = black;
                tmp->color = black;
                no->parent->parent->color = red;
                no = no->parent->parent;
            }
            else
            {
                if(no == no->parent->rightchild)
                {
                    no = no->parent;
                    leftrotate(no);
                }
                no->parent->color = black;
                no->parent->parent->color = red;
                no->parent->rightchild->color = black;
                rightrotate(no->parent->parent);
            }
        }
        else 
        {
            rbnode<T>* tmp = no->parent->parent->leftchild;
            if(tmp->color == red)
            {
                no->parent->color = black;
                tmp->color = black;
                no->parent->parent->color = red;
                no = no->parent->parent;
            }
            else{
                if(no == no->parent->leftchild)
                {
                    no = no->parent;
                    rightrotate(no);
                }
                no->parent->color = black;
                no->parent->parent->color = red;
                no->parent->leftchild->color = black;
                leftrotate(no->parent->parent);
            }
        }
    }
    root->color = black;
}

template <typename T>
rbnode<T>* rbtree<T>::findmover(rbnode<T>* no)
{
    rbnode<T>* tmp = no;
    if(tmp->leftchild == nil)
    {
        tmp = no->rightchild;
        while(tmp->leftchild)
        {
            tmp = tmp->leftchild;
        }
        return tmp;
    }
    tmp = no->leftchild;
    while(tmp->rightchild != nil)
    {
        tmp = tmp->rightchild;
    }
    return tmp;
}

template <typename T>
void rbtree<T>::balancedelete(rbnode<T>* no)
{
    while(no != root && no->color == black)
    {
        rbnode<T>* tmp = nullptr;
        if(no->parent->leftchild == no)
        {
            tmp = no->parent->rightchild;
            if(tmp->color == red)
            {
                tmp->color = black;
                no->parent->color = red;
                leftrotate(no->parent);
                tmp = no->parent->rightchild;
            }
            if(tmp->leftchild->color == black && tmp->rightchild->color == black)
            {
                tmp->color = red;
                no = no->parent;
            }
            else 
            {
                if(tmp->rightchild->color == black)
                {
                    tmp->leftchild->color = black;
                    tmp->color = red;
                    rightrotate(tmp);
                    tmp = no->parent->rightchild;
                }
                tmp->color = no->parent->color;
                no->parent->color = black;
                tmp->rightchild->color = black;
                leftrotate(no->parent);
                no = root;
            }
        }
        else{
            tmp = no->parent->leftchild;
            if(tmp->color == red)
            {
                tmp->color = black;
                no->parent->color = red;
                rightrotate(no->parent);
                tmp = no->parent->leftchild;
            }
            if(tmp->rightchild->color == black && tmp->leftchild->color == black)
            {
                tmp->color = red;
                no = no->parent;
            }
            else 
            {
                if(tmp->leftchild->color == black)
                {
                    tmp->rightchild->color = black;
                    tmp->color = red;
                    leftrotate(tmp);
                    tmp = no->parent->leftchild;
                }
                tmp->color = no->parent->color;
                no->parent->color = black;
                tmp->leftchild->color = black;
                rightrotate(no->parent);
                no = root;

            }
        }
    }
    no->color = black;
}

template <typename T>
void rbtree<T>::deletee(rbnode<T>* no)
{
    rbnode<T>* tm1 = nil;
    rbnode<T>* tm2 = nil;
    if(no->leftchild == nil || no->rightchild == nil)
    {
        tm1 = no;
    }
    else
    {
        tm1 = findmover(no);
    }
    if(tm1->leftchild != nil)
    {
        tm2 = tm1->leftchild;
    }
    else{
        tm2 = tm1->rightchild;
    }
    tm2->parent = tm1->parent;
    if(tm1->parent == nil)
    {
        root = tm2;
    }
    else 
    {
        if(tm1 == tm1->parent->leftchild)
        {
            tm1->parent->leftchild = tm2;
        }
        else 
        {
            tm1->parent->rightchild = tm2;
        }
    }
    if(tm1 != no)
    {
        no->value = tm1->value;
    }
    if(tm1->color == black)
    {
        balancedelete(tm2);
    }
    delete tm1;
}

template <typename T>
void rbtree<T>::print()
{
    if(root != nil)
    {
        printinorder(root);
        std::cout << std::endl;
        printpreorder(root);
        std::cout << std::endl;
        printpostorder(root);
        std::cout << std::endl;
    }
    else     std::cout << "tree is empty" << std::endl;
}

template <typename T>
void rbtree<T>::printpostorder(rbnode<T>* no)
{
    if(no == nil) return;
    printpostorder(no->leftchild);
    printpostorder(no->rightchild);
    std::cout << no->value << " is child of " << no->parent->value <<" color is " << no->color<< " .   "<<std::endl;
}

template <typename T>
void rbtree<T>::printpreorder(rbnode<T>* no)
{
    if(no == nil) return;
    std::cout << no->value <<" is child of " << no->parent->value <<" color is " << no->color <<" .   "<<std::endl;
    printpreorder(no->leftchild);
    printpreorder(no->rightchild);
}

template <typename T>
void rbtree<T>::printinorder(rbnode<T>* no)
{
    if(no == nil) return;
    printinorder(no->leftchild);
    std::cout << no->value <<" is child of " << no->parent->value <<" color is " << no->color<< " .   "<<std::endl;
    printinorder(no->rightchild);
}

#endif