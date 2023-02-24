#ifndef VAULTFILE_H
#define VAULTFILE_H

#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "WordCryption.h"


#define FILE_NAME "vault.xcs"

vector<vector<string>> host_words;

void delete_stored_data() {
    host_words.clear();
}

void withdraw_items_files(HWND hwnd) {
    int itemCount = ListView_GetItemCount(hwnd);

    for (int i = 0; i < itemCount; i++) {
        vector<string> lineWords;
        LVITEM lvItem = { 0 };
        lvItem.iItem = i;
        lvItem.cchTextMax = 64;
        lvItem.pszText = new TCHAR[lvItem.cchTextMax];

        ListView_GetItemText(hwnd, i, 0, lvItem.pszText, lvItem.cchTextMax);
        lineWords.push_back(convert_ptstr_to_string(lvItem.pszText));

        ListView_GetItemText(hwnd, i, 1, lvItem.pszText, lvItem.cchTextMax);
        lineWords.push_back(convert_ptstr_to_string(lvItem.pszText));

        delete[] lvItem.pszText;
        host_words.push_back(lineWords);
    }
}

void update_encrypted_file() {
    remove(FILE_NAME);
    ofstream file(FILE_NAME);

    for (int i = 0; i < host_words.size(); i++) {

        if (host_words[i][0] == "")
            continue;

        file << encrypt(host_words[i][0]) << " " << encrypt(host_words[i][1]) << endl;
    }

    file.close();
}

void read_encrypted_file() {
    ifstream file(FILE_NAME);

    if (!file.good()) {
        ofstream newFile(FILE_NAME);
        file.close();
    }
    else {
        string line;

        while (getline(file, line)) {
            vector<string> lineWords;
            string word;
            int count = 0;
            stringstream ss(line);

            while (ss >> word) {
                count++;
                lineWords.push_back(decrypt(word));
            }

            if (count != 2)
                continue;

            host_words.push_back(lineWords);
        }

        file.close();
    }
}

#endif