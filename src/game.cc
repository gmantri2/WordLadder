#include "game.hpp"

Game::Game(int initialLevel,bool updateGraph): level_(initialLevel) {
    // create word graph + adjacency list OR read it from file
    if (updateGraph) {
        wg_.MakeAdjacencyListAndWriteToFile(kWORD_LIST_FILE);
    } 
    wg_.ReadFromFile(wg_.GetGraphFile());

    // create path and starting lists for problem creation
    wg_.SetPathLengths();
    wg_.SetStartingLengths();
}

int Game::RunGame() {
    IntroScene();
    GameScene();
    return 0;
}

/**************************************************
 * game progression
 **************************************************/
void Game::IntroScene() {
    PrintDivider();
    
    std::cout << "Welcome to WordLadder!" << std::endl;
    std::vector<char> validInputs {'1', '2'};
    PressForXMessage(validInputs);

    char c;
    InputLoop(&c, validInputs);

    std::cout << "\n\n"; // for readability
    if (c == '1') {
        return;
    } else if (c == '2') {
        InstructionsScene();
        IntroScene();
    }
}

void Game::InstructionsScene() {
    std::cout << "HOW TO PLAY: " << std::endl;
    std::cout << "In Word Ladder, you aim to get from one word to the other by a sequence of"
                " single-character modifications. For example:\n\n";
    std::cout << "head --> tail (5 steps)\n\nhead\nheal\nteal\ntell\ntall\ntail\n\n";
    std::cout << "Each turn, you may add, remove, or change one character. You may also get hints"
                " or give up if you are stuck. After you finish each problem, you will move up in"
                " difficulty to harder problems.\n\n";
}

void Game::GameScene() {
    // create problem
    auto prob = wg_.GetProblem(level_);
    auto answer = wg_.BFS(prob.first.first,prob.first.second);
    int needToGuessIndex = 1;

    // have the user play a problem and increase level after
    ProblemScene(prob, answer, needToGuessIndex);

    PrintDivider();
    
    std::vector<char> validInputs;
    if(try_again){
        std::cout << "Try again?" << std::endl;
        validInputs = {'0', '6'};
    }
    else if(level_ < 10){
        std::cout << "Advance to next level?" << std::endl;
        validInputs = {'5', '6'};
    }
    else{
        std::cout << "Congratulations, you reached the end! Play again?" << std::endl;
        validInputs = {'8', '6'};
    }
    PressForXMessage(validInputs);
    char c;
    InputLoop(&c, validInputs);

    if(!(try_again)){
        level_ = level_ < 10 ? level_+1 : 1;
    }
    else{
        try_again = false;
    }
    std::cout << "\n\n"; // for readability
    if (c == '5' || c == '8' || c == '0') { // play again
        GameScene();
    } else { // quit
        std::cout << "Goodbye! Hope you had fun!\n";
        return;
    }
}

void Game::ProblemScene(const std::pair<std::pair<std::string,std::string>,int>& prob,
    const std::vector<std::string>& answer, int& needToGuessIndex) {
    PrintDivider();

    // print level number and problem, progress, and instructions
    std::cout << "Level " << level_ << '\n' << std::endl;
    PrintProblem(prob);
    
    std::vector<char> validInputs = kGAME_SCENE_INPUTS;
    PressForXMessage(validInputs);
    std::cout << "\nEnter your guess and press enter.\n\n";
    PrintProblemProgress(answer, needToGuessIndex);

    // get input
    std::string c;
    InputLoop(&c, validInputs);
    std::cout << "\n\n"; // for readability
    if (c == "2") { // instructions
        InstructionsScene();
        ProblemScene(prob, answer, needToGuessIndex);
    } else if (c == "3") { // hint
        GiveHint(answer, needToGuessIndex);
        ProblemScene(prob, answer, needToGuessIndex);
    } else if (c == "4") { // give up
        GiveUp(answer);
        try_again = true;
        return;
    } else if (c == "7") {
        // needToGuessIndex = 1;
        // ProblemScene(prob, answer, needToGuessIndex);
        level_ = 1;
        GameScene();
    } else if (c == "9") { // cheat
        unsigned long i = 0;
        for (auto step : answer) {
            if(i == answer.size()-1){
                std::cout << step;
            } 
            else{
                std::cout << step << " --> "; 
            }
            i++;
        }
        std::cout << std::endl;
        ProblemScene(prob, answer, needToGuessIndex);
    } else { // anything else
        if (c == answer.at(needToGuessIndex)) {
            // correct
            std::cout << "Correct!\n\n";
            needToGuessIndex++;

            if ((size_t)needToGuessIndex == answer.size()) {
                // if guessed entire thing!
                return;
            } else {
               // still have more to guess
               ProblemScene(prob, answer, needToGuessIndex);
            }
        } else {
            // wrong or other wack input
            std::cout << "Incorrect!\n\n";
            ProblemScene(prob, answer, needToGuessIndex);
        }
    }
}

