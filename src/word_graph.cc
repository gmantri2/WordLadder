#include "word_graph.hpp"

/**************************************************
 * 1) creating adj list
 **************************************************/
void WordGraph::MakeAdjacencyList(const std::string& filename) {
    GetWords(filename);
    FillAdjacencyList();
}
void WordGraph::MakeAdjacencyListAndWriteToFile(const std::string& filename) {
    FillFrequent(kWORD_FREQUENCY_FILE, 500);
    MakeAdjacencyList(filename);
    WriteToFile(GetAdj(), kGRAPH_FILE);
}

void WordGraph::GetWords(const std::string& filename){
    std::ifstream ifs(filename);
    while(ifs.peek() != EOF){
        // go through each line in file (each line is one word)
        std::string temp;
        ifs >> temp;
        if(temp.length() > 6){
            continue;
        }
        if(temp.find('(') != std::string::npos || temp.find(')') != std::string::npos
         || temp.find('-') != std::string::npos || temp.find('\'') != std::string::npos){
            // if word is invalid skip it
            // in this case, invalid = has hypthens, is a repeat (repeats have parantheses)
            continue;
         }
        if(frequent_.find(temp) == frequent_.end()){
            continue;
        }
        // if valid word, insert it as a key in adj_ map.
        adj_.insert(std::pair<std::string, std::vector<std::string> >(temp, std::vector<std::string>()));
    }
}

void WordGraph::FillAdjacencyList() {
    for (auto curr = adj_.begin(); curr != adj_.end(); ++curr) {
        //std::cout << curr->first << std::endl;
        // for every word in the dictionary
        for (auto other = curr; other != adj_.end(); ++other) {
            // look through all other words in the dictionary
            if (curr == other) {
                // edge case: don't compare a word to itself; skip if so
                continue;
            }
            std::string s1 = curr->first;
            std::string s2 = other->first;
            if (IsEdge(s1, s2)) {
                // if the two words are connected by an edge and they are not in each other's
                // adj list, add them to each other's adj list
                InsertToAdjIfNewTwoWay(s1, s2);
            }
        }
    }
}

void WordGraph::InsertToAdjIfNewTwoWay(const std::string& s1, const std::string& s2) {
    InsertToAdjIfNewOneWay(s1, s2);
    InsertToAdjIfNewOneWay(s2, s1);
}

void WordGraph::InsertToAdjIfNewOneWay(const std::string& s1, const std::string& s2) {
    auto& v1 = adj_.find(s1)->second;
    if (std::find(v1.begin(), v1.end(), s2) == v1.end()) v1.push_back(s2);
}

bool WordGraph::IsEdge(const std::string& s1, const std::string& s2) const {
    // checks if two words are equal
    if(s1 == s2){
        return false;
    }
    // checks if words cannot possibly be edges because of the difference of their lengths
    if(int(s1.length() - s2.length()) < -1 || int(s2.length() - s1.length()) > 1){
        return false;
    }
    // if lengths are equal, count the number of substitutions
    if(s1.length() == s2.length()){
        int count = 0;
        for(size_t i = 0;i< s1.length();i++){
            if(s1[i] != s2[i]){
                count++;
            }
            if(count > 1){
                return false;
            }
        }
        return true;
    }
    // if lengths differ by one, check possible substrings of longer word 
    // and see if it  matches the shorter word when you remove any single letter
    // if s1 is longer than s2
    if(s1.length() > s2.length()){
        for(size_t i = 0;i < s1.length();i++){
            if(s1.substr(0,i).append(s1.substr(i+1,s1.length()-i)) == s2){
                return true;
            }
        }
        return false;
    }
    // if s2 is longer than s1
    for(size_t i = 0;i < s2.length();i++){
        if(s2.substr(0,i).append(s2.substr(i+1,s2.length()-i)) == s1){
            return true;
        }
    }
    return false;
}

void WordGraph::PrintAdjacencyPath(std::vector<std::string> path){
    for(std::string word: path){
        std::cout << word << std::endl;
    }
}


/**************************************************
 * 2) BFS
 **************************************************/
std::vector<std::string> WordGraph::BFS(const std::string& s1, const std::string& s2) {
    //checks if node is visited 
    std::map<std::string, int> visited;
    //keeps a track of the distance of each node from s1
    std::map<std::string, int> distance;
    //parent of each node
    std::map<std::string, std::string> parent;
    //vists s1
    visited[s1] = 1;
    distance[s1] = 0;
    //parent of s1 set as an space
    parent[s1] = " ";
    std::queue<std::string> queue;
    queue.push(s1);
    while(!queue.empty()) {
        std::string front = queue.front();
        queue.pop();
        std::vector<std::string> children = adj_.find(front)->second;
        for (size_t x = 0; x < children.size(); x++) {
            std::string curr = children[x];
            if(!visited[curr]) {
                visited[curr] = 1;
                queue.push(curr);
                distance[curr] = distance[front] + 1;
                parent[curr] = front;  
            }
        }
    }
    //stores the shortest path of each node from s1
    std::vector<std::string> path;
    //if not visited, s2 does not exist
    if (visited[s2] == 0) {
        std::cout << "No path found" << std::endl;
    }
    std::string destination = s2;
    while(parent[destination] != " ") {
        path.push_back(destination);
        destination = parent[destination];
    }
    path.push_back(destination);
    //final path stored in reverse of path
    std::reverse(path.begin(), path.end());
    // PrintAdjacencyPath(path);
    return path;
}

