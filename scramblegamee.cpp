#include <iostream>
#include <string>
#include <algorithm>
#include <ctime>
using namespace std;

string scrambleWord(const string& word) {
    string scrambled = word;
    random_shuffle(scrambled.begin(), scrambled.end());
    return scrambled;
}


int main() {
    system("Color E");

    string wordBank[] = { "12345", "iman", "uzma", "mayur", "hello" };
    int wordCount = sizeof(wordBank) / sizeof(wordBank[0]);
    cout << "Welcome to the Word Scramble Game!\n";
    cout << "Unscramble the word to earn points.\n";
    cout << "Type 'exit' to quit the game.\n\n";

    srand(time(0));
    int score = 0;

    while (true) {
        string originalWord = wordBank[rand() % wordCount];//random kr k choose krna
        string scrambledWord = scrambleWord(originalWord);//os random number ko scramble krna

        cout << "Scrambled Word: " << scrambledWord << endl;
        cout << "Your guess: ";
        string userGuess;
        cin >> userGuess;

        if (userGuess == "exit") {
            break;
        }
        if (userGuess == originalWord) {
            cout << "Correct! +10 points.\n";
            score = score + 10;
        }
        else {
            cout << "Wrong! The correct word was: " << originalWord << endl;
        }

        cout << "Current Score: " << score << "\n\n";
    }

    cout << "Game Over! Your final score: " << score << endl;
    return 0;
}
