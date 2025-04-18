# SkydivePRO

Un sistem de gestiune pentru centre de parașutism, care permite administrarea echipamentelor, rezervărilor de sloturi în avion și gestionarea clienților.

## Structura proiectului

```
SkydivePRO/
├── include/              # Fișiere header (.h)
├── src/                  # Fișiere sursă (.cpp)
├── .vscode/              # Configurații VS Code
│   ├── tasks.json        # Configurare build
│   ├── launch.json       # Configurare debugging
│   └── c_cpp_properties.json # Configurare IntelliSense
└── CMakeLists.txt        # Configurare CMake (opțional)
```

## Compilare și rulare în VS Code

### Metoda 1: Utilizând tasks.json

1. Deschideți proiectul în VS Code
2. Apăsați `Ctrl+Shift+B` (sau `Cmd+Shift+B` pe Mac) pentru a compila
3. Apăsați `F5` pentru a rula cu debugging sau `Ctrl+F5` pentru a rula fără debugging

### Metoda 2: Utilizând CMake

1. Instalați extensia CMake Tools pentru VS Code
2. Deschideți Command Palette (`Ctrl+Shift+P` sau `Cmd+Shift+P`)
3. Executați "CMake: Configure"
4. Executați "CMake: Build"
5. Executați "CMake: Debug"

## Compilare manuală

```bash
g++ -std=c++11 -I./include ./src/*.cpp -o SkydivePRO
```

## Rulare

```bash
./SkydivePRO
```

## Funcționalități principale

- Gestionarea echipamentelor de parașutism (adăugare, editare, ștergere)
- Gestionarea sloturilor de avion
- Sistem de rezervări pentru clienți
- Sistem de fidelizare cu puncte de loialitate
- Statistici și rapoarte pentru administratori

## Utilizatori

- **Client**: Poate vizualiza echipamente și sloturi disponibile, poate face rezervări și poate vedea istoricul rezervărilor
- **Administrator**: Poate gestiona toate aspectele sistemului, inclusiv echipamente, sloturi, rezervări și statistici