/**************************************************
 * 3) creating problems
 **************************************************/
void WordGraph::FillFrequent(const std::string& filename,int limit){
    std::ifstream ifs{filename};
    std::string line;
    while(std::getline(ifs,line)){
        std::istringstream iss(line);
        std::string word;
        int length,freq;
        iss >> word;
        iss >> length;
        iss >> freq;
        if(freq < limit){
            break;
        }
        frequent_.insert(word);
    }
}

std::pair<std::pair<std::string,std::string>,int>  WordGraph::GetProblem(int level){
    if (level < 1 || level > 10) throw std::runtime_error("level must be between 1 and 10 inclusive");

    std::vector<std::string> words1,words2;
    std::string word1,word2 = "";
    while(words2.size() == 0){
        srand((unsigned) time(0));
        words1 = wordLength_[starting_lengths_[level]];
        int randNum1 = rand()%(words1.size());
        word1 = words1.at(randNum1);

        words2 = GetSecondWord(word1,path_lengths_[level]);
    }
    int diff = 2;
    int count = 0;
    while(std::abs(diff) > 1){
        int randNum2 = rand()%(words2.size());
        word2 = words2.at(randNum2);
        diff = word1.length() - word2.length();
        count++;
        if(count > 5){
            break;
        }
    }

    std::pair<std::pair<std::string,std::string>,int> ret;
    std::pair<std::string,std::string> retfirst;
    retfirst.first = word1;
    retfirst.second = word2;
    ret.first = retfirst;
    ret.second = path_lengths_[level];
    return ret;
}

void WordGraph::SetStartingLengths(){
    starting_lengths_.push_back(-1);
    starting_lengths_.push_back(3); //1
    starting_lengths_.push_back(4); //2
    starting_lengths_.push_back(4); //3
    starting_lengths_.push_back(5); //4
    starting_lengths_.push_back(4); //5
    starting_lengths_.push_back(5); //6
    starting_lengths_.push_back(5); //7
    starting_lengths_.push_back(6); //8
    starting_lengths_.push_back(5); //9
    starting_lengths_.push_back(6); //10
}

void WordGraph::SetPathLengths(){
    path_lengths_.push_back(-1);
    path_lengths_.push_back(2); //1
    path_lengths_.push_back(2); //2
    path_lengths_.push_back(3); //3
    path_lengths_.push_back(3); //4
    path_lengths_.push_back(4); //5
    path_lengths_.push_back(4); //6
    path_lengths_.push_back(5); //7
    path_lengths_.push_back(5); //8
    path_lengths_.push_back(6); //9
    path_lengths_.push_back(6); //10
}

std::vector<std::string> WordGraph::GetSecondWord(const std::string& str, int level) {
    //checks if node is visited 
    std::map<std::string, int> visited;
    //keeps a track of the distance of each node from s1
    std::map<std::string, int> distance;
    //parent of each node
    std::map<std::string, std::string> parent;
    //vists s1
    visited[str] = 1;
    distance[str] = 0;
    std::vector<std::string> words;
    std::queue<std::string> queue;
    queue.push(str);
    while(!queue.empty()) {
        std::string front = queue.front();
        queue.pop();
        if(distance[front] == level){
            words.push_back(front);
            continue;
        }
        std::vector<std::string> children = adj_.find(front)->second;
        for (size_t x = 0; x < children.size(); x++) {
            std::string curr = children[x];
            if(!visited[curr]) {
                visited[curr] = 1;
                queue.push(curr);
                distance[curr] = distance[front] + 1;
            }
        }
    }
    return words;
}


/**************************************************
 * misc
 **************************************************/
void WordGraph::WriteToFile(std::map<std::string, std::vector<std::string> > graph, const std::string& filename){
    // saves graph in external file so we don't have to keep generating the graph every time
    std::ofstream ofs{filename};
    for (auto curr = graph.begin(); curr != graph.end(); ++curr){
        if(curr->second.size() == 0){
            continue;
        }
        ofs << curr->first << " ";
        for(std::string word:curr->second){
            ofs << word << " ";
        }
        ofs << '\n';
    }
}

void WordGraph::ReadFromFile(const std::string& filename){
    // reads saved graph from input file to construct graph
    std::ifstream ifs{filename};
    std::string line;
    while(std::getline(ifs,line)){
        std::istringstream iss(line);
        std::string word;
        std::string firstWord = "";
        bool first = true;
        while(iss >> word){
            if(first){
                adj_.insert(std::pair<std::string, std::vector<std::string> >(word, std::vector<std::string>()));
                first = false;
                firstWord = word;
            }
            else{
                InsertToAdjIfNewOneWay(firstWord,word);
            }
        }
        //storing word based on length
        if(wordLength_.find(firstWord.length()) == wordLength_.end()){
            std::vector<std::string> fir;
            fir.push_back(firstWord);
            wordLength_[firstWord.length()] = fir;
        }
        else{
            std::vector<std::string> words = wordLength_[firstWord.length()];
            words.push_back(firstWord);
            wordLength_[firstWord.length()] = words;
        }
    }
}

