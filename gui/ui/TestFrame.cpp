#include "TestFrame.hpp"
#include "ui_TestFrame.h"


#include <vector>
#include <QVector>

#include "TestFrame.hpp"
#include "TestFrameLogic.hpp"
#include "DialogAlert.hpp"

#include <Grid.hpp>
#include <StackedTile.hpp>
#include <ComputationUtils.hpp>

#include <third-party/ipg-marking-library-wrapper/include/Scanner.h>
#include <third-party/ipg-marking-library-wrapper/include/PointList.h>

#include <Logger.hpp>

using namespace PROGRAM_NAMESPACE;

/**********************************************
 *    T E S T   F R A M E  L O G I C
 *********************************************/

TestFrameLogic::TestFrameLogic() :
    isProcessStopped(false) {

}

TestFrameLogic::~TestFrameLogic() { }

void TestFrameLogic::setupMotionManager(const QSharedPointer<MotionManager>& motionManager) {

    traceEnter;
    this->motionManager = motionManager;
    traceExit;

}

void TestFrameLogic::setupIOManager(const QSharedPointer<IOManager>& ioManager) {

    traceEnter;
    this->ioManager = ioManager;
    traceExit;

}

void TestFrameLogic::startProcess() {

    traceEnter;

    namespace imlw = ipg_marking_library_wrapper;

    this->isProcessStopped = false;

    int tileSizeMm = qPtr->ui->sbTileSize->value();
    double offsetXmm = qPtr->ui->dsbOffsetX->value();
    double offsetYmm = qPtr->ui->dsbOffsetY->value();

    int numberOfPulses = qPtr->ui->sbPulses->value();
    int frequency = qPtr->ui->sbFrequency->value() * 1000;

    //bool moveAxisYEachTile = qPtr->ui->cbMoveYEachTile->isChecked();
    //bool moveAxisYEachLayerTile = qPtr->ui->cbMoveYEachStackedTile->isChecked();

    int waitTimeMs = qPtr->ui->sbWaitTime->value();
    int waitTimeAfterYMovementMs = qPtr->ui->sbWaitTimeYMovement->value();
    double angleMRad = qPtr->ui->dsbAngleMrad->value();

    float angleRad = angleMRad * 0.001;
    traceInfo() << "Angolo rotazione [mrad]: " << angleMRad;
    traceInfo() << "Angolo rotazione [rad]: " << angleRad;

//    if (moveAxisYEachTile && moveAxisYEachLayerTile) {
//        DialogAlert diag;
//        diag.setupLabels("Error", "Controllare checkbox spostamenti Asse Y ");
//        diag.exec();
//        return;
//    }

    // widget random
    bool isRandomAlgorithm = qPtr->ui->cbRandomChoice->isChecked();
    bool isNeighborhoodAlgorithm = qPtr->ui->cbNHChoice->isChecked();

    if (isRandomAlgorithm && isNeighborhoodAlgorithm) {
        DialogAlert diag;
        diag.setupLabels("Error", "Controllare checkbox algoritmo tiling");
        diag.exec();
        return;
    }

    int randomPointsPerTile = qPtr->ui->sbRPointsPerTile->value();
    bool randomIsShuffleRowTiles = qPtr->ui->cbRShuffleRowTiles->isChecked();

    // widget neighborhood
    int neighborhoodMinDistanceUm = qPtr->ui->sbNHMinDistance->value();
    bool neighborhoodIsShuffleStackedTiles = qPtr->ui->cbNHShuffleStackedTiles->isChecked();
    bool neighborhoodIsShuffleRowTiles = qPtr->ui->cbNHShuffleRowTiles->isChecked();


    // inizio gestione file

    QString filePath = qPtr->ui->leFilePath->text();
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        traceErr() << "Impossibile aprire il file: " << filePath;
        DialogAlert diag;
        diag.setupLabels("Error", "File non trovato");
        diag.exec();
        return;
    }

    QString firstLine(file.readLine());
    int pointsNumber = firstLine.split(':', QString::SkipEmptyParts).at(1).toInt();
    file.readLine();
    file.readLine();

    // leggo tutti i punti e li aggiungo al pointSet set
    mibdv::PointSetI set(pointsNumber);
    bool okX;
    bool okY;
    int count = 0;
    int fixedSize = 10;
    while (!file.atEnd()) {
        QByteArray line = file.readLine().trimmed();
        QList<QByteArray> lineSplit = line.split(';');
        if (lineSplit.size()>1) {
            int x = lineSplit.at(0).toInt(&okX);
            int y = lineSplit.at(1).toInt(&okY);
            if (!(okX && okY)) {
                traceErr() << "Errore nella lettura del file";
                DialogAlert diag;
                diag.setupLabels("Error", "Errore nella lettura del file");
                diag.exec();
                return;
            }
            set.addPoint(x, y);

        }
    }

    // inizio configurazione testa scansione ipg

    ioManager->setDigitalOutput(IOType::COMPRESSED_AIR_1);

