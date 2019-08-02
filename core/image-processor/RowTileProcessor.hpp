#ifndef ROWTILEPROCESSOR_HPP
#define ROWTILEPROCESSOR_HPP

#include <QStack>

#include <configure.h>
#include <Grid.hpp>
#include "AbstractProcessor.hpp"
#include <core/json/FilterJsonStreamDecoder.hpp>
#include <Logger.hpp>


namespace PROGRAM_NAMESPACE {

static constexpr ProcessErrorCode ROWTILEPROCESSOR_JSON_CHUNK_ERROR_CODE = PROCESSOR_THREAD_END_CODE + 1;

class RowTileProcessor : public AbstractProcessor<GridRowI> {
public:
    using Ptr = RowTileProcessor*;
    using ConstPtr = const RowTileProcessor*;

private:
    static constexpr int STACK_SIZE = 2;

protected:
    using FilterStripe = FilterStream::Stripe;
    QVector<GridRowI> doubleBuffer;
    FilterStream filterStream;
    QScopedPointer<IAbstractJsonStreamDecoder> jsonDecoder;
    QScopedPointer<GridI> grid;
    const QString jsonPath;
    const int tileSizeUm;
    int elaboratedStripe;
    int nextRowToInsert;

public:
    RowTileProcessor(const QString& threadName, const QString& filePath, int tileSizeUm, QObject* parent = Q_NULLPTR) :
        AbstractProcessor(threadName, parent),
        doubleBuffer(), jsonPath(filePath), tileSizeUm(tileSizeUm) {

        elaboratedStripe = 0;
        nextRowToInsert = 0;
    }

protected:

    virtual bool isResultsReady() const override {
        return !doubleBuffer.isEmpty();
    }

    virtual bool initialize() override {

        traceEnter;

        traceInfo() << "Inizializzazione RowTileProcessor";
        traceInfo() << "Nome file: " << jsonPath;
        traceInfo() << "Tile size [um]: " << tileSizeUm;

        if (!QFile::exists(jsonPath)) {
            traceErr() << "File non trovato: " << jsonPath;
            traceExit;
            return false;
        }

        if (tileSizeUm == 0) {
            traceErr() << "Il TileSizeUm deve essere > 0";
            traceExit;
            return false;
        }

        jsonDecoder.reset(new FilterJsonStreamDecoder(jsonPath));
        JsonStreamDecoderError headerParseError = jsonDecoder->decodeHeader(&filterStream);
        if (headerParseError != JSON_STREAM_DECODER_NO_ERROR) {
            traceErr() << "Errore nel parsing dell'header: " << headerParseError;
            traceExit;
            return false;
        }

        PointI m = filterStream.getMin();
        PointI M = filterStream.getMax();
        int w = M.getX() - m.getX();
        int h = M.getY() - m.getY();

        traceInfo() << "Header FilterStream";
        traceInfo() << "Punto Min: " << m;
        traceInfo() << "Punto Max: " << M;
        traceInfo() << "Larghezza: " << w;
        traceInfo() << "Altezza: " << h;

        grid.reset(new GridI(m, w, h, tileSizeUm));

        traceExit;

        return true;

    }

    bool canContinue() const {
        QMutexLocker locker(&mutex);
        bool isStopped = this->stopRequest;
        bool hasProccessedAllData = elaboratedStripe == filterStream.getStripesNumber();
        bool canContinue = !isStopped && !hasProccessedAllData;
        return canContinue;
    }

    virtual ProcessErrorCode loopProcess() override {

        traceEnter;

        bool isStopped = false;

        while (this->canContinue()) {

            QMutexLocker locker(&mutex);
            while (this->pauseRequest && !stopRequest)
                pauseCondition.wait(locker.mutex());

            if (this->stopRequest)
                return PROCESSOR_THREAD_STOP_REQUEST;
            locker.unlock();

            JsonStreamDecoderError chunkParseError = jsonDecoder->decodeChunk(&filterStream, elaboratedStripe);
            if (chunkParseError != JSON_STREAM_DECODER_NO_ERROR) {
                traceErr() << "Errore nel parsing dello stripe " << elaboratedStripe << " - Codice errore: " << chunkParseError;
                traceExit;
                return ROWTILEPROCESSOR_JSON_CHUNK_ERROR_CODE;
            }

            FilterStripe stripe = filterStream.getStripe(elaboratedStripe);
            auto&& pointsStripe = stripe.getPoints();
            if (pointsStripe.isEmpty()) {
                traceInfo() << "Nessun punto presente nello stripe: " << elaboratedStripe;
                ++elaboratedStripe;
                continue;
            }

            ++elaboratedStripe;

            PointI pointStart = pointsStripe.first();
            PointI pointEnd = pointsStripe.last();
            int xTilePointStart = this->grid->getTileIndexX(pointStart);
            int xTilePointEnd = this->grid->getTileIndexX(pointEnd);

            for (auto&& p: pointsStripe)
                this->grid->addPoint(p);

            while (nextRowToInsert < xTilePointEnd && !isStopped) {

                locker.relock();
                while (!isSpaceAvailable() && !stopRequest)
                    this->spaceAvailableCondition.wait(locker.mutex());

                while (this->pauseRequest && !stopRequest)
                    pauseCondition.wait(locker.mutex());

                if (stopRequest) {
                    isStopped = true;
                    locker.unlock();
                    continue;
                }

                this->doubleBuffer.append(grid->getRow(nextRowToInsert));
                traceInfo() << "Inserisco nel buffer il RowTile: " << nextRowToInsert;
                nextRowToInsert++;
                resultsReadyCondition.notify_all();
                locker.unlock();

            }

            // qui controllo se sto verificando l'ultimo stripe; in questo caso, devo iterare il ciclo
            // fino a quando non ho inserito tutti gli ultimi RowTiles appartenenti all'ultimo stripe
            if (elaboratedStripe == filterStream.getStripesNumber()) {

                int lastRowTileIndex = this->grid->getRows() - 1;
                while ((nextRowToInsert <= lastRowTileIndex) && !isStopped) {

                    locker.relock();
                    while (!isSpaceAvailable() && !stopRequest)
                        this->spaceAvailableCondition.wait(locker.mutex());

                    while (this->pauseRequest && !stopRequest)
                        pauseCondition.wait(locker.mutex());

                    if (stopRequest) {
                        isStopped = true;
                        locker.unlock();
                        continue;
                    }

                    this->doubleBuffer.append(grid->getRow(nextRowToInsert));
                    traceInfo() << "Inserisco nel buffer il RowTile: " << nextRowToInsert;
                    nextRowToInsert++;
                    resultsReadyCondition.notify_all();
                    locker.unlock();

                }

            }

        }

        traceExit;

        return PROCESSOR_THREAD_NO_ERROR;
    }

    virtual bool isSpaceAvailable() const override {
        return doubleBuffer.size() < STACK_SIZE;
    }

    virtual void getNextImpl(GridRowI& data) override {
        data = doubleBuffer.first();
        doubleBuffer.removeFirst();
    }

};

}

#endif // ROWTILEPROCESSOR_HPP
