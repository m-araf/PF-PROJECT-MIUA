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
            } else {
                attempts++;
                cout << "Wrong! Try again. Attempts left: " << (maxAttempts - attempts) << "\n";

                // Offer a hint after 2 wrong attempts
                if (attempts == 2) {
                    char choice;
                    cout << "Would you like a hint? (y/n): ";
                    cin >> choice;
                    if (choice == 'y' || choice == 'Y') {
                        cout << "Hint: The first letter is '" << originalWord[0] << "'.\n";
                    }
                }
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

            char replayChoice;
            cout << "Would you like to replay the game? (y/n): ";
            cin >> replayChoice;
            return (replayChoice == 'y' || replayChoice == 'Y');
        }

        cout << "Current Score: " << score << "\n\n";
    }
}

int main() {
    system("Color E");

    // File paths
    const string wordFile = "../words.txt";
    const string highScoreFile = "../highscore.txt";

    int choice;
    do {
        cout << "Welcome to the Word Scramble Game!\n";
        cout << "1. Play\n";
        cout << "2. Highscore\n";
        cout << "3. About Us\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                // Display levels
                cout << "Choose a difficulty level:\n";
                cout << "1. Easy (3-letter words)\n";
                cout << "2. Medium (4-letter words)\n";
                cout << "3. Hard (5+ letter words)\n";
                cout << "Enter your choice: ";

                int level;
                cin >> level;

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

                cout << "Unscramble the word to earn points.\n";
                cout << "You have 3 chances for each word. Type 'exit' to quit the game early.\n\n";

                bool replay;
                do {
                    replay = playGame(words, wordCount, highScoreFile, level);
                } while (replay);

                break;
            }
            case 2: {
                // Highscore
                int highScore = loadHighScore(highScoreFile);
                cout << "High Score: " << highScore << "\n";

                char resetChoice;
                cout << "Would you like to reset the high score? (y/n): ";
                cin >> resetChoice;

                if (resetChoice == 'y' || resetChoice == 'Y') {
                    saveHighScore(highScoreFile, 0);
                    cout << "High score has been reset.\n";
                }

                break;
            }
            case 3: {
                // About Us
                cout << "About the Game:\n";
                cout << "Welcome to Word Scramble! Unscramble the words to score points.\n";
                cout << "Choose a difficulty level and challenge yourself!\n";
                cout << "You can view and reset your high score anytime.\n";
                break;
            }
            case 4: {
                cout << "Thank you for playing Word Scramble! Goodbye!\n";
                break;
            }
            default: {
                cerr << "Invalid choice. Please try again.\n";
                break;
            }
        }

    } while (choice != 4);

    return 0;
}