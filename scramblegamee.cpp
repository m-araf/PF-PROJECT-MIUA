#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

// Function to scramble a word
string scrambleWord(const string& word) {
    string scrambled = word;
    do {
        scrambled = word;
        for (size_t i = 0; i < scrambled.size(); ++i) {
            swap(scrambled[i], scrambled[rand() % scrambled.size()]);
        }
    } while (scrambled == word); // Ensure scrambled word is different from the original
    return scrambled;
}

// Function to load words from a file based on level
int loadWordsByLevel(const string& filename, string words[], int maxWords, int level) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Unable to open file '" << filename << "'.\n";
        return 0; // Return 0 words loaded
    }

    int count = 0;
    string word;
    while (file >> word && count < maxWords) {
        if ((level == 1 && word.length() == 3) ||      // Easy: 3 letters
            (level == 2 && word.length() == 4) ||      // Medium: 4 letters
            (level == 3 && word.length() >= 5)) {      // Hard: 5+ letters
            words[count++] = word;
        }
    }

    file.close();
    return count; // Return the number of words loaded
}

// Function to load high score from a file
int loadHighScore(const string& filename) {
    ifstream file(filename);
    int highScore = 0;
    if (file) {
        file >> highScore;
    }
    file.close();
    return highScore;
}

// Function to save high score to a file
void saveHighScore(const string& filename, int score) {
    ofstream file(filename);
    if (file) {
        file << score;
    }
    file.close();
}

// Function to delete the high score file
void deleteHighScore(const string& filename) {
    if (remove(filename.c_str()) == 0) {
        cout << "High score deleted successfully!\n";
    }
    else {
        cerr << "Error: Unable to delete high score file.\n";
    }
}

// Function to start the game for a specific set of words
bool playGame(const string words[], int wordCount, const string& highScoreFile, int level) {
    srand(static_cast<unsigned int>(time(0))); // Seed random number generator
    int highScore = loadHighScore(highScoreFile);
    int score = 0;
    const int maxAttempts = 3;

    cout << "High Score: " << highScore << "\n";

    while (true) {
        string originalWord = words[rand() % wordCount]; // Pick a random word
        string scrambledWord = scrambleWord(originalWord);
        int attempts = 0;

        while (attempts < maxAttempts) {
            cout << "Scrambled Word: " << scrambledWord << endl;
            cout << "Your guess: ";
            string userGuess;
            cin >> userGuess;

            if (userGuess == "exit") {
                cout << "Game Over! Your final score: " << score << endl;

                // Save high score if it's a new record
                if (score > highScore) {
                    cout << "Congratulations! You set a new high score!\n";
                    saveHighScore(highScoreFile, score);
                }
                return false;
            }

            if (userGuess == originalWord) {
                cout << "Correct! +10 points.\n";
                score += 10;

                // Bonus points for guessing on the first attempt
                if (attempts == 0) {
                    cout << "Bonus! +5 points for guessing on the first try.\n";
                    score += 5;
                }
                break; // Exit loop and go to the next word
            }
            else {
                attempts++;
                cout << "Wrong! Try again. Attempts left: " << (maxAttempts - attempts) << "\n";
            }
        }

        if (attempts == maxAttempts) {
            cout << "You've used all attempts. The correct word was: " << originalWord << endl;
            cout << "Game Over! Your final score: " << score << endl;

            // Save high score if it's a new record
            if (score > highScore) {
                cout << "Congratulations! You set a new high score!\n";
                saveHighScore(highScoreFile, score);
            }

            // Ask the user if they want to replay or go to the main menu
            cout << "Do you want to replay the game? (y/n): ";
            char replayChoice;
            cin >> replayChoice;

            if (replayChoice == 'y' || replayChoice == 'Y') {
                return true; // Replay the game
            }
            else {
                return false; // Go back to the main menu
            }
        }

        cout << "Current Score: " << score << "\n\n";
    }
}

void displayAbout() {
    cout << "\n--- About Us ---\n";
    cout << "Welcome to the Word Scramble Game!\n";
    cout << "In this game, you will test your vocabulary and quick-thinking skills.\n";
    cout << "Instructions:\n";
    cout << "1. Choose a difficulty level.\n";
    cout << "2. Unscramble the scrambled words to earn points.\n";
    cout << "3. You have 3 attempts per word. Type 'exit' to quit early.\n";
    cout << "Enjoy and challenge yourself!\n\n";
}

int main() {
    system("Color E");

    // File paths
    const string wordFile = "../words.txt";
    const string highScoreFile = "../highscore.txt";

    int choice;
    do {
        // Main menu
        cout << "\n--- Main Menu ---\n";
        cout << "1. Play Game\n";
        cout << "2. View Highscore\n";
        cout << "3. About Us\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            // Ask if the user wants to go back to the main menu
            cout << "Choose a difficulty level:\n";
            cout << "1. Easy (3-letter words)\n";
            cout << "2. Medium (4-letter words)\n";
            cout << "3. Hard (5+ letter words)\n";
            cout << "4. Go back to the main menu\n";
            cout << "Enter your choice: ";
            int level;
            cin >> level;

            if (level == 4) {
                break; // Go back to the main menu
            }

            if (level < 1 || level > 3) {
                cerr << "Invalid choice. Returning to the main menu.\n";
                break;
            }

            // Load words for the selected level
            const int maxWords = 100;
            string words[maxWords];
            int wordCount = loadWordsByLevel(wordFile, words, maxWords, level);

            if (wordCount == 0) {
                cerr << "No words available for the selected level. Please update '" << wordFile << "' with suitable words.\n";
                break;
            }

            // Play the game and handle replay
            bool replay;
            do {
                replay = playGame(words, wordCount, highScoreFile, level);
            } while (replay);

            break;
        }
        case 2: {
            cout << "\n--- Highscore ---\n";
            int highScore = loadHighScore(highScoreFile);
            cout << "High Score: " << highScore << "\n";
            cout << "Do you want to delete the high score? (y/n): ";
            char deleteChoice;
            cin >> deleteChoice;
            if (deleteChoice == 'y' || deleteChoice == 'Y') {
                deleteHighScore(highScoreFile);
            }
            break;
        }
        case 3:
            displayAbout();
            break;
        case 4:
            cout << "Thank you for playing! Goodbye!\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);

    return 0;
}