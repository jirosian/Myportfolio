#include "Announcement.h"
#include "JingleNet_announcer.h"
#include "Queue_base.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

//Class for queue to store announcements
class Queue:public Queue_base<Announcement> {
private:
    struct Node {
        Announcement data;
        Node* next;

        Node(const Announcement& announcement) : data(announcement), next(nullptr) {}
    };

    Node* head;
    Node* back;
    int count;


public:
    Queue() : head(nullptr), back(nullptr), count(0) {}

    ~Queue() {
        while (!isEmpty()) {
            Node* current = head;
            head = head->next;
            delete current;
        }
    }

    int size() const {
        return count;
    }

    bool isEmpty() const {
        return head == nullptr;
    }

    void enqueue(const Announcement& announcement) {
        Node* newNode = new Node(announcement);

        if (isEmpty()) {
            head = newNode;
            back = newNode;
        } else {
            back->next = newNode;
            back = newNode;
        }
        count++;
    }

    void dequeue() {
        if (isEmpty()) {
            cout << "Queue is empty in dequeue() function" << endl;
            return;
        }

        Node* temp = head;
        head = head->next;
        delete temp;
        count--;
        if (head == nullptr) back = nullptr;
     
    }

    const Announcement& front() const {
        if (isEmpty()) {
             cout << "Queue is empty in front() function" << endl;
             throw std::runtime_error("Queue is empty"); // Throw an exception if the queue is empty
            }     
        return head->data;
    }
};

//Class for handling the command and store an announcement into a queue.
class JingleNet {
private:
    Queue santa;
    Queue reindeer;
    Queue elf2;
    Queue elf1;
    Queue snowman;

public:
    JingleNet() {}

    // A function to send an announcement to a specific queue
    void send(const string& username, const Rank& rank, const string& message) {      
        Announcement newAM = Announcement(username, rank, message);

        switch (rank)
        {
        case Rank::SNOWMAN:         
            snowman.enqueue(newAM);      
            break;   
        case Rank::ELF1:
            elf1.enqueue(newAM);  
            break;     
        case Rank::ELF2:
            elf2.enqueue(newAM);   
            break;
        case Rank::REINDEER:
            reindeer.enqueue(newAM);
            break;
        case Rank::SANTA:
            santa.enqueue(newAM);      
            break;   
        default:
            cout << "Invalid username: " << username << endl;
            break;
        }       
    }

    // A function to remove all an announcement with specific username.
    void removeAll(const string& username) {
        int i = 0;
        int santaSize = santa.size();
        int reindeerSize = reindeer.size();
        int elf2Size = elf2.size();
        int elf1Size = elf1.size();
        int snowmanSize = snowman.size();

        while(i < santaSize) {
            if(santa.front().get_sender_name() == username) {
                santa.dequeue();
                i++;
            }
            else {
                santa.enqueue(santa.front());
                santa.dequeue();     
                i++; 
            }
        }
        i = 0; 
        while(i < reindeerSize) {
            if(reindeer.front().get_sender_name() == username) {
                reindeer.dequeue();
                i++;
            }
            else {
                reindeer.enqueue(reindeer.front());
                reindeer.dequeue();   
                i++;   
            }
        }
        i = 0;
        while(i < elf2Size) {
            if(elf2.front().get_sender_name() == username) {
                elf2.dequeue();
                i++;
            }
            else {
                elf2.enqueue(elf2.front());
                elf2.dequeue();   
                i++;   
            }
        }
        i = 0;
        while(i < elf1Size) {
            if(elf1.front().get_sender_name() == username) {
                elf1.dequeue();
                i++;
            }
            else {
                elf1.enqueue(elf1.front());
                elf1.dequeue();   
                i++;   
            }
        }
        i = 0;
         while(i < snowmanSize) {
            if(snowman.front().get_sender_name() == username) {
                snowman.dequeue();
                i++;
            }
            else {
                snowman.enqueue(snowman.front());
                snowman.dequeue();    
                i++;  
            }
        }
    }

    //A function to promote an announcement.
    void promoteAnnouncements(const string& username) {     
        Queue* queues[] = {&snowman, &elf1, &elf2, &reindeer, &santa};

        for(int i = 3; i >= 0; i--) {
            Queue& current = *(queues[i]); 
            Queue& next = *(queues[i+1]);

            int sizeOfCurrentQ = current.size();
            while(sizeOfCurrentQ > 0) {
                if(current.front().get_sender_name() == username) {
                    int rankValue = static_cast<int>(current.front().get_rank()); // Convert a rank to an integer.
                    rankValue++; // Increment the rank value by 1.
                    Rank updatedRank = static_cast<Rank>(rankValue); // Get an updated rank.
                    Announcement a = Announcement(current.front().get_sender_name(), updatedRank, current.front().get_text());
                    current.dequeue();
                    next.enqueue(a);
                }
                else {
                    current.enqueue(current.front());
                    current.dequeue();
                }
                sizeOfCurrentQ--;
            }
        }
    }

    //A class to dequeue an annoucement and announce the annoucement.
    vector<Announcement> announce(int n){
        vector<Announcement> announceList;
        while(n > 0) {
            if(!santa.isEmpty()) {
                Announcement a = santa.front();
                santa.dequeue();
                announceList.push_back(a);
            }
            else if(!reindeer.isEmpty()) {
                Announcement a = reindeer.front();
                reindeer.dequeue();
                announceList.push_back(a);
            }
            else if(!elf2.isEmpty()) {
                Announcement a = elf2.front();
                elf2.dequeue();
                announceList.push_back(a);
            }
            else if(!elf1.isEmpty()) {            
                Announcement a = elf1.front();
                elf1.dequeue();
                announceList.push_back(a);
            }
            else if(!snowman.isEmpty()) {
                Announcement a = snowman.front();
                snowman.dequeue();
                announceList.push_back(a);
            } 
            n--;           
        }
        return announceList;
    }
   
};

int main(int argc, char *argv[])
{
    if (argc != 2) return 1; 
    
    string filename = argv[1]; 
    ifstream infile(filename);
    string line;
    JingleNet jn;
   
    while (getline(infile, line))
    {
        int pos = line.find(' ');
        string cmd = line.substr(0, pos); // Get command such as SEND, REMOVE_ALL, etc...
        line = line.substr(pos + 1); // Get text for SEND, username for REMOVE_ALL and PROMOTE_ANNOUNCEMENTS, and number for ANNOUNCE
              
        if(cmd == "SEND") { 
            Announcement a = Announcement(line);         
            jn.send(a.get_sender_name(), a.get_rank(), a.get_text());
        }
        else if(cmd == "REMOVE_ALL") {   
            jn.removeAll(line);     
        }
        else if(cmd == "PROMOTE_ANNOUNCEMENTS") {        
            jn.promoteAnnouncements(line);
        }
        else if(cmd == "ANNOUNCE") {        
            vector<Announcement> announceList = jn.announce(stoi(line));
            int i = 0;      
            while(i < announceList.size()) {
                jnet.announce(announceList[i]);  
                i++;
            }
        }
        else {
            cout << cmd <<  "is unknown command: " << endl;
        }
       
    }  
}

