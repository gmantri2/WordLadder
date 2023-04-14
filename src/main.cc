#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "word_graph.hpp"
#include "game.hpp"

int main(){
    auto game = Game(1,false);
    return game.RunGame();
    
    // WordGraph wg;

    // wg.ReadFromFile("graph.txt");

    // wg.SetPathLengths();
    // wg.SetStartingLengths();

    // std::pair<std::pair<std::string,std::string>,int>  prob = wg.GetProblem(10); //level #
    // std::cout << prob.first.first << " --> " << prob.first.second << " " << prob.second << '\n' << std::endl;
    // wg.BFS(prob.first.first,prob.first.second);


    // wg.fillFrequent("src/word_frequency.txt",500);
    // wg.MakeAdjacencyList("src/wordlist.txt");
    // const std::map<std::string, std::vector<std::string> >& temp = wg.GetAdj();
    // wg.writeToFile(temp, "graph.txt");


    // wg.BFS("junk","food");
    // std::cout << '\n' << std::endl;
    // wg.BFS("speed","force");
    // std::cout << '\n' << std::endl;
    // wg.BFS("flash","point");
    // std::cout << '\n' << std::endl;
    // wg.BFS("sand","dune");
    // std::cout << '\n' << std::endl;
    // wg.BFS("elder","scroll");
    // std::cout << '\n' << std::endl;
    // wg.BFS("squid","game");
    // std::cout << '\n' << std::endl;


    // wg.GetWords("src/wordlist.txt");
    // const std::map<std::string, std::vector<std::string> >& adj_ = wg.GetAdj();
    // int count = 0;
    // for (auto curr = adj_.begin(); curr != adj_.end(); ++curr) {
    //     if(curr->first.length() > 7){
    //         count++;
    //     }
    // }
    // std::cout << count << std::endl;
}