#ifndef RESUMEROWTILEPROCESSOR_HPP
#define RESUMEROWTILEPROCESSOR_HPP

#include <QStack>

#include "RowTileProcessor.hpp"


namespace PROGRAM_NAMESPACE {

class ResumeRowTileProcessor : public RowTileProcessor {
public:
    using Ptr = ResumeRowTileProcessor*;
    using ConstPtr = const ResumeRowTileProcessor*;

private:
    int firstRowToProcess;

public:
    ResumeRowTileProcessor(const QString& threadName, const QString& filePath, int tileSizeUm, int firstRowToProcess, QObject* parent = Q_NULLPTR) :
        RowTileProcessor(threadName, filePath, tileSizeUm, parent), firstRowToProcess(firstRowToProcess) { }

public:
    int getFirstRowTileToProcess() {
        return firstRowToProcess;
    }

protected:

    virtual bool initialize() override {

        traceEnter;

        traceInfo() << "Inizializzazione ResumeRowTileProcessor";
        traceInfo() << "First row to process:" << firstRowToProcess;

        traceInfo() << "Chiamo costruttore class RowTileProcessor";

        if (!RowTileProcessor::initialize())
            return false;

        int rows = grid->getRows();
        if (!(firstRowToProcess < rows)) {
            traceErr() << "Prima riga da processare inferiore al numero di righe presenti nella griglia";
            traceErr() << "Riga da processare:" << firstRowToProcess;
            traceErr() << "Righe della griglia:" << rows << QString("[%0, %1]").arg(0).arg(rows-1);
            return false;
        }

        int stripesWidthUm = filterStream.getStripeWidthUm();
        int xPointStartUm = grid->getStart().getX() + firstRowToProcess * grid->getTileSize();
        elaboratedStripe = qFloor((xPointStartUm - grid->getStart().getX()) / stripesWidthUm);

        traceInfo() << "Larghezza stripe file json [um]:" << stripesWidthUm;
        traceInfo() << "Coordinata X da cui partire [um]:" << xPointStartUm;
        traceInfo() << "Stripe da elaborare:" << elaboratedStripe;
        traceInfo() << "Inizio ad inserire le righe dalla numero: " << firstRowToProcess;

        nextRowToInsert = firstRowToProcess;

        traceExit;
        return true;

    }

};

}

#endif // RESUMEROWTILEPROCESSOR_HPP
