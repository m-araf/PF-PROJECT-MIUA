#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <regex>
#include<windows.h>
#include <vector>
#include <conio.h> // For getch() to mask password input
using namespace std;


void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Function to submit feedback with username
void submitFeedback() {
    string username, feedback;
    ofstream file("../feedback.txt", ios::app);

    if (!file) {
        setColor(12);
        cerr << "Error: Unable to open feedback file.\n";
        setColor(7);
        return;
    }
    setColor(14);
    cout << "\n========== Feedback Form ==========\n";
    cout << "We'd love to hear your thoughts about the Word Scramble game!\n";
    setColor(7);

    // Prompt for username
     setColor(11);
    cout << "Enter your username: ";
    cin.ignore(); // Clear input buffer
    getline(cin, username);
    setColor(7);

    // Prompt for feedback
    setColor(14);
    cout << "Please write your feedback below (Press Enter when done):\n";
    getline(cin, feedback);
    setColor(7);
    // Save username and feedback to the file
    file << "Username: " << username << "\n";
    file << "Feedback: " << feedback << "\n";
    file << "------------------------------------\n";
    file.close();
    setColor(10);
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
    setColor(14);
    cout << "========== Sign up ==========\n";
    setColor(11);
    string username, password, email;
    cout << "Enter a username: ";
    setColor(7);
    cin >> username;

    if (isUserExists(username)) {
        setColor(12);
        cout << "Username already exists! Try a different one.\n";
        setColor(7);
        return;
    }

    do {
        setColor(11);
        cout << "Enter a password (at least 8 characters): ";
        setColor(7);
        password = getMaskedInput();
        if (password.length() < 8) {
            setColor(11);
            cout << "Password must be at least 8 characters long.\n";
            setColor(7);
        }
    } while (password.length() < 8);

    do {
        setColor(11);
        cout << "Enter your email (must end with '@gmail.com'): ";
        setColor(7);
        cin >> email;
        if (!isValidEmail(email)) {
            setColor(12);
            cout << "Invalid email format. Please try again.\n";
            setColor(7);
        }
    } while (!isValidEmail(email));

    ofstream file("../users.txt", ios::app);
    if (file) {
        file << username << ":" << password << ":" << email << "\n";
        setColor(14);
        cout << "Signup successful! You can now log in.\n";
        setColor(7);
    }
    else {
        setColor(12);
        cerr << "Error: Unable to open file.\n";
         setColor(7);
    }
}

// Function to handle login
bool login() {
    cout << endl;
    setColor(14);
    cout << "========== Login ==========\n";
    string username, password;
    setColor(11);
    cout << "Enter your username: ";
    setColor(7);
    cin >> username;
    setColor(11);
    cout << "Enter your password: ";
     setColor(7);
    password = getMaskedInput();

    if (validateLogin(username, password)) {
        setColor(10);
        cout << "Login successful! Welcome, " << username << ".\n";
        setColor(7);
        return true;
    }
    else {
        setColor(12);
        cout << "Invalid credentials. Please try again.\n";
         setColor(7);
        return false;
    }
}

// Authentication menu
// Authentication menu
bool handleAuthentication() {
    char choice;
    int failedAttempts = 0; // Counter for failed login attempts
    const int maxAttempts = 3; // Max attempts before session expires

    do {
        setColor(11);
        cout << "1. Login\n";
        cout << "2. Signup\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
         setColor(7);
        cin >> choice;

        switch (choice) {
        case '1':
            if (login()) {
                return true; // Successful login
            }
            else {
                failedAttempts++;
                if (failedAttempts >= maxAttempts) {
                    setColor(12);
                    cout << "Account suspended! Too many failed attempts.\n";
                     setColor(7);
                    return false; // Exit after too many failed attempts
                }
            }
            break;
        case '2':
            signup();
            break;
        case '3':

            cout << "Exiting...\n";
            return false;
        default:
            setColor(12);
            cout << "Invalid choice. Please try again.\n";
             setColor(7);
        }
    } while (choice != '3');
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
        setColor(12);
        cerr << "Error: Unable to open file '" << filename << "'.\n";
         setColor(7);
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
            setColor(14);
            cout << "All words for this level have been used! No more words to play.\n";
             setColor(7);
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
                setColor(11);
                cout << "Game Over! Your final score: " << score << endl;
                 setColor(7);
                // Save high score if it's a new record
                if (score > highScore) {
                    setColor(10);
                    cout << "Congratulations! You set a new high score!\n";
                    saveHighScore(highScoreFile, score);
                    setColor(7);
                }
                return false;
            }

            if (userGuess == originalWord) {
                setColor(11);
                cout << "Correct! +10 points.\n";
                 setColor(7);
                score += 10;

                // Bonus points for guessing on the first attempt
                if (attempts == 0) {
                    setColor(11);
                    cout << "Bonus! +5 points for guessing on the first try.\n";
                     setColor(7);
                    score += 5;
                }
                break; // Exit loop and go to the next word
            }
            else {
                attempts++;
                setColor(12);
                cout << "Wrong! Try again. Attempts left: " << (maxAttempts - attempts) << "\n";
                 setColor(7);
                // Offer a hint after 2 wrong attempts
                if (attempts == 2) {
                    char choice;
                    setColor(13);
                    cout << "Would you like a hint? (y/n): ";
                     setColor(7);
                    cin >> choice;
                    if (choice == 'y' || choice == 'Y') {
                        setColor(13);
                        cout << "Hint: The first letter is '"
                            << originalWord[0] << "'.\n";
                         setColor(7);
                    }
                }
            }
        }

        if (attempts == maxAttempts) {
            setColor(14);
            cout << "You've used all attempts. The correct word was: " << originalWord << endl;
            cout << "Game Over! Your final score: " << score << endl;
            setColor(7);
            // Save high score if it's a new record
            if (score > highScore) {
                setColor(10);
                cout << "Congratulations! You set a new high score!\n";
                saveHighScore(highScoreFile, score);
                 setColor(7);
            }

            char replayChoice;
            setColor(14);
            cout << "Would you like to replay the game? (y/n): ";
             setColor(7);
            cin >> replayChoice;
            return (replayChoice == 'y' || replayChoice == 'Y');
        }
        setColor(10);
        cout << "Current Score: " << score << "\n\n";
         setColor(7);
    }

    return false;
}

