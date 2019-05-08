    #include "TestFrame.hpp"
#include "ui_TestFrame.h"

#include <vector>
#include <QVector>

#include "TestFrame.hpp"
#include "TestFrameLogic.hpp"
#include "DialogAlert.hpp"

#include <iostream>

#include <Grid.hpp>
#include <StackedTile.hpp>
#include <ComputationUtils.hpp>

#include <third-party/ipg-marking-library-wrapper/include/Scanner.h>
#include <third-party/ipg-marking-library-wrapper/include/PointList.h>
#include <third-party/ipg-marking-library-wrapper/include/LibraryException.h>
#include <third-party/ipg-marking-library-wrapper/include/PointParametersWrapper.h>
#include <third-party/ipg-marking-library-wrapper/include/VectorParametersWrapper.h>
#include <laser-ipg-temporary/utility/IpgYLPNLaserConfiguration.hpp>

#include <Logger.hpp>
#include <QMouseEvent>
#include <QElapsedTimer>

#include <json/FilterJsonParser.hpp>

using namespace PROGRAM_NAMESPACE;

/**********************************************
 *    T E S T   F R A M E  L O G I C
 *********************************************/

TestFrameLogic::TestFrameLogic() :
    isProcessStopped(false),
    isLaserInitialized(false) { }

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

void TestFrameLogic::setupLaserIpgYLPN(const QSharedPointer<ipg::IpgSyncInterface>& ipgInterface) {

    traceEnter;
    this->ipgInterface = ipgInterface;
    traceExit;

}

bool TestFrameLogic::setupLaserOn() {

    traceEnter;

    // prima di ogni comando aggiundo dei ritardi di 200ms
    // per evitare di inviare troppo velocemente comandi al laser;

    Settings& settings = Settings::instance();

    if (!ipgInterface->getIsConnected())
        if (!ipgInterface->connectToLaser(settings.getIpgYLPNLaserIpAddress(), settings.getIpgYLPNLaserPort())) {
            traceErr() << "Impossibile connettersi al laser IPG";
            DialogAlert diag;
            diag.setupLabels("Error", "Impossibile connettersi al laser IPG");
            diag.exec();
            return false;
        }
    traceInfo() << "Connessione al laser avvenuta con successo";

    ipg::IPG_USHORT executionCode = 0;

    /* NOTE NIC 28/11/2018 - prima di settare emissione enable a on, lo setto a OFF;
     * infatti se faccio il reset ma sono in questa condizione:
     * 1. power supply off
     * 2. emission on
     * anche facendo il reset e dando power supply on,
     * il laser non va in ready; questo perche' il power supply funziona
     * sse il segnale di emission enable e' a OFF
     * allora prima di tutto imposto emissione enable a OFF
     */
    this->thread()->msleep(200);
    if (!ipgInterface->setEE(false, executionCode) || executionCode > 0) {
        traceErr() << "Impossibile inviare il comando emission OFF";
        if (executionCode > 0)
            traceErr() << "Descrizion errore ipg: " << ipg::getExecOpCodeDescription(executionCode);
        DialogAlert diag;
        diag.setupLabels("Error", "Impossibile inviare il comando emission OFF");
        diag.exec();
        return false;
    }

    traceInfo() << "Emissione enable: OFF";

    this->thread()->msleep(200);
    // reset errori
    if (!ipgInterface->reset(executionCode) || executionCode > 0) {
        traceErr() << "Impossibile inviare il comando di reset al laser";
        if (executionCode > 0)
            traceErr() << "Descrizion errore ipg: " << ipg::getExecOpCodeDescription(executionCode);
        DialogAlert diag;
        diag.setupLabels("Error", "Impossibile inviare il comando di reset al laser");
        diag.exec();
        return false;
    }

    traceInfo() << "Reset errori OK";

    IpgYLPNLaserConfiguration& laserConf = IpgYLPNLaserConfiguration::instance();
    int pulse = laserConf.getMode(laserConf.getCurrentModeIndex()).pulseDuration;
    int power = laserConf.getCurrentPower();

    // imposto il modo
    if (!ipgInterface->setModeIndex(laserConf.getCurrentModeIndex(), executionCode) || executionCode > 0) {
        traceErr() << "Impossibile impostare la modalita' selezionata";
        if (executionCode > 0)
            traceErr() << "Descrizion errore ipg: " << ipg::getExecOpCodeDescription(executionCode);
        DialogAlert diag;
        diag.setupLabels("Error", "Impossibile inviare il comando di reset al laser");
        diag.exec();
        return false;
    }
    traceInfo() << "Larghezza impulso impostata: " << pulse;

    this->thread()->msleep(200);

    // imposto la frequenza
    bool result;
    if (!ipgInterface->setFrequency(laserConf.getCurrentFrequency(), result, executionCode) || executionCode > 0) {
        traceErr() << "Impossibile impostare la frequenza del laser";
        if (executionCode > 0)
            traceErr() << "Descrizion errore ipg: " << ipg::getExecOpCodeDescription(executionCode);
        DialogAlert diag;
        diag.setupLabels("Error", "Impossibile inviare il comando di reset al laser");
        diag.exec();
        return false;
    }

    traceInfo() << "Impostato PRR: " << laserConf.getCurrentFrequency();

    this->thread()->msleep(200);

    // imposto la potenza del laser
    if (!ipgInterface->setPower(power, result, executionCode) || executionCode > 0) {
        traceErr() << "Impossibile impostare la potenza del laser";
        if (executionCode > 0)
            traceErr() << "Descrizion errore ipg: " << ipg::getExecOpCodeDescription(executionCode);
        DialogAlert diag;
        diag.setupLabels("Error", "Impossibile impostare la potenza del laser");
        diag.exec();
        return false;
    }

    traceInfo() << "Potenza laser: " << power;

    this->thread()->msleep(200);

    // abilito l'emissione
    if (!ipgInterface->setEE(true, executionCode) || executionCode > 0) {
        traceErr() << "Impossibile abilitare l'emissione del laser";
        if (executionCode > 0)
            traceErr() << "Descrizion errore ipg: " << ipg::getExecOpCodeDescription(executionCode);
        DialogAlert diag;
        diag.setupLabels("Error", "Impossibile impostare la potenza del laser");
        diag.exec();
        return false;
    }

    traceInfo() << "Emissione abilitata";

    traceExit;
    return true;

}