#ifdef FLAG_SCANNER_HEAD_PRESENT
    const std::vector<imlw::ScannerInfo>& scannerList = imlw::Scanner::scanners();

    if (scannerList.empty()) {
        traceErr() << "Impossibile trovare lo scanner";
        DialogAlert diag;
        diag.setupLabels("Error", "Impossibile trovare lo scanner");
        diag.exec();
        return;
    }

    std::string err;
    imlw::Scanner* scanner = new imlw::Scanner(scannerList[0].getName(), true, imlw::Units::MICRONS, err);
    imlw::OutputPointsProperties pointParameters(0.001f);

    scanner->config(pointParameters, 0.0f);
    scanner->clearLaserEntry();
#endif


    float powerpercent = 100.0;
    float width =  1.0f / frequency;
    float dwell = width;

#ifdef FLAG_SCANNER_HEAD_PRESENT
    scanner->addLaserEntry(dwell, width, powerpercent, numberOfPulses);
    scanner->guide(false);
//    scanner->laser(imlw::LaserAction::Enable);
#endif
    // fine configurazione testa scansione ipg


    // creo la griglia (in tile)
    PointI m = set.getMin();
    PointI M = set.getMax();
    int w = M.getX() - m.getX();
    int h = M.getY() - m.getY();

    // la grigli la creo suddividendola in tile
    GridI grid(m, w, h, tileSizeMm*1000);

    for (auto&& p: set.getVector())
        grid.addPoint(p);

    int countTile = 0;

    // griglia creata

    // ciclo su tutte le righe
    for (int r=0; r<grid.getRows(); ++r) {

        // mi prendo la riga i-esima
        const QVector<TileI>& row = grid.getRow(r);

        int xUm = row.at(0).getBoundingBox().getMin().getX();
        int offsetXum = offsetXmm * 1000;
        int tileSizeUm = tileSizeMm * 1000 * 0.5;

        int moveXum = xUm + offsetXum + tileSizeUm;
        float moveXmm = (float) moveXum * 0.001;

        traceDebug() << "Spostamento asse lungo l'asse X per bounding box immagine: " << xUm << "um";
        traceDebug() << "Offset asse lungo l'asse X: " << offsetXum << "um";
        traceDebug() << "Offset per dimensione tile X (diviso per 2): " << tileSizeUm << "um";
        traceDebug() << "Spostamento asse lungo l'asse X totale: " << moveXum << "um";
        traceDebug() << "Spostamento asse lungo l'asse X totale: " << moveXmm << "mm";

        int res = this->motionManager->moveX(moveXmm);

        if (motionManager->isErr(res)) {

            QString descrErr = MotionManager::decodeError(res);
            traceErr() << "Errore comando move asse X - codice:" << res;
            traceErr() << "Descrizione:" << descrErr;

            DialogAlert diag;
            diag.setupLabels("Error", descrErr);
            diag.exec();
#ifdef FLAG_SCANNER_HEAD_PRESENT
            scanner->close();
#endif
            return;

        } else {

            QEventLoop loop;
            QTimer t;
            t.setSingleShot(true);
            t.setInterval(1000);

            res = MOTION_MANAGER_NO_ERR;

            QMetaObject::Connection c1 = connect(&t, &QTimer::timeout, [&]() {
                if (!qPtr->motionBean.getAxisXMoveInProgress()) {
                    if (loop.isRunning()) {
                        res = MOTION_MANAGER_MOTION_X_STOP_CORRECTLY;
                        loop.quit();
                    }
                }
            });
            QMetaObject::Connection c2 = connect(motionManager.data(), static_cast<void (MotionManager::*)(MotionStopCode)>(&MotionManager::axisXMotionStopSignal), [&](MotionStopCode sc) {
                if (loop.isRunning()) {
                    if (sc == MotionStopCode::MOTION_STOP_CORRECTLY)
                        res = MOTION_MANAGER_MOTION_X_STOP_CORRECTLY;
                    else
                        res = MOTION_MANAGER_MOTION_X_STOP_ERROR;
                    loop.quit();
                }
            });

            t.start();
            loop.exec();
            t.stop();
            QObject::disconnect(c1);
            QObject::disconnect(c2);

        }

        // per ogni riga, dai tile creo gli stacked tile
        QVector<StackedTileI> stackedTiles;

        if (isRandomAlgorithm) {

            for (auto&& item: row)
                stackedTiles.append(StackedTileI(ComputationUtils::shuffleTile(item), randomPointsPerTile));

        } else if (isNeighborhoodAlgorithm) {

            for (auto&& item: row)
                stackedTiles.append(StackedTileI(item, neighborhoodMinDistanceUm, neighborhoodMinDistanceUm));

        } else {

            for (auto&& item: row)
                stackedTiles.append(StackedTileI(item, randomPointsPerTile));

        }

        // una volta creati gli stacked tile, devo creare la lista di tutti i tile per mescolarli
        QList<TileI> rowTiles;
        for (const StackedTileI& stack: stackedTiles) {

            if (isNeighborhoodAlgorithm && neighborhoodIsShuffleStackedTiles)
                rowTiles.append(ComputationUtils::shuffleList(stack.getTiles()));
            else
                rowTiles.append(stack.getTiles());
        }

        QList<TileI> sl;
        // disordino i tiles di ogni singola riga
        if (isRandomAlgorithm && randomIsShuffleRowTiles)
            sl = ComputationUtils::shuffleList(rowTiles);
        else if (isNeighborhoodAlgorithm && neighborhoodIsShuffleRowTiles)
            sl = ComputationUtils::shuffleList(rowTiles);
        else
            sl = rowTiles;


        float lastMoveYmm = 0;

        for (auto&& currentTile: sl) {

            // dalla lista, inizio a stampare ogni singolo tile
            int yUm = currentTile.getBoundingBox().getMin().getY();
            int offsetYum = offsetYmm * 1000;
            int tileSizeUm = tileSizeMm * 1000 * 0.5;

            int moveYum = yUm + offsetYum + tileSizeUm;
            float moveYmm = (float) moveYum * 0.001;

            traceDebug() << "Spostamento asse lungo l'asse Y per bounding box immagine: " << yUm << "um";
            traceDebug() << "Offset asse lungo l'asse Y: " << offsetYum << "um";
            traceDebug() << "Offset per dimensione tile Y (diviso per 2): " << tileSizeUm << "um";
            traceDebug() << "Spostamento asse lungo l'asse Y totale: " << moveYum << "um";
            traceDebug() << "Spostamento asse lungo l'asse Y totale: " << moveYmm << "mm";

            if (lastMoveYmm != moveYmm) {

                res = this->motionManager->moveY(moveYmm);
                lastMoveYmm = moveYmm;

                if (motionManager->isErr(res)) {

                    QString descrErr = MotionManager::decodeError(res);
                    traceErr() << "Errore comando move asse Y - codice:" << res;
                    traceErr() << "Descrizione:" << descrErr;

                    DialogAlert diag;
                    diag.setupLabels("Error", descrErr);
                    diag.exec();
                    return;

                } else {

                    QEventLoop loop;
                    QTimer t;
                    t.setSingleShot(true);
                    t.setInterval(1000);

                    res = MOTION_MANAGER_NO_ERR;

                    QMetaObject::Connection c1 = connect(&t, &QTimer::timeout, [&]() {
                        if (!qPtr->motionBean.getAxisYMoveInProgress()) {
                            if (loop.isRunning()) {
                                res = MOTION_MANAGER_MOTION_Y_STOP_CORRECTLY;
                                loop.quit();
                            }
                        }
                    });
                    QMetaObject::Connection c2 = connect(motionManager.data(), static_cast<void (MotionManager::*)(MotionStopCode)>(&MotionManager::axisYMotionStopSignal), [&](MotionStopCode sc) {
                        if (loop.isRunning()) {
                            if (sc == MotionStopCode::MOTION_STOP_CORRECTLY)
                                res = MOTION_MANAGER_MOTION_Y_STOP_CORRECTLY;
                            else
                                res = MOTION_MANAGER_MOTION_Y_STOP_ERROR;
                            loop.quit();
                        }
                    });

                    t.start();
                    loop.exec();
                    t.stop();
                    QObject::disconnect(c1);
                    QObject::disconnect(c2);

                    this->thread()->msleep(waitTimeAfterYMovementMs);
                }
            }

            this->thread()->msleep(waitTimeMs);

            if (isProcessStopped)
                break;

#ifdef FLAG_SCANNER_HEAD_PRESENT
//            scanner->laser(imlw::LaserAction::Disable);
            scanner->laser(imlw::LaserAction::Enable);
#endif
            std::list<imlw::Point> listOfPoints;
            PointI offset = currentTile.getCenter();
            offset.setX(-offset.getX());
            offset.setY(-offset.getY());
            PointSetI movePoints = ComputationUtils::movePointSet(currentTile.getPointSet(), offset);
            movePoints = ComputationUtils::axisBase2HeadBase(movePoints);
            const QVector<PointI>& vectorPoints = movePoints.getVector();

            for (auto&& p: vectorPoints)
                listOfPoints.push_back(imlw::Point(p.getX(), p.getY()));

            imlw::PointList outputPoints(listOfPoints);
            outputPoints.rotate(angleRad);
#ifdef FLAG_SCANNER_HEAD_PRESENT
            scanner->output(outputPoints);
            listOfPoints.clear();

//            scanner->laser(imlw::LaserAction::Enable);
            scanner->laser(imlw::LaserAction::Disable);
#endif
            QEventLoop loopIO;
            QTimer t;
            t.setSingleShot(true);
            t.setInterval(1000);

            QMetaObject::Connection c1 = connect(&t, &QTimer::timeout, [&]() {
                if (loopIO.isRunning())
                    if (qPtr->digitalInputStatus[IOType::MARK_IN_PROGRESS].getValue() == false)
                        loopIO.quit();
            });
            QMetaObject::Connection c2 = connect(ioManager.data(), &IOManager::markInProgressOffSignal, [&]() {
                if (loopIO.isRunning())
                    loopIO.quit();
            });

            t.start();
            loopIO.exec();
            t.stop();
            listOfPoints.clear();
            ++countTile;
            QObject::disconnect(c1);
            QObject::disconnect(c2);

        }

        if (isProcessStopped)
            break;

    }

    ioManager->unsetDigitalOutput(IOType::COMPRESSED_AIR_1);

