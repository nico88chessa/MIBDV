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

TestFrameLogic::TestFrameLogic() { }

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

    int numberOfPulses = qPtr->ui->sbPulses->text().toInt();
    int frequency = qPtr->ui->sbFrequency->text().toInt();
    int pointsPerTile = qPtr->ui->sbPointsPerTile->text().toInt();
    int tileSize = qPtr->ui->sbTileSize->text().toInt();

    // inizio configurazione testa scansione ipg

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
    imlw::OutputPointsProperties pointParameters(0.001);

    scanner->guide(false);
    scanner->config(pointParameters, 0.0f);

    float powerpercent = 100.0;
    float width = (float) 1.0f / frequency;
    float dwell = width;

    scanner->clearLaserEntry();
    scanner->addLaserEntry(dwell, width, powerpercent, numberOfPulses);

    // fine configurazione testa scansione ipg

    // inizio gestione file

    QString filePath = qPtr->ui->leFilePath->text();
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        traceErr() << "Impossibile aprire il file: " << filePath;
        DialogAlert diag;
        diag.setupLabels("Error", "File non trovato");
        diag.exec();
        scanner->close();
        return;
    }

    QString firstLine(file.readLine());
    int pointsNumber = firstLine.split(':', QString::SkipEmptyParts).at(1).toInt();
    file.readLine();
    file.readLine();

    mibdv::PointSetI set(pointsNumber);
    bool okX;
    bool okY;
    int count = 0;
    int fixedSize = 10;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QList<QByteArray> lineSplit = line.split(';');
        if (lineSplit.size()>2) {
            int x = lineSplit.at(0).toInt(&okX);
            int y = lineSplit.at(1).toInt(&okY);
            if (!(okX && okY)) {
                traceErr() << "Errore nella lettura del file";
                DialogAlert diag;
                diag.setupLabels("Error", "Errore nella lettura del file");
                diag.exec();
                scanner->close();
                return;
            }
            set.addPoint(x, y);

        }
    }


    // creo la griglia (in tile)

    PointI m = set.getMin();
    PointI M = set.getMax();
    int w = M.getX() - m.getX();
    int h = M.getY() - m.getY();

    GridI grid(m, w, h, 10000);

    for (auto&& p: set.getVector())
        grid.addPoint(p);

    // griglia creata

    scanner->laser(imlw::LaserAction::Enable);

    // ciclo su tutte le righe
    for (int r=0; r<grid.getRows(); ++r) {

        // mi prendo la riga i-esima
        const QVector<TileI>& row = grid.getRow(r);

        int xUm = row.at(0).getBoundingBox().getMin().getX();
        float xMM = (float) xUm / 1000;

        traceDebug() << "Spostamento asse lungo l'asse X: " << xUm << "um";
        traceDebug() << "Spostamento asse lungo l'asse X: " << xMM << "mm";

        int res = this->motionManager->moveX(xMM);

        if (motionManager->isErr(res)) {

            QString descrErr = MotionManager::decodeError(res);
            traceErr() << "Errore comando move asse X - codice:" << res;
            traceErr() << "Descrizione:" << descrErr;

            DialogAlert diag;
            diag.setupLabels("Error", descrErr);
            diag.exec();
            scanner->close();
            return;

        } else {

            QEventLoop loop;
            res = MOTION_MANAGER_NO_ERR;

            QMetaObject::Connection c1 = connect(motionManager.data(), static_cast<void (MotionManager::*)(MotionStopCode)>(&MotionManager::axisXMotionStopSignal), [&](MotionStopCode sc) {
                if (loop.isRunning()) {
                    if (sc == MotionStopCode::MOTION_STOP_CORRECTLY)
                        res = MOTION_MANAGER_MOTION_X_STOP_CORRECTLY;
                    else
                        res = MOTION_MANAGER_MOTION_X_STOP_ERROR;
                    loop.quit();
                }
            });

            loop.exec();
            QObject::disconnect(c1);

        }

        // per ogni riga, dai tile creo gli stacked tile
        QVector<StackedTileI> stackedTiles;
        for (auto&& item: row)
            stackedTiles.append(StackedTileI(ComputationUtils::shuffleTile(item), pointsPerTile));

        // una volta creati gli stacked tile, devo creare la lista di tutti i tile per mescolarli
        QList<TileI> rowTiles;
        for (auto&& stack: stackedTiles)
            rowTiles.append(stack.getTiles());

        // disordino i tiles di ogni singola riga
        QList<TileI> sl = ComputationUtils::shuffleList(rowTiles);

        for (auto&& currentTile: sl) {

            // dalla lista, inizio a stampare ogni singolo tile
            int yUm = currentTile.getBoundingBox().getMin().getY();
            float yMM = (float) yUm / 1000;
            traceDebug() << "Spostamento asse lungo l'asse Y: " << yUm << "um";
            traceDebug() << "Spostamento asse lungo l'asse Y: " << yMM << "mm";

            res = this->motionManager->moveY(yMM);

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
                res = MOTION_MANAGER_NO_ERR;

                QMetaObject::Connection c1 = connect(motionManager.data(), static_cast<void (MotionManager::*)(MotionStopCode)>(&MotionManager::axisYMotionStopSignal), [&](MotionStopCode sc) {
                    if (loop.isRunning()) {
                        if (sc == MotionStopCode::MOTION_STOP_CORRECTLY)
                            res = MOTION_MANAGER_MOTION_Y_STOP_CORRECTLY;
                        else
                            res = MOTION_MANAGER_MOTION_Y_STOP_ERROR;
                        loop.quit();
                    }
                });

                loop.exec();
                QObject::disconnect(c1);

            }

            std::list<imlw::Point> listOfPoints;
            imlw::PointList outputPoints(listOfPoints);

            PointSetI movePoints = ComputationUtils::movePointSet(currentTile.getPointSet(), currentTile.getCenter());
            const QVector<PointI>& vectorPoints = movePoints.getVector();

            for (auto&& p: vectorPoints)
                listOfPoints.push_back(imlw::Point(p.getX(), p.getY()));

            scanner->output(outputPoints, pointParameters);
            listOfPoints.clear();

            QEventLoop loopIO;
            QMetaObject::Connection c1 = connect(ioManager.data(), &IOManager::markInProgressOffSignal, [&]() {
                if (loopIO.isRunning())
                    loopIO.quit();
            });
            loopIO.exec();
            QObject::disconnect(c1);

        }

    }

    scanner->laser(imlw::LaserAction::Disable);
    scanner->close();

    delete scanner;

    // fine gestione file






