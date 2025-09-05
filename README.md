# celtibero

I wrote this program for my uncle __Luis Alberto de la Fuente__, a very ambitious historian and expert on Spanish history with a particular focus on the pre-Roman period of the Iberians and Celtiberians.

He has studied this era intensively for many years and has used, among other books, the 17-volume encyclopaedia Paläohispanica for his studies. Luis Alberto catalogued tens of thousands of inscriptions on coins and gravestones found at numerous excavation sites throughout Spain in an Excel spreadsheet, each with references to the respective volume, page and even line in the Palaeo-Hispanica encyclopaedia. This way for each inscription, it was easy to find where it appeared in the encyclopaedia. Unfortunately, one day my uncle mistakenly sorted a single column of the Excel sheet, completely messing up the references to the encyclopaedia and effectively destroying years of work.

With this programme written in C++, I implemented a virtual reader that expects a list of all search terms (=inscriptions) as input (keywords.txt), which then opens the 17 volumes of the encyclopaedia one after the other (documents.txt), ‘reads’ them, so to speak (documents.txt), locates the search terms and writes them to an output file (results.txt). The programme uses the Aho Corasick algorithm and is extremely efficient. Implementations in Python were unsatisfactory in terms of performance.

The program requires the Poppler library for processing PDFs.

__Compilation__:

```bash
g++ main.cpp -o pdftext -std=c++17 -lpoppler-cpp
```

__Usage:__

```bash
./pdftext keywords.txt documents.txt results.txt <SomeIntegerNumber>
```

<SomeIntegerNumber> is number that is passed as the last argument to define the minimum length a keyword must have in order to be processed. This allows for skipping very short keywords. Passing 3 means that keywords with only one or two characters are skipped.
