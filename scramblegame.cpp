#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <regex>
#include <vector>
#include <conio.h> // For getch() to mask password input
using namespace std;

// Function to submit feedback with username
void submitFeedback() {
    string username, feedback;
    ofstream file("../feedback.txt", ios::app);

    if (!file) {
        cerr << "Error: Unable to open feedback file.\n";
        return;
    }

    cout << "\n========== Feedback Form ==========\n";
    cout << "We'd love to hear your thoughts about the Word Scramble game!\n";

    // Prompt for username
    cout << "Enter your username: ";
    cin.ignore(); // Clear input buffer
    getline(cin, username);

    // Prompt for feedback
    cout << "Please write your feedback below (Press Enter when done):\n";
    getline(cin, feedback);

    // Save username and feedback to the file
    file << "Username: " << username << "\n";
    file << "Feedback: " << feedback << "\n";
    file << "------------------------------------\n";
    file.close();

    cout << "Thank you for your feedback! It has been submitted successfully.\n";
}

// Function to check if a username exists in the file
bool isUserExists(const string& username) {
    ifstream file("../users.txt");
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string storedUsername;
        getline(ss, storedUsername, ':');
        if (storedUsername == username) {
            return true;
        }
    }
    return false;
}

// Function to validate login credentials
bool validateLogin(const string& username, const string& password) {
    ifstream file("../users.txt");
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string storedUsername, storedPassword;
        getline(ss, storedUsername, ':');
        getline(ss, storedPassword, ':');
        if (storedUsername == username && storedPassword == password) {
            return true;
        }
    }
    return false;
}

// Function to mask password input
string getMaskedInput() {
    string password;
    char ch;
    while ((ch = _getch()) != '\r') { // Enter key
        if (ch == '\b' && !password.empty()) { // Handle backspace
            cout << "\b \b";
            password.pop_back();
        }
        else if (isprint(ch)) {
            cout << "*";
            password += ch;
        }
    }
    cout << endl;
    return password;
}

// Function to validate email
bool isValidEmail(const string& email) {
    const regex emailPattern("^[a-zA-Z0-9._%+-]+@gmail\\.com$");
    return regex_match(email, emailPattern);
}

void signup() {
    cout << endl;
    cout << "========== Sign up ==========\n";
    string username, password, email;
    cout << "Enter a username: ";
    cin >> username;

    if (isUserExists(username)) {
        cout << "Username already exists! Try a different one.\n";
        return;
    }

    do {
        cout << "Enter a password (at least 8 characters): ";
        password = getMaskedInput();
        if (password.length() < 8) {
            cout << "Password must be at least 8 characters long.\n";
        }
    } while (password.length() < 8);

    do {
        cout << "Enter your email (must end with '@gmail.com'): ";
        cin >> email;
        if (!isValidEmail(email)) {
            cout << "Invalid email format. Please try again.\n";
        }
    } while (!isValidEmail(email));

    ofstream file("../users.txt", ios::app);
    if (file) {
        file << username << ":" << password << ":" << email << "\n";
        cout << "Signup successful! You can now log in.\n";
    }
    else {
        cerr << "Error: Unable to open file.\n";
    }
}

// Function to handle login
bool login() {
    cout << endl;
    cout << "========== Login ==========\n";
    string username, password;
    cout << "Enter your username: ";
    cin >> username;

    cout << "Enter your password: ";
    password = getMaskedInput();

    if (validateLogin(username, password)) {
        cout << "Login successful! Welcome, " << username << ".\n";
        return true;
    }
    else {
        cout << "Invalid credentials. Please try again.\n";
        return false;
    }
}

// Authentication menu
// Authentication menu
bool handleAuthentication() {
    int choice;
    int failedAttempts = 0; // Counter for failed login attempts
    const int maxAttempts = 3; // Max attempts before session expires

    do {
        cout << "1. Login\n";
        cout << "2. Signup\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            if (login()) {
                return true; // Successful login
            }
            else {
                failedAttempts++;
                if (failedAttempts >= maxAttempts) {
                    cout << "Account suspended! Too many failed attempts.\n";
                    return false; // Exit after too many failed attempts
                }
            }
            break;
        case 2:
            signup();
            break;
        case 3:
            cout << "Exiting...\n";
            return false;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 3);
    return false;
}

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