#ifdef FLAG_SCANNER_HEAD_PRESENT
    scanner->laser(imlw::LaserAction::Disable);
    scanner->close();

    delete scanner;
#endif

    // fine gestione file

    DialogAlert diag;
    if (isProcessStopped)
        diag.setupLabels("Info", "Processo fermato");
    else
        diag.setupLabels("Info", "Bucato tutto");
    diag.exec();

    traceExit;

}

void TestFrameLogic::stopProcess() {
    traceEnter;
    this->isProcessStopped = true;
    traceExit;
}



/**********************************************
 *         T E S T   F R A M E
 *********************************************/

TestFrame::TestFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TestFrame), dPtr(new TestFrameLogic()) {

    traceEnter;

    dPtr->qPtr = this;

    this->setupUi();
    this->setupSignalsAndSlots();

    traceExit;

}

void TestFrame::setupDevices(
        const QSharedPointer<MotionManager> &motionManager,
        const QSharedPointer<IOManager> &ioManager) {

    traceEnter;
    dPtr->setupMotionManager(motionManager);
    dPtr->setupIOManager(ioManager);
    traceExit;

}

TestFrame::~TestFrame() {

    traceEnter;
    delete dPtr;
    delete ui;
    traceExit;
}

void TestFrame::updateMotionBean(const MotionBean& b) {

    traceEnter;
    this->motionBean = b;
    traceExit;

}

