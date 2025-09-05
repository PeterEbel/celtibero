# celtibero

I wrote this program for my uncle __Luis Alberto de la Fuente__, a very ambitious historian and expert on Spanish history with a particular focus on the pre-Roman period of the Iberians and Celtiberians.

He has studied this era intensively for many years and has used, among other books, the 17-volume [Encyclopaedia Paleohispánica](https://ifc-ojs.es/index.php/palaeohispanica) for his studies. Luis Alberto catalogued tens of thousands of inscriptions on coins and gravestones found at numerous excavation sites throughout Spain in an Excel spreadsheet, each with references to the respective volume, page and even line in the Paleohispánica encyclopaedia where they are mentioned. A quick and easy way to locate the place in the encyclopaedia where an inscription appears and eventually is described in more depth. Of course, that presumes he's read all the volumes. Thousands of pages. Unfortunately, one day my uncle mistakenly sorted a single column of the Excel sheet, completely messing up the references to the encyclopaedia and effectively destroying years of work.

This program, written in C++, implements a virtual reader that expects a list of all search terms (=inscriptions) as input (keywords.txt), it then opens the 17 volumes of the encyclopaedia (PDF files) one after the other (documents.txt), ‘reads’ them, so to speak, locates the search terms and writes the source of information (volume, page, line) to an output file (results.txt). The program uses the Aho Corasick algorithm and is extremely efficient. Implementations in Python were unsatisfactory in terms of performance.

The program requires the Poppler library for processing PDFs.

__Compilation__:

```bash
g++ main.cpp -o celtibero -std=c++17 -lpoppler-cpp
```

__Usage:__

```bash
./celtibero keywords.txt documents.txt results.txt <SomeIntegerNumber>
```

`SomeIntegerNumber` is a number that is passed as the last argument to define the minimum length a keyword must have in order to be processed. This allows for skipping very short keywords. Passing 3 means that keywords with only one or two characters are skipped.
