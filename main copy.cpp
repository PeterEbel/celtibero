#include <iostream>
#include <string>
#include <vector>
#include <poppler/cpp/poppler-document.h>
#include <poppler/cpp/poppler-page.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <regex>
#include "aho_corasick.hpp"

std::string escape_regex(const std::string& s) {
    // Alle Regex-Sonderzeichen escapen
    static const std::string special = R"([\^$.|?*+(){}'])";
    std::string result;
    for (char c : s) {
        if (special.find(c) != std::string::npos) {
            result += '\\';
        }
        result += c;
    }
    return result;
}

// Hilfsfunktion: String in Kleinbuchstaben umwandeln
std::string to_lower(const std::string& str) {
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

int main(int argc, char *argv[]) {
    std::ofstream debug_file("debug.txt");
    if (argc != 5) {
        std::cout << "Usage: " << argv[0] << " <Begriffsdatei> <PDF-Liste> <Ergebnisdatei> <MinLaenge>\n";
        return 1;
    }

    std::string begriffsdatei = argv[1];
    std::string pdflist_datei = argv[2];
    std::string ergebnis_datei = argv[3];
    int min_len = std::stoi(argv[4]);

    // PDF-Dateinamen aus Datei einlesen
    struct PdfInfo {
        std::string filename;
        int start_page;
    };
    std::vector<PdfInfo> pdf_files;
    std::ifstream pdf_filelist(pdflist_datei);
    if (!pdf_filelist) {
        std::cerr << "Fehler beim Öffnen der PDF-Liste!\n";
        return 1;
    }
    std::string pdf_line;
    while (std::getline(pdf_filelist, pdf_line)) {
        if (!pdf_line.empty()) {
            if (pdf_line.back() == '\r') pdf_line.pop_back();
            // Zeile überspringen, wenn sie mit # beginnt
            if (pdf_line[0] == '#') continue;
            size_t sep = pdf_line.find(';');
            if (sep != std::string::npos) {
                std::string fname = pdf_line.substr(0, sep);
                int start = std::stoi(pdf_line.substr(sep + 1));
                pdf_files.push_back({fname, start});
            }
        }
    }
    pdf_filelist.close();

    // Begriffe aus Datei einlesen
    std::vector<std::string> begriffe;
    std::ifstream begriffe_file(begriffsdatei);
    if (!begriffe_file) {
    std::cerr << "Fehler beim Öffnen von begriffe.txt!\n";
    return 1;
    }
    std::string line;
    while (std::getline(begriffe_file, line)) {
        if (!line.empty()) {
            if (line.back() == '\r') line.pop_back();
            if ((int)line.size() >= min_len) {
                begriffe.push_back(line);
            }
        }
    }
    begriffe_file.close();

    // Begriffe in den Aho-Corasick-Trie laden
    AhoCorasick trie;
    for (const auto& begriff : begriffe) {
        trie.add(begriff);
    }
    trie.build();

    std::ofstream ergebnisse_file(ergebnis_datei);
    if (!ergebnisse_file) {
        std::cerr << "Fehler beim Öffnen von ergebnisse.txt!\n";
        return 1;
    }

    // Alle PDFs aus der Liste durchsuchen
    for (const auto& filename : pdf_files) {
        poppler::document *doc = poppler::document::load_from_file(filename.filename);
        if (!doc) {
            std::cerr << "Fehler beim Laden der PDF-Datei: " << filename.filename << "\n";
            continue;
        }
        int num_pages = doc->pages();
        std::cout << "Suche in " << filename.filename << " (Seiten: " << num_pages << ")\n";
        for (int i = 0; i < num_pages; ++i) {
            poppler::page *p = doc->create_page(i);
            if (!p) continue;
            std::vector<char> text_vec = p->text().to_utf8();
            std::string text(text_vec.begin(), text_vec.end());
            // Debug: Schreibe die ersten 10 Zeichen der Seite in debug.txt
            debug_file << filename.filename << ";" << (i+1) << ";" << text.substr(0, 10) << "\n";
            std::istringstream iss(text);
            std::string zeile;
            int zeilennummer = 0;
            while (std::getline(iss, zeile)) {
                if (!zeile.empty()) {
                    ++zeilennummer;
                    trie.search(zeile, [&](int keyword_index, int position) {
                        const std::string& word = begriffe[keyword_index];
                        int start = position + 1 - word.size();
                        int end = position + 1;
                        auto is_word_char = [](char c) {
                            return std::isalnum(c) || c == '-' || c == '\'';
                        };
                        bool left_ok = (start == 0) || !is_word_char(zeile[start - 1]);
                        bool right_ok = (end == (int)zeile.size()) || !is_word_char(zeile[end]);
                        if (left_ok && right_ok) {
                            int referenz_seite = filename.start_page + i;
                            ergebnisse_file << word << ";" << filename.filename << ";" << referenz_seite << ";" << zeilennummer << "\n";
                        }
                    });
                }
            }
            delete p;
            }
        delete doc;
    }
    ergebnisse_file.close();
    debug_file.close();
    return 0;
}