void TestFrame::updateDigitalInputStatus(const DigitalInputStatus& i) {

    traceEnter;
    this->digitalInputStatus = i;
    traceExit;

}

void TestFrame::setupUi() {

    traceEnter;

    ui->setupUi(this);

    ui->sbFrequency->setRange(TEST_FRAME_MIN_FREQUENCY, TEST_FRAME_MAX_FREQUENCY);
    ui->sbPulses->setRange(TEST_FRAME_PULSES_MIN, TEST_FRAME_PULSES_MAX);

    ui->sbTileSize->setRange(TEST_FRAME_TILE_SIZE_MIN, TEST_FRAME_TILE_SIZE_MAX);

    ui->sbWaitTime->setRange(TEST_FRAME_WAIT_TIME_MS_MIN, TEST_FRAME_WAIT_TIME_MS_MAX);
    ui->sbWaitTimeYMovement->setRange(TEST_FRAME_Y_MOVEMENTS_WAIT_TIME_MS_MIN, TEST_FRAME_Y_MOVEMENTS_WAIT_TIME_MS_MAX);

    ui->dsbOffsetX->setRange(TEST_FRAME_OFFSET_X_MIN, TEST_FRAME_OFFSET_X_MAX);
    ui->dsbOffsetX->setSingleStep(TEST_FRAME_DSB_STEP);
    ui->dsbOffsetX->setDecimals(3);

    ui->dsbOffsetY->setRange(TEST_FRAME_OFFSET_Y_MIN, TEST_FRAME_OFFSET_Y_MAX);
    ui->dsbOffsetY->setSingleStep(TEST_FRAME_DSB_STEP);
    ui->dsbOffsetY->setDecimals(3);

    ui->sbRPointsPerTile->setRange(TEST_FRAME_POINTS_PER_TILE_MIN, TEST_FRAME_POINTS_PER_TILE_MAX);
    ui->sbNHMinDistance->setRange(TEST_FRAME_POINTS_DISTANCE_UM_MIN, TEST_FRAME_POINTS_DISTANCE_UM_MAX);

    ui->dsbAngleMrad->setRange(TEST_FRAME_ANGLE_MRAD_MIN, TEST_FRAME_ANGLE_MRAD_MAX);
    ui->dsbAngleMrad->setDecimals(3);
    ui->dsbAngleMrad->setSingleStep(TEST_FRAME_ANGLE_STEP);

    traceExit;

}

void TestFrame::setupSignalsAndSlots() {

    traceEnter;
    connect(ui->pbStartProcess, &QPushButton::clicked, dPtr, &TestFrameLogic::startProcess);
    connect(ui->pbStopProcess, &QPushButton::clicked, dPtr, &TestFrameLogic::stopProcess);

    traceExit;

}