bool TestFrameLogic::setupLaserOff() {

    traceEnter;

    Settings& settings = Settings::instance();

    if (!ipgInterface->getIsConnected())
        if (!ipgInterface->connectToLaser(settings.getIpgYLPNLaserIpAddress(), settings.getIpgYLPNLaserPort())) {
            traceErr() << "Impossibile connettersi al laser IPG";
            DialogAlert diag;
            diag.setupLabels("Error", "Impossibile connettersi al laser IPG");
            diag.exec();
            return false;
        }
    traceInfo() << "Connessione al laser avvenuta con successo";

    ipg::IPG_USHORT executionCode = 0;

    this->thread()->msleep(200);
    if (!ipgInterface->setEE(false, executionCode) || executionCode > 0) {
        traceErr() << "Impossibile inviare il comando emission OFF";
        if (executionCode > 0)
            traceErr() << "Descrizion errore ipg: " << ipg::getExecOpCodeDescription(executionCode);
        DialogAlert diag;
        diag.setupLabels("Error", "Impossibile inviare il comando emission OFF");
        diag.exec();
        return false;
    }

    traceInfo() << "Impostato emissione a OFF";

    this->thread()->msleep(200);

    bool result;
    // imposto la potenza del laser
    if (!ipgInterface->setPower(0, result, executionCode) || executionCode > 0) {
        traceErr() << "Impossibile impostare la potenza del laser a 0";
        if (executionCode > 0)
            traceErr() << "Descrizion errore ipg: " << ipg::getExecOpCodeDescription(executionCode);
        DialogAlert diag;
        diag.setupLabels("Error", "Impossibile impostare la potenza del laser a 0");
        diag.exec();
        return false;
    }

    traceInfo() << "Impostato potenza a 0";

    traceExit;
    return true;

}

bool TestFrameLogic::getPulseEnergy(float& energyJoule) {

    traceEnter;

    traceInfo() << "Interrogazione stato laser";

    Settings& settings = Settings::instance();

    if (!ipgInterface->getIsConnected())
        if (!ipgInterface->connectToLaser(settings.getIpgYLPNLaserIpAddress(), settings.getIpgYLPNLaserPort())) {
            traceErr() << "Impossibile connettersi al laser IPG";
            DialogAlert diag;
            diag.setupLabels("Error", "Impossibile connettersi al laser IPG");
            diag.exec();
            return false;
        }

    traceInfo() << "Connessione al laser avvenuta con successo";

    ipg::IPG_USHORT executionCode = 0;

    ipg::GetLaserStatusOutput status;
    if (!ipgInterface->getLaserStatus(status, executionCode) || executionCode > 0) {
        traceErr() << "Impossibile interrogare lo stato del laser";
        if (executionCode > 0)
            traceErr() << "Descrizion errore ipg: " << ipg::getExecOpCodeDescription(executionCode);
        DialogAlert diag;
        diag.setupLabels("Error", "Impossibile interrogare lo stato del laser");
        diag.exec();
        return false;
    }

    energyJoule = status.getPulseEnergy() * 0.001;

    traceExit;
    return true;

}

