# Word Scramble Game

## Overview
The **Word Scramble Game** is a fun and engaging console-based game that challenges players to unscramble randomly jumbled words within a time limit. The game features user authentication, multiple difficulty levels, high score tracking, and an interactive UI. 

## Features
- **User Authentication**: Players can sign up with a username, password, and Gmail ID, or log in to access the game.
- **Difficulty Levels**:
  - Easy: 3-letter words.
  - Medium: 4-letter words.
  - Hard: 5+ letter words.
- **High Score Tracking**: 
  - View and reset high scores.
  - Set new high scores by earning points.
- **Timed Gameplay**: Players must respond within a set time limit.
- **Hints**: Players can request hints after two incorrect guesses.
- **Interactive Menu**: Access different features like "Play," "View High Score," "About Us," and "Exit."
- **Replay Option**: Replay the game without restarting the application.
- **Secure Password Input**: Passwords are masked during entry for enhanced security.

---

## Prerequisites
- C++ compiler (e.g., GCC or MSVC)
- A text editor or IDE (e.g., Visual Studio Code, Code::Blocks)
- Files required:
  - `users.txt`: Stores user credentials in the format `username:password:email`.
  - `words.txt`: Contains words for the game.
  - `highscore.txt`: Stores the high score.

---

## How to Run
1. **Clone or Download the Repository**:
   ```bash
   git clone https://github.com/m-araf/PF-Project-MIAU.git
   cd F-Project-MIAU
## How to Play
- ***Login or Sign Up:***
- If you don’t have an account, sign up by providing a username, password, and valid Gmail ID.
- Log in to access the game.
- Choose a Difficulty Level:
- Easy: Unscramble 3-letter words.
- Medium: Unscramble 4-letter words.
- Hard: Unscramble words with 5 or more letters.
- ***Start Unscrambling:***
- The game displays scrambled words, and you have three attempts to guess each word.
- Type your guesses within the time limit.
- ***Earn Points:***
- +10 points for each correct answer.
- +5 bonus points for guessing on the first attempt.
- ***End the Game:***
- Type exit during gameplay to end the game and see your final score.
- High scores are automatically saved.
