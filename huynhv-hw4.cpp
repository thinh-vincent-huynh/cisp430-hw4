#include <fstream>
#include <iostream>
using namespace std;

//a tree node struct is sufficient to model a tree
struct atree{
    int value;
    atree* left;
    atree* right;
    atree* parent;
    atree(int _value, atree* _left,
       atree* _right, atree* _parent){
        value  = _value;
        left   = _left;
        right  = _right;
        parent = _parent;
    }
};

//work just like a singly linked list
struct aqueue{
    atree* n;
    aqueue* next;
    aqueue(atree* _n, aqueue* _next){
        n    = _n;
        next = _next;
    }
};

//enqueue at the end of queue
void enqueue(aqueue* queue, atree* tree){
    if (!queue){            //empty queue
        queue = new aqueue(tree, nullptr);
    }else{
        aqueue* curr = queue;
        while (curr->next){ //traverse through queue
            curr = curr->next;
        }
        curr->next = new aqueue(tree, nullptr);
    }
}

//move front to the next one in queue & delete current front
atree* dequeue(aqueue* queue){
    aqueue* curr = queue;
    atree* tree = queue->n;
    queue = queue->next;
    delete curr;
    return tree;
}

void siftUp(atree* nn){
    if (nn->parent->value < nn->value){
        int temp = nn->parent->value;
        nn->parent->value = nn->value;
        nn->value = temp;
    }
    if (nn->parent){
        siftUp(nn->parent);
    }
}

//insert new node into heap in the complete way
void insertIntoHeap(atree* root, int value){
    if (!root){
        root = new atree(value, nullptr, nullptr, nullptr);
        return;
    }
    aqueue* queue = nullptr;
    atree*  nn    = nullptr;
    enqueue(queue, root);
    while (true){
        atree* curr = dequeue(queue);
        if (!curr->left){
            nn = curr->left = new atree(value, nullptr,
                nullptr, curr);
            break;
        }else{
            enqueue(queue, curr->left);
        }
        if (!curr->right){
            nn = curr->right = new atree(value, nullptr,
                nullptr, curr);
            break;
        }else{
            enqueue(queue, curr->right);
        }
    }
    siftUp(nn);
}

void siftDown(atree* parent){
    atree* child = nullptr;
    if (!parent->left){
        child = parent->right;
    }else if (!parent->right){
        child = parent->left;
    }else{
        child = (parent->left->value > parent->right->value)
               ? parent->left : parent->right;
    }

    if (parent->value < child->value){
        int temp = parent->value;
        parent->value = child->value;
        child->value = temp;
    }
    if (child->left || child->right){
        siftDown(child);
    }
}

void deleteFromHeap(atree* root){
    aqueue* queue = nullptr;
    atree*  last  = nullptr;
    enqueue(queue, root);
    while (queue){
        last = dequeue(queue);
        if (last->left){
            enqueue(queue, last->left);
        }
        if (last->right){
            enqueue(queue, last->right);
        }
    }
    root->value = last->value;
    delete last;
    siftDown(root);
}

// display