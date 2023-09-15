// Wordlist.h

#pragma once

/////////////////////////////////////////////////////////////////////////
//
// Student Info
// ------------
//
// Name : <Sayaka Gozkara>
// St.# : <301592385>
// Email: <sga193@sfu.ca>
//
//
// Statement of Originality
// ------------------------
//
// All the code and comments below are my own original work. For any non-
// original work, I have provided citations in the comments with enough
// detail so that someone can see the exact source and extent of the
// borrowed work.
//
// In addition, I have not shared this work with anyone else, and I have
// not seen solutions from other students, tutors, websites, books,
// etc.
//
/////////////////////////////////////////////////////////////////////////

//
// Do not use any other #includes
//
#include "Wordlist_base.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>


using namespace std;

class Wordlist : public Wordlist_base
{
    private: 
        /*
        * Node of AVL tree.
        */
        struct Node
        {
            string word;
            int count;
            Node* left;
            Node* right;
            int height;

            Node(const string& w) : word(w), count(1), left(nullptr), right(nullptr), height(1) {}
        };

    public: 
        Node *root = nullptr;
        int totalWords;
        int totalUniqueWords;

        //Helper functions (Implementations for those are outside of the class.)
        void deleteAVLTree(Node* node);
        void inOrderTraversal(Node* node) const;
        bool is_sorted(Node* root, string* prevWord) const;
        void most_frequent(Node* node, Node*& mostFrequentNode, int& maxCount) const;
        int num_singletons(Node* node) const;

        /*
        * Default constructor 
        */
        Wordlist() { 
            root = nullptr;
            totalWords = 0;
            totalUniqueWords = 0;
        }

        /*
        * Constructor that takes a file and make a wordlist.
        */
        Wordlist(const string& filename) : Wordlist() {
            ifstream file(filename);
            if (!file.is_open()) {
                cout << "Error to open " << filename << endl;
                return;
            }

            string word;
            while (file >> word) {
                if (!word.empty()) add_word(word);  
            }

            file.close();
        }

        /*
         * Destructor that deletes AVL tree. 
        */
        ~Wordlist() {
            deleteAVLTree(root);
        }  

        /*
         * This returns height of a node.
        */
        int GetHeight(Node* node) {
            if(!node) return 0;
            else return node->height;
        }

        /*
        * This returns a balance of a node.
        */
        int GetBalanceFactor(Node* node) {
            return GetHeight(node->left) - GetHeight(node->right);
        }

       /*
        * This rotates a substree of a tree if the tree needs to be balanced.
        */
        Node* leftRotate(Node* x) {
            Node* y = x->right;
            Node* z = y->left;

            // Rotate nodes 
            y->left = x;
            x->right = z;

            // Renew heights
            x->height = max(GetHeight(x->left), GetHeight(x->right)) + 1;
            y->height = max(GetHeight(y->left), GetHeight(y->right)) + 1;

            return y;
        }

       /*
        * This rotates a substree of a tree if the tree needs to be balanced.
        */
        Node* rightRotate(Node* y) {
            Node* x = y->left;
            Node* z = x->right;

            // Rotate nodes 
            x->right = y;
            y->left = z;

            // Renew heights        
            y->height = max(GetHeight(y->left), GetHeight(y->right)) + 1;
            x->height = max(GetHeight(x->left), GetHeight(x->right)) + 1;

            return x;
        }

       /*
        * This inserts a node into AVL tree. It will balance the tree if it finds unbalanced during the inserting.
        */
        Node* insert_word(Node* node, const string& w) {
            // If the tree is empty, insert a new node.
            if (!node) {
                totalUniqueWords++;      
                return new Node(w);
            }
            
            if (w < node->word) node->left = insert_word(node->left, w);
            else if (w > node->word) node->right = insert_word(node->right, w);
            else {  // If the word is already in a tree , only increment a count of node.   
                node->count++;   
                return node;
            }

            // Get the balance factor to check if this node is balanced or not.
            int balance = GetBalanceFactor(node);

            // Renew the present node's height.
            node->height = max(GetHeight(node->left), GetHeight(node->right)) + 1;

            // Do LL Rotatation 
            if (balance > 1 && w < node->left->word) {
                return rightRotate(node);
            }

            // Do RR Rotation
            if (balance < -1 && w > node->right->word) {
                return leftRotate(node);
            }

            // Do LR Rotation 
            if (balance > 1 && w > node->left->word) {
                node->left = leftRotate(node->left);
                return rightRotate(node);
            }

            // Do RL rotation
            if (balance < -1 && w < node->right->word) {
                node->right = rightRotate(node->right);
                return leftRotate(node);
            }

            return node;
        }

