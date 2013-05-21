#include <iostream>
#include "spell_corrector.hpp"

int main(int argc, char *argv[])
{
    spell_corrector corrector;

    corrector.file_load("big.txt");

    std::string word;
    while ((std::cout << "Input words: ") && (std::cin >> word)) {
        std::string correct_word = corrector.correct(word);

        if (correct_word != " ")
            std::cout << "Did you means: " << correct_word << std::endl;
        else
            std::cout << "Sorry, no available word" << std::endl;
    } 

    return 0;
}



