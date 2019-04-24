Per le dll e le lib devo distinguere se sono in debug o release;
questo perche' le dll utilizzano funzioni della libreria stl; a seconda dal modo di compilazione,
cambia lo spazio di memoria delle strutture dati (vector/list/...)
vedere: https://www.quora.com/What-is-the-best-approach-to-pass-a-std-vector-or-any-STL-container-between-a-client-application-and-a-DLL