void TestFrameLogic::startProcess() {

    traceEnter;

    namespace imlw = ipg_marking_library_wrapper;

    this->isProcessStopped = false;

    int tileSizeMm = qPtr->ui->sbTileSize->value();
    double offsetXmm = qPtr->ui->dsbOffsetX->value();
    double offsetYmm = qPtr->ui->dsbOffsetY->value();

    int frequency = qPtr->ui->sbLaserFrequency->value() * 1000;

    // point
    int numberOfPulses = qPtr->ui->sbPointPulses->value();


    // circle points
    int circlePointsRadiusUm = qPtr->ui->sbCirclePointsRadius->value();
    int circlePointsNumberOfSides = qPtr->ui->sbCirclePointsNumberSides->value();
    int circlePointsNumberOfPulses = qPtr->ui->sbCirclePointsPulses->value();

    // circle vector
    int circleVectorRadiusUm = qPtr->ui->sbCircleVectorRadius->value();
    int circleVectorNumberOfRevolutions = qPtr->ui->sbCircleVectorNumberRevolutions->value();
    int circleVectorNumberOfSides = qPtr->ui->sbCircleVectorNumberSides->value();
    int circleVectorPitch = qPtr->ui->sbCircleVectorPointsPitch->value();

    float tileScaleXPercent = qPtr->ui->dsbScaleX->value();
    float tileScaleYPercent = qPtr->ui->dsbScaleY->value();
    float tileScaleXUnit = tileScaleXPercent * 0.01;
    float tileScaleYUnit = tileScaleYPercent * 0.01;

    traceInfo() << "Scale Tile X [%]: " << tileScaleXPercent;
    traceInfo() << "Scale Tile Y [%]: " << tileScaleYPercent;
    traceInfo() << "Scale Tile X [unit]: " << tileScaleXUnit;
    traceInfo() << "Scale Tile Y [unit]: " << tileScaleXUnit;

    //bool moveAxisYEachTile = qPtr->ui->cbMoveYEachTile->isChecked();
    //bool moveAxisYEachLayerTile = qPtr->ui->cbMoveYEachStackedTile->isChecked();
    int waitTimeMs = qPtr->ui->sbTileTime->value();
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

    QString fileData = file.readAll();
    file.close();

    QScopedPointer<IAbstractJsonParser> parser(new FilterJsonParser());
    Filter filter;
    parser->decodeJson(fileData.toUtf8(), &filter);
    QApplication::processEvents();
    qApp->processEvents();

    int pointsNumber = filter.getNumOfPoints();

    // inizio configurazione testa scansione ipg
    ioManager->setDigitalOutput(IOType::COMPRESSED_AIR_1);

    float energy = TestFrame::TEST_FRAME_PULSE_ENERGY_DFLT;

#ifdef FLAG_IPG_YLPN_LASER_PRESENT
    if (!this->setupLaserOn())
        return;
    if (!this->getPulseEnergy(energy))
        return;
#endif

    TestFrame::PointShapeEnum pointShape = static_cast<TestFrame::PointShapeEnum>(qPtr->pointShapeGroup->checkedId());
    if (pointShape == TestFrame::PointShapeEnum::UNDEFINED) {
        traceErr() << "Non e' stato selezionato alcuna forma di punto";
        DialogAlert diag;
        diag.setupLabels("Error", "Forma di punto non selezionata");
        diag.exec();
        return;
    }

    imlw::VectorList circleVectorsWRevolutions;
    if (pointShape == TestFrame::PointShapeEnum::CIRCLE_VECTOR) {
        imlw::VectorList circleVectors(imlw::PolygonProperties(circleVectorNumberOfSides, circleVectorRadiusUm));
        for (int i=0; i<circleVectorNumberOfRevolutions; ++i)
            circleVectorsWRevolutions.append(circleVectors);
    }

    imlw::PointList singleCirclePointList;
    if (pointShape == TestFrame::PointShapeEnum::CIRCLE_POINTS) {
        singleCirclePointList.append(imlw::PointList(imlw::PolygonProperties(circlePointsNumberOfSides, circlePointsRadiusUm)));
    }


    std::cout << "Circle Vector list";
    std::cout << circleVectorsWRevolutions;
    std::cout << "Single Circle point list";
    std::cout << singleCirclePointList;

#ifdef FLAG_SCANNER_HEAD_PRESENT
    const std::vector<imlw::ScannerInfo>& scannerList = imlw::Scanner::scanners();

    if (scannerList.empty()) {
        traceErr() << "Impossibile trovare lo scanner";
        DialogAlert diag;
        diag.setupLabels("Error", "Impossibile trovare lo scanner");
        diag.exec();
        return;
    }

    if (scannerList.at(0).getStatus() != imlw::ConnectionStatus::AVAILABLE) {
        traceErr() << "Lo scanner e' nello stato di busy";
        DialogAlert diag;
        diag.setupLabels("Error", "Lo scanner e' nello stato di busy");
        diag.exec();
        return;
    }

    QScopedPointer<imlw::Scanner> scanner;
    std::string err;

    try {

        /*
         * NOTE NIC 12/04/2019 - Scanner e' un oggetto RAII, quindi nel distruttore, se c'e' una connessione
         * aperta, prima di distruggere l'oggetto viene chiusa e successivamente
         * viene eliminato l'oggetto
         */

        scanner.reset(new imlw::Scanner(scannerList[0].getName(), true, imlw::Units::MICRONS, err));

        float powerpercent = 100.0;
        float width =  1.0f / frequency; // WARNING NIC 24/04/2019 - sarebbe da mettere 0.5 (chiedere a Longoni),
        float dwell = width;

        /*
         * NOTE NIC 15/04/2019 - per i singoli fori, configuro il laser in questo modo;
         * per i vettori, configuro il laser con OutputVectorProperties
         */
        if (pointShape == TestFrame::PointShapeEnum::POINT || pointShape == TestFrame::PointShapeEnum::CIRCLE_POINTS) {

            imlw::OutputPointsProperties pointsProperties(energy);
            scanner->config(pointsProperties, 0.0f);

            imlw::PointParametersWrapper ppw = scanner->getPointParameters();
            ppw.clearLaserEntries();
            ppw.addLaserEntry(dwell, width, powerpercent, numberOfPulses);

        } else if (pointShape == TestFrame::PointShapeEnum::CIRCLE_VECTOR) {

            imlw::OutputVectorsProperties vectorProperties(circleVectorPitch, energy);
            scanner->config(vectorProperties);

        }

        scanner->guide(false);

    } catch (imlw::LibraryException& ex) {
        traceErr() << "Eccezione in fase di connessione con testa scansione";
        traceErr() << "Descrizione eccezione: " << ex.what();
        DialogAlert diag;
        diag.setupLabels("Error", ex.what());
        diag.exec();
        return;
    }

    // fine configurazione testa scansione ipg
#endif

    // creo la griglia (in tile)
    PointI m = filter.getMin();
    PointI M = filter.getMax();
    int w = M.getX() - m.getX();
    int h = M.getY() - m.getY();

    // la griglia la creo suddividendola in tile
    GridI grid(m, w, h, tileSizeMm*1000);

    for (auto&& p: filter.getPoints())
        grid.addPoint(p);

    qApp->processEvents();
    int countTile = 0;

    // griglia creata

    QElapsedTimer tileMeasureTimer;
    QElapsedTimer stackedTileMeasureTimer;

    // ciclo su tutte le righe
    for (int r=0; r<grid.getRows(); ++r) {

        stackedTileMeasureTimer.start();

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

        qApp->processEvents();
        int res = this->motionManager->moveX(moveXmm);

        if (motionManager->isErr(res)) {

            QString descrErr = MotionManager::decodeError(res);
            traceErr() << "Errore comando move asse X - codice:" << res;
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
                qApp->processEvents();
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

            tileMeasureTimer.start();

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

                qApp->processEvents();
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
                    t.setInterval(100);

                    res = MOTION_MANAGER_NO_ERR;

                    QMetaObject::Connection c1 = connect(&t, &QTimer::timeout, [&]() {
                        if (loop.isRunning()) {
                            qApp->processEvents();
                            if (!qPtr->motionBean.getAxisYMoveInProgress()) {
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
                    qApp->processEvents();
                }
            }

//            this->thread()->msleep(waitTimeMs);
            qApp->processEvents();

#ifdef FLAG_SCANNER_HEAD_PRESENT
            try {
                scanner->laser(imlw::LaserAction::Enable);
            } catch (imlw::LibraryException& ex) {
                traceErr() << "Eccezione testa scansione al comando laser enable";
                traceErr() << "Descrizione eccezione: " << ex.what();
                DialogAlert diag;
                diag.setupLabels("Error", ex.what());
                diag.exec();
                isProcessStopped = true;
            }
#endif
            if (isProcessStopped)
                break;

            PointI offset = currentTile.getCenter();
            offset.setX(-offset.getX());
            offset.setY(-offset.getY());
            PointSetI movePoints = ComputationUtils::movePointSet(currentTile.getPointSet(), offset);
            movePoints = ComputationUtils::axisBase2HeadBase(movePoints);
            const QVector<PointI>& vectorPoints = movePoints.getVector();

            QTimer waitTimeTimer;
            waitTimeTimer.setInterval(waitTimeMs);
            waitTimeTimer.setSingleShot(true);

            if (pointShape == TestFrame::PointShapeEnum::POINT) {

                std::list<imlw::Point> listOfPoints;
                for (auto&& p: vectorPoints)
                    listOfPoints.push_back(imlw::Point(p.getX(), p.getY()));

                imlw::PointList outputPoints(listOfPoints);
                outputPoints.rotate(angleRad);
                outputPoints.scale(tileScaleXUnit, tileScaleYUnit, 1);

                waitTimeTimer.start();
                qApp->processEvents();

#ifdef FLAG_SCANNER_HEAD_PRESENT
                try {

                    scanner->output(outputPoints);
                    listOfPoints.clear();
                    scanner->laser(imlw::LaserAction::Disable);

                } catch (imlw::LibraryException& ex) {
                    traceErr() << "Eccezione testa scansione al comando output punti";
                    traceErr() << "Descrizione eccezione: " << ex.what();
                    DialogAlert diag;
                    diag.setupLabels("Error", ex.what());
                    diag.exec();
                    isProcessStopped = true;
                }
#endif

            } else if (pointShape == TestFrame::PointShapeEnum::CIRCLE_POINTS) {

                imlw::PointList circles;
                for (auto&& p: vectorPoints) {

                    imlw::PointList singleCirclePoints;
                    singleCirclePoints.append(singleCirclePointList);
                    singleCirclePoints.shift(p.getX(), p.getY(), 0);
                    circles.append(singleCirclePoints);
                }
                circles.rotate(angleRad);
                circles.scale(tileScaleXUnit, tileScaleYUnit, 1);

                waitTimeTimer.start();
                qApp->processEvents();

#ifdef FLAG_SCANNER_HEAD_PRESENT
                try {

                    scanner->output(circles);
                    scanner->laser(imlw::LaserAction::Disable);

                } catch (imlw::LibraryException& ex) {
                    traceErr() << "Eccezione testa scansione al comando output vettori";
                    traceErr() << "Descrizione eccezione: " << ex.what();
                    DialogAlert diag;
                    diag.setupLabels("Error", ex.what());
                    diag.exec();
                    isProcessStopped = true;
                }
#endif

            } else if (pointShape == TestFrame::PointShapeEnum::CIRCLE_VECTOR) {

                imlw::VectorList circlesVet;
                for (auto&& p: vectorPoints) {

                    imlw::VectorList singleCircle;
                    singleCircle.append(circleVectorsWRevolutions);
                    singleCircle.shift(p.getX(), p.getY(), 0);
                    circlesVet.append(singleCircle);
                }
                circlesVet.rotate(angleRad);
                circlesVet.scale(tileScaleXUnit, tileScaleYUnit, 1);

                waitTimeTimer.start();
                qApp->processEvents();

#ifdef FLAG_SCANNER_HEAD_PRESENT
                try {

                    scanner->output(circlesVet);
                    scanner->laser(imlw::LaserAction::Disable);

                } catch (imlw::LibraryException& ex) {
                    traceErr() << "Eccezione testa scansione al comando output vettori";
                    traceErr() << "Descrizione eccezione: " << ex.what();
                    DialogAlert diag;
                    diag.setupLabels("Error", ex.what());
                    diag.exec();
                    isProcessStopped = true;
                }
#endif

            }

            if (isProcessStopped)
                break;

            QEventLoop loopIO;
            QTimer t;
            t.setInterval(100);

            /*
             * NOTE NIC 05/04/2019 - il funzionamento teorico e' il seguente:
             * una volta inviato i punti alla testa, aspetto 2 eventi:
             * 1. tempo attesa tile non scada
             * 2. segnale che la testa ha finito di stampare
             * se il tempo di attesa tile e' scaduto ma la testa sta ancora stampando,
             * faccio partire un terzo timer con intervallo di 100ms, che interroga lo stato della testa
             * se arriva il segnale che la testa ha finito di stampare ma il tempo di attesa
             * non e' ancora scaduto, aspetto che scada il tempo di attesa
             * Se nessuna delle due condizioni e' soddisfatta, si occupera' il timer di 100ms
             * a interrompere l'eventLoop
             */
            QMetaObject::Connection c1 = connect(&t, &QTimer::timeout, [&]() {
                if (loopIO.isRunning()) {
                    qApp->processEvents();
                    if (qPtr->digitalInputStatus[IOType::MARK_IN_PROGRESS].getValue() == false)
                        loopIO.quit();
                }
            });
            QMetaObject::Connection c2 = connect(ioManager.data(), &IOManager::markInProgressOffSignal, [&]() {
                if (loopIO.isRunning()) {
                    if (!waitTimeTimer.isActive())
                        loopIO.quit();
                }

            });
            QMetaObject::Connection c3 = connect(&waitTimeTimer, &QTimer::timeout, [&]() {
                if (loopIO.isRunning()) {
                    qApp->processEvents();
                    if (qPtr->digitalInputStatus[IOType::MARK_IN_PROGRESS].getValue() == false)
                        loopIO.quit();
                    else
                        t.start();
                }
            });

//            t.start();
            loopIO.exec();
            t.stop();
            waitTimeTimer.stop();

            qint64 tileTimeMeasureMs = tileMeasureTimer.elapsed();
            qPtr->ui->leTileTimeMeasure->setText(QString::number(tileTimeMeasureMs));

            ++countTile;
            QObject::disconnect(c1);
            QObject::disconnect(c2);
            QObject::disconnect(c3);

        }

        qint64 stackedTileMeasureMs = stackedTileMeasureTimer.elapsed();
        qPtr->ui->leStackTimeMeasure->setText(QString::number(stackedTileMeasureMs));

        if (isProcessStopped)
            break;

    }

#ifdef FLAG_IPG_YLPN_LASER_PRESENT
    if (!this->setupLaserOff())
        return;
#endif

    ioManager->unsetDigitalOutput(IOType::COMPRESSED_AIR_1);

#ifdef FLAG_SCANNER_HEAD_PRESENT
    try {
        scanner->laser(imlw::LaserAction::Disable);
    } catch (imlw::LibraryException& ex) {
        traceErr() << "Eccezione laser disable con testa scansione";
        traceErr() << "Descrizione eccezione: " << ex.what();
        DialogAlert diag;
        diag.setupLabels("Error", ex.what());
        diag.exec();
    }
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

void TestFrameLogic::changeGuideLaserState() {

    traceEnter;

    bool currentGuideLaserStatus = qPtr->ui->cbGuideLaser->isChecked();
    bool valueToSet = !currentGuideLaserStatus;

    ipg::IPG_USHORT res;

    Settings& settings = Settings::instance();

    if (!this->ipgInterface->getIsConnected()) {
        if (!this->ipgInterface->connectToLaser(settings.getIpgYLPNLaserIpAddress(), settings.getIpgYLPNLaserPort())) {
            DialogAlert diag;
            diag.setupLabels("Error", "Impossibile connettersi al laser");
            diag.exec();
            return;
        }
    }

    if (!this->ipgInterface->setGlMode(valueToSet, res)) {
        DialogAlert diag;
        diag.setupLabels("Error", "Impossibile settare il diodo rosso");
        diag.exec();
        return;
    }

    qPtr->ui->cbGuideLaser->setChecked(valueToSet);

    traceExit;

}



/**********************************************
 *         T E S T   F R A M E
 *********************************************/

TestFrame::TestFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TestFrame), dPtr(new TestFrameLogic()),
    laserParametersChanged(false) {

    traceEnter;

    dPtr->qPtr = this;

    this->setupUi();
    this->setupSignalsAndSlots();

    traceExit;

}

void TestFrame::setupDevices(const QSharedPointer<MotionManager> &motionManager,
        const QSharedPointer<IOManager> &ioManager,
        const QSharedPointer<ipg::IpgSyncInterface>& ipgInterface) {

    traceEnter;
    dPtr->setupMotionManager(motionManager);
    dPtr->setupIOManager(ioManager);
    dPtr->setupLaserIpgYLPN(ipgInterface);
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

void TestFrame::setFilePath(const QString& filePath) {

    traceEnter;
    ui->leFilePath->setText(filePath);
    traceExit;

}

void TestFrame::laserIpgYLPNConfigurationReady() {

    traceEnter;

    IpgYLPNLaserConfiguration& configuration = IpgYLPNLaserConfiguration::instance();
    for (auto&& item: configuration.getModes())
        ui->cbLaserPulseWidth->addItem(QString::number(item.pulseDuration), qRound(item.pulseDuration));

    ui->cbLaserPulseWidth->setCurrentIndex(configuration.getCurrentModeIndex());
    ui->sbLaserPower->setValue(qRound(configuration.getCurrentPower()));

    // NOTE NIC 04/04/2019 - quando imposto setCurrentIndex, in automatico viene generato il signal
    // currentIndexChanged, che mi imposta anche gli altri valori delle widget e mi aggiunge
    // l'asterisco nel tab; allora rimuovo l'asterisco con la funzione updateTabLaserLabel(false);
    // e' brutto, ma e' una cosa provvisoria...

    this->updateTabLaserLabel(false);

    ui->hsLaserPower->setEnabled(true);
    ui->sbLaserFrequency->setEnabled(true);
    ui->hsLaserFrequency->setEnabled(true);
    ui->cbLaserPulseWidth->setEnabled(true);
    ui->sbLaserPower->setEnabled(true);
    ui->pbGuideLaser->setEnabled(true);
    ui->pbSet->setEnabled(true);
    ui->pbReset->setEnabled(true);
    ui->cbLaserInitialized->setChecked(true);
    dPtr->isLaserInitialized = true;

    traceExit;

}

void TestFrame::setupUi() {

    traceEnter;

    ui->setupUi(this);

    ui->tabWidget->tabBar()->installEventFilter(this);
    ui->tabWidget->setCurrentIndex(0);

    ui->leFilePath->setReadOnly(true);

    ui->sbTileSize->setRange(TEST_FRAME_TILE_SIZE_MIN, TEST_FRAME_TILE_SIZE_MAX);

    ui->sbTileTime->setRange(TEST_FRAME_WAIT_TIME_MS_MIN, TEST_FRAME_WAIT_TIME_MS_MAX);
    ui->sbWaitTimeYMovement->setRange(TEST_FRAME_Y_MOVEMENTS_WAIT_TIME_MS_MIN, TEST_FRAME_Y_MOVEMENTS_WAIT_TIME_MS_MAX);

    ui->dsbOffsetX->setRange(TEST_FRAME_OFFSET_X_MIN, TEST_FRAME_OFFSET_X_MAX);
    ui->dsbOffsetX->setSingleStep(TEST_FRAME_DSB_STEP);
    ui->dsbOffsetX->setDecimals(3);

    ui->dsbOffsetY->setRange(TEST_FRAME_OFFSET_Y_MIN, TEST_FRAME_OFFSET_Y_MAX);
    ui->dsbOffsetY->setSingleStep(TEST_FRAME_DSB_STEP);
    ui->dsbOffsetY->setDecimals(3);

    ui->dsbScaleX->setRange(TEST_FRAME_SCALE_X_MIN, TEST_FRAME_SCALE_X_MAX);
    ui->dsbScaleX->setSingleStep(TEST_FRAME_DSB_SCALE_STEP);
    ui->dsbScaleX->setDecimals(3);
    ui->dsbScaleX->setValue(TEST_FRAME_SCALE_X_DFLT);

    ui->dsbScaleY->setRange(TEST_FRAME_SCALE_Y_MIN, TEST_FRAME_SCALE_Y_MAX);
    ui->dsbScaleY->setSingleStep(TEST_FRAME_DSB_SCALE_STEP);
    ui->dsbScaleY->setDecimals(3);
    ui->dsbScaleY->setValue(TEST_FRAME_SCALE_Y_DFLT);

    ui->sbRPointsPerTile->setRange(TEST_FRAME_POINTS_PER_TILE_MIN, TEST_FRAME_POINTS_PER_TILE_MAX);
    ui->sbNHMinDistance->setRange(TEST_FRAME_POINTS_DISTANCE_UM_MIN, TEST_FRAME_POINTS_DISTANCE_UM_MAX);

    ui->dsbAngleMrad->setRange(TEST_FRAME_ANGLE_MRAD_MIN, TEST_FRAME_ANGLE_MRAD_MAX);
    ui->dsbAngleMrad->setDecimals(3);
    ui->dsbAngleMrad->setSingleStep(TEST_FRAME_ANGLE_STEP);

    // point shape tab
    pointShapeGroup =  new QButtonGroup(this);
    ui->sbPointPulses->setRange(TEST_FRAME_PULSES_MIN, TEST_FRAME_PULSES_MAX);

    ui->sbCirclePointsNumberSides->setRange(TEST_FRAME_CIRCLE_NUM_SIDES_MIN, TEST_FRAME_CIRCLE_NUM_SIDES_MAX);
    ui->sbCirclePointsNumberSides->setSingleStep(TEST_FRAME_CIRCLE_NUM_SIDES_STEP);
    ui->sbCirclePointsPulses->setRange(TEST_FRAME_PULSES_MIN, TEST_FRAME_PULSES_MAX);
    ui->sbCirclePointsRadius->setRange(TEST_FRAME_CIRCLE_RADIUS_MIN, TEST_FRAME_CIRCLE_RADIUS_MAX);
    ui->sbCirclePointsRadius->setSingleStep(TEST_FRAME_CIRCLE_RADIUS_STEP);

    ui->sbCircleVectorRadius->setRange(TEST_FRAME_CIRCLE_RADIUS_MIN, TEST_FRAME_CIRCLE_RADIUS_MAX);
    ui->sbCircleVectorRadius->setSingleStep(TEST_FRAME_CIRCLE_RADIUS_STEP);
    ui->sbCircleVectorNumberRevolutions->setRange(TEST_FRAME_CIRCLE_NUM_REVOLUTIONS_MIN, TEST_FRAME_CIRCLE_NUM_REVOLUTIONS_MAX);
    ui->sbCircleVectorNumberRevolutions->setSingleStep(TEST_FRAME_CIRCLE_NUM_REVOLUTIONS_STEP);
    ui->sbCircleVectorNumberSides->setRange(TEST_FRAME_CIRCLE_NUM_SIDES_MIN, TEST_FRAME_CIRCLE_NUM_SIDES_MAX);
    ui->sbCircleVectorNumberSides->setSingleStep(TEST_FRAME_CIRCLE_NUM_SIDES_STEP);
    ui->sbCircleVectorPointsPitch->setRange(TEST_FRAME_CIRCLE_POINTS_PITCH_MIN, TEST_FRAME_CIRCLE_POINTS_PITCH_MAX);
    ui->sbCircleVectorPointsPitch->setSingleStep(TEST_FRAME_CIRCLE_POINTS_PITCH_STEP);

    pointShapeGroup->addButton(ui->rbPoint, static_cast<int>(PointShapeEnum::POINT));
    pointShapeGroup->addButton(ui->rbCirclePoints, static_cast<int>(PointShapeEnum::CIRCLE_POINTS));
    pointShapeGroup->addButton(ui->rbCircleVector, static_cast<int>(PointShapeEnum::CIRCLE_VECTOR));
    pointShapeGroup->button(static_cast<int>(PointShapeEnum::POINT))->setChecked(true);

    // laser tab
    ui->sbLaserPower->setRange(TEST_FRAME_LASER_MIN_POWER, TEST_FRAME_LASER_MAX_POWER);
    ui->hsLaserPower->setRange(TEST_FRAME_LASER_MIN_POWER, TEST_FRAME_LASER_MAX_POWER);
    ui->hsLaserPower->setSingleStep(TEST_FRAME_LASER_POWER_STEP);

    ui->leStackTimeMeasure->setReadOnly(true);
    ui->leTileTimeMeasure->setReadOnly(true);

#ifdef FLAG_IPG_YLPN_LASER_PRESENT
    ui->cbGuideLaser->setEnabled(false);
    ui->cbLaserInitialized->setEnabled(false);
    ui->hsLaserPower->setEnabled(false);
    ui->sbLaserFrequency->setEnabled(false);
    ui->hsLaserFrequency->setEnabled(false);
    ui->cbLaserPulseWidth->setEnabled(false);
    ui->sbLaserPower->setEnabled(false);
    ui->pbGuideLaser->setEnabled(false);
    ui->pbSet->setEnabled(false);
    ui->pbReset->setEnabled(false);
#endif

    traceExit;

}

void TestFrame::setupSignalsAndSlots() {

    traceEnter;
    connect(ui->pbStartProcess, &QPushButton::clicked, dPtr, &TestFrameLogic::startProcess);
    connect(ui->pbStopProcess, &QPushButton::clicked, dPtr, &TestFrameLogic::stopProcess);

    // signals/slots della tab laser
    connect(ui->sbLaserPower, static_cast<void (MDSpinBox::*)(int)>(&MDSpinBox::valueChanged), [&](int value) {
        this->ui->hsLaserPower->setValue(value);
        laserParametersChanged = true;
        this->updateTabLaserLabel(true);
    });
    connect(ui->hsLaserPower, &QSlider::valueChanged, [&](int value) {
        this->ui->sbLaserPower->setValue(value);
        laserParametersChanged = true;
        this->updateTabLaserLabel(true);
    });
    connect(ui->sbLaserFrequency, static_cast<void (MDSpinBox::*)(int)>(&MDSpinBox::valueChanged), [&](int value) {
        this->ui->hsLaserFrequency->setValue(value);
        laserParametersChanged = true;
        this->updateTabLaserLabel(true);
    });
    connect(ui->hsLaserFrequency, &QSlider::valueChanged, [&](int value) {
        this->ui->sbLaserFrequency->setValue(value);
        laserParametersChanged = true;
        this->updateTabLaserLabel(true);
    });
    connect(ui->cbLaserPulseWidth, static_cast<void (MDComboBox::*)(int)>(&MDComboBox::currentIndexChanged), [&](int index) {
        IpgYLPNLaserConfiguration& laserConfiguration = IpgYLPNLaserConfiguration::instance();
        IpgYLPNLaserConfiguration::Mode currentMode = laserConfiguration.getMode(index);
        ui->sbLaserFrequency->setRange(currentMode.minFrequency, currentMode.maxFrequency);
        ui->sbLaserFrequency->setValue(currentMode.nominalFrequency);
        ui->hsLaserFrequency->setRange(currentMode.minFrequency, currentMode.maxFrequency);
        ui->hsLaserFrequency->setValue(currentMode.nominalFrequency);
        laserParametersChanged = true;
        this->updateTabLaserLabel(true);
    });

    connect(ui->pbSet, &QPushButton::clicked, [&]() {
        int power = ui->sbLaserPower->value();
        int frequency = ui->sbLaserFrequency->value();
        int modeIndex = ui->cbLaserPulseWidth->currentIndex();

        IpgYLPNLaserConfiguration& laserConfiguration = IpgYLPNLaserConfiguration::instance();
        laserConfiguration.setCurrentPower(power);
        laserConfiguration.setCurrentModeIndex(modeIndex);
        laserConfiguration.setCurrentFrequency(frequency);
        laserParametersChanged = false;
        this->updateTabLaserLabel(false);

    });

    connect(ui->pbReset, &QPushButton::clicked, [&]() {

        if (dPtr->isLaserInitialized) {
            IpgYLPNLaserConfiguration& laserConfiguration = IpgYLPNLaserConfiguration::instance();
            int laserPower = qRound(laserConfiguration.getCurrentPower());
            this->ui->hsLaserPower->setValue(laserPower);
            ui->sbLaserPower->setValue(laserPower);

            int currentModeIndex = laserConfiguration.getCurrentModeIndex();
            int currentFrequency = laserConfiguration.getCurrentFrequency();
            ui->sbLaserFrequency->setRange(laserConfiguration.getMode(currentModeIndex).minFrequency, laserConfiguration.getMode(currentModeIndex).maxFrequency);
            ui->sbLaserFrequency->setValue(currentFrequency);

            ui->hsLaserFrequency->setRange(laserConfiguration.getMode(currentModeIndex).minFrequency, laserConfiguration.getMode(currentModeIndex).maxFrequency);
            ui->hsLaserFrequency->setValue(currentFrequency);
        }

        laserParametersChanged = false;
        this->updateTabLaserLabel(false);

    });

    connect(ui->pbGuideLaser, &QPushButton::clicked, dPtr, &TestFrameLogic::changeGuideLaserState);

    connect(pointShapeGroup, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), [&](int id) {

        switch (id) {
        case PointShapeEnum::POINT: ui->swPointShapeDetails->setCurrentIndex(0); break;
        case PointShapeEnum::CIRCLE_POINTS: ui->swPointShapeDetails->setCurrentIndex(1); break;
        case PointShapeEnum::CIRCLE_VECTOR: ui->swPointShapeDetails->setCurrentIndex(2); break;
        default: break;
        }

    });

//    connect(ui->tabWidget, &QTabWidget::currentChanged, [&](int index) {

//        if (this->laserParametersChanged) {
//            int currentIndex = ui->tabWidget->currentIndex();
//            if ((currentIndex == 1) && (index != 1)) {


//                DialogAlert diag;
//                diag.setupLabels("Warning", "Se non si impostano i parametri del laser, verranno ripristinati gli ultimi salvati. Continuare?");

//                if (diag.exec() == QDialog::Accepted) {

//                    IpgYLPNLaserConfiguration& laserConfiguration = IpgYLPNLaserConfiguration::instance();
//                    int laserPower = qRound(laserConfiguration.getCurrentPower());
//                    this->ui->hsLaserPower->setValue(laserPower);
//                    ui->sbLaserPower->setValue(laserPower);

//                    int currentModeIndex = laserConfiguration.getCurrentModeIndex();
//                    int currentFrequency = laserConfiguration.getCurrentFrequency();
//                    ui->sbLaserFrequency->setRange(laserConfiguration.getMode(currentModeIndex).minFrequency, laserConfiguration.getMode(currentModeIndex).maxFrequency);
//                    ui->sbLaserFrequency->setValue(currentFrequency);

//                    ui->hsLaserFrequency->setRange(laserConfiguration.getMode(currentModeIndex).minFrequency, laserConfiguration.getMode(currentModeIndex).maxFrequency);
//                    ui->hsLaserFrequency->setValue(currentFrequency);

//                    laserParametersChanged = false;
//                    this->updateTabUiLaserChanged(false);

//                } else {

//                    ui->tabWidget->setCurrentIndex(1);

//                }
//            }
//        }

//    });

    traceExit;

}

void TestFrame::updateTabLaserLabel(bool setAsterisk) {

    int tabLaserIndex = 1;
    QString tabText = ui->tabWidget->tabText(tabLaserIndex);

    if (!(tabText.right(1).compare("*") == 0) && setAsterisk)
        ui->tabWidget->setTabText(tabLaserIndex, tabText.append("*"));

    if ((tabText.right(1).compare("*") == 0) && !setAsterisk)
        ui->tabWidget->setTabText(tabLaserIndex, tabText.left(tabText.size()-1));

}

bool TestFrame::eventFilter(QObject* object, QEvent* event) {

    // NOTE NIC 04/04/2019 - filtro gli eventi del cambio tab;
    // se modifico qualche parametro e non lo imposto, informo l'utente che i dati
    // saranno resettati con una dialog
    if (qobject_cast<QTabBar*>(object)) {

        // abilito solamente il click del mouse
        if (event->type() == QEvent::Wheel || event->type() == QEvent::KeyPress)
            return true;

        if (this->laserParametersChanged) {

            if (event->type() == QEvent::MouseButtonPress) {

                int currentIndex = ui->tabWidget->currentIndex();

                if (currentIndex == TEST_FRAME_LASER_TAB_INDEX) {
                    auto mouseEvent = static_cast<QMouseEvent*>(event);
                    auto tabBar = ui->tabWidget->tabBar();
                    int tabIndex = tabBar->tabAt(mouseEvent->pos());

                    if (tabIndex != currentIndex) {
                        DialogAlert diag;
                        diag.setupLabels("Warning", "Se non si impostano i parametri del laser,\r\nverranno ripristinati gli ultimi salvati. Continuare?");
                        if (diag.exec() == QDialog::Rejected)
                            return true;
                        else {

                            if (dPtr->isLaserInitialized) {

                                IpgYLPNLaserConfiguration& laserConfiguration = IpgYLPNLaserConfiguration::instance();
                                int laserPower = qRound(laserConfiguration.getCurrentPower());
                                this->ui->hsLaserPower->setValue(laserPower);
                                ui->sbLaserPower->setValue(laserPower);

                                int currentModeIndex = laserConfiguration.getCurrentModeIndex();
                                int currentFrequency = laserConfiguration.getCurrentFrequency();
                                ui->sbLaserFrequency->setRange(laserConfiguration.getMode(currentModeIndex).minFrequency, laserConfiguration.getMode(currentModeIndex).maxFrequency);
                                ui->sbLaserFrequency->setValue(currentFrequency);

                                ui->hsLaserFrequency->setRange(laserConfiguration.getMode(currentModeIndex).minFrequency, laserConfiguration.getMode(currentModeIndex).maxFrequency);
                                ui->hsLaserFrequency->setValue(currentFrequency);

                            }
                            laserParametersChanged = false;
                            this->updateTabLaserLabel(false);

                        }
                    }

                }
            }

        }

    }

    return QFrame::eventFilter(object, event);

}
