#ifndef __SPELL_CORRECTOR__HPP__
#define __SPELL_CORRECTOR__HPP__

#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

class spell_corrector {
public:
    //load corpus and train probability model
    int file_load(const std::string& filename);

    //correct wrong word
    std::string correct(std::string& word);

private:
    static std::map<std::string, int> dict;
    
    static char to_lower(char letter);

    //list all combinations
    static void edit_distance_1(const std::string& word, std::vector<std::string>& word_set);

    //remove non-word
    static void is_known(std::vector<std::string>& word_set);
};

std::map<std::string, int> spell_corrector::dict;

char spell_corrector::to_lower(char letter)
{
    if (isalpha(letter))
        return ::tolower(letter);
    return ' ';
}

void spell_corrector::edit_distance_1(const std::string& word, std::vector<std::string>& word_set)
{
    //deletion n ---> n
    for (std::string::size_type i = 0; i < word.size(); ++i) {
        word_set.push_back(word.substr(0, i) + word.substr(i+1)); 
    }

    //transposition n ---> n-1
    for (std::string::size_type i = 0; i < word.size() - 1; ++i) {
        word_set.push_back(word.substr(0, i) + word.at(i+1) + word.at(i) + word.substr(i+2));    
    }

    for (char c = 'a'; c <= 'z'; ++c) {
        //alteration n ---> 25n
        for (std::string::size_type i = 0; i < word.size(); ++i) {
            word_set.push_back(word.substr(0, i) + c + word.substr(i+1));    
        }

        //insertion n ---> 26(n+1)
        for (std::string::size_type i = 0; i < word.size() + 1; ++i) {
            word_set.push_back(word.substr(0, i) + c + word.substr(i));    
        }
    }
}

void spell_corrector::is_known(std::vector<std::string>& word_set)
{
    for (std::vector<std::string>::iterator it = word_set.begin(); it != word_set.end();) {
        if (spell_corrector::dict.find(*it) == spell_corrector::dict.end())
            it = word_set.erase(it);
        else
            it++;
    }
}

int spell_corrector::file_load(const std::string& filename)
{
    std::ifstream file(filename.c_str(), std::ios::binary | std::ios::in);

    if (!file) return -1;

    std::string content(std::istreambuf_iterator<char> (file), (std::istreambuf_iterator<char> ()));

    std::transform(content.begin(), content.end(), content.begin(), spell_corrector::to_lower);
    
    std::string::size_type i = 0;
    while (i < content.size() - 1) {
        while (!isalpha(content.at(i))) {
            i++;
        }
        std::string::size_type begin = i;

        while (isalpha(content.at(i))) {
            i++;
        }
        std::string::size_type end = i;

        spell_corrector::dict[content.substr(begin, end - begin)]++;
    }
    
    file.close();
}

std::string spell_corrector::correct(std::string& word)
{
    std::vector<std::string> words;
    std::map<std::string, int> words_map;

    if (spell_corrector::dict.find(word) != spell_corrector::dict.end())
        return word; 

    edit_distance_1(word, words);
    is_known(words);

    if (words.size() > 0) {
        for (std::string::size_type i = 0; i < words.size(); ++i)
            words_map[words.at(i)] = spell_corrector::dict[words.at(i)];
    
        std::string correct_word;
        int max_value = 0;
        for (std::map<std::string, int>::const_iterator it = words_map.begin(); it != words_map.end(); ++it) {
            if (it->second > max_value) {
                max_value = it->second;
                correct_word = it->first;
            }
        }

        return correct_word;
    }

    return " ";
}



#endif




