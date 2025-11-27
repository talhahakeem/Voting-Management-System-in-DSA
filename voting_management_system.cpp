#include <iostream>
#include <string>
#include <conio.h>
#include <unordered_map>
#include <fstream>
#include <limits>
#include <windows.h>
using namespace std;
void beepOnError()
{
    Beep(1000, 500); // Frequency: 1000 Hz, Duration: 500 ms
}
class VoterInformation
{
public:
    string id;
    string name;
    int age;
    bool has_voted = false;
    VoterInformation *next = nullptr;
    void display() const
    {
        cout << "\n--- Voter Details ---\n";
        cout << "Name: " << name << "\n";
        cout << "ID: " << id << "\n";
        cout << "Age: " << age << "\n";
        cout << "Has Voted: " << (has_voted ? "Yes" : "No") << "\n";
    }
};
class CandidateInformation
{
public:
    string name;
    int age;
    string education;
    int nationality;
    bool criminal_record;
    string party_symbol;
    string party_name;
    int vote_count = 0;
    CandidateInformation *next = nullptr;
    void display() const
    {
        cout << "\n--- Candidate Details ---\n";
        cout << "Name: " << name << "\n";
        cout << "Age: " << age << "\n";
        cout << "Education Level: " << education << "\n";
        cout << "Nationality: " << (nationality == 1 ? "Pakistani" : "Others") << "\n";
        cout << "Criminal Record: " << (criminal_record ? "Yes" : "No") << "\n";
        cout << "Party Symbol: " << party_symbol << "\n";
        cout << "Party Name: " << party_name << "\n";
    }
};
class VotingSystem
{
private:
    VoterInformation *voter_head = nullptr;
    CandidateInformation *candidate_head = nullptr;
    unordered_map<string, CandidateInformation *> candidates_map;

public:
    ~VotingSystem();
    void loadVoters();
    void loadCandidates();
    void saveVoter(const VoterInformation &voter);
    void saveCandidate(const CandidateInformation &candidate);
    void registerVoter();
    void registerCandidate();
    void castVote();
    void adminPanel();
    void displayVotes();
    void displayWinner();
    void exitPortal();
    void mainMenu();
    void printBanner(const string &message);
    bool isVoterRegistered(const string &voter_id);
    bool isCandidateRegistered(const string &candidate_name);

private:
    void clearVoterData();
    void clearCandidateData();
    void addVoterToList(VoterInformation *voter);
    void addCandidateToList(CandidateInformation *candidate);
    bool login();
    void displayAllVoters();
    void displayAllCandidates();
};
VotingSystem::~VotingSystem()
{
    clearVoterData();
    clearCandidateData();
}
void VotingSystem::clearVoterData()
{
    while (voter_head)
    {
        VoterInformation *temp = voter_head;
        voter_head = voter_head->next;
        delete temp;
    }
}
void VotingSystem::clearCandidateData()
{
    while (candidate_head)
    {
        CandidateInformation *temp = candidate_head;
        candidate_head = candidate_head->next;
        delete temp;
    }
}
bool VotingSystem::login()
{
    system("cls");
    cout << "\n\t\t\t\t--------WELCOME TO VOTING MANAGEMENT SYSTEM--------" << endl;
    cout << "\n\n\t\t\t\t\t***Security Required***";
    string username, password;
    const string correctUsername = "admin";
    const string correctPassword = "password";
    cout << "\n\n\n\t\t\t\t\tEnter Username: ";
    cin >> username;
    cout << "\n\n\t\t\t\t\tEnter Password: ";
    cin >> password;
    if (username == correctUsername && password == correctPassword)
    {
        return true;
    }
    else
    {
        cout << "\n\n\t\t\t\tInvalid Username or Password. Try again.\n";
        beepOnError(); // Beep for wrong input
        getch();
        return false;
    }
}
void VotingSystem::mainMenu()
{
    loadVoters();
    loadCandidates();
    while (true)
    {
        if (!login())
        {
            continue;
        }
        while (true)
        {
            system("cls");
            printBanner("WELCOME TO VOTING MANAGEMENT SYSTEM");
            cout << "\n\n\t\t\t\t\t 1. Register Voter" << endl;
            cout << "\n\t\t\t\t\t 2. Register Candidate" << endl;
            cout << "\n\t\t\t\t\t 3. Cast Vote " << endl;
            cout << "\n\t\t\t\t\t 4. Admin Panel" << endl;
            cout << "\n\t\t\t\t\t 5. View Winner" << endl;
            cout << "\n\t\t\t\t\t 6. Exit" << endl;
            cout << "\n\t\t\t\t\t Enter your choice: ";
            int choice;
            cin >> choice;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number.\n";
                getch();
                continue;
            }
            switch (choice)
            {
            case 1:
                registerVoter();
                break;
            case 2:
                registerCandidate();
                break;
            case 3:
                castVote();
                break;
            case 4:
                adminPanel();
                break;
            case 5:
                displayWinner();
                break;
            case 6:
                exitPortal();
                return;
            default:
                cout << "Invalid choice. Try again.\n";
                getch();
            }
        }
    }
}
void VotingSystem::printBanner(const string &message)
{
    cout << "\n\n\t\t\t\t ****** " << message << " ****** \n\n ";
}
void VotingSystem::loadVoters()
{
    ifstream voterFile("voters.txt");
    if (!voterFile.is_open())
    {
        cout << "Error loading voter data.\n";
        return;
    }
    VoterInformation *last = nullptr;
    while (true)
    {
        VoterInformation *voter = new VoterInformation();
        if (!(voterFile >> voter->id))
            break;
        voterFile.ignore();
        getline(voterFile, voter->name);
        voterFile >> voter->age;
        voterFile >> voter->has_voted;
        voterFile.ignore();
        if (last)
            last->next = voter;
        else
            voter_head = voter;

        last = voter;
    }
    voterFile.close();
}
void VotingSystem::loadCandidates()
{
    ifstream candidateFile("candidates.txt");
    if (!candidateFile.is_open())
    {
        cerr << "Error loading candidate data.\n";
        return;
    }
    CandidateInformation *last = nullptr;
    while (true)
    {
        CandidateInformation *candidate = new CandidateInformation();
        if (!getline(candidateFile, candidate->name))
            break;
        candidateFile >> candidate->age;
        candidateFile.ignore();
        getline(candidateFile, candidate->education);
        candidateFile >> candidate->nationality;
        candidateFile >> candidate->criminal_record;
        candidateFile.ignore();
        getline(candidateFile, candidate->party_symbol);
        getline(candidateFile, candidate->party_name);
        candidateFile >> candidate->vote_count;
        candidateFile.ignore();
        if (last)
            last->next = candidate;
        else
            candidate_head = candidate;

        candidates_map[candidate->party_symbol] = candidate;
        last = candidate;
        cout << "Loaded candidate: " << candidate->name
             << " with symbol " << candidate->party_symbol << endl;
    }
    candidateFile.close();
}
void VotingSystem::saveVoter(const VoterInformation &voter)
{
    ofstream voterFile("voters.txt", ios::app);
    if (voterFile.is_open())
    {
        voterFile << voter.id << endl;
        voterFile << voter.name << endl;
        voterFile << voter.age << endl;
        voterFile << voter.has_voted << endl;
        voterFile.close();
    }
    else
    {
        cout << "Error saving voter data.\n";
    }
}
void VotingSystem::saveCandidate(const CandidateInformation &candidate)
{
    unordered_map<string, bool> existingCandidates;
    ifstream candidateFileIn("candidates.txt");
    if (candidateFileIn.is_open())
    {
        string line;
        while (getline(candidateFileIn, line))
        {
            existingCandidates[line] = true;
            for (int i = 0; i < 7; i++)
                getline(candidateFileIn, line);
        }
        candidateFileIn.close();
    }
    if (existingCandidates.find(candidate.name) == existingCandidates.end())
    {
        ofstream candidateFileOut("candidates.txt", ios::app);
        if (candidateFileOut.is_open())
        {
            candidateFileOut << candidate.name << endl;
            candidateFileOut << candidate.age << endl;
            candidateFileOut << candidate.education << endl;
            candidateFileOut << candidate.nationality << endl;
            candidateFileOut << candidate.criminal_record << endl;
            candidateFileOut << candidate.party_symbol << endl;
            candidateFileOut << candidate.party_name << endl;
            candidateFileOut << candidate.vote_count << endl;
            candidateFileOut.close();
        }
        else
        {
            cerr << "Error saving candidate data.\n";
        }
    }
}
void VotingSystem::registerCandidate()
{
    while (true)
    {
        system("cls");
        CandidateInformation *candidate = new CandidateInformation();
        cout << "\n\n Enter Candidate's Name: ";
        cin.ignore();
        getline(cin, candidate->name);
        if (isCandidateRegistered(candidate->name))
        {
            cout << "\n\n This candidate is already registered.\n";
            beepOnError(); // Beep for wrong input
            delete candidate;
            getch();
            continue;
        }
        cout << "\n\n Enter Candidate's Age: ";
        cin >> candidate->age;
        if (candidate->age < 18)
        {
            cout << "\n\n Candidate must be at least 18 years old to register.\n";
            beepOnError(); // Beep for wrong input
            delete candidate;
            getch();
            continue;
        }
        cout << "\n\n Choose Candidate's Education Level: \n";
        cout << "1. 12th Grade (F.A., F.Sc., ICS, etc.)\n";
        cout << "2. Bachelor's (e.g., B.A., B.Sc., B.Com, etc.)\n";
        cout << "3. Master's (e.g., M.A., M.Sc., M.Com, etc.)\n";
        cout << "4. MPhil\n";
        cout << "5. PhD\n";
        cout << "6. Less Than 12th\n";
        cout << "Enter the number corresponding to the education level: ";
        int educationChoice;
        cin >> educationChoice;
        switch (educationChoice)
        {
        case 1:
            candidate->education = "12th Grade (F.A., F.Sc., ICS, etc.)";
            break;
        case 2:
            candidate->education = "Bachelor's (e.g., B.A., B.Sc., B.Com, etc.)";
            break;
        case 3:
            candidate->education = "Master's (e.g., M.A., M.Sc., M.Com, etc.)";
            break;
        case 4:
            candidate->education = "MPhil";
            break;
        case 5:
            candidate->education = "PhD";
            break;
        case 6:
            candidate->education = "Less than 12th";
            break;
        default:
            cout << "\n\n Invalid choice. Please choose a valid education level.\n";
            beepOnError(); // Beep for wrong input
            delete candidate;
            getch();
            continue;
        }
        if (candidate->education != "12th Grade (F.A., F.Sc., ICS, etc.)" &&
            candidate->education != "Bachelor's (e.g., B.A., B.Sc., B.Com, etc.)" &&
            candidate->education != "Master's (e.g., M.A., M.Sc., M.Com, etc.)" &&
            candidate->education != "MPhil" && candidate->education != "PhD")
        {
            cout << "\n\n Candidate's education must be at least 12th Grade.\n";
            beepOnError(); // Beep for wrong input
            delete candidate;
            getch();
            continue;
        }
        cout << "\n\n Select Candidate's Nationality (1 for Pakistani, 2 for Others): ";
        cin >> candidate->nationality;
        cin.ignore();
        if (candidate->nationality != 1)
        {
            cout << "\n\n Only Pakistani candidates are eligible to register.\n";
            beepOnError(); // Beep for wrong input
            delete candidate;
            getch();
            continue;
        }
        cout << "\n\n Does Candidate have any Criminal Record (1 for Yes, 0 for No): ";
        cin >> candidate->criminal_record;
        cin.ignore();
        if (candidate->criminal_record)
        {
            cout << "\n\n Candidates with a criminal record are not eligible to register.\n";
            beepOnError(); // Beep for wrong input
            delete candidate;
            getch();
            continue;
        }
        cout << "\n\n Enter Party Name: ";
        getline(cin, candidate->party_name);
        cout << "\n\n Enter Party Symbol: ";
        getline(cin, candidate->party_symbol);
        addCandidateToList(candidate);
        saveCandidate(*candidate);
        cout << "\n\nCandidate registered successfully!\n";
        getch();
        break;
    }
}
bool VotingSystem::isVoterRegistered(const string &voter_id)
{
    VoterInformation *voter = voter_head;
    while (voter)
    {
        if (voter->id == voter_id)
        {
            return true;
        }
        voter = voter->next;
    }
    return false;
}
bool VotingSystem::isCandidateRegistered(const string &candidate_name)
{
    CandidateInformation *candidate = candidate_head;
    while (candidate)
    {
        if (candidate->name == candidate_name)
        {
            return true;
        }
        candidate = candidate->next;
    }
    return false;
}
void VotingSystem::registerVoter()
{
    while (true)
    {
        system("cls");
        VoterInformation *voter = new VoterInformation();
        cout << "\n\n Enter Voter's Name: ";
        cin.ignore();
        getline(cin, voter->name);
        cout << "\n\n Enter Voter's ID: ";
        cin >> voter->id;
        if (isVoterRegistered(voter->id))
        {
            cout << "\n\n This voter is already registered.\n";
            beepOnError(); // Beep for wrong input
            delete voter;
            getch();
            continue;
        }
        cout << "\n\n Enter Voter's Age: ";
        cin >> voter->age;
        if (voter->age < 18)
        {
            cout << "\n\n Voter must be at least 18 years old to register.\n";
            beepOnError(); // Beep for wrong input
            delete voter;
            getch();
            continue;
        }
        addVoterToList(voter);
        saveVoter(*voter);
        cout << "\n\nVoter registered successfully!\n";
        getch();
        break;
    }
}
void VotingSystem::castVote()
{
    system("cls");
    cout << "\n--- Cast Your Vote ---\n";
    CandidateInformation *candidate = candidate_head;
    if (!candidate)
    {
        cout << "No candidates registered yet.\n";
        getch();
        return;
    }
    cout << "\nRegistered Candidates:\n";
    int index = 1;
    unordered_map<int, CandidateInformation *> candidate_map;
    while (candidate)
    {
        cout << index << ". Symbol: " << candidate->party_symbol << " | Name: " << candidate->name << "\n";
        candidate_map[index] = candidate;
        candidate = candidate->next;
        index++;
    }
    int selectedNumber;
    cout << "\nSelect the number of the candidate you wish to vote for: ";
    cin >> selectedNumber;
    if (candidate_map.find(selectedNumber) == candidate_map.end())
    {
        cout << "Invalid number. Please try again.\n";
        beepOnError();
        getch();
        return;
    }
    string voter_id;
    cout << "\nEnter your Voter ID: ";
    cin >> voter_id;
    VoterInformation *voter = voter_head;
    while (voter)
    {
        if (voter->id == voter_id)
        {
            if (voter->has_voted)
            {
                cout << "You have already voted. Thank you!\n";
                getch();
                return;
            }
            CandidateInformation *selectedCandidate = candidate_map[selectedNumber];
            selectedCandidate->vote_count++;
            voter->has_voted = true;
            ofstream voterFile("voters.txt");
            if (voterFile.is_open())
            {
                VoterInformation *current = voter_head;
                while (current)
                {
                    voterFile << current->id << endl;
                    voterFile << current->name << endl;
                    voterFile << current->age << endl;
                    voterFile << current->has_voted << endl;
                    current = current->next;
                }
                voterFile.close();
            }
            else
            {
                cout << "Error saving voter data after casting vote.\n";
            }
            saveCandidate(*selectedCandidate);

            cout << "Vote cast successfully for " << selectedCandidate->name << "!\n";
            getch();
            return;
        }
        voter = voter->next;
    }
    cout << "Voter ID not found. Please try again.\n";
    getch();
}
void VotingSystem::adminPanel()
{
    while (true)
    {
        system("cls");
        cout << "---- Admin Panel ----\n";
        cout << "1. Display All Voters\n";
        cout << "2. Display All Candidates\n";
        cout << "3. Display Votes\n";
        cout << "4. Exit Admin Panel\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        switch (choice)
        {
        case 1:
            displayAllVoters();
            break;
        case 2:
            displayAllCandidates();
            break;
        case 3:
            displayVotes();
            break;
        case 4:
            return;
        default:
            cout << "Invalid choice. Please try again.\n";
            getch();
        }
    }
}
void VotingSystem::displayAllVoters()
{
    system("cls");
    cout << "\n--- All Registered Voters ---\n";
    VoterInformation *voter = voter_head;
    if (!voter)
    {
        cout << "No voters registered yet.\n";
        getch();
        return;
    }
    while (voter)
    {
        voter->display();
        voter = voter->next;
    }
    getch();
}
void VotingSystem::displayAllCandidates()
{
    system("cls");
    cout << "\n--- All Registered Candidates ---\n";
    CandidateInformation *candidate = candidate_head;
    if (!candidate)
    {
        cout << "No candidates registered yet.\n";
        getch();
        return;
    }
    while (candidate)
    {
        candidate->display();
        candidate = candidate->next;
    }

    getch();
}
void VotingSystem::displayVotes()
{
    system("cls");
    cout << "\n--- Vote Counts for All Candidates ---\n";
    CandidateInformation *candidate = candidate_head;
    if (!candidate)
    {
        cout << "No candidates have been registered yet.\n";
        getch();
        return;
    }
    while (candidate)
    {
        cout << "Candidate Name: " << candidate->name
             << " | Party Symbol: " << candidate->party_symbol
             << " | Vote Count: " << candidate->vote_count << "\n";
        candidate = candidate->next;
    }
    getch();
}
void VotingSystem::displayWinner()
{
    system("cls");
    CandidateInformation *winner = candidate_head;
    if (!winner)
    {
        cout << "No candidates have been registered yet.\n";
        getch();
        return;
    }
    int highestVotes = winner->vote_count;
    int secondHighestVotes = 0;
    bool allZeroVotes = true;
    for (CandidateInformation *candidate = candidate_head; candidate != nullptr; candidate = candidate->next)
    {
        if (candidate->vote_count > 0)
        {
            allZeroVotes = false;
        }
        if (candidate->vote_count > highestVotes)
        {
            secondHighestVotes = highestVotes;
            highestVotes = candidate->vote_count;
            winner = candidate;
        }
        else if (candidate->vote_count > secondHighestVotes && candidate != winner)
        {
            secondHighestVotes = candidate->vote_count;
        }
    }
    if (allZeroVotes)
    {
        cout << "All candidates have zero votes. No winner.\n";
    }
    else
    {
        cout << "\n--- Election Results ---\n";
        cout << "Winner:\n";
        cout << "Name: " << winner->name << " | Symbol: " << winner->party_symbol << " | Votes: " << winner->vote_count << "\n";

        if (secondHighestVotes > 0)
        {
            cout << "Votes more than runner-up: " << (winner->vote_count - secondHighestVotes) << "\n";
        }
        else
        {
            cout << "No runner-up votes to compare.\n";
        }
        cout << "\nAll Candidates Votes:\n";
        for (CandidateInformation *candidate = candidate_head; candidate != nullptr; candidate = candidate->next)
        {
            cout << "Name: " << candidate->name << " | Symbol: " << candidate->party_symbol << " | Votes: " << candidate->vote_count << "\n";
        }
    }
    getch();
}
void VotingSystem::exitPortal()
{
    clearVoterData();
    clearCandidateData();
    cout << "\nThank you for using the Voting System. Goodbye!\n";
    getch();
}
void VotingSystem::addVoterToList(VoterInformation *voter)
{
    voter->next = voter_head;
    voter_head = voter;
}
void VotingSystem::addCandidateToList(CandidateInformation *candidate)
{
    candidate->next = candidate_head;
    candidate_head = candidate;
    candidates_map[candidate->party_symbol] = candidate;
}

int main()
{
    system("color F0");
    VotingSystem votingSystem;
    votingSystem.mainMenu();
    return 0;
}