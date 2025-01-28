#include <iostream>
#include <string>
#include <algorithm>
#include <ctime>
#include <vector>

using namespace std;

// Function to scramble the word
string scrambleWord(const string& word) {
    string scrambled = word;
    random_shuffle(scrambled.begin(), scrambled.end());
    return scrambled;
}

// Function to give a hint (first letter of the word)
string getHint(const string& word) {
    return "Hint: The first letter is '" + string(1, word[0]) + "'.";
}

int main() {
    system("Color E");
    cout << "Welcome to the Word Scramble Game!\n";
    cout << "Unscramble the word to earn points.\n";
    cout << "Type 'exit' to quit the game.\n\n";

    // Word bank with difficulty levels
    vector<string> easyWords = { "cat", "dog", "bat", "cow", "hat" };
    vector<string> mediumWords = { "iman", "uzma", "mayur", "hello", "apple" };
    vector<string> hardWords = { "planet", "pyramid", "complex", "jupiter", "octopus" };

    // Select difficulty
    string difficulty;
    vector<string> wordBank;
    while (true) {
        cout << "Choose a difficulty (Easy, Medium, Hard): ";
        cin >> difficulty;

        if (difficulty == "Easy") {
            wordBank = easyWords;
            break;
        }
        else if (difficulty == "Medium") {
            wordBank = mediumWords;
            break;
        }
        else if (difficulty == "Hard") {
            wordBank = hardWords;
            break;
        }
        else {
            cout << "Invalid choice. Please try again.\n";
        }
    }

    srand(time(0)); // Seed for random number generator
    int score = 0;

    while (true) {
        // Pick a random word and scramble it
        string originalWord = wordBank[rand() % wordBank.size()];
        string scrambledWord = scrambleWord(originalWord);

        cout << "\nScrambled Word: " << scrambledWord << endl;

        // Get user guess
        cout << "Your guess (or type 'hint' for a clue): ";
        string userGuess;
        cin >> userGuess;

        if (userGuess == "exit") {
            break;
        }
        else if (userGuess == "hint") {
            cout << getHint(originalWord) << endl;
            continue;
        }

        // Check if the guess is correct
        if (userGuess == originalWord) {
            cout << "Correct! +10 points.\n";
            score += 10;
        }
        else {
            cout << "Wrong! The correct word was: " << originalWord << endl;
        }

        cout << "Current Score: " << score << "\n";
    }

    cout << "\nGame Over! Your final score: " << score << endl;
    return 0;
}