        /*
        * This function returns total number of the occurences of a specific word "w" in the word list.
        */
        int get_count(const string &w) const override {
                Node* node = root;
                while (node) {
                    if (w < node->word) node = node->left; //Traverse left node
                    else if (w > node->word) node = node->right; //Traverse right node
                    else return node->count;                   
                }
                return 0;
        }

        /*
         * This function returns the number of nodes in the Wordlist.
        */
        int num_different_words() const override {
            return totalUniqueWords;
        }

        /*
         * This function returns the number of total words in the Wordlist.
        */
        int total_words() const override {
            return totalWords;
        }

        /*
         * This function checks if the AVL tree is Binary Search Tree. If not, return false.
        */
        bool is_sorted() const override {
            string prevWord = "";
            return is_sorted(root, &prevWord);
        }

        /*
        * This returns the most frequent word occured in a Wordlist. 
        */
        string most_frequent() const override {
            if(!root) return ""; 

            Node* mostFreqNode = nullptr;
            int maxCount = 0;

            most_frequent(root, mostFreqNode, maxCount);
            return mostFreqNode->word + " " + to_string(mostFreqNode->count); 
        }

        /*
         * This returns the number of words that occured only one time in a Wordlist.
        */
        int num_singletons() const override {
            Node* n = root;
            return num_singletons(n);
        }

        /*
        * This functions insert a word in an AVL tree. 
        */
        void add_word(const string &w) override {
            root = insert_word(root, w);
            totalWords++;
        
            return;
        }

        /*
         * This function prints words in a Wordlist. 
        */
        void print_words() const override {
            inOrderTraversal(root);
            return;
        }


}; // class Wordlist

//
// Make sure to thoroughly test your code as you go!
//

/*
* This deletes every node to delete AVL tree. A helper function of destructor. 
*/
void Wordlist::deleteAVLTree(Node* node) {
    if(!node) return;

    deleteAVLTree(node->left);
    deleteAVLTree(node->right);

    delete node;
} 

/*
* This traverse a tree and print each node. (Helper function of print_words())
*/
void Wordlist::inOrderTraversal(Node* node) const {
    if (node) {
        inOrderTraversal(node->left);
        cout << "{\"" << node->word << "\", " << node->count << "}" << endl;
        inOrderTraversal(node->right);
    }
}


 /*
  *  This is a helper function of is_sorted() to traverse a tree to determine if the tree is BST or not.
  */ 
bool Wordlist::is_sorted(Node* root, string* prevWord) const {
    if(!root) return true; 

    if(!is_sorted(root->left, prevWord)) return false; 

    if(root->word <= *prevWord) return false; 

    *prevWord = root->word;

    return is_sorted(root->right,prevWord);
}

 /*
* This is a helper function for most_frequent_h
*/
void Wordlist::most_frequent(Node* node, Node*& mostFrequentNode, int& maxCount) const {
    if(!node) return;

    most_frequent(node->left, mostFrequentNode, maxCount);  //Traverse left substree.

    if (node->count > maxCount) {
                mostFrequentNode = node;
                maxCount = node->count;
    }

    most_frequent(node->right, mostFrequentNode, maxCount);  // Traverse right subtree
}

/*
* This is a helper function of num_singletons() that traverse an AVL tree and counts number of 
* words that occurs only once. 
*/
int Wordlist::num_singletons(Node* node) const {
    if (!node) return 0;

    int leftCount = num_singletons(node->left);
    int rightCount = num_singletons(node->right);
            
    if(node->count == 1) return 1 + leftCount + rightCount;
    else return leftCount + rightCount;
}