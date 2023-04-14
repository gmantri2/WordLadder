#ifndef GAME_HPP
#define GAME_HPP

#include <fstream>
#include <sstream>
#include <iostream>

#include "word_graph.hpp"

class Game{
    public:
        Game() = delete; // no default constructor
        Game(int initialLevel,bool updateGraph);

        int RunGame();

    private:
        /**************************************************
         * member variables
         **************************************************/
        WordGraph wg_;
        int level_;
        bool try_again;
        const std::string kWORD_LIST_FILE = "src/wordlist.txt";

        // map of key and relevant action
        std::map<char, std::string> kKEYBOARD_MAP {
            {'0', "to try again"},
            {'1', "to begin"},
            {'2', "for instructions"},
            {'3', "for a hint"},
            {'4', "to give up"},
            {'5', "to advance"},
            {'6', "to quit"},
            {'7', "to reset levels"},
            {'8', "to play again"},
            {'9', "to cheat and see the answer"}
        };

        const std::vector<char> kGAME_SCENE_INPUTS {'2','3','4','7', '9'};

        /**************************************************
         * game progression
         **************************************************/
        // intro sequence (hi, start, how to play)
        void IntroScene();
        // instructions. literally just prints out instructions.
        void InstructionsScene();
        // the where the actual game is played
        void GameScene();
        // loop for answering each problem. returns true if guessed, false if not
        void ProblemScene(const std::pair<std::pair<std::string,std::string>,int>& prob,
            const std::vector<std::string>& answer, int& needToGuessIndex);
        // hint
        void GiveHint(const std::vector<std::string>& answer, const int& needToGuessIndex);
        // give up
        void GiveUp(const std::vector<std::string>& answer);


        // generic input loop. sets c to inputed value
        void InputLoop(char* c, const std::vector<char>& validInputs);
        // generic input loop for strings
        void InputLoop(std::string* c, const std::vector<char>& validInputs);
        // returns true if c is in validInputs
        bool IsValidInput(char c, const std::vector<char>& validInputs);
        // returns true if c is in validInputs OR is an alpha string
        bool IsValidInput(const std::string& c, const std::vector<char>& validInputs);

        /**************************************************
         * messages
         **************************************************/
        // does PressForXMessage for each char in a vector
        void PressForXMessage(const std::vector<char>& inputs);
        // using kKEYBOARD_MAP returns a message eg. Press [1] to begin.
        void PressForXMessage(char key);
        // prints message for invalid inputs
        void InvalidInputMessage();
        // [word1] --> [word2] ([length] steps)
        void PrintProblem(const std::pair<std::pair<std::string,std::string>,int>& prob);
        // prints progress so far
        void PrintProblemProgress(const std::vector<std::string>& answer, const int& needToGuessIndex);
        // prints divider line
        void PrintDivider();
        
        /**************************************************
         * misc / helper
         **************************************************/
        // returns true if a file w/ the given path exists
        bool FileExists(const std::string& filename) const;
        // returns kKEYBOARD_MAP key for given char
        std::string KeyboardKey(char key);
        // returns kKEYBOARD_MAP value for given char
        std::string KeyboardValue(char key);
};

#endif