# marcatrice
Software for controlling scan laser head with Qt programming language

Usando il compilatore MSVC (Visual Studio 2015 - msvc 14),
e' importante prestare attenzione al modo di rilascio;
a differenza di gcc/mingw, il progetto contiene 4 tipi di rilascio e
la selezione del modo di build avviene al momento del make.
(MSVC e' un compilatore che usa piu' configurazioni - multiconfiguration tools)

Per importare in QtCreator il progetto correttamente, seguire le seguenti istruzioni:

- modalita' Debug
  bisogna modificare il modo con cui QtCreator crea il progetto nel seguente modo:
  1. eliminare il Build Step di Default
  2. creare un Build Step / Custom process step e compilare i seguenti campi:
  command: cmake.exe
  arguments: --build . --target ALL_BUILD --config Debug
  working directory: %{buildDir}
  
- modalita' Release
  bisogna modificare il modo con cui QtCreator crea il progetto nel seguente modo:
  1. eliminare il Build Step di Default
  2. creare un Build Step / Custom process step e compilare i seguenti campi:
  command: cmake.exe
  arguments: --build . --target ALL_BUILD --config Release
  working directory: %{buildDir}
