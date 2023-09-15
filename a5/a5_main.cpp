// a5_main.cpp

#include "Wordlist.h"

//
// test_read() is a helper function that reads words, one at a time, from cin,
// into a Wordlist and print the stats. You can use it to help test your
// Wordlist class, e.g.:
//
//    > make a5_main
//    g++ -std=c++17 -Wall -Wextra -Werror -Wfatal-errors -Wno-sign-compare -Wnon-virtual-dtor -g   a5_main.cpp   -o a5_main
//   
//    > ./a5_main < tiny_shakespeare.txt
//    Number of different words: 25670
//        Total number of words: 202651
//           Most frequent word: the 5437
//         Number of singletons: 14919 (58%)

 void test_read()
 {
   
    Wordlist lst("tiny_shakespeare.txt");
    //Wordlist lst("small.txt");
    lst.print_stats();
    cout << lst.is_sorted() << endl;
   // lst.print_words();

// lst.print_words();
   // cout << endl;
  //  lst.print_stats();
  //  cout << "///////////////////// Traversal ////////////////////" << endl;
  //  lst.inOrderTraversal(lst.root);
  //  cout << "///////////////////// Traversal ////////////////////" << endl;
     
 }

int main()
{
    test_read();
  


}
