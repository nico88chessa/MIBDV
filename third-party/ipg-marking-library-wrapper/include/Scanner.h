// Il blocco ifdef seguente viene in genere usato per creare macro che semplificano
// l'esportazione da una DLL. Tutti i file all'interno della DLL sono compilati con il simbolo IPGMARKINGLIBRARYWRAPPER_EXPORTS
// definito nella riga di comando. Questo simbolo non deve essere definito in alcun progetto
// che usa questa DLL. In questo modo qualsiasi altro progetto i cui file di origine includono questo file vedranno le funzioni
// le funzioni di IPGMARKINGLIBRARYWRAPPER_API come se fossero importate da una DLL, mentre questa DLL considera i simboli
// definiti con questa macro come esportati.

#pragma once

#include "IpgmlDef.h"
#include <iostream>
#include <vector>

#include "Enums.h"
#include "ScannerInfo.h"
#include "OutputPointsProperties.h"
#include "OutputVectorsProperties.h"
#include "PointParameters.h"
#include "PointParametersWrapper.h"
#include "PointList.h"

namespace ipg_marking_library_wrapper {

    class ScannerPrivate;
    class PointParametersHandler;

    class IPGMARKINGLIBRARYWRAPPER_API Scanner {
    private:
        ScannerPrivate* dPtr;
        bool isToCloseBeforeDelete;

    public:
        Scanner(const std::string& name, bool lock, Units u, std::string& err);
        ~Scanner();

        void close();
        void config(OutputPointsProperties& properties, float pitch);
        void config(OutputVectorsProperties);
        void exit();
        void finalize();
        bool isDone();
        bool isWaitingEvent();
        void lock();
        void output(PointList& list);
        void output(PointList& list, OutputPointsProperties& properties);
        /*void output(VectorList);
        void output(VectorList, OutputVectorsProperties);*/
        static std::vector<ScannerInfo> scanners();
        void unlock();
        void wait(float);
        void wait(WaitEvent);
        void laser(LaserAction l);
        void guide(bool guideValue);
        void clearLaserEntry();
        void addLaserEntry(float dwell, float width, float powerPercent, int count);
        
        void* getManagedObject();
        void releaseManagedObject();

        //void collect();

    };

}