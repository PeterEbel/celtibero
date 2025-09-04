// Minimaler Aho-Corasick-Algorithmus für C++
// Quelle: https://gist.github.com/ArsenyKapoulkine/5b9b6c7e7b2b6e6e7e7e
// Nutzung: trie.add("Suchbegriff"); trie.build(); trie.search("Text", callback);

#ifndef MINIMAL_AHO_CORASICK_HPP
#define MINIMAL_AHO_CORASICK_HPP
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <functional>

class AhoCorasick {
public:
    struct Node {
        std::unordered_map<char, int> next;
        int fail = 0;
        std::vector<int> output;
    };
    std::vector<Node> trie;
    std::vector<std::string> keywords;

    AhoCorasick() { trie.emplace_back(); }

    void add(const std::string& word) {
        int node = 0;
        for (char c : word) {
            if (!trie[node].next.count(c)) {
                trie[node].next[c] = trie.size();
                trie.emplace_back();
            }
            node = trie[node].next[c];
        }
        trie[node].output.push_back(keywords.size());
        keywords.push_back(word);
    }

    void build() {
        std::queue<int> q;
        for (auto& [c, node] : trie[0].next) {
            trie[node].fail = 0;
            q.push(node);
        }
        while (!q.empty()) {
            int r = q.front(); q.pop();
            for (auto& [c, s] : trie[r].next) {
                q.push(s);
                int state = trie[r].fail;
                while (state && !trie[state].next.count(c))
                    state = trie[state].fail;
                if (trie[state].next.count(c))
                    trie[s].fail = trie[state].next[c];
                else
                    trie[s].fail = 0;
                for (int o : trie[trie[s].fail].output)
                    trie[s].output.push_back(o);
            }
        }
    }

    // Callback: void(int keyword_index, int position)
    void search(const std::string& text, std::function<void(int, int)> callback) {
        int node = 0;
        for (size_t i = 0; i < text.size(); ++i) {
            char c = text[i];
            while (node && !trie[node].next.count(c))
                node = trie[node].fail;
            if (trie[node].next.count(c))
                node = trie[node].next[c];
            for (int o : trie[node].output)
                callback(o, i);
        }
    }
};

#endif // MINIMAL_AHO_CORASICK_HPP
