#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
using namespace std;

struct playerList {
    string name;
    int score;
    playerList* next;
};

// Helper function to find a player by name
playerList* findPlayer(playerList* head, const string& playerName) {
    playerList* current = head;
    while (current != nullptr) {
        if (current->name == playerName) {
            return current;
        }
        current = current->next;
    }
    return nullptr; // Player not found
}

// Adds a player to the leaderboard with an initial 0-0 record
void addPlayer(playerList*& head, const string& playerName) {
    playerList* newPlayer = new playerList{playerName, 0, head};
    head = newPlayer;
}

// Simulates a game between two players and updates their record
void playGame(playerList* head, const string& player1, const string& player2) {
    // Seed the random number generator
    srand(time(nullptr));
    
    // Find both players in the list
    playerList* p1 = findPlayer(head, player1);
    playerList* p2 = findPlayer(head, player2);

    // Check if both players were found
    if (p1 == nullptr || p2 == nullptr) {
        cout << "One or both players not found in the list." << endl;
        return;
    }

    // Randomly choose a winner
    int winner = rand() % 2; // 0 for player1, 1 for player2
    
    if (winner == 0) {
        // player1 wins, player2 loses
        p1->score += 1;
        if (p2->score > 0) {
            p2->score -= 1;
        }
    } else {
        // player2 wins, player1 loses
        p2->score += 1;
        if (p1->score > 0) {
            p1->score -= 1;
        }
    }

    cout << "Game result: " << (winner == 0 ? player1 : player2) << " won, "
         << (winner == 0 ? player2 : player1) << " lost." << endl;
}

///////////////////////////////////////////////////////////////////////////////////
// BEGIN MERGE SORT

playerList* sortedMerge(playerList* a, playerList* b) {
    playerList* result = nullptr;

    if (a == nullptr) return b;
    else if (b == nullptr) return a;

    if (a->score >= b->score) {
        result = a;
        result->next = sortedMerge(a->next, b);
    } else {
        result = b;
        result->next = sortedMerge(a, b->next);
    }
    return result;
}

// Function to split the list into two halves
void splitList(playerList* source, playerList** front, playerList** back) {
    playerList* slow = source;
    playerList* fast = source->next;

    while (fast != nullptr) {
        fast = fast->next;
        if (fast != nullptr) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *front = source;
    *back = slow->next;
    slow->next = nullptr;
}

// Function to sort the list using merge sort
void sortPlayersByScore(playerList** headRef) {
    playerList* head = *headRef;
    playerList* a;
    playerList* b;

    if ((head == nullptr) || (head->next == nullptr)) {
        return;
    }

    splitList(head, &a, &b);
    sortPlayersByScore(&a);
    sortPlayersByScore(&b);
    *headRef = sortedMerge(a, b);
}

// END MERGE SORT
///////////////////////////////////////////////////////////////////////////////////

// Displays the leaderboard
void printList(playerList* head) {
    int totalLength = 10;

    while (head != nullptr) {
        string name = head->name;

        int leftSpace = (totalLength - name.length())/2;
        int rightSpace = totalLength - leftSpace - name.length();

        string space1(leftSpace, ' ');
        string space2(rightSpace, ' ');

        cout << "|" << space1 << head->name << space2 << "|   " << head->score << "   |" << endl;
        head = head->next;
    }
    cout << "|__________|_______|\n";
}



int main() {
    playerList* head = nullptr;
    string choice;

    srand(time(nullptr)); // Seed the random number generator
    int playerCounter = 0;
    while (true) {
        system("cls");
        cout << "     Leaderboard" << endl;
        cout << " __________________ \n";
        cout << "|   Name   | Score |\n";
        sortPlayersByScore(&head);
        printList(head);

        for(int i = 10; i > playerCounter; i--){
            cout << "\n";
        }

        cout << "Options:\n";
        cout << "1. Add a player\n";
        cout << "2. Simulate a game\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == "1") {
            // Add a player
            if (playerCounter == 10){
                cout << "Max players met.\n";
            } else{
                playerCounter++;
                string playerName;
                cout << "Enter player's name: ";
                cin >> playerName;
                if (playerName.length() >= 10){
                    cout << "Name too long\n";
                } else{
                    addPlayer(head, playerName);
                    cout << "Player " << playerName << " added.\n";
                }
            }

        } else if (choice == "2") {
            // Simulate a game
            if(playerCounter <=1){
                cout << "Not enough players.\n";
            } else{
                string player1, player2;
                cout << "Enter the first player's name: ";
                cin >> player1;
                cout << "Enter the second player's name: ";
                cin >> player2;

                playGame(head, player1, player2);
            }

        } else if (choice == "3") {
            // Exit
            cout << "Exiting program.\n";
            break;

        } else {
            cout << "Invalid choice, please try again.\n";
        }
        std::this_thread::sleep_for(chrono::seconds(1));

    }

    return 0;
}