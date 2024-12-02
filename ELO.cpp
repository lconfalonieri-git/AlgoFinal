#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <chrono>
#include <thread>
using namespace std;

struct playerList {
    string name;
    int score; // Elo score
    playerList* next;
};

// Function to find a player by name
playerList* findPlayer(playerList* head, const string& playerName) {
    while (head != nullptr) {
        if (head->name == playerName) return head;
        head = head->next;
    }
    return nullptr; // Player not found
}

// Adds a player with default Elo score
void addPlayer(playerList*& head, const string& playerName) {
    playerList* newPlayer = new playerList{playerName, 1000, head}; // Default Elo score of 1000
    head = newPlayer;
}

// Updates Elo scores after a match
void updateElo(playerList* p1, playerList* p2, bool player1Wins) {
    const double K = 32.0; // K-factor
    double R1 = p1->score;
    double R2 = p2->score;

    double E1 = 1.0 / (1.0 + pow(10, (R2 - R1) / 400.0));
    double E2 = 1.0 / (1.0 + pow(10, (R1 - R2) / 400.0));

    if (player1Wins) {
        p1->score = round(R1 + K * (1 - E1));
        p2->score = round(R2 + K * (0 - E2));
    } else {
        p1->score = round(R1 + K * (0 - E1));
        p2->score = round(R2 + K * (1 - E2));
    }
}

// Simulates a game between two players
void playGame(playerList* head, const string& player1, const string& player2) {
    playerList* p1 = findPlayer(head, player1);
    playerList* p2 = findPlayer(head, player2);

    if (!p1 || !p2) {
        cout << "One or both players not found.\n";
        return;
    }

    // Randomly determine the winner
    bool player1Wins = rand() % 2;
    cout << (player1Wins ? player1 : player2) << " wins against " << (player1Wins ? player2 : player1) << "!\n";

    updateElo(p1, p2, player1Wins);
}

// Function to merge sort the list by score
playerList* sortedMerge(playerList* a, playerList* b) {
    playerList* result = nullptr;

    if (!a) return b;
    if (!b) return a;

    if (a->score >= b->score) {
        result = a;
        result->next = sortedMerge(a->next, b);
    } else {
        result = b;
        result->next = sortedMerge(a, b->next);
    }
    return result;
}

// Splits the linked list into two halves
void splitList(playerList* source, playerList** front, playerList** back) {
    playerList* slow = source;
    playerList* fast = source->next;

    while (fast) {
        fast = fast->next;
        if (fast) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *front = source;
    *back = slow->next;
    slow->next = nullptr;
}

// Sorts players by score using merge sort
void sortPlayersByScore(playerList** headRef) {
    playerList* head = *headRef;
    if (!head || !head->next) return;

    playerList *a, *b;
    splitList(head, &a, &b);

    sortPlayersByScore(&a);
    sortPlayersByScore(&b);

    *headRef = sortedMerge(a, b);
}

// Displays the leaderboard 
void printList(playerList* head) {
    cout << " __________________ \n";
    cout << "|   Name   | Score |\n";

    while (head) {
        string name = head->name;

        // Ensure name fits into the "Name" column
        if (name.length() > 10) {
            name = name.substr(0, 10);
        }

        int leftSpace = (10 - name.length()) / 2;
        int rightSpace = 10 - leftSpace - name.length();

        string space1(leftSpace, ' ');
        string space2(rightSpace, ' ');

        cout << "|" << space1 << name << space2 << "| " 
             << head->score << string(7 - to_string(head->score).length(), ' ') << "|\n";

        head = head->next;
    }
    cout << "|__________|_______|\n";
}
int main() {
    playerList* head = nullptr;

    srand(time(nullptr)); // Seed RNG

    // Automatically add 25 players
    for (int i = 1; i <= 25; i++) {
        addPlayer(head, "Player" + to_string(i));
    }

    // Simulate 200 matches
    for (int i = 0; i < 200; i++) {
        playerList* p1 = head;
        playerList* p2 = head;

        // Select two random players
        int p1Index = rand() % 25;
        int p2Index = rand() % 25;

        while (p1Index == p2Index) {
            p2Index = rand() % 25;
        }

        for (int j = 0; j < p1Index; j++) {
            p1 = p1->next;
        }

        for (int j = 0; j < p2Index; j++) {
            p2 = p2->next;
        }

        playGame(head, p1->name, p2->name);
    }

    // Sort and display leaderboard
    sortPlayersByScore(&head);
    cout << "Final Leaderboard:\n";
    printList(head);

    return 0;
}
