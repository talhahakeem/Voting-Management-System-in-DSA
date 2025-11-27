#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <queue>

using namespace std;

// Voter struct for linked list
struct Voter {
    string name;
    string id;
    Voter* next;
};

// Candidate struct for binary tree
struct Candidate {
    string name;
    int votes;
    Candidate* left;
    Candidate* right;

    Candidate(string n) : name(n), votes(0), left(nullptr), right(nullptr) {}
};

// Voting system class
class VotingSystem {
private:
    Voter* voterHead; // Linked list head
    Candidate* candidateRoot; // Binary tree root

    // Helper function to add a candidate to the tree
    Candidate* addCandidate(Candidate* root, string name) {
        if (root == nullptr) {
            return new Candidate(name);
        }
        if (name < root->name) {
            root->left = addCandidate(root->left, name);
        } else {
            root->right = addCandidate(root->right, name);
        }
        return root;
    }

    // Helper function to save candidates to a file
    void saveCandidates(Candidate* root, ofstream& outFile) {
        if (root) {
            saveCandidates(root->left, outFile);
            outFile << root->name << " " << root->votes << endl;
            saveCandidates(root->right, outFile);
        }
    }

    // Helper function to load candidates from a file
    void loadCandidates(ifstream& inFile) {
        string name;
        int votes;
        while (inFile >> name >> votes) {
            Candidate* newCandidate = new Candidate(name);
            newCandidate->votes = votes;
            candidateRoot = addCandidate(candidateRoot, name);
        }
    }

public:
    VotingSystem() : voterHead(nullptr), candidateRoot(nullptr) {}

    // Register a voter
    void registerVoter(string name, string id) {
        Voter* newVoter = new Voter{name, id, voterHead};
        voterHead = newVoter;
        cout << "Voter registered: " << name << endl;
    }

    // Add a candidate
    void addCandidate(string name) {
        candidateRoot = addCandidate(candidateRoot, name);
        cout << "Candidate added: " << name << endl;
    }

    // Cast a vote
    void castVote(string voterId, string candidateName) {
        // Check if voter is registered (simple implementation)
        Voter* current = voterHead;
        while (current) {
            if (current->id == voterId) {
                break;
            }
            current = current->next;
        }
        if (!current) {
            cout << "Voter not registered!" << endl;
            return;
        }

        // Find the candidate and increment vote count
        Candidate* currentCandidate = candidateRoot;
        while (currentCandidate) {
            if (candidateName == currentCandidate->name) {
                currentCandidate->votes++;
                cout << "Vote cast for " << candidateName << " by " << current->name << endl;
                return;
            }
            currentCandidate = (candidateName < currentCandidate->name) ? currentCandidate->left : currentCandidate->right;
        }
        cout << "Candidate not found!" << endl;
    }

    // Save candidates to a file
    void saveCandidatesToFile(const string& filename) {
        ofstream outFile(filename);
        if (outFile.is_open()) {
            saveCandidates(candidateRoot, outFile);
            outFile.close();
            cout << "Candidates saved to " << filename << endl;
        } else {
            cout << "Failed to open file!" << endl;
        }
    }

    // Load candidates from a file
    void loadCandidatesFromFile(const string& filename) {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            loadCandidates(inFile);
            inFile.close();
            cout << "Candidates loaded from " << filename << endl;
        } else {
            cout << "Failed to open file!" << endl;
        }
    }

    // Display candidate results
    void displayResults() {
        cout << "Candidate Results:" << endl;
        displayResults(candidateRoot);
    }

    // Helper function to display results
    void displayResults(Candidate* root) {
        if (root) {
            displayResults(root->left);
            cout << root->name << ": " << root->votes << " votes" << endl;
            displayResults(root->right);
        }
    }
};

// Main function
int main() {
    VotingSystem votingSystem;
    
    // Load candidates from file (if exists)
    votingSystem.loadCandidatesFromFile("candidates.txt");

    // Menu for interacting with the system
    while (true) {
        cout << "\nVoting Management System" << endl;
        cout << "1. Register Voter" << endl;
        cout << "2. Add Candidate" << endl;
        cout << "3. Cast Vote" << endl;
        cout << "4. Save Candidates" << endl;
        cout << "5. Display Results" << endl;
        cout << "6. Exit" << endl;
        cout << "Choose an option: ";
        
        int choice;
        cin >> choice;

        if (choice == 1) {
            string name, id;
            cout << "Enter voter name: ";
            cin >> name;
            cout << "Enter voter ID: ";
            cin >> id;
            votingSystem.registerVoter(name, id);
        } else if (choice == 2) {
            string name;
            cout << "Enter candidate name: ";
            cin >> name;
            votingSystem.addCandidate(name);
        } else if (choice == 3) {
            string voterId, candidateName;
            cout << "Enter voter ID: ";
            cin >> voterId;
            cout << "Enter candidate name: ";
            cin >> candidateName;
            votingSystem.castVote(voterId, candidateName);
        } else if (choice == 4) {
            votingSystem.saveCandidatesToFile("candidates.txt");
        } else if (choice == 5) {
            votingSystem.displayResults();
        } else if (choice == 6) {
            break;
        } else {
            cout << "Invalid choice!" << endl;
        }
    }

    return 0;
}