int loadWordsByCategoryAndLevel(const string& filename, vector<string>& words, int level, const string& category) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Unable to open file '" << filename << "'.\n";
        return 0; // Return 0 words loaded
    }

    string word;
    while (getline(file, word)) {
        stringstream ss(word);
        string storedCategory;
        string storedWord;
        getline(ss, storedCategory, ','); // Assume words are stored with categories
        getline(ss, storedWord);

        // Trim leading and trailing spaces from category and word
        storedCategory = regex_replace(storedCategory, regex("^\\s+|\\s+$"), "");
        storedWord = regex_replace(storedWord, regex("^\\s+|\\s+$"), "");

        // Load word based on category and level
        if (category.empty() || storedCategory == category) {
            if ((level == 1 && storedWord.length() == 3) ||      // Easy: 3 letters
                (level == 2 && storedWord.length() == 4) ||      // Medium: 4 letters
                (level == 3 && storedWord.length() >= 5)) {      // Hard: 5+ letters
                words.push_back(storedWord);
            }
        }
    }

    file.close();

    // Debugging output to check if any "Food" category words are loaded
    cout << "Words loaded for category '" << category << "': " << words.size() << endl;

    return words.size(); // Return the number of words loaded
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
bool playGame(const vector<string>& words, const string& highScoreFile, int level) {
    srand(static_cast<unsigned int>(time(0))); // Seed random number generator
    int highScore = loadHighScore(highScoreFile);
    int score = 0;
    const int maxAttempts = 3;

    vector<bool> usedWords(words.size(), false); // Track used words
    int usedCount = 0; // Number of words used so far

    cout << "High Score: " << highScore << "\n";

    while (true) {
        if (usedCount == words.size()) {
            cout << "All words for this level have been used! No more words to play.\n";
            break;
        }

        int randomIndex;
        do {
            randomIndex = rand() % words.size();
        } while (usedWords[randomIndex]); // Pick a new word if it's already used

        usedWords[randomIndex] = true; // Mark the word as used
        usedCount++;

        string originalWord = words[randomIndex];
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

                // Offer a hint after 2 wrong attempts
                if (attempts == 2) {
                    char choice;
                    cout << "Would you like a hint? (y/n): ";
                    cin >> choice;
                    if (choice == 'y' || choice == 'Y') {
                        cout << "Hint: The first letter is '"
                            << originalWord[0] << "'.\n";
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

    return false;
}

int main() {
    system("Color E");

    cout << "\n==========================================\n";
    cout << "      Welcome to the Word Scramble Game!   \n";
    cout << "==========================================\n";
    cout << "Log in to continue or sign up to join the fun!\n";
    cout << "Save your progress and aim for the high score!\n\n";

    // Authentication
    if (!handleAuthentication()) {
        return 0; // Exit if user does not log in or sign up
    }

    // File paths
    const string wordFile = "../words.txt";
    const string highScoreFile = "../highscore.txt";

    // Main menu
    int choice;
    do {
        cout << "\n========== Main Menu ==========\n";
        cout << "Welcome to the Word Scramble Game!\n";
        cout << "1. Play\n";
        cout << "2. Highscore\n";
        cout << "3. About Us\n";
        cout << "4. Feedback Form\n"; // Feedback option added
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            // Play option
            cout << endl;
            cout << "Would you like to play within a category or without a category?\n";
            cout << "1. Within a category\n";
            cout << "2. Without a category\n";
            cout << "Enter your choice: ";

            int playChoice;
            cin >> playChoice;

            if (playChoice == 1) {
                // Play within a category
                cout << endl;
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

                // Ask user for a category
                cout << "Choose a category:\n";
                cout << "1. Animals\n";
                cout << "2. Objects\n";
                cout << "3. Food\n"; // Added Food category
                cout << "Enter your choice: ";

                int categoryChoice;
                cin >> categoryChoice;

                string category;
                switch (categoryChoice) {
                case 1:
                    category = "Animals";
                    break;
                case 2:
                    category = "Objects";
                    break;
                case 3:
                    category = "Food";
                    break;
                default:
                    cerr << "Invalid category. Returning to the main menu.\n";
                    break;
                }

                // Load words for the selected category and level
                vector<string> words;
                int wordCount = loadWordsByCategoryAndLevel(wordFile, words, level, category);

                if (wordCount == 0) {
                    cerr << "No words available for the selected category and level. Please update '" << wordFile << "' with suitable words.\n";
                    break;
                }

                cout << "Unscramble the word to earn points.\n";
                cout << "You have 3 chances for each word. Type 'exit' to quit the game early.\n\n";

                bool replay;
                do {
                    replay = playGame(words, highScoreFile, level);
                } while (replay);

            }
            else if (playChoice == 2) {
                // Play without a category
                cout << endl;
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

                // Load words for all categories at the selected level
                vector<string> words;
                ifstream file(wordFile);
                if (!file) {
                    cerr << "Error: Unable to open file '" << wordFile << "'.\n";
                    break;
                }

                string line;
                while (getline(file, line)) {
                    stringstream ss(line);
                    string storedCategory;
                    string storedWord;
                    getline(ss, storedCategory, ',');
                    getline(ss, storedWord);

                    if ((level == 1 && storedWord.length() == 3) ||
                        (level == 2 && storedWord.length() == 4) ||
                        (level == 3 && storedWord.length() >= 5)) {
                        words.push_back(storedWord);
                    }
                }

                file.close();

                if (words.empty()) {
                    cerr << "No words available for the selected level. Please update '" << wordFile << "' with suitable words.\n";
                    break;
                }

                cout << "Unscramble the word to earn points.\n";
                cout << "You have 3 chances for each word. Type 'exit' to quit the game early.\n\n";

                bool replay;
                do {
                    replay = playGame(words, highScoreFile, level);
                } while (replay);

            }
            else {
                cerr << "Invalid choice. Returning to the main menu.\n";
            }

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
            cout << endl;
            cout << "About the Game:\n";
            cout << "Welcome to Word Scramble! Unscramble the words to score points.\n";
            cout << "Choose a difficulty level and challenge yourself!\n";
            cout << "You can view and reset your high score anytime.\n";
            break;
        }
        case 4: {
            // Feedback Form
            submitFeedback();
            break;
        }
        case 5: {
            cout << "Thank you for playing Word Scramble! Goodbye!\n";
            break;
        }
        default: {
            cerr << "Invalid choice. Please try again.\n";
            break;
        }
        }

    } while (choice != 5);

    return 0;
}