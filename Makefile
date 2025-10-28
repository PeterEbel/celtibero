# Makefile für das Celtibero-Projekt
# Kompiliert das PDF-Durchsuchungsprogramm mit Aho-Corasick-Algorithmus

# Compiler und Flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LIBS = -lpoppler-cpp

# Zieldatei
TARGET = celtibero

# Quelldateien
SOURCES = main.cpp
HEADERS = aho_corasick.hpp

# Objektdateien
OBJECTS = $(SOURCES:.cpp=.o)

# Standard-Ziel
all: $(TARGET)

# Hauptprogramm kompilieren
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

# Objektdateien erstellen
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Debug-Version erstellen
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

# Alles bereinigen
clean:
	rm -f $(OBJECTS) $(TARGET)

# Programm ausführen (Beispiel-Aufruf)
run: $(TARGET)
	./$(TARGET) keywords.txt documents.txt results.txt 3

# Prüfen ob alle benötigten Dateien vorhanden sind
check:
	@echo "Prüfe benötigte Dateien..."
	@test -f keywords.txt || echo "WARNUNG: keywords.txt nicht gefunden"
	@test -f documents.txt || echo "WARNUNG: documents.txt nicht gefunden"
	@test -f aho_corasick.hpp || echo "WARNUNG: aho_corasick.hpp nicht gefunden"
	@pkg-config --exists poppler-cpp || echo "WARNUNG: poppler-cpp Bibliothek nicht installiert"
	@echo "Prüfung abgeschlossen."

# Installation der Abhängigkeiten (Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install libpoppler-cpp-dev pkg-config

# Hilfe anzeigen
help:
	@echo "Verfügbare Ziele:"
	@echo "  all          - Kompiliert das Programm (Standard)"
	@echo "  debug        - Kompiliert mit Debug-Informationen"
	@echo "  clean        - Entfernt kompilierte Dateien"
	@echo "  run          - Führt das Programm mit Beispiel-Parametern aus"
	@echo "  check        - Prüft ob alle benötigten Dateien vorhanden sind"
	@echo "  install-deps - Installiert Abhängigkeiten (Ubuntu/Debian)"
	@echo "  help         - Zeigt diese Hilfe an"

# Phony-Ziele (keine echten Dateien)
.PHONY: all clean debug run check install-deps help