void Game::GiveHint(const std::vector<std::string>& answer, const int& needToGuessIndex) { 
    if (answer.at(needToGuessIndex).size() < answer.at(needToGuessIndex - 1).size()) {
        std::cout << "Consider deleting a letter, that might help :)\n\n";
    } else if (answer.at(needToGuessIndex).size() > answer.at(needToGuessIndex - 1).size()) {
        std::cout << "Consider adding a letter, that might help :)\n\n";
    } else {
        for (unsigned int i = 0; i < answer.at(needToGuessIndex).size(); i++) {
            if (answer.at(needToGuessIndex).at(i) != answer.at(needToGuessIndex - 1).at(i)) {
                std::cout << "Consider changing the letter at index: " + std::to_string(i) + ", that might help :)\n\n";
                return;
            }
        }
    } 
}

void Game::GiveUp(const std::vector<std::string>& answer) {
    unsigned long i = 0;
    std::cout << "The answer was: " << std::endl;
    for (auto step : answer) {
        if(i == answer.size()-1){
            std::cout << step << std::endl;
        } 
        else{
            std::cout << step << " --> "; 
        }
        i++;
    }
    //level_ = 1;
}
void Game::InputLoop(char* c, const std::vector<char>& validInputs) {
    std::cin >> *c;
    while (!IsValidInput(*c, validInputs)) {
        InvalidInputMessage();
        std::cout << " ";
        PressForXMessage(validInputs);
        std::cin >> *c;
        std::cout << std::endl;
    }
}

void Game::InputLoop(std::string* c, const std::vector<char>& validInputs) {
    std::cin >> *c;
    while (!IsValidInput(*c, validInputs)) {
        InvalidInputMessage();
        std::cout << " ";
        PressForXMessage(validInputs);
        std::cin >> *c;
        std::cout << std::endl;
    }
}

bool Game::IsValidInput(char c, const std::vector<char>& validInputs) {
    return std::find(validInputs.begin(), validInputs.end(), c) != validInputs.end();
}

bool Game::IsValidInput(const std::string& c, const std::vector<char>& validInputs) {
    bool isAlphaString = true;
    for (auto character : c) {
        if (!std::isalpha(character)) {
            isAlphaString = false;
            break;
        }
    }

    bool isInValidInputs = c.length() == 1 && std::find(validInputs.begin(), validInputs.end(), c.at(0)) != validInputs.end();
    return isAlphaString || isInValidInputs;
}

/**************************************************
 * messages
 **************************************************/
void Game::PressForXMessage(const std::vector<char>& inputs) {
    for (auto input : inputs) {
        PressForXMessage(input);
        std::cout << " ";
    }
}
void Game::PressForXMessage(char key) {
    std::cout << "Press " + KeyboardKey(key) + " " + KeyboardValue(key) + ".";
}

void Game::InvalidInputMessage() {
    std::cout << "Invalid input.";
}

void Game::PrintProblem(const std::pair<std::pair<std::string,std::string>,int>& prob) {
    std::cout << prob.first.first << " --> " << prob.first.second << " (" << prob.second << " steps)" << '\n' << std::endl;
}

void Game::PrintProblemProgress(const std::vector<std::string>& answer, const int& needToGuessIndex) {
    for (int i = 0; i < needToGuessIndex; i++) {
        std::cout << answer.at(i) << std::endl;
    }
}

void Game::PrintDivider() {
    std::cout << "--------------------------------------------------\n";
}
/**************************************************
 * misc / helper
 **************************************************/
bool Game::FileExists(const std::string& filename) const {
    std::ifstream f(filename.c_str());
    return f.good();
}
std::string Game::KeyboardKey(char key) {
    if (kKEYBOARD_MAP.find(key) == kKEYBOARD_MAP.end()) throw std::runtime_error("key not in map");
    return "[" + std::string(1, kKEYBOARD_MAP.find(key)->first) + "]";
}
std::string Game::KeyboardValue(char key) {
    if (kKEYBOARD_MAP.find(key) == kKEYBOARD_MAP.end()) throw std::runtime_error("key not in map");
    return kKEYBOARD_MAP.find(key)->second;
}