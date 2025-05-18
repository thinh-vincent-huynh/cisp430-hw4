#include <fstream>      //file stuff
#include <iostream>     //display to console stuff
using namespace std;
const int SPACE_AMOUNT  = 4;    //tab; changable
const int DELETE_AMOUNT = 3;    //delete how many times?

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
    }else{                  //non-empty queue
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

//recursively sift up to fulfill MAX heap requirement
void siftUp(atree* nn){
    if (nn->parent->value < nn->value){ //need to swap?
        int temp = nn->parent->value;
        nn->parent->value = nn->value;
        nn->value = temp;
    }
    if (nn->parent){        //not yet at root?
        siftUp(nn->parent); //recursive
    }
}

//insert new node into heap in the complete way
void insertIntoHeap(atree* root, int value){
    if (!root){ //empty heap
        root = new atree(value, nullptr, nullptr, nullptr);
        return;
    }

    //initializations
    aqueue* queue = nullptr;
    atree*  nn    = nullptr;    //last inserted leaf node

    enqueue(queue, root);
    while (true){   //traverse through the heap
        atree* curr = dequeue(queue);
        if (!curr->left){   //empty left child -> insert
            nn = curr->left = new atree(value, nullptr,
                nullptr, curr);
            break;
        }else{  //non-empty left child -> continue traversing
            enqueue(queue, curr->left);
        }
        if (!curr->right){  //empty right child -> insert
            nn = curr->right = new atree(value, nullptr,
                nullptr, curr);
            break;
        }else{  //non-empty right child -> continue traversing
            enqueue(queue, curr->right);
        }
    }
    while (queue){  //clean queue when done using
        atree* curr = dequeue(queue);
    }
    siftUp(nn);     //fulfill MAX requirement
}

//compare parent with their children, swap if necessary
void siftDown(atree* parent){
    atree* child = nullptr;
    if (!parent->left){         //only right child
        child = parent->right;
    }else if (!parent->right){  //only left child
        child = parent->left;
    }else{  //both children -> need to find max child
        child = (parent->left->value > parent->right->value)
               ? parent->left : parent->right;
    }

    if (parent->value < child->value){  //need to swap?
        int temp = parent->value;
        parent->value = child->value;
        child->value = temp;
    }
    if (child->left || child->right){   //not yet leaf node?
        siftDown(child);                //recursive
    }
}

//cut & paste last node to root, call recursive siftDown()
void deleteFromHeap(atree* root){
    //initializations
    aqueue* queue = nullptr;
    atree*  last  = nullptr;    //last node in heap

    enqueue(queue, root);
    while (queue){  //find last node in heap
        last = dequeue(queue);
        if (last->left){
            enqueue(queue, last->left);
        }
        if (last->right){
            enqueue(queue, last->right);
        }
    }
    root->value = last->value;  //replace root with last node
    delete last;                //cut & paste last node
    siftDown(root);             //fulfill MAX requirement
}

//necessary for proper spacing like a complete pyramid
int depth(atree* root){
    //initializations
    int nodes = 0;              //amount of nodes in heap
    aqueue* queue = nullptr;
    int depth = 0, two = 2;     //ceil( log2(nodes) )

    enqueue(queue, root);
    while (queue){  //count amount of nodes
        atree* curr = dequeue(queue);
        nodes++;
        if (curr->left){
            enqueue(queue, curr->left);
        }
        if (curr->right){
            enqueue(queue, curr->right);
        }
    }
    while (nodes >= two){   //ceil( log2(nodes) )
        depth++;
        two = two * 2;
    }
    return depth;
}

//this will make heap look like a complete pyramid
void spacing(int depth){
    for (int i = 0; i < depth * SPACE_AMOUNT; i++){
        cout << " ";
    }
}

//print the whole heap beautifully with proper spacing
void display(atree* root){
    //initializations
    int curr = depth(root); //necessary for proper spacing
    aqueue *currLevel = nullptr, *nextLevel = nullptr;

    enqueue(currLevel, root);
    while (currLevel){  //process if there are still childrens
        while (currLevel){  //process all nodes in currLevel
            if (currLevel->n->left){
                enqueue(nextLevel, currLevel->n->left);
            }
            if (currLevel->n->right){
                enqueue(nextLevel, currLevel->n->right);
            }
            //print value with proper spacing
            spacing(curr);
            cout << dequeue(currLevel)->value;
            spacing(curr);
        }
        currLevel = nextLevel;  //move onto nextLevel
        nextLevel = nullptr;    //empty it for the next level
        cout << endl;
        curr = curr / 2;        //half spacing of prev level
    }
}

int main(){
    //initializations
    ifstream in("input.txt");   //file
    int key;                    //insert
    atree* tree = nullptr;      //heap

    //populate from file and insert into & display heap
    while (in >> key){
        insertIntoHeap(tree, key);
        display(tree);
    }

    //delete from & display heap
    for (int i = 0; i < DELETE_AMOUNT; i++){
        deleteFromHeap(tree);
        display(tree);
    }
    
    return 0;
}