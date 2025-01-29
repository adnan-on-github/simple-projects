#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class VotingSystem {
private:
    map<string, int> candidates;
    string adminUsername = "admin";
    string adminPassword = "admin123";
    string voterFile = "voters.txt";
    string candidateFile = "candidates.txt";

    void loadCandidates() {
        ifstream file(candidateFile);
        string name;
        int votes;
        while (file >> name >> votes) {
            candidates[name] = votes;
        }
        file.close();
    }

    void saveCandidates() {
        ofstream file(candidateFile);
        for (auto& pair : candidates) {
            file << pair.first << " " << pair.second << endl;
        }
        file.close();
    }

public:
    VotingSystem() {
        loadCandidates();
    }

    bool adminLogin(string username, string password) {
        return (username == adminUsername && password == adminPassword);
    }

    bool isVoterEligible(string voterId) {
        ifstream file(voterFile);
        string line;
        while (getline(file, line)) {
            if (line == voterId) {
                file.close();
                return false; // Already voted
            }
        }
        file.close();
        return true;
    }

    void addCandidate(string name) {
        candidates[name] = 0;
        saveCandidates();
    }

    void vote(string candidate) {
        if (candidates.find(candidate) != candidates.end()) {
            candidates[candidate]++;
            saveCandidates();
        } else {
            cout << "Invalid candidate!" << endl;
        }
    }

    void displayResults() {
        cout << "\n--- Election Results ---" << endl;
        for (auto& pair : candidates) {
            cout << pair.first << ": " << pair.second << " votes" << endl;
        }
    }
};

int main() {
    VotingSystem system;
    int choice;
    string username, password, voterId, candidate;

    do {
        cout << "\n1. Admin Login\n2. Vote\n3. Exit\nEnter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: // Admin login
                cout << "Username: ";
                cin >> username;
                cout << "Password: ";
                cin >> password;
                if (system.adminLogin(username, password)) {
                    cout << "Admin logged in!\n";
                    cout << "1. Add Candidate\n2. View Results\nEnter choice: ";
                    cin >> choice;
                    if (choice == 1) {
                        cout << "Enter candidate name: ";
                        cin >> candidate;
                        system.addCandidate(candidate);
                    } else if (choice == 2) {
                        system.displayResults();
                    }
                } else {
                    cout << "Invalid credentials!" << endl;
                }
                break;

            case 2: // Vote
                cout << "Enter your voter ID: ";
                cin >> voterId;
                if (system.isVoterEligible(voterId)) {
                    cout << "Enter candidate name: ";
                    cin >> candidate;
                    system.vote(candidate);
                    // Mark voter as having voted
                    ofstream file("voters.txt", ios::app);
                    file << voterId << endl;
                    file.close();
                    cout << "Vote recorded!" << endl;
                } else {
                    cout << "You have already voted!" << endl;
                }
                break;

            case 3:
                cout << "Exiting..." << endl;
                break;

            default:
                cout << "Invalid choice!" << endl;
        }
    } while (choice != 3);

    return 0;
}