//    std::list<Point> listOfPoints;
//    float pitch = 250;

//    int x = 20;
//    int y = 20;
//    for (int j = 0; j < y; j++) {
//        for (int i = 0; i < x; i++) {
//            listOfPoints.push_back(Point(pitch*i, pitch*j));
//        }
//    }

//    {
//        PointList outputpoint(listOfPoints);

//        s->guide(false);
//        s->wait(WaitEvent::StartBit);
//        s->laser(LaserAction::Enable);
//        s->output(outputpoint);
//        s->laser(LaserAction::Disable);
//    }

//    int w = 10;

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
    ui->sbPointsPerTile->setRange(TEST_FRAME_POINTS_PER_TILE_MIN_VALUE, TEST_FRAME_POINTS_PER_TILE_MAX_VALUE);
    ui->sbPulses->setRange(TEST_FRAME_PULSES_MIN_VALUE, TEST_FRAME_PULSES_MAX_VALUE);
    ui->sbTileSize->setRange(TEST_FRAME_TILE_SIZE_MIN_VALUE, TEST_FRAME_TILE_SIZE_MAX_VALUE);

    traceExit;

}

void TestFrame::setupSignalsAndSlots() {

    traceEnter;
    connect(ui->pbStartProcess, &QPushButton::clicked, dPtr, &TestFrameLogic::startProcess);
    traceExit;

}