int main() {
    
    setColor(14);
    cout << "\n==========================================\n";
    cout << "      Welcome to the Word Scramble Game!   \n";
    cout << "==========================================\n";
    cout << "Log in to continue or sign up to join the fun!\n";
    cout << "Save your progress and aim for the high score!\n\n";
     setColor(7);
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
        setColor(14);
        cout << "\n========== Main Menu ==========\n";
        cout << "Welcome to the Word Scramble Game!\n";
        setColor(11);
        cout << "1. Play\n";
        cout << "2. Highscore\n";
        cout << "3. About Us\n";
        cout << "4. Feedback Form\n"; // Feedback option added
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
         setColor(7);
        cin >> choice;

        switch (choice) {
        case 1: {
            // Play option
            cout << endl;
            setColor(14);
            cout << "Would you like to play within a category or without a category?\n";
            setColor(11);
            cout << "1. Within a category\n";
            cout << "2. Without a category\n";
            cout << "Enter your choice: ";
             setColor(7);

            int playChoice;
            cin >> playChoice;

            if (playChoice == 1) {
                // Play within a category
                cout << endl;
                setColor(14);
                cout << "Choose a difficulty level:\n";
                setColor(11);
                cout << "1. Easy (3-letter words)\n";
                cout << "2. Medium (4-letter words)\n";
                cout << "3. Hard (5+ letter words)\n";
                cout << "Enter your choice: ";
                 setColor(7);

                int level;
                cin >> level;

                if (level < 1 || level > 3) {
                    setColor(12);
                    cerr << "Invalid choice. Returning to the main menu.\n";
                     setColor(7);
                    break;
                }

                // Ask user for a category
                setColor(14);
                cout << "Choose a category:\n";
                setColor(11);
                cout << "1. Animals\n";
                cout << "2. Objects\n";
                cout << "3. Food\n"; // Added Food category
                cout << "Enter your choice: ";
                 setColor(7);

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
                    setColor(12);
                    cerr << "Invalid category. Returning to the main menu.\n";
                     setColor(7);
                    break;
                }

                // Load words for the selected category and level
                vector<string> words;
                int wordCount = loadWordsByCategoryAndLevel(wordFile, words, level, category);

                if (wordCount == 0) {
                    setColor(12);
                    cerr << "No words available for the selected category and level. Please update '" << wordFile << "' with suitable words.\n";
                     setColor(7);
                    break;
                }
                setColor(14);
                cout << "Unscramble the word to earn points.\n";
                cout << "You have 3 chances for each word. Type 'exit' to quit the game early.\n\n";
                 setColor(7);

                bool replay;
                do {
                    replay = playGame(words, highScoreFile, level);
                } while (replay);

            }
            else if (playChoice == 2) {
                // Play without a category
                cout << endl;
                setColor(14);
                cout << "Choose a difficulty level:\n";
                setColor(11);
                cout << "1. Easy (3-letter words)\n";
                cout << "2. Medium (4-letter words)\n";
                cout << "3. Hard (5+ letter words)\n";
                cout << "Enter your choice: ";
                 setColor(7);

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
                    setColor(12);
                    cerr << "No words available for the selected level. Please update '" << wordFile << "' with suitable words.\n";
                    setColor(7);
                    break;
                }
                setColor(14);
                cout << "Unscramble the word to earn points.\n";
                cout << "You have 3 chances for each word. Type 'exit' to quit the game early.\n\n";
                setColor(7);
                bool replay;
                do {
                    replay = playGame(words, highScoreFile, level);
                } while (replay);

            }
            else {
                setColor(12);
                cerr << "Invalid choice. Returning to the main menu.\n";
                 setColor(7);
            }

            break;
        }
        case 2: {
            // Highscore
            int highScore = loadHighScore(highScoreFile);
            setColor(10);
            cout << "High Score: " << highScore << "\n";
            setColor(7);

            char resetChoice;
            setColor(12);
            cout << "Would you like to reset the high score? (y/n): ";
            setColor(7);
            cin >> resetChoice;

            if (resetChoice == 'y' || resetChoice == 'Y') {
                saveHighScore(highScoreFile, 0);
                setColor(10);
                cout << "High score has been reset.\n";
                 setColor(7);
            }

            break;
        }
        case 3: {
            // About Us
            cout << endl;
            setColor(14);
            cout << "About the Game:\n";
            setColor(13);
            cout << "Welcome to Word Scramble! Unscramble the words to score points.\n";
            cout << "Choose a difficulty level and challenge yourself!\n";
            cout << "You can view and reset your high score anytime.\n";
             setColor(7);
            break;
        }
        case 4: {
            // Feedback Form
            submitFeedback();
            break;
        }
        case 5: {
            setColor(10);
            cout << "Thank you for playing Word Scramble! Goodbye!\n";
             setColor(7);
            break;
        }
        default: {
            setColor(12);
            cerr << "Invalid choice. Please try again.\n";
             setColor(7);
            break;
        }
        }

    } while (choice != 5);

    return 0;
}