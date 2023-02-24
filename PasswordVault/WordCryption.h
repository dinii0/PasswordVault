#ifndef WORDCRYPTION_H
#define WORDCRYPTION_H

#include <string>

using namespace std;

char const KEY_CHAR = '@';

string encrypt(string word) {
    string encrypted_word;
    char buffer;
    bool check_positional_char = false;

    for (int i = 0; i < word.size(); i++) {
        buffer = word[i] ^ KEY_CHAR;

        if (check_positional_char) encrypted_word = buffer + encrypted_word;
        else encrypted_word = encrypted_word + buffer;

        check_positional_char = check_positional_char ? false : true;
    }

    return encrypted_word;
}

string decrypt(string encrypted_word) {
    int total_word_size = static_cast<int>(encrypted_word.size()) / 2;

    char buffer = encrypted_word[total_word_size] ^ KEY_CHAR;
    string decrypted_word;
    decrypted_word = buffer;

    for (int i = 1; i <= total_word_size; i++) {
        buffer = encrypted_word[total_word_size - i] ^ KEY_CHAR;
        decrypted_word += buffer;
        buffer = encrypted_word[total_word_size + i] ^ KEY_CHAR;
        decrypted_word = decrypted_word + buffer;
    }

    if (encrypted_word.size() % 2 == 0)
        decrypted_word[decrypted_word.size() - 1] = '\0';

    return decrypted_word;
}

#endif
