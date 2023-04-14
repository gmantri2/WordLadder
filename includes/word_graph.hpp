#ifndef WORD_GRAPH_HPP
#define WORD_GRAPH_HPP

#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <map>
#include <vector>
#include <string>
#include <queue>
#include <tuple>
#include <unordered_set>
#include <cmath>

class WordGraph{
    public:
        /**************************************************
         * 1) creating adj list
         **************************************************/
        // calls GetWords and FillAdjacencyList
        void MakeAdjacencyList(const std::string& filename);
        // makes adj list + writes it to a file
        void MakeAdjacencyListAndWriteToFile(const std::string& filename);

        // returns true if s1 can be turned into s2 via single-character substitution, addition, or deletion
        // note that this does NOT use the adj list
        bool IsEdge(const std::string& s1, const std::string& s2) const;

        /**************************************************
         * 2) BFS
         **************************************************/
        // performs bfs from s1 to s2
        std::vector<std::string> BFS(const std::string& s1, const std::string& s2);

        /**************************************************
         * 3) creating problems
         **************************************************/
        //fill vector of most frequent words
        void FillFrequent(const std::string& filename,int limit);

        // returns two words to solve for the path
        std::pair<std::pair<std::string,std::string>,int> GetProblem(int level);
        // starting lengths of words for each level
        void SetStartingLengths();
        // length of path solutions for each level
        void SetPathLengths();


        /**************************************************
         * misc
         **************************************************/
        // returns const ref to adj_
        const std::map<std::string, std::vector<std::string> >& GetAdj() { return adj_; }

        // writes and stores graph to file
        void WriteToFile(std::map<std::string, std::vector<std::string> > graph, const std::string& filename);
        // reads graph from file
        void ReadFromFile(const std::string& filename);
        // returns name of graph file
        const std::string& GetGraphFile() { return kGRAPH_FILE; }

    private:
        /**************************************************
         * member variables
         **************************************************/
        std::vector<int> starting_lengths_;
        std::vector<int> path_lengths_;
        // adj list w/ key <word> and value <(>list of words following IsEdge requirements>
        std::map<std::string, std::vector<std::string> > adj_;
        // map of <length, list of words w/ that lenght>
        std::map<int,std::vector<std::string> > wordLength_;
        std::unordered_set<std::string> frequent_;

        const std::string kWORD_FREQUENCY_FILE = "src/word_frequency.txt";
        const std::string kGRAPH_FILE = "graph.txt";

        /**************************************************
         * 1) creating adj list
         **************************************************/
        // stores all words from english dictionary into map
        void GetWords(const std::string& filename);
        // fills in adj_ w/ adjacent words for each word
        void FillAdjacencyList();
        // inserts s1 into s2's adj_ list and vice versa if they are not already in each other's adj
        void InsertToAdjIfNewTwoWay(const std::string& s1, const std::string& s2);
        // inserts s1 into s2's adj_ list if s2 is not already in s1's adj
        void InsertToAdjIfNewOneWay(const std::string& s1, const std::string& s2);
        // prints out path
        void PrintAdjacencyPath(std::vector<std::string> path);

        /**************************************************
         * 3) creating problems
         **************************************************/
        // returns all words with a distance of 'level' away from str
        std::vector<std::string> GetSecondWord(const std::string& str, int level);
};

#endif