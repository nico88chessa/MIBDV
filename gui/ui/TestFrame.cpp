#include "TestFrame.hpp"
#include "ui_TestFrame.h"

#include <QVector>
#include <QMouseEvent>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QDir>
#include <QCryptographicHash>

#include <vector>
#include <iostream>

#include "TestFrame.hpp"
#include "TestFrameLogic.hpp"
#include "DialogAlert.hpp"
#include <Grid.hpp>
#include <StackedTile.hpp>
#include <ComputationUtils.hpp>
#include <third-party/ipg-marking-library-wrapper/include/PointList.h>
#include <third-party/ipg-marking-library-wrapper/include/LibraryException.h>
#include <third-party/ipg-marking-library-wrapper/include/PointParametersWrapper.h>
#include <third-party/ipg-marking-library-wrapper/include/VectorParametersWrapper.h>
#include <laser-ipg-temporary/utility/IpgYLPNLaserConfiguration.hpp>
#include <Logger.hpp>
#include <DeviceFactory.hpp>
#include <MotionAnalizer.hpp>
#include <IOSignaler.hpp>
#include <ErrorHandler.hpp>
#include <MachineStatusHandler.hpp>
#include <Types.hpp>
#include <json/FilterJsonParser.hpp>
#include <core/image-processor/RowTileProcessor.hpp>
#include <core/image-processor/ResumeRowTileProcessor.hpp>
#include <core/json/FilterJsonStreamDecoder.hpp>



using namespace PROGRAM_NAMESPACE;

static constexpr const char* WORKER_THREAD_NAME = "WORKER_THREAD";
static constexpr const char* ROWTILE_PROCESSOR_THREAD_NAME = "ROW_TILE_PROCESSOR_THREAD";
static constexpr const char* RESUME_ROWTILE_PROCESSOR_THREAD_NAME = "RESUME_TILE_PROCESSOR_THREAD";

static bool checkSWActive(const QString& status, const QString& key) {

    traceEnter;
    bool isSwActive = false;
//    QSettings settings(settingsPath);
//    QString validityEnabled = settings.value(PREF_VALIDITY_STATUS).toString();
//    QString validityKey = settings.value(PREF_VALIDITY_KEY).toString();
//    QString falseString = MD5::md5sum("false");

    QString falseString = QCryptographicHash::hash(QByteArray("false"), QCryptographicHash::Algorithm::Md5).toHex();

    if (status != falseString) {
        QDate todayDate= QDate::currentDate();

        QString validityDateString = QString("20%1%2/%3%4/%5%6")
                               .arg(key.at(0))
                               .arg(key.at(3))
                               .arg(key.at(1))
                               .arg(key.at(4))
                               .arg(key.at(2))
                               .arg(key.at(5));

        QDate validityDate = QDate::fromString(validityDateString, "yyyy/MM/dd");

        if (todayDate.daysTo(validityDate) > 0)
            isSwActive = true;
        else {
            traceErr() << "SYSTEM IS NOT ACTIVE!!!";
            isSwActive = false;
        }
        traceInfo() << QString("SYSTEM CONTROL ENABLED (%0 - %1)").arg(validityDate.toString()).arg(todayDate.toString());

    }
    else
        isSwActive = true;

    traceExit;
    return isSwActive;
}

/*
 *  W O R K E R   T H R E A D
 */

Worker::Worker(QObject* parent) : QObject(parent), printConfiguration(),
    hasToStop(false), hasToPause(false), hasErrors(false), commandsExecuted(PrintCommandExecuted::IDLE) {
    this->setupSignalsAndSlots();
}

void Worker::setupSignalsAndSlots() {

    traceEnter;
    connect(this, &Worker::stopRequestSignal, [&]() {
        traceInfo() << "Premuto pulsante di stop";
        this->hasToStop = true;
        if (!fileProcessorThread.isNull())
            fileProcessorThread->stop();
    });

    connect(this, &Worker::pauseRequestSignal, [&]() {
        traceInfo() << "Premuto pulsante di pause";
        this->hasToPause = true;
        if (!fileProcessorThread.isNull())
            fileProcessorThread->stop();
    });

    connect(this, &Worker::hasFatalsSignal, [&](QList<Error> errors) {
        traceErr() << "Rilevato errori";
        for (auto&& e: errors) {
            if (!errorList.contains(e))
                errorList.append(e);
            traceErr() << "Device:" << Utils::getStringFromDeviceKey(e.getDeviceKey());
            traceErr() << "Error id:" << e.getErrorId();
            traceErr() << "Error type:" << static_cast<int>(e.getErrorType());
            traceErr() << "Error Description:" << e.getErrorDescription();
        }
        hasErrors = true;
        emit stopProcessByErrorSignal();
    });

    connect(this, &Worker::hasErrorsSignal, [&](QList<Error> errors) {
        traceErr() << "Rilevato errori";
        for (auto&& e: errors) {
            if (!errorList.contains(e))
                errorList.append(e);
            traceErr() << "Device:" << Utils::getStringFromDeviceKey(e.getDeviceKey());
            traceErr() << "Error id:" << e.getErrorId();
            traceErr() << "Error type:" << static_cast<int>(e.getErrorType());
            traceErr() << "Error Description:" << e.getErrorDescription();
        }
        hasErrors = true;
        emit stopProcessByErrorSignal();
    });

    traceExit;

}

void Worker::startProcess() {

    QString format = "dd/MM/yyyy - HH:mm:ss";
    QString startTime = QDateTime::currentDateTime().toString(format);

    traceEnter;
    bool canProcess = this->beforeProcess();

    ExitCause exitCause = ExitCause::NOT_VALID;
    if (canProcess)
        exitCause = this->process();

    this->afterProcess();

    if (canProcess) {
        switch (exitCause) {
        case ExitCause::NOT_VALID:
        {
            // NOTE NIC 02/08/2019 - da verificare
            machineStatusNotifier->setCurrentStatus(MachineStatus::IDLE);
        } break;
        case ExitCause::PROCESS_COMPLETED_CORRECTLY:
        {
            machineStatusNotifier->setCurrentStatus(MachineStatus::IDLE);
            showDialogAsync("Info", "Foratura completata correttamente" \
                            "\nInizio: " + startTime + \
                            "\nFine: " + QDateTime::currentDateTime().toString(format));
        } break;
        case ExitCause::EXIT_BY_TIMEOUT:
        {
            if (printStatus.isValid())
                machineStatusNotifier->setCurrentStatus(MachineStatus::STOP_RESUMABLE);
            else
                machineStatusNotifier->setCurrentStatus(MachineStatus::IDLE);
        } break;
        case ExitCause::STOP_BY_USER:
        {
            machineStatusNotifier->setCurrentStatus(MachineStatus::IDLE);
            showDialogAsync("Warning", "Processo fermato" \
                            "\nInizio: " + startTime + \
                            "\nFine: " + QDateTime::currentDateTime().toString(format));
        } break;
        case ExitCause::PAUSE_BY_USER:
        {
            machineStatusNotifier->setCurrentStatus(MachineStatus::PAUSE);
            showDialogAsync("Warning", "Processo messo in pausa dall'utente" \
                            "\nInizio: " + startTime + \
                            "\nFine: " + QDateTime::currentDateTime().toString(format));
        } break;
        case ExitCause::ERROR_CATCHED:
        {
            QString errors;
            for (auto&& e: errorList)
                errors += Utils::getStringFromDeviceKey(e.getDeviceKey()) + " - " \
                       + QString::number(e.getErrorId()) + " - " \
                       + QString::number(static_cast<int>(e.getErrorType())) + " - " \
                       + e.getErrorDescription() + "\n";
            errors += "\n\nInizio: " + startTime + \
                    "\nFine: " + QDateTime::currentDateTime().toString(format);
            if (printStatus.isValid())
                machineStatusNotifier->setCurrentStatus(MachineStatus::STOP_RESUMABLE);
            else
                machineStatusNotifier->setCurrentStatus(MachineStatus::IDLE);
            showDialogAsync("Errori rilevati", errors);
        } break;
        case ExitCause::INTERNAL_ERROR:
        {
            if (printStatus.isValid())
                machineStatusNotifier->setCurrentStatus(MachineStatus::STOP_RESUMABLE);
            else
                machineStatusNotifier->setCurrentStatus(MachineStatus::IDLE);
        } break;
        }
    }

    emit finishedSignal();

    traceExit;

}

bool Worker::beforeProcess() {

    traceEnter;
    // TODO NIC 03/06/2019 - gestire gli errori

    updateStatusAsync("Device initializiation...");
    ioManager = DeviceFactoryInstance.instanceIOManager();
    motionManager = DeviceFactoryInstance.instanceMotionManager();
    machineStatusNotifier = DeviceFactoryInstance.instanceMachineStatusNotifier();

    if (!ioManager->isConnected())
        if (!ioManager->connect()) {
            traceErr() << "Impossibile connettersi all'IOManager";
            updateStatusAsync("Device initializiation... KO");
            return false;
        }

    if (!motionManager->isConnected())
        if (!motionManager->connect()) {
            traceErr() << "Impossibile connettersi al MotionManager";
            updateStatusAsync("Device initializiation... KO");
            return false;
        }

    if (machineStatusNotifier.isNull()) {
        traceErr() << "Impossibile istanziare l'oggetto MachineStatusNotifier";
        return false;
    }

    updateStatusAsync("Device initializiation... OK");

    traceExit;
    return true;

}

ExitCause Worker::process() {

    namespace imlw = ipg_marking_library_wrapper;
    using namespace PROGRAM_NAMESPACE;

    traceEnter;

    updateLastCommandExecute(PrintCommandExecuted::START);

    // inizio la validazione dei parametri

    traceInfo() << "Inizio validazione dati processo di stampa";
    updateStatusAsync("Reading parameters...");
    // prendo i parametri di stampa
    int frequencyHz = printConfiguration.getLaserFrequency() * 1000;
    double offsetXmm = printConfiguration.getOffsetXmm();
    double offsetYmm = printConfiguration.getOffsetYmm();
    double offsetZmm = printConfiguration.getOffsetZmm();
    int tileSizeMm = printConfiguration.getTileSizeMm();
    int tileSizeUm = tileSizeMm * 1000;
    float tileScaleXUnit = printConfiguration.getTileScaleXPercent() * 0.01;
    float tileScaleYUnit = printConfiguration.getTileScaleYPercent() * 0.01;
    double angleMRad = printConfiguration.getAngleMRad();
    float angleRad = angleMRad * 0.001;
    int waitTimeMs = printConfiguration.getWaitTimeMs();
    int waitTimeAfterYMovementMs = printConfiguration.getWaitTimeAfterYMovementMs();

    traceInfo() << "Frequency [Hz]: " << frequencyHz;
    traceInfo() << "OffsetXMm: " << offsetXmm;
    traceInfo() << "OffsetYMm: " << offsetYmm;
    traceInfo() << "OffsetZMm: " << offsetZmm;
    traceInfo() << "Tile size [mm]: " << tileSizeMm;
    traceInfo() << "Tile size [um]: " << tileSizeUm;
    traceInfo() << "Scale Tile X [%]: " << this->printConfiguration.getTileScaleXPercent();
    traceInfo() << "Scale Tile Y [%]: " << this->printConfiguration.getTileScaleYPercent();
    traceInfo() << "Scale Tile X [unit]: " << tileScaleXUnit;
    traceInfo() << "Scale Tile Y [unit]: " << tileScaleYUnit;
    traceInfo() << "Angolo rotazione [mrad]: " << angleMRad;
    traceInfo() << "Angolo rotazione [rad]: " << angleRad;
    traceInfo() << "WaitTime [ms]: " << waitTimeMs;
    traceInfo() << "WaitTimeAfterYMovements [ms]: " << waitTimeAfterYMovementMs;


    // prendo i parametri sul tipo di punto
    // point
    int numberOfPulses = printConfiguration.getNumberOfPulses();
    // circle points
    int circlePointsRadiusUm = printConfiguration.getCirclePointsRadiusUm();
    int circlePointsNumberOfSides = printConfiguration.getCirclePointsNumberOfSides();
    int circlePointsNumberOfPulses = printConfiguration.getCirclePointsNumberOfPulses();
    // circle vector
    int circleVectorRadiusUm = printConfiguration.getCircleVectorRadiusUm();
    int circleVectorNumberOfRevolutions = printConfiguration.getCircleVectorNumberOfRevolutions();
    int circleVectorNumberOfSides = printConfiguration.getCircleVectorNumberOfSides();
    int circleVectorPitch = printConfiguration.getCircleVectorPitch();


    // verifico il tipo di algoritmo scelto
    bool isRandomAlgorithm = printConfiguration.getIsRandomAlgorithm();
    bool isNeighborhoodAlgorithm = printConfiguration.getIsNeighborhoodAlgorithm();

    if (isRandomAlgorithm && isNeighborhoodAlgorithm) {
        traceErr() << "Scelti entrambi gli algoritmi";
        showDialogAsync("Error", "Controllare checkbox algoritmo tiling");
        return ExitCause::INTERNAL_ERROR;
    }

    if (!isRandomAlgorithm && !isNeighborhoodAlgorithm) {
        traceErr() << "Nessun algoritmo scelto";
        showDialogAsync("Error", "Non e' stato scelto alcun algoritmo di tiling");
        return ExitCause::INTERNAL_ERROR;
    }

    // parametri algoritmo random
    int randomPointsPerTile = printConfiguration.getRandomPointsPerTile();
    bool randomIsShuffleRowTiles = printConfiguration.getRandomIsShuffleRowTiles();
    if (isRandomAlgorithm) {
        traceInfo() << "Parametri algoritmo Neighborhood";
        traceInfo() << "RandomPointsPerTile: " << randomPointsPerTile;
        traceInfo() << "RandomIsShuffleRowTiles: " << randomIsShuffleRowTiles;
    }

    // parametri algoritmo neighborhood
    int neighborhoodMinDistanceUm = printConfiguration.getNeighborhoodMinDistanceUm();
    bool neighborhoodIsShuffleStackedTiles = printConfiguration.getNeighborhoodIsShuffleStackedTiles();
    bool neighborhoodIsShuffleRowTiles = printConfiguration.getNeighborhoodIsShuffleRowTiles();
    bool neighborhoodIsReverseY = printConfiguration.getNeighborhoodIsReverseY();

    if (isNeighborhoodAlgorithm) {
        traceInfo() << "NeighborhoodMinDistanceUm: " << neighborhoodMinDistanceUm;
        traceInfo() << "NeighborhoodIsShuffleStackedTiles: " << neighborhoodIsShuffleStackedTiles;
        traceInfo() << "NeighborhoodIsShuffleRowTiles: " << neighborhoodIsShuffleRowTiles;
        traceInfo() << "NeighborhoodIsReverseY: " << neighborhoodIsReverseY;
    }

    // leggo l'header del file
    updateStatusAsync("Reading file header...");
    QString filePath = printConfiguration.getFilePath();

    // verifico se il file esiste
    traceInfo() << "Percorso file: " << filePath;
    if (!QFile::exists(filePath)) {
        QString descr = QString("Impossibile trovare il file %1").arg(filePath);
        traceErr() << descr;
        showDialogAsync("Error", descr);
        return ExitCause::INTERNAL_ERROR;
    }


    // inizio processo di stampa vero e proprio
    updateStatusAsync("Start process");
    QWeakPointer<IMotionAnalizer> motionAnalizer = DeviceFactoryInstance.getMotionAnalizer();
    QWeakPointer<IOSignaler> ioSignaler = DeviceFactoryInstance.getIOSignaler();

    // EventLoop e timer per gestione eventi
    QEventLoop localEventLoop;
    QTimer localTimer;
    localTimer.setSingleShot(true);
    localTimer.setInterval(WAIT_THREAD_STARTED_TIME_MS);

    bool canContinue = false;
    bool isStopByUser = false;
    bool isPauseByUser = false;
    bool errorCatched = false;

    // controllo che la macchina sia in Start
    {
        updateStatusAsync("Wait for Start...");
        canContinue = false;

        QTimer countDownTimer;
        countDownTimer.setInterval(COUNTDOWN_INTERVAL_MS);
        int countDownTick = 0;

        QMetaObject::Connection ce = connect(this, &Worker::stopProcessByErrorSignal, [&]() {
            errorCatched = true;
            if (localEventLoop.isRunning())
                localEventLoop.quit();
        });
        QMetaObject::Connection cs = connect(ioSignaler.data(), &IOSignaler::statusSignal, [&](auto dIn, auto dOut, auto aIn) {
            if (localEventLoop.isRunning()) {
                Q_UNUSED(dOut);
                Q_UNUSED(aIn);
                if (dIn.value(IOType::POWER).getValue()) {
                    localEventLoop.quit();
                    canContinue = true;
                }
                QObject::disconnect(cs);
            }
        });
        QMetaObject::Connection c1 = connect(&countDownTimer, &QTimer::timeout, [&]() {
            updateStatusAsync(QString("Wait for Start... %1").arg(MAX_COUNTDOWN_TICKS - (++countDownTick)));
            if (localEventLoop.isRunning())
                if (countDownTick == MAX_COUNTDOWN_TICKS)
                    localEventLoop.quit();
        });
        QMetaObject::Connection c2 = connect(ioSignaler.data(), &IOSignaler::powerOnSignal, [&]() {
            canContinue = true;
            localEventLoop.quit();
        });
        QMetaObject::Connection c3 = connect(this, &Worker::stopRequestSignal, [&]() {
            isStopByUser = true;
            traceInfo() << "Stop richiesto dall'utente";
            localEventLoop.quit();
        });

        countDownTimer.start();
        localEventLoop.exec();
        countDownTimer.stop();
        QObject::disconnect(ce);
        QObject::disconnect(cs);
        QObject::disconnect(c1);
        QObject::disconnect(c2);
        QObject::disconnect(c3);

        if (!canContinue) {
            if (errorCatched)
                return ExitCause::ERROR_CATCHED;
            if (isStopByUser)
                return ExitCause::STOP_BY_USER;

            traceErr() << "Premere Start per avviare il processo";
            showDialogAsync("Error", "Premere Start per avviare il processo");

            return ExitCause::EXIT_BY_TIMEOUT;
        }

    }


    // controllo che gli assi siano in coppia
    {
        updateStatusAsync("Wait for motors on...");
        canContinue = false;

        QTimer countDownTimer;
        countDownTimer.setInterval(COUNTDOWN_INTERVAL_MS);
        int countDownTick = 0;

        QMetaObject::Connection ce = connect(this, &Worker::stopProcessByErrorSignal, [&]() {
            errorCatched = true;
            if (localEventLoop.isRunning())
                localEventLoop.quit();
        });
        QMetaObject::Connection c1 = connect(&countDownTimer, &QTimer::timeout, [&]() {
            updateStatusAsync(QString("Wait for motors on... %1").arg(MAX_COUNTDOWN_TICKS - (++countDownTick)));
            if (localEventLoop.isRunning())
                if (countDownTick == MAX_COUNTDOWN_TICKS)
                    localEventLoop.quit();
        });
        QMetaObject::Connection c2 = connect(motionAnalizer.data(), &IMotionAnalizer::motionBeanSignal, [&](const MotionBean& mb) {
            if (localEventLoop.isRunning()) {
                if (!mb.getAxisXMotorOff() && !mb.getAxisYMotorOff() && !mb.getAxisZMotorOff()) {
                    canContinue = true;
                    localEventLoop.quit();
                }
            }
        });
        QMetaObject::Connection c3 = connect(this, &Worker::stopRequestSignal, [&]() {
            isStopByUser = true;
            traceInfo() << "Stop richiesto dall'utente";
            localEventLoop.quit();
        });

        countDownTimer.start();
        localEventLoop.exec();
        countDownTimer.stop();
        QObject::disconnect(ce);
        QObject::disconnect(c1);
        QObject::disconnect(c2);
        QObject::disconnect(c3);

        if (!canContinue) {

            if (errorCatched)
                return ExitCause::ERROR_CATCHED;
            if (isStopByUser)
                return ExitCause::STOP_BY_USER;

            traceErr() << "Gli assi non sono tutti in coppia";
            showDialogAsync("Error", "Gli assi non sono tutti in coppia");

            return ExitCause::EXIT_BY_TIMEOUT;
        }
    }


    // controllo che sia abilitato il Cycle
    {
        updateStatusAsync("Wait for Cycle...");
        canContinue = false;

        QTimer countDownTimer;
        countDownTimer.setInterval(COUNTDOWN_INTERVAL_MS);
        int countDownTick = 0;

        QMetaObject::Connection ce = connect(this, &Worker::stopProcessByErrorSignal, [&]() {
            errorCatched = true;
            if (localEventLoop.isRunning())
                localEventLoop.quit();
        });
        QMetaObject::Connection cc = connect(ioSignaler.data(), &IOSignaler::statusSignal, [&](auto dIn, auto dOut, auto aIn) {
            if (localEventLoop.isRunning()) {
                Q_UNUSED(dOut);
                Q_UNUSED(aIn);
                if (dIn.value(IOType::CYCLE).getValue()) {
                    localEventLoop.quit();
                    canContinue = true;
                }
                QObject::disconnect(cc);
            }
        });
        QMetaObject::Connection c1 = connect(&countDownTimer, &QTimer::timeout, [&]() {
            updateStatusAsync(QString("Wait for Cycle... %1").arg(MAX_COUNTDOWN_TICKS - (++countDownTick)));
            if (localEventLoop.isRunning())
                if (countDownTick == MAX_COUNTDOWN_TICKS)
                    localEventLoop.quit();
        });
        QMetaObject::Connection c2 = connect(ioSignaler.data(), &IOSignaler::cycleOnSignal, [&]() {
            canContinue = true;
            localEventLoop.quit();
        });
        QMetaObject::Connection c3 = connect(this, &Worker::stopRequestSignal, [&]() {
            isStopByUser = true;
            traceInfo() << "Stop richiesto dall'utente";
            localEventLoop.quit();
        });

        countDownTimer.start();
        localEventLoop.exec();
        countDownTimer.stop();
        QObject::disconnect(ce);
        QObject::disconnect(cc);
        QObject::disconnect(c1);
        QObject::disconnect(c2);
        QObject::disconnect(c3);

        if (!canContinue) {
            if (errorCatched)
                return ExitCause::ERROR_CATCHED;
            if (isStopByUser)
                return ExitCause::STOP_BY_USER;

            traceErr() << "Abilitare il cycle per continuare la stampa";
            showDialogAsync("Error", "Abilitare il cycle per continuare la stampa");

            return ExitCause::EXIT_BY_TIMEOUT;
        }
    }

    machineStatusNotifier->setCurrentStatus(MachineStatus::PRINTING);
    updateLastCommandExecute(PrintCommandExecuted::CYCLE);

    // sposto l'asse z a fuoco correttamente
    int offsetZUm = offsetZmm * 1000;
    float offsetZMm = (float) offsetZUm * 0.001;
    int res = this->motionManager->moveZ(offsetZMm);

    if (motionManager->isErr(res)) {
        QString descrErr = MotionManager::decodeError(res);
        traceErr() << "Errore comando move asse Z - codice:" << res;
        traceErr() << "Descrizione:" << descrErr;
        showDialogAsync("Error move asse Z", QString("Descrizione errore: %1").arg(descrErr));
        canContinue = false;
        return ExitCause::INTERNAL_ERROR;

    } else {

        localTimer.setInterval(MOTION_CHECK_TIME_MS);

        res = MOTION_MANAGER_NO_ERR;
        canContinue = false;

        QMetaObject::Connection ce = connect(this, &Worker::stopProcessByErrorSignal, [&]() {
            errorCatched = true;
            if (localEventLoop.isRunning())
                localEventLoop.quit();
        });
        QMetaObject::Connection c1 = connect(motionAnalizer.data(), &IMotionAnalizer::motionBeanSignal, [&](const MotionBean& mb) {
            if (localEventLoop.isRunning() && !localTimer.isActive()) {
                if (!mb.getAxisZMoveInProgress()) {
                    if (mb.getAxisZStopCode() == MotionStopCode::MOTION_STOP_CORRECTLY) {
                        res = MOTION_MANAGER_MOTION_Z_STOP_CORRECTLY;
                        canContinue = true;
                    } else
                        res = MOTION_MANAGER_MOTION_Z_STOP_ERROR;

                    localEventLoop.quit();
                }
            }
        });
        QMetaObject::Connection c2 = connect(motionAnalizer.data(), static_cast<void (IMotionAnalizer::*)(MotionStopCode)>(&IMotionAnalizer::axisZMotionStopSignal), [&](MotionStopCode sc) {
            if (localEventLoop.isRunning()) {
                if (sc == MotionStopCode::MOTION_STOP_CORRECTLY) {
                    res = MOTION_MANAGER_MOTION_Z_STOP_CORRECTLY;
                    canContinue = true;
                } else
                    res = MOTION_MANAGER_MOTION_Z_STOP_ERROR;

                localEventLoop.quit();
            }
        });
        QMetaObject::Connection c3 = connect(this, &Worker::stopRequestSignal, [&]() {
            isStopByUser = true;
            traceInfo() << "Stop richiesto dall'utente";
            localEventLoop.quit();
        });

        localTimer.start();
        localEventLoop.exec();
        localTimer.stop();
        QObject::disconnect(ce);
        QObject::disconnect(c1);
        QObject::disconnect(c2);
        QObject::disconnect(c3);

        if (!canContinue) {
            if (errorCatched)
                return ExitCause::ERROR_CATCHED;
            if (isStopByUser)
                return ExitCause::STOP_BY_USER;
            else {
                traceErr() << "L'asse Z si e' fermato in modo anomalo";
                showDialogAsync("Errore stop asse Z", QString("L'asse X si e' fermato in modo anomalo"));
                return ExitCause::INTERNAL_ERROR;
            }
        }

    }


    // decodifico l'header del file
    FilterStream filter;
    QScopedPointer<IAbstractJsonStreamDecoder> streamDecoder(new FilterJsonStreamDecoder(filePath));
    JsonStreamDecoderError headerErr = streamDecoder->decodeHeader(&filter);
    if (headerErr != JSON_STREAM_DECODER_NO_ERROR) {
        QString descr = QString("Errore nela decodifica dell'header del file json; codice errore: %1").arg(headerErr);
        traceErr() << descr;
        showDialogAsync("Error", descr);
        return ExitCause::INTERNAL_ERROR;
    }
    streamDecoder.reset();


    // faccio svuotare la coda degli eventi (mi serve per controllare se e' stato premuto il pulsante stop)
    qApp->processEvents();
    if (hasErrors) return ExitCause::ERROR_CATCHED;
    if (hasToStop) return ExitCause::STOP_BY_USER;


    bool isResumePrint = printStatus.isValid();
    if (!isResumePrint) {

        // lancio il thread di esecuzione dei punti (da nuovo)
        updateStatusAsync("Starting file process thread...");
        fileProcessorThread.reset(
                    new RowTileProcessor(
                        ROWTILE_PROCESSOR_THREAD_NAME,
                        filePath,
                        tileSizeUm));

    } else {

        // lancio il thread di esecuzione dei punti (da stato salvato)
        int rowIndex = printStatus.getRowIndex();
        if (printStatus.getTileYToPrint().isEmpty())
            rowIndex++;

        updateStatusAsync("Starting file process thread for resume previous print...");
        fileProcessorThread.reset(
                    new ResumeRowTileProcessor(
                        RESUME_ROWTILE_PROCESSOR_THREAD_NAME,
                        filePath,
                        tileSizeUm,
                        rowIndex
                        ));

    }


    // aspetto che il thread sia avviato
    {
        updateStatusAsync("Starting file process thread... OK");
        canContinue = false;

        QMetaObject::Connection ce = connect(this, &Worker::stopProcessByErrorSignal, [&]() {
            errorCatched = true;
            if (localEventLoop.isRunning())
                localEventLoop.quit();
        });
        QMetaObject::Connection c1 = connect(&localTimer, &QTimer::timeout, [&]() {
            localEventLoop.quit();
        });
        QMetaObject::Connection c2 = connect(fileProcessorThread.data(), &ProcessorThread::loopRunning, this, [&]() {
            canContinue = true;
            localEventLoop.quit();
        });
        QMetaObject::Connection c3 = connect(fileProcessorThread.data(), &ProcessorThread::processFinished, this, [&]() {
            canContinue = false;
            localEventLoop.quit();
        });
        QMetaObject::Connection c4 = connect(this, &Worker::stopRequestSignal, [&]() {
            isStopByUser = true;
            traceInfo() << "Stop richiesto dall'utente";
            localEventLoop.quit();
        });

        fileProcessorThread->start();
        updateLastCommandExecute(PrintCommandExecuted::PROCESSOR_THREAD_RUN);

        localTimer.start();
        localEventLoop.exec();
        localTimer.stop();
        QObject::disconnect(ce);
        QObject::disconnect(c1);
        QObject::disconnect(c2);
        QObject::disconnect(c3);
        QObject::disconnect(c4);

        if (!canContinue) {
            if (errorCatched)
                return ExitCause::ERROR_CATCHED;
            if (isStopByUser)
                return ExitCause::STOP_BY_USER;

            traceErr() << "Il thread di processo dei dati non si e' avviato";
            showDialogAsync("Error", "Il thread di processo dei dati non si e' avviato");

            return ExitCause::INTERNAL_ERROR;
        }

    }


    // attivazione aria compressa 1
    updateStatusAsync("Enable air 1");
    if (!ioManager->setDigitalOutput(IOType::COMPRESSED_AIR_1)) {
        traceErr() << "Impossibile attivare l'aria compressa";
        showDialogAsync("Error", "Impossibile attivare l'aria compressa");
        return ExitCause::INTERNAL_ERROR;
    }


    // attivazione aria compressa 2
    updateStatusAsync("Enable air 2");
    if (!ioManager->setDigitalOutput(IOType::COMPRESSED_AIR_2)) {
        traceErr() << "Impossibile attivare l'aria compressa 2";
        showDialogAsync("Error", "Impossibile attivare l'aria compressa 2");
        return ExitCause::INTERNAL_ERROR;
    }


    // attivazione aspirazione
    updateStatusAsync("Enable suction");
    if (!ioManager->setDigitalOutput(IOType::SUCTION)) {
        traceErr() << "Impossibile attivare l'aspirazione";
        showDialogAsync("Error", "Impossibile attivare l'aspirazione");
        return ExitCause::INTERNAL_ERROR;
    }


    // attivazione power laser
    updateStatusAsync("Enable laser power");
    if (!ioManager->setDigitalOutput(IOType::LASER_POWER)) {
        traceErr() << "Impossibile attivare la potenza del laser";
        showDialogAsync("Error", "Impossibile attivare la potenza del laser");
        return ExitCause::INTERNAL_ERROR;
    }


    // attivazione power scan
    updateStatusAsync("Enable power scan");
    if (!ioManager->setDigitalOutput(IOType::POWER_SCAN)) {
        traceErr() << "Impossibile abilitare il power scan";
        showDialogAsync("Error", "Impossibile abilitare il power scan");
        return ExitCause::INTERNAL_ERROR;
    }

    updateLastCommandExecute(PrintCommandExecuted::IO_ACTIVATED);


    // inizio configurazione testa scansione ipg
    updateStatusAsync("Initalizing laser...");
    float energy = TestFrame::TEST_FRAME_PULSE_ENERGY_DFLT;
#ifdef FLAG_IPG_YLPN_LASER_PRESENT
    if (!setupLaserOn()) {
        traceErr() << "Impossibile accendere il laser";
        showDialogAsync("Error", "Impossibile accendere il laser." \
                                 "\nVedere il log per maggiori dettagli.");
        return ExitCause::INTERNAL_ERROR;
    }

    updateLastCommandExecute(PrintCommandExecuted::LASER_ON);

    if (!getPulseEnergy(energy)) {
        traceErr() << "Impossibile interrogare l'energia dell'impulso del laser";
        showDialogAsync("Error", "Impossibile interrogare l'energia dell'impulso del laser" \
                                 "\nVedere il log per maggiori dettagli.");
        return ExitCause::INTERNAL_ERROR;
    }
//    if (!getPulseEnergyLaserAirCooled(energy)) {
//        traceErr() << "Impossibile interrogare l'energia dell'impulso del laser air cooled";
//        showDialogAsync("Error", "Impossibile interrogare l'energia dell'impulso del laser air cooled" \
//                                 "\nVedere il log per maggiori dettagli.");
//        return ExitCause::INTERNAL_ERROR;
//    }
#endif
    updateStatusAsync("Initalizing laser... OK");


    PointShapeEnum pointShape = static_cast<PointShapeEnum>(printConfiguration.getPointShape());
    if (pointShape == PointShapeEnum::UNDEFINED) {
        traceErr() << "Non e' stato selezionato alcuna forma di punto";
        showDialogAsync("Error", "Non e' stato selezionato alcuna forma di punto");
        return ExitCause::INTERNAL_ERROR;
    }

    imlw::VectorList circleVectorsWRevolutions;
    if (pointShape == PointShapeEnum::CIRCLE_VECTOR) {
        imlw::VectorList circleVectors(imlw::PolygonProperties(circleVectorNumberOfSides, circleVectorRadiusUm));
        for (int i=0; i<circleVectorNumberOfRevolutions; ++i)
            circleVectorsWRevolutions.append(circleVectors);
    }

    imlw::PointList singleCirclePointList;
    if (pointShape == PointShapeEnum::CIRCLE_POINTS) {
        singleCirclePointList.append(imlw::PointList(imlw::PolygonProperties(circlePointsNumberOfSides, circlePointsRadiusUm)));
    }


#ifdef FLAG_SCANNER_HEAD_PRESENT
    updateStatusAsync("Initalizing scanner head...");
    const std::vector<imlw::ScannerInfo>& scannerList = imlw::Scanner::scanners();

    if (scannerList.empty()) {
        traceErr() << "Impossibile trovare lo scanner";
        showDialogAsync("Error", "Impossibile trovare lo scanner");
        return ExitCause::INTERNAL_ERROR;
    }

    if (scannerList.at(0).getStatus() != imlw::ConnectionStatus::AVAILABLE) {
        traceErr() << "Lo scanner e' nello stato di busy";
        showDialogAsync("Error", "Lo scanner e' nello stato di busy");
        return ExitCause::INTERNAL_ERROR;
    }

    std::string err;

    try {

        /*
         * NOTE NIC 12/04/2019 - Scanner e' un oggetto RAII, quindi nel distruttore, se c'e' una connessione
         * aperta, prima di distruggere l'oggetto viene chiusa e successivamente
         * viene eliminato l'oggetto
         */

        scanner.reset(new imlw::Scanner(scannerList[0].getName(), true, imlw::Units::MICRONS, err));

        float powerpercent = 100.0;
        float width =  1.0f / frequencyHz; // WARNING NIC 24/04/2019 - sarebbe da mettere 0.5 (chiedere a Longoni),
        float dwell = width;

        /*
         * NOTE NIC 15/04/2019 - per i singoli fori, configuro il laser in questo modo;
         * per i vettori, configuro il laser con OutputVectorProperties
         */
        if (pointShape == PointShapeEnum::POINT || pointShape == PointShapeEnum::CIRCLE_POINTS) {

            imlw::OutputPointsProperties pointsProperties(energy);
            scanner->config(pointsProperties, 0.0f);

            imlw::PointParametersWrapper ppw = scanner->getPointParameters();
            ppw.clearLaserEntries();

            if (pointShape == PointShapeEnum::POINT)
                ppw.addLaserEntry(dwell, width, powerpercent, numberOfPulses);
            else if (pointShape == PointShapeEnum::CIRCLE_POINTS)
                ppw.addLaserEntry(dwell, width, powerpercent, circlePointsNumberOfPulses);

        } else if (pointShape == PointShapeEnum::CIRCLE_VECTOR) {

            imlw::OutputVectorsProperties vectorProperties(circleVectorPitch, energy);
            scanner->config(vectorProperties);

        }

        scanner->guide(false);

    } catch (imlw::LibraryException& ex) {
        traceErr() << "Eccezione in fase di connessione con testa scansione";
        traceErr() << "Descrizione eccezione: " << ex.what();
        showDialogAsync("Error", "Eccezione in fase di connessione con testa scansione");
        return ExitCause::INTERNAL_ERROR;
    }

    /* NOTE NIC 24/06/2019 - sleep connessione testa
     * questo tempo lo metto perche' se mi connetto e sconnetto troppo velocemente dalla testa
     * di scansione, per qualche motivo (librerie IPG probabilmente) la connessione resta appesa
     */
    this->thread()->msleep(TEST_FRAME_SCANNER_WAIT_TIME_AFTER_CONNECTION_MS);
    #endif

    updateLastCommandExecute(PrintCommandExecuted::SCANNER_HEAD_SETUP);
    updateStatusAsync("Initalizing scanner head... OK");


    // inizio ciclo di stampa
    updateStatusAsync("Starting print loop...");
    QElapsedTimer tileMeasureTimer;
    QElapsedTimer stackedTileMeasureTimer;
    QElapsedTimer printMeasureTimer;

    int totalNumberOfPoints = filter.getNumOfPoints();
    int numberOfPointsPrinted = 0;
    int numberOfPointsRemains = totalNumberOfPoints;

    // faccio svuotare la coda degli eventi (mi serve per controllare se e' stato premuto il pulsante stop)
    qApp->processEvents();
    if (hasErrors) return ExitCause::ERROR_CATCHED;
    if (hasToStop) return ExitCause::STOP_BY_USER;

    printMeasureTimer.start();

    updateLastCommandExecute(PrintCommandExecuted::PRINT_LOOP);

    ExitCause exitCause = ExitCause::NOT_VALID;

    // ciclo su tutte le righe RowTile
    while (canContinue && fileProcessorThread->hasNext()) {

        GridRowI row;
        qApp->processEvents();
        updateStatusAsync("Creating row data...");
        bool isNextValid = fileProcessorThread->getNext(&row);
        if (!isNextValid) {
            traceErr() << "Il thread processor non ha generato una riga valida";
            showDialogAsync("Error", "Il thread processor non ha generato una riga valida");
            canContinue = false;
            exitCause = ExitCause::INTERNAL_ERROR;
            continue;
        }

        if (row.getTileList().isEmpty()) {
            traceErr() << "Tile list vuota; qualche problema nel file processor";
            showDialogAsync("Error", "Tile list vuota; qualche problema nel file processor");
            canContinue = false;
            exitCause = ExitCause::INTERNAL_ERROR;
            continue;
        }

        int rowIndex = row.getTileList().at(0).getRowIndex();
        printStatus.setRowIndex(rowIndex);
        if (printStatus.getTileYToPrint().isEmpty()) {
            int maxCols = row.getMaxCols();
            printStatus.reloadTileY(maxCols);
        }

        updateStatusAsync("Creating row data... OK");

        int xUm = row.getTileList().at(0).getBoundingBox().getMin().getX();
        int offsetXum = offsetXmm * 1000;
        int tileSizeUm = tileSizeMm * 1000 * 0.5;

        int moveXum = xUm + offsetXum + tileSizeUm;
        float moveXmm = (float) moveXum * 0.001;

        traceInfo() << "Spostamento testa lungo l'asse X per bounding box immagine: " << xUm << "um";
        traceInfo() << "Offset lungo l'asse X: " << offsetXum << "um";
        traceInfo() << "Offset per dimensione tile X (diviso per 2): " << tileSizeUm << "um";
        traceInfo() << "Spostamento testa lungo l'asse X totale: " << moveXum << "um";
        traceInfo() << "Spostamento testa lungo l'asse X totale: " << moveXmm << "mm";

        updateStatusAsync("Spostamento testa lungo l'asse X");

#ifdef FLAG_MOCK_MOTION_PRESENT
        moveXmm = 0;
#endif

        // inizio spostamento asse x
        int res = this->motionManager->moveX(moveXmm);

        if (motionManager->isErr(res)) {
            QString descrErr = MotionManager::decodeError(res);
            traceErr() << "Errore comando move asse X - codice:" << res;
            traceErr() << "Descrizione:" << descrErr;
            showDialogAsync("Error move asse X", QString("Descrizione errore: %1").arg(descrErr));
            canContinue = false;
            exitCause = ExitCause::INTERNAL_ERROR;
            continue;

        } else {

            localTimer.setInterval(MOTION_CHECK_TIME_MS);

            res = MOTION_MANAGER_NO_ERR;
            canContinue = false;

            QMetaObject::Connection ce = connect(this, &Worker::stopProcessByErrorSignal, [&]() {
                errorCatched = true;
                if (localEventLoop.isRunning())
                    localEventLoop.quit();
            });
            QMetaObject::Connection c1 = connect(motionAnalizer.data(), &IMotionAnalizer::motionBeanSignal, [&](const MotionBean& mb) {
                if (localEventLoop.isRunning() && !localTimer.isActive()) {
                    if (!mb.getAxisXMoveInProgress()) {
                        if (mb.getAxisXStopCode() == MotionStopCode::MOTION_STOP_CORRECTLY) {
                            res = MOTION_MANAGER_MOTION_X_STOP_CORRECTLY;
                            canContinue = true;
                        } else
                            res = MOTION_MANAGER_MOTION_X_STOP_ERROR;

                        localEventLoop.quit();
                    }
                }
            });
            QMetaObject::Connection c2 = connect(motionAnalizer.data(), static_cast<void (IMotionAnalizer::*)(MotionStopCode)>(&IMotionAnalizer::axisXMotionStopSignal), [&](MotionStopCode sc) {
                if (localEventLoop.isRunning()) {
                    if (sc == MotionStopCode::MOTION_STOP_CORRECTLY) {
                        res = MOTION_MANAGER_MOTION_X_STOP_CORRECTLY;
                        canContinue = true;
                    } else
                        res = MOTION_MANAGER_MOTION_X_STOP_ERROR;

                    localEventLoop.quit();
                }
            });
            QMetaObject::Connection c3 = connect(this, &Worker::stopRequestSignal, [&]() {
                isStopByUser = true;
                traceInfo() << "Stop richiesto dall'utente";
                localEventLoop.quit();
            });

            localTimer.start();
            localEventLoop.exec();
            localTimer.stop();
            QObject::disconnect(ce);
            QObject::disconnect(c1);
            QObject::disconnect(c2);
            QObject::disconnect(c3);

            if (!canContinue) {
                if (errorCatched)
                    exitCause = ExitCause::ERROR_CATCHED;
                if (isStopByUser)
                    exitCause = ExitCause::STOP_BY_USER;
                else {
                    traceErr() << "L'asse X si e' fermato in modo anomalo";
                    showDialogAsync("Errore stop asse X", QString("L'asse X si e' fermato in modo anomalo"));
                    exitCause = ExitCause::INTERNAL_ERROR;
                }
            }

        }

        if (!canContinue)
            continue;


        // se necessario, randomizzo l'ordine dei tile presenti in una RowTile
        GridRowI rowTiles;
        if (isRandomAlgorithm) {

            traceInfo() << "Algoritmo random";
            if (randomIsShuffleRowTiles) {
                traceInfo() << "Disordino i GridTile della riga corrente";
                rowTiles = ComputationUtils::shuffleRowTile(row);
            } else
                rowTiles = row;

        } else if (isNeighborhoodAlgorithm) {

            traceInfo() << "Algoritmo neighborhood";

            if (neighborhoodIsShuffleRowTiles) {
                traceInfo() << "Disordino i GridTile della riga corrente";
                rowTiles = ComputationUtils::shuffleRowTile(row);
            } else if (neighborhoodIsReverseY) {
                traceInfo() << "Ordino i GridTile in Y-Reverse";
                rowTiles = ComputationUtils::reverseYOrder(row);
            } else {
                rowTiles = row;
            }

        } else {

            traceErr() << "Nessun algoritmo selezionato";
            showDialogAsync("Algoritmo foratura", QString("Nessun algoritmo selezionato"));
            exitCause = ExitCause::INTERNAL_ERROR;

        }


        // qui inizio a ciclare su tutti i tile della RowTile
        for (const GridTileI& tile: rowTiles.getTileList()) {

            if (!canContinue)
                break;

            int colIndex = tile.getColIndex();
            traceInfo() << QString("Verifico che il tile [%0, %1] non sia stato gia' stampato").arg(rowIndex).arg(colIndex);

            if (tile.isEmpty()) {
                traceInfo() << "Il tile non contiene punti, vado al successivo";
                printStatus.removeTileY(colIndex);
                continue;
            }

            if (!printStatus.isTileYToPrint(colIndex)) {
                traceInfo() << "Tile gia' stampato; vado al successivo tile";
                continue;
            }
            traceInfo() << QString("Stampo tile con indice [%0, %1]").arg(rowIndex).arg(colIndex);

            // per ogni riga, dai tile creo gli stacked tile
            updateStatusAsync("Creazione stacked tile");
            StackedTileI stackedTile;

            if (isRandomAlgorithm)
                stackedTile = StackedTileI(ComputationUtils::shuffleTile(tile), randomPointsPerTile);
            else if (isNeighborhoodAlgorithm)
                stackedTile = StackedTileI(tile, neighborhoodMinDistanceUm, neighborhoodMinDistanceUm);
            else
                stackedTile = StackedTileI(tile, randomPointsPerTile);

            // una volta creati gli stacked tile, devo creare la lista di tutti i tile per mescolarli
            QList<TileI> tiles = stackedTile.getTiles();

            // disordino i tiles di ogni singola riga (se necessario)
            if (isNeighborhoodAlgorithm && neighborhoodIsShuffleStackedTiles)
                tiles = ComputationUtils::shuffleList(tiles);

            // inizio la logica per lo spostamento della testa lungo l'asse y
            int yUm = tile.getBoundingBox().getMin().getY();
            int offsetYum = offsetYmm * 1000;
            int tileSizeUm = tileSizeMm * 1000 * 0.5;

            int moveYum = yUm + offsetYum + tileSizeUm;
            float moveYmm = (float) moveYum * 0.001;

            traceInfo() << "Spostamento testa lungo l'asse Y per bounding box immagine: " << yUm << "um";
            traceInfo() << "Offset lungo l'asse Y: " << offsetYum << "um";
            traceInfo() << "Offset per dimensione tile Y (diviso per 2): " << tileSizeUm << "um";
            traceInfo() << "Spostamento testa lungo l'asse Y totale: " << moveYum << "um";
            traceInfo() << "Spostamento testa lungo l'asse Y totale: " << moveYmm << "mm";

            updateStatusAsync("Spostamento testa lungo l'asse Y");

#ifdef FLAG_MOCK_MOTION_PRESENT
            moveYmm = 0;
#endif

            // inizio spostamento asse y
            res = this->motionManager->moveY(moveYmm);

            if (motionManager->isErr(res)) {

                QString descrErr = MotionManager::decodeError(res);
                traceErr() << "Errore comando move asse Y - codice:" << res;
                traceErr() << "Descrizione:" << descrErr;
                showDialogAsync("Error move asse Y", QString("Descrizione errore: %1").arg(descrErr));
                canContinue = false;
                exitCause = ExitCause::INTERNAL_ERROR;
                continue;

            } else {

                localTimer.setInterval(MOTION_CHECK_TIME_MS);

                res = MOTION_MANAGER_NO_ERR;
                canContinue = false;

                QMetaObject::Connection ce = connect(this, &Worker::stopProcessByErrorSignal, [&]() {
                    errorCatched = true;
                    if (localEventLoop.isRunning())
                        localEventLoop.quit();
                });
                QMetaObject::Connection c1 = connect(motionAnalizer.data(), &IMotionAnalizer::motionBeanSignal, [&](const MotionBean& mb) {
                    if (localEventLoop.isRunning() && !localTimer.isActive()) {
                        if (!mb.getAxisYMoveInProgress()) {
                            if (mb.getAxisYStopCode() == MotionStopCode::MOTION_STOP_CORRECTLY) {
                                res = MOTION_MANAGER_MOTION_Y_STOP_CORRECTLY;
                                canContinue = true;
                            } else
                                res = MOTION_MANAGER_MOTION_Y_STOP_ERROR;

                            localEventLoop.quit();
                        }
                    }
                });
                QMetaObject::Connection c2 = connect(motionAnalizer.data(), static_cast<void (IMotionAnalizer::*)(MotionStopCode)>(&IMotionAnalizer::axisYMotionStopSignal), [&](MotionStopCode sc) {
                    if (localEventLoop.isRunning()) {
                        if (sc == MotionStopCode::MOTION_STOP_CORRECTLY) {
                            res = MOTION_MANAGER_MOTION_Y_STOP_CORRECTLY;
                            canContinue = true;
                        } else
                            res = MOTION_MANAGER_MOTION_Y_STOP_ERROR;

                        localEventLoop.quit();
                    }
                });
                QMetaObject::Connection c3 = connect(this, &Worker::stopRequestSignal, [&]() {
                    isStopByUser = false;
                    traceInfo() << "Stop richiesto dall'utente";
                    localEventLoop.quit();
                });

                localTimer.start();
                localEventLoop.exec();
                localTimer.stop();
                QObject::disconnect(ce);
                QObject::disconnect(c1);
                QObject::disconnect(c2);
                QObject::disconnect(c3);

                if (!canContinue) {
                    if (errorCatched)
                        exitCause = ExitCause::ERROR_CATCHED;
                    if (isStopByUser)
                        exitCause = ExitCause::STOP_BY_USER;
                    else {
                        traceErr() << "L'asse Y si e' fermato in modo anomalo";
                        showDialogAsync("Errore stop asse Y", QString("L'asse Y si e' fermato in modo anomalo"));
                        exitCause = ExitCause::INTERNAL_ERROR;
                    }
                } else
                    this->thread()->msleep(waitTimeAfterYMovementMs);
            }

            if (!canContinue)
                break;

            /* una volta completato lo spostamento dell'asse y, inizio con
             * iterare ciascun tile presente nello StackedTile
             */

            stackedTileMeasureTimer.start();

            emit enablePauseSignal(true);

            QTimer waitTimeTimer;
            waitTimeTimer.setInterval(waitTimeMs);
            waitTimeTimer.setSingleShot(true);

            int tileCounter = 0;
            int stackTileSize = tiles.size();

            for (auto&& currentTile: tiles) {

                if (!canContinue)
                    break;

                // dalla lista, inizio a stampare ogni singolo tile
                tileMeasureTimer.start();

                updateStatusAsync(QString("Stampa tile [%0, %1] - %2 di %3").arg(rowIndex).arg(colIndex).arg(tileCounter).arg(stackTileSize));

#ifdef FLAG_SCANNER_HEAD_PRESENT
                try {
                    scanner->laser(imlw::LaserAction::Enable);
                } catch (imlw::LibraryException& ex) {
                    traceErr() << "Eccezione testa scansione al comando laser enable";
                    traceErr() << "Descrizione eccezione: " << ex.what();
                    showDialogAsync("Errore testa scanzione", QString("Descrizione eccezione: %1").arg(ex.what()));
                    exitCause = ExitCause::INTERNAL_ERROR;
                    canContinue = false;
                }
#endif
                if (!canContinue)
                    break;

                PointI offset = currentTile.getCenter();
                offset.setX(-offset.getX());
                offset.setY(-offset.getY());
                PointSetI movePoints = ComputationUtils::movePointSet(currentTile.getPointSet(), offset);
                movePoints = ComputationUtils::axisBase2HeadBase(movePoints);
                const QVector<PointI>& vectorPoints = movePoints.getVector();
                numberOfPointsPrinted += vectorPoints.size();
                numberOfPointsRemains -= vectorPoints.size();

                if (pointShape == PointShapeEnum::POINT) {

                    std::list<imlw::Point> listOfPoints;
                    for (auto&& p: vectorPoints)
                        listOfPoints.push_back(imlw::Point(p.getX(), p.getY()));

                    imlw::PointList outputPoints(listOfPoints);
                    outputPoints.rotate(angleRad);
                    outputPoints.scale(tileScaleXUnit, tileScaleYUnit, 1);

                    waitTimeTimer.start();

#ifdef FLAG_SCANNER_HEAD_PRESENT
                    try {

                        scanner->output(outputPoints);
                        listOfPoints.clear();
                        scanner->laser(imlw::LaserAction::Disable);

                    } catch (imlw::LibraryException& ex) {
                        traceErr() << "Eccezione testa scansione al comando output punti";
                        traceErr() << "Descrizione eccezione: " << ex.what();
                        showDialogAsync("Errore testa scanzione", QString("Descrizione eccezione: %1").arg(ex.what()));
                        exitCause = ExitCause::INTERNAL_ERROR;
                        canContinue = false;
                    }
#endif

                } else if (pointShape == PointShapeEnum::CIRCLE_POINTS) {

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

#ifdef FLAG_SCANNER_HEAD_PRESENT
                    try {

                        scanner->output(circles);
                        scanner->laser(imlw::LaserAction::Disable);

                    } catch (imlw::LibraryException& ex) {
                        traceErr() << "Eccezione testa scansione al comando output vettori";
                        traceErr() << "Descrizione eccezione: " << ex.what();
                        showDialogAsync("Errore testa scanzione", QString("Descrizione eccezione: %1").arg(ex.what()));
                        exitCause = ExitCause::INTERNAL_ERROR;
                        canContinue = false;
                    }
#endif

                } else if (pointShape == PointShapeEnum::CIRCLE_VECTOR) {

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

#ifdef FLAG_SCANNER_HEAD_PRESENT
                    try {

                        scanner->output(circlesVet);
                        scanner->laser(imlw::LaserAction::Disable);

                    } catch (imlw::LibraryException& ex) {
                        traceErr() << "Eccezione testa scansione al comando output vettori";
                        traceErr() << "Descrizione eccezione: " << ex.what();
                        showDialogAsync("Errore testa scanzione", QString("Descrizione eccezione: %1").arg(ex.what()));
                        exitCause = ExitCause::INTERNAL_ERROR;
                        canContinue = false;
                    }
#endif
                }

                if (!canContinue)
                    break;

                /*
                 * NOTE NIC 31/07/2019 - il funzionamento teorico e' il seguente:
                 * una volta inviato i punti alla testa, aspetto 2 eventi:
                 * 1. tempo attesa tile non scada
                 * 2. segnale che la testa ha finito di stampare
                 * se il tempo di attesa tile e' scaduto ma la testa sta ancora stampando,
                 * non faccio piu' partire un terzo timer (da quando gestisco a thread) in quanto
                 * non ha senso far partire un terzo timer che si basa sul segnale del IOSignaler;
                 * tanto vale basarsi sul signal del IOSignaler stesso e valutare lo stato della variabile
                 * MARK_IN_PROGRESS;
                 * quindi:
                 * se arriva il segnale che la testa ha finito di stampare ma il tempo di attesa
                 * non e' ancora scaduto, aspetto che scada il tempo di attesa;
                 * se il tempo di attesa e' scaduto ma la testa sta ancora stampando, aspetto che
                 * la testa finisca di stampare (dal signal IOSignaler);
                 */
                bool isMarkInProgress = true;
                QMetaObject::Connection ce = connect(this, &Worker::stopProcessByErrorSignal, [&]() {
                    canContinue = false;
                    errorCatched = true;
                    if (localEventLoop.isRunning())
                        localEventLoop.quit();
                });
                QMetaObject::Connection c1 = connect(ioSignaler.data(), &IOSignaler::markInProgressOffSignal, [&]() {
                    isMarkInProgress = false;
                    if (localEventLoop.isRunning() && !waitTimeTimer.isActive())
                        localEventLoop.quit();
                });
                QMetaObject::Connection c2 = connect(ioSignaler.data(), &IOSignaler::statusSignal, [&](auto dIn, auto dOut, auto aIn) {
                    if (localEventLoop.isRunning()) {
                        Q_UNUSED(dOut);
                        Q_UNUSED(aIn);
                        isMarkInProgress = dIn.value(IOType::MARK_IN_PROGRESS).getValue();
                        if (!isMarkInProgress && !waitTimeTimer.isActive())
                            localEventLoop.quit();
                    }
                });
                QMetaObject::Connection c3 = connect(this, &Worker::stopRequestSignal, [&]() {
                    isStopByUser = true;
                    canContinue = false;
                    traceInfo() << "Stop richiesto dall'utente";
                    localEventLoop.quit();
                });
                QMetaObject::Connection c4 = connect(&waitTimeTimer, &QTimer::timeout, [&]() {
                    if (!isMarkInProgress)
                        localEventLoop.quit();
                });
                QMetaObject::Connection cp = connect(this, &Worker::pauseRequestSignal, [&]() {
                    isPauseByUser = true;
                    showDialogAsync("Info pause", QString("Attesa completamento Stacked tile prima della pausa"));
                    traceInfo() << "Ricevuto segnalazione di pausa";
                });

                localEventLoop.exec();

                qint64 tileTimeMeasureMs = tileMeasureTimer.elapsed();
                updateTileTimeAsync(tileTimeMeasureMs);

                QObject::disconnect(ce);
                QObject::disconnect(c1);
                QObject::disconnect(c2);
                QObject::disconnect(c3);
                QObject::disconnect(c4);
                QObject::disconnect(cp);

                qint64 printTimeMeasureMs = printMeasureTimer.elapsed();
                qint64 estimatedResidualTimeMs = numberOfPointsRemains * printTimeMeasureMs / numberOfPointsPrinted;

                updateEstimatedResidualTimeAsync(estimatedResidualTimeMs);
                ++tileCounter;

                // faccio svuotare la coda degli eventi (mi serve per controllare se e' stato premuto il pulsante stop)
                qApp->processEvents();
                if (!canContinue) {
                    if (errorCatched)
                        exitCause = ExitCause::ERROR_CATCHED;
                    if (isStopByUser)
                        exitCause = ExitCause::STOP_BY_USER;

                    continue;
                }

            }

            emit enablePauseSignal(false);

            if (canContinue) {
                /* NOTE NIC 01/08/2019 - rimozione tile stampato
                 * il tile corrente lo rimuovo da quelli in lista se posso continuare a stampare oppure
                 * ho completato il tile prima della pausa (se richiesta dall'utente)
                 */

                /* TODO NIC 01/08/2019 - ripresa stampa in caso di errore
                 * in ottica futura, la ripresa della stampa in caso di errore dovrebbe venire a gratis;
                 * e' necessario pero' abilitare il pulsante resume anche in caso di stampa in errore;
                 */
                printStatus.removeTileY(colIndex);
                if (isPauseByUser) {
                    canContinue = false;
                    exitCause = ExitCause::PAUSE_BY_USER;
                }
            }

            if (!canContinue)
                continue;

            qint64 stackedTileMeasureMs = stackedTileMeasureTimer.elapsed();
            updateStackedTimeAsync(stackedTileMeasureMs);
        }

        traceInfo() << "Completata la stampa della riga:" << rowIndex;
        if (!canContinue)
            continue;

    }

    emit enablePauseSignal(false);

    if (exitCause == ExitCause::NOT_VALID)
        exitCause = ExitCause::PROCESS_COMPLETED_CORRECTLY;

    traceExit;
    return exitCause;

}

bool Worker::afterProcess() {

    traceEnter;

    switch (commandsExecuted) {
    case PrintCommandExecuted::PRINT_LOOP:
    case PrintCommandExecuted::SCANNER_HEAD_SETUP:
    {
#ifdef FLAG_SCANNER_HEAD_PRESENT
        if (!scanner.isNull())
            scanner->close();
#endif
    }
    case PrintCommandExecuted::LASER_ON:
    {
#ifdef FLAG_IPG_YLPN_LASER_PRESENT
        if (!this->setupLaserOff()) {
            traceErr() << "Impossibile spegnere il laser";
            showDialogAsync("Error", "Impossibile spegnere il laser" \
                                     "\nVedere il log per maggiori dettagli.");
        }
#endif
    }
    case PrintCommandExecuted::IO_ACTIVATED:
    {
        // attivazione power laser
        updateStatusAsync("Disable laser power");
        if (!ioManager->unsetDigitalOutput(IOType::LASER_POWER)) {
            traceErr() << "Impossibile disattivare la potenza del laser";
            showDialogAsync("Error", "Impossibile disattivare la potenza del laser");
        }


        // attivazione power scan
        updateStatusAsync("Disable power scan");
        if (!ioManager->unsetDigitalOutput(IOType::POWER_SCAN)) {
            traceErr() << "Impossibile disattivare il power scan";
            showDialogAsync("Error", "Impossibile disattivare il power scan");
        }


        // disattivazione aria compressa 1
        updateStatusAsync("Disable air");
        if (!ioManager->unsetDigitalOutput(IOType::COMPRESSED_AIR_1)) {
            traceErr() << "Impossibile disattivare l'aria compressa 1";
            showDialogAsync("Error", "Impossibile disattivare l'aria compressa 1");
        }


        // disattivazione aria compressa 2
        updateStatusAsync("Disable air 2");
        if (!ioManager->unsetDigitalOutput(IOType::COMPRESSED_AIR_2)) {
            traceErr() << "Impossibile disattivare l'aria compressa 2";
            showDialogAsync("Error", "Impossibile disattivare l'aria compressa 2");
        }


        // disattivazione aspirazione
        updateStatusAsync("Disable suction");
        if (!ioManager->unsetDigitalOutput(IOType::SUCTION)) {
            traceErr() << "Impossibile disattivare l'aspirazione";
            showDialogAsync("Error", "Impossibile disattivare l'aspirazione");
        }
    }
    case PrintCommandExecuted::PROCESSOR_THREAD_RUN: {
        if (!fileProcessorThread.isNull()) {
            fileProcessorThread->stop();
            updateStatusAsync("Waiting closing file process thread...");
            fileProcessorThread->wait();
            fileProcessorThread.reset();
        }
    }
    case PrintCommandExecuted::CYCLE:
    case PrintCommandExecuted::START:
    case PrintCommandExecuted::IDLE: break;
    }

    updateStatusAsync("Disconnecting devices");
    ioManager->disconnect();
    motionManager->disconnect();

    updateStatusAsync("Detach devices");
    DeviceFactoryInstance.detachManagers();

    updateStatusAsync("End");
    traceExit;
    return true;

}

void Worker::showDialogAsync(const QString& err, const QString& descr) {

    traceEnter;
    QMetaObject::invokeMethod(
                this->tfl->qPtr,
                "showPopup",
                Qt::QueuedConnection,
                Q_ARG(QString, err), Q_ARG(QString, descr));
    traceExit;

}

void Worker::updateStatusAsync(const QString& status) {

    traceEnter;
    QMetaObject::invokeMethod(
                this->tfl,
                "updateStatus",
                Qt::QueuedConnection,
                Q_ARG(QString, status));
    traceExit;

}

void Worker::updateTileTimeAsync(unsigned int tileTimeMs) {

    traceEnter;
    QMetaObject::invokeMethod(
                this->tfl,
                "updateTileTime",
                Qt::QueuedConnection,
                Q_ARG(unsigned int, tileTimeMs));
    traceExit;

}

void Worker::updateStackedTimeAsync(unsigned int stackedTimeMs) {

    traceEnter;
    QMetaObject::invokeMethod(
                this->tfl,
                "updateStackedTime",
                Qt::QueuedConnection,
                Q_ARG(unsigned int, stackedTimeMs));
    traceExit;

}

void Worker::updateEstimatedResidualTimeAsync(unsigned int residualEstimatedTimsMs) {

    traceEnter;
    QMetaObject::invokeMethod(
                this->tfl,
                "updateEstimatedResidualTime",
                Qt::QueuedConnection,
                Q_ARG(unsigned int, residualEstimatedTimsMs));
    traceExit;

}

bool Worker::setupLaserOn() {

    traceEnter;

    // prima di ogni comando aggiungo dei ritardi di 200ms
    // per evitare di inviare troppo velocemente comandi al laser;

    Settings& settings = Settings::instance();

    QScopedPointer<ipg::IpgSyncInterface> ipgInterface(new ipg::IpgSyncInterface());

    if (!ipgInterface->getIsConnected())
        if (!ipgInterface->connectToLaser(settings.getIpgYLPNLaserIpAddress(), settings.getIpgYLPNLaserPort())) {
            traceErr() << "Impossibile connettersi al laser IPG";
//            showDialogAsync("Error", "Impossibile connettersi al laser IPG");
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
//        showDialogAsync("Error", "Impossibile inviare il comando emission OFF");
        return false;
    }

    traceInfo() << "Emissione enable: OFF";

    this->thread()->msleep(200);
    // reset errori
    if (!ipgInterface->reset(executionCode) || executionCode > 0) {
        traceErr() << "Impossibile inviare il comando di reset al laser";
        if (executionCode > 0)
            traceErr() << "Descrizion errore ipg: " << ipg::getExecOpCodeDescription(executionCode);
//        showDialogAsync("Error", "Impossibile inviare il comando di reset al laser");
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
//        showDialogAsync("Error", "Impossibile inviare il comando di reset al laser");
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
//        showDialogAsync("Error", "Impossibile inviare il comando di reset al laser");
        return false;
    }

    traceInfo() << "Impostato PRR: " << laserConf.getCurrentFrequency();

    this->thread()->msleep(200);

    // imposto la potenza del laser
    if (!ipgInterface->setPower(power, result, executionCode) || executionCode > 0) {
        traceErr() << "Impossibile impostare la potenza del laser";
        if (executionCode > 0)
            traceErr() << "Descrizion errore ipg: " << ipg::getExecOpCodeDescription(executionCode);
        return false;
    }

    traceInfo() << "Potenza laser: " << power;

    this->thread()->msleep(200);

    // abilito l'emissione
    if (!ipgInterface->setEE(true, executionCode) || executionCode > 0) {
        traceErr() << "Impossibile abilitare l'emissione del laser";
        if (executionCode > 0)
            traceErr() << "Descrizion errore ipg: " << ipg::getExecOpCodeDescription(executionCode);
//        showDialogAsync("Error",  "Impossibile impostare la potenza del laser");
        return false;
    }

    traceInfo() << "Emissione abilitata";

    ipgInterface->disconnectLaser();

    traceExit;
    return true;

}

bool Worker::setupLaserOff() {

    traceEnter;

    Settings& settings = Settings::instance();

    QScopedPointer<ipg::IpgSyncInterface> ipgInterface(new ipg::IpgSyncInterface());

    if (!ipgInterface->getIsConnected())
        if (!ipgInterface->connectToLaser(settings.getIpgYLPNLaserIpAddress(), settings.getIpgYLPNLaserPort())) {
            traceErr() << "Impossibile connettersi al laser IPG";
//            showDialogAsync("Error", "Impossibile connettersi al laser IPG");
            return false;
        }
    traceInfo() << "Connessione al laser avvenuta con successo";

    ipg::IPG_USHORT executionCode = 0;

    this->thread()->msleep(200);
    if (!ipgInterface->setEE(false, executionCode) || executionCode > 0) {
        traceErr() << "Impossibile inviare il comando emission OFF";
        if (executionCode > 0)
            traceErr() << "Descrizion errore ipg: " << ipg::getExecOpCodeDescription(executionCode);
//        showDialogAsync("Error", "Impossibile inviare il comando emission OFF");
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
//        showDialogAsync("Error", "Impossibile impostare la potenza del laser a 0");
        return false;
    }

    traceInfo() << "Impostato potenza a 0";

    ipgInterface->disconnectLaser();

    traceExit;
    return true;

}

bool Worker::getPulseEnergy(float& energyJoule) {

    traceEnter;

    traceInfo() << "Interrogazione stato laser";

    QScopedPointer<ipg::IpgSyncInterface> ipgInterface(new ipg::IpgSyncInterface());

    Settings& settings = Settings::instance();

    if (!ipgInterface->getIsConnected())
        if (!ipgInterface->connectToLaser(settings.getIpgYLPNLaserIpAddress(), settings.getIpgYLPNLaserPort())) {
            traceErr() << "Impossibile connettersi al laser IPG";
//            showDialogAsync("Error", "Impossibile connettersi al laser IPG");
            return false;
        }

    traceInfo() << "Connessione al laser avvenuta con successo";

    ipg::IPG_USHORT executionCode = 0;

    ipg::GetLaserStatusOutput status;
    if (!ipgInterface->getLaserStatus(status, executionCode) || executionCode > 0) {
        traceErr() << "Impossibile interrogare lo stato del laser";
        if (executionCode > 0)
            traceErr() << "Descrizion errore ipg: " << ipg::getExecOpCodeDescription(executionCode);
//        showDialogAsync("Error", "Impossibile interrogare lo stato del laser");
        return false;
    }

    energyJoule = status.getPulseEnergy() * 0.001;

    ipgInterface->disconnectLaser();

    traceExit;
    return true;

}

bool Worker::getPulseEnergyLaserAirCooled(float& energyJoule) {

    traceEnter;

    traceInfo() << "Interrogazione stato laser air cooled";

    QScopedPointer<ipg::IpgSyncInterface> ipgInterface(new ipg::IpgSyncInterface());

    Settings& settings = Settings::instance();

    if (!ipgInterface->getIsConnected())
        if (!ipgInterface->connectToLaser(settings.getIpgYLPNLaserIpAddress(), settings.getIpgYLPNLaserPort())) {
            traceErr() << "Impossibile connettersi al laser IPG";
//            showDialogAsync("Error", "Impossibile connettersi al laser IPG");
            return false;
        }

    traceInfo() << "Connessione al laser air cooled avvenuta con successo";

    ipg::IPG_USHORT executionCode = 0;

    ipg::GetAirCooledLaserStatusOutput status;
    if (!ipgInterface->getAirCooledLaserStatus(status, executionCode) || executionCode > 0) {
        traceErr() << "Impossibile interrogare lo stato del laser air cooled";
        if (executionCode > 0)
            traceErr() << "Descrizion errore ipg: " << ipg::getExecOpCodeDescription(executionCode);
//        showDialogAsync("Error", "Impossibile interrogare lo stato del laser");
        return false;
    }

    energyJoule = status.getPulseEnergy() * 0.001;

    ipgInterface->disconnectLaser();

    traceExit;
    return true;

}


/*
 *  T E S T   F R A M E   L O G I C
 */

TestFrameLogic::TestFrameLogic(QObject* parent) :
    QObject(parent),
    motionBean(), laserParametersChanged(false), hasErrors(false), machineStatus(MachineStatus::STATUS_NAN),
    isProcessStopped(false), isLaserInitialized(false) {

#ifdef FLAG_IPG_YLPN_LASER_PRESENT
    QTimer::singleShot(1000, this, &TestFrameLogic::initIpgYLPNLaser);
#endif
    machineStatusReceiver.reset(new PROGRAM_NAMESPACE::MachineStatusReceiver(this));
    DeviceFactoryInstance.subscribeMachineStatusReceiver(*machineStatusReceiver.data());

}

TestFrameLogic::~TestFrameLogic() { }

void TestFrameLogic::startWork() {

    traceEnter;

    auto&& settings = Settings::instance();

    if (!checkSWActive(settings.getUiSwIsaStatus(), settings.getUiSwIsaKey())) {
        qPtr->showPopup("SW Error", "Internal error: contact DV service.");
        return;
    }

    traceInfo() << "*** START PROCESS ***";
    //qPtr->ui->pbStartProcess->setEnabled(false);
    //qPtr->ui->pbStopProcess->setEnabled(true);
    //qPtr->ui->pbPauseProcess->setEnabled(false);

    qPtr->updatePrintConfiguration();

    PrintConfiguration printConfiguration = this->currentConfiguration;
    QWeakPointer<ErrorManager> errorManager = DeviceFactoryInstance.getErrorManager();

    NamedThread::Ptr workerThread = new NamedThread(WORKER_THREAD_NAME);
    Worker::Ptr worker = new Worker();
    worker->setPrintConfiguration(printConfiguration);
    worker->setPrintStatus(PrintStatus()); // <-- questo rende il pause status non valido
    worker->tfl = this;

    connect(workerThread, &QThread::started, worker, &Worker::startProcess);

    connect(worker, &Worker::enablePauseSignal, this, &TestFrameLogic::enablePauseSignal);
    connect(errorManager.data(), &ErrorManager::hasErrors, worker, &Worker::hasErrorsSignal);
    connect(errorManager.data(), &ErrorManager::hasFatals, worker, &Worker::hasFatalsSignal);
    connect(this, &TestFrameLogic::stopRequestSignal, worker, &Worker::stopRequestSignal);
    connect(this, &TestFrameLogic::pauseRequestSignal, worker, &Worker::pauseRequestSignal);

    connect(worker, &Worker::finishedSignal, this, [&, worker]() {
        this->printStatus = worker->getPrintStatus();
        worker->deleteLater(); // <--- lambda expression 1
    });
    connect(worker, &Worker::finishedSignal, workerThread, &QThread::quit);
//    connect(worker, &Worker::finishedSignal, worker, &Worker::deleteLater); // non agganciare questo segnale, viene chiamato delateLater sopra
    connect(workerThread, &QThread::finished, workerThread, &QThread::deleteLater);
    //connect(workerThread, &QThread::destroyed, [&]() {
    //    this->qPtr->ui->pbStartProcess->setEnabled(true);
    //    this->qPtr->ui->pbStopProcess->setEnabled(false);
    //});

    worker->moveToThread(workerThread);
    workerThread->start();

    traceExit;

}

void TestFrameLogic::stopWork() {

    traceEnter;

    if (this->machineStatus == MachineStatus::PAUSE || this->machineStatus == MachineStatus::STOP_RESUMABLE) {
        QSharedPointer<MachineStatusNotifier> machineStatusNotifier = DeviceFactoryInstance.instanceMachineStatusNotifier();
        machineStatusNotifier->setCurrentStatus(MachineStatus::IDLE);
    }
    emit stopRequestSignal();
    traceExit;

}

void TestFrameLogic::resumeWork() {

    traceEnter;
    traceInfo() << "*** RESUME PROCESS ***";

    qPtr->ui->pbPauseProcess->setEnabled(false);

    PrintConfiguration printConfiguration = this->currentConfiguration;
    QWeakPointer<ErrorManager> errorManager = DeviceFactoryInstance.getErrorManager();
    PrintStatus printStatus = this->printStatus;

    NamedThread::Ptr workerThread = new NamedThread(WORKER_THREAD_NAME);
    Worker::Ptr worker = new Worker();
    worker->setPrintConfiguration(printConfiguration);
    worker->setPrintStatus(printStatus);
    worker->tfl = this;

    connect(workerThread, &QThread::started, worker, &Worker::startProcess);

    connect(worker, &Worker::enablePauseSignal, this, &TestFrameLogic::enablePauseSignal);
    connect(errorManager.data(), &ErrorManager::hasErrors, worker, &Worker::hasErrorsSignal);
    connect(errorManager.data(), &ErrorManager::hasFatals, worker, &Worker::hasFatalsSignal);
    connect(this, &TestFrameLogic::stopRequestSignal, worker, &Worker::stopRequestSignal);
    connect(this, &TestFrameLogic::pauseRequestSignal, worker, &Worker::pauseRequestSignal);

    connect(worker, &Worker::finishedSignal, this, [&, worker]() {
        this->printStatus = worker->getPrintStatus();
        worker->deleteLater(); // lambda expression 2
    });
    connect(worker, &Worker::finishedSignal, workerThread, &QThread::quit);
//    connect(worker, &Worker::finishedSignal, worker, &Worker::deleteLater); // non agganciare questo segnale, viene chiamato delateLater sopra

    connect(workerThread, &QThread::finished, workerThread, &QThread::deleteLater);
    //connect(workerThread, &QThread::destroyed, [&]() {
    //    this->qPtr->ui->pbStartProcess->setEnabled(true);
    //    this->qPtr->ui->pbStopProcess->setEnabled(false);
    //});

    worker->moveToThread(workerThread);
    workerThread->start();

    traceExit;

}

void TestFrameLogic::pauseWork() {

    traceEnter;
    emit pauseRequestSignal();
    traceExit;

}

void TestFrameLogic::changeGuideLaserState() {

    traceEnter;

    bool currentGuideLaserStatus = qPtr->ui->cbGuideLaser->isChecked();
    bool valueToSet = !currentGuideLaserStatus;

    ipg::IPG_USHORT res;

    Settings& settings = Settings::instance();

    QScopedPointer<ipg::IpgSyncInterface> ipgInterface(new ipg::IpgSyncInterface());

    if (!ipgInterface->getIsConnected()) {
        if (!ipgInterface->connectToLaser(settings.getIpgYLPNLaserIpAddress(), settings.getIpgYLPNLaserPort())) {
            DialogAlert diag;
            diag.setupLabels("Error", "Impossibile connettersi al laser");
            diag.exec();
            return;
        }
    }

    if (!ipgInterface->setGlMode(valueToSet, res)) {
        DialogAlert diag;
        diag.setupLabels("Error", "Impossibile settare il diodo rosso");
        diag.exec();
        return;
    }

    ipgInterface->disconnect();

    qPtr->ui->cbGuideLaser->setChecked(valueToSet);

    traceExit;

}

void TestFrameLogic::updateStatus(const QString& status) {
    traceEnter;
    qPtr->ui->leStatus->setText(status);
    traceExit;
}

void TestFrameLogic::updateTileTime(unsigned int tileTimeMs) {

    traceEnter;
    qPtr->ui->leTileTimeMeasure->setText(QString::number(tileTimeMs));
    traceExit;

}

void TestFrameLogic::updateStackedTime(unsigned int stackedTimeMs) {

    traceEnter;
    qPtr->ui->leStackTimeMeasure->setText(QString::number(stackedTimeMs));
    traceExit;

}

void TestFrameLogic::updateEstimatedResidualTime(unsigned int residualEstimatedTimsMs) {

    traceEnter;
    int totalSeconds = qCeil(residualEstimatedTimsMs * 0.001);
    int seconds = totalSeconds % 60;
    int minutes = qCeil(totalSeconds / 60) % 60;
    int hours = qFloor(totalSeconds / 3600);

    QString residualTime = QString("%1 : %2 : %3").arg(hours).arg(minutes).arg(seconds);
    qPtr->ui->leEstimatedResidualTime->setText(residualTime);
    traceExit;

}

void TestFrameLogic::initIpgYLPNLaser() {

    traceEnter;
    // WARNING NIC 03/04/2019 - inizio gestione temporanea laser YLPN (da rifare)

    traceInfo() << "Start inizializzazione laser ipg YLPN";

    QScopedPointer<ipg::IpgSyncInterface> ipgInterface(new ipg::IpgSyncInterface());

    Settings& instance = Settings::instance();

    if (!ipgInterface->getIsConnected())
        if (!ipgInterface->connectToLaser(instance.getIpgYLPNLaserIpAddress(), instance.getIpgYLPNLaserPort())) {
            traceErr() << "Connessione al laser fallita";
            traceErr() << "IP: " << instance.getIpgYLPNLaserIpAddress();
            traceErr() << "Port: " << instance.getIpgYLPNLaserPort();
            QTimer::singleShot(5000, this, &TestFrameLogic::initIpgYLPNLaser);
            return;
        }

    ipg::GetLaserModelInfoOutput laserModelInfo;
    ipg::IPG_USHORT res;
    if (!ipgInterface->getLaserModelInfo(laserModelInfo, res)) {
        traceErr() << "Errore nella chiamata a getLaserModelInfo";
        traceErr() << "Codice errore: " << res;
        QTimer::singleShot(5000, this, &TestFrameLogic::initIpgYLPNLaser);
        return;
    }

    QList<IpgYLPNLaserConfiguration::Mode> modes;

    for (int i=0; i<laserModelInfo.getInstalledModes(); ++i) {
        ipg::GetModeParameterOutput output;
        if (!ipgInterface->getModeParameter(i, output, res)) {
            traceErr() << "Errore nella chiamata a getModeParameter";
            traceErr() << "Codice errore: " << res;
            QTimer::singleShot(5000, this, &TestFrameLogic::initIpgYLPNLaser);
            return;
        }
        IpgYLPNLaserConfiguration::Mode mode;
        mode.minFrequency = output.getMinimumFrequency();
        mode.maxFrequency = output.getMaximumFrequency();
        mode.nominalFrequency = output.getNominalFrequency();
        mode.pulseDuration = output.getNominalPulseDuration();
        modes.append(mode);
    }

    IpgYLPNLaserConfiguration& laserConfiguration = IpgYLPNLaserConfiguration::instance();

    float currentPulseDuration = laserModelInfo.getNominalPulseDuration();
    int currentModeIndex = -1;
    int i = 0;
    for (auto&& mode: modes) {
        laserConfiguration.addMode(mode.pulseDuration, mode.nominalFrequency, mode.minFrequency, mode.maxFrequency);
        if (qRound(currentPulseDuration) == qRound(mode.pulseDuration))
            currentModeIndex = i;
        ++i;
    }

    laserConfiguration.setCurrentModeIndex(currentModeIndex == -1 ? 0 : currentModeIndex);
    laserConfiguration.setCurrentFrequency(qRound(laserModelInfo.getNominalFrequency()));
    laserConfiguration.setCurrentPower(laserConfiguration.getCurrentPower());
    laserConfiguration.setIsInitialized(true);

    emit laserIpgYLPNinitializedSignal();
    traceInfo() << "Inizializzazione laser ipg YLPN completata con successo";

    ipgInterface->disconnectLaser();
    traceExit;
}



/*
 *  T E S T   F R A M E
 */

TestFrame::TestFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TestFrame), dPtr(new TestFrameLogic()) {

    traceEnter;

    dPtr->qPtr = this;

    this->setupUi();
    this->setupSignalsAndSlots();

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
    dPtr->motionBean = b;
    this->updateUi();
    traceExit;

}

void TestFrame::updateHasErrors(bool hasErrors) {

    traceEnter;
    dPtr->hasErrors = hasErrors;
    this->updateUi();
    traceExit;

}

void TestFrame::updateMachineStatus(const MachineStatus& s) {

    traceEnter;
    dPtr->machineStatus = s;
    this->updateUi();
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

void TestFrame::showPopup(const QString& err, const QString& descr) {

    traceEnter;
    DialogAlert diag;
    diag.setupLabels(err, descr);
    diag.exec();
    traceExit;

}

void TestFrame::updatePrintConfiguration() {

    traceEnter;
    PrintConfiguration& pc = dPtr->currentConfiguration;

    // percorso file
    pc.setFilePath(ui->leFilePath->text());

    // setup movimentazione
    pc.setTileSizeMm(ui->sbTileSize->value());
    pc.setAngleMRad(ui->dsbAngleMrad->value());
    pc.setOffsetXmm(ui->dsbOffsetX->value());
    pc.setOffsetYmm(ui->dsbOffsetY->value());
    pc.setOffsetZmm(ui->dsbOffsetZ->value());
    pc.setTileScaleXPercent(ui->dsbScaleX->value());
    pc.setTileScaleYPercent(ui->dsbScaleY->value());
    pc.setWaitTimeMs(ui->sbTileTime->value());
    pc.setWaitTimeAfterYMovementMs(ui->sbWaitTimeYMovement->value());
    pc.setLaserFrequency(ui->sbLaserFrequency->value());

    // scelta algoritmo
    pc.setIsRandomAlgorithm(ui->rbRandomChoice->isChecked());
    pc.setIsNeighborhoodAlgorithm(ui->rbNHChoice->isChecked());

    // configurazione random
    pc.setRandomPointsPerTile(ui->sbRPointsPerTile->value());
    pc.setRandomIsShuffleRowTiles(ui->cbRShuffleRowTiles->isChecked());

    // configurazione neighborhood
    pc.setNeighborhoodMinDistanceUm(ui->sbNHMinDistance->value());
    pc.setNeighborhoodIsShuffleRowTiles(ui->rbNHShuffleRowTiles->isChecked());
    pc.setNeighborhoodIsReverseY(ui->rbNHReverseY->isChecked());
    pc.setNeighborhoodIsShuffleStackedTiles(ui->cbNHShuffleStackedTiles->isChecked());

    // scelta del punto di stampa
    pc.setPointShape(static_cast<PointShapeEnum>(pointShapeGroup->checkedId()));

    // point
    pc.setNumberOfPulses(ui->sbPointPulses->value());

    // circle points
    pc.setCirclePointsRadiusUm(ui->sbCirclePointsRadius->value());
    pc.setCirclePointsNumberOfSides(ui->sbCirclePointsNumberSides->value());
    pc.setCirclePointsNumberOfPulses(ui->sbCirclePointsPulses->value());

    // circle vector
    pc.setCircleVectorRadiusUm(ui->sbCircleVectorRadius->value());
    pc.setCircleVectorNumberOfRevolutions(ui->sbCircleVectorNumberRevolutions->value());
    pc.setCircleVectorNumberOfSides(ui->sbCircleVectorNumberSides->value());
    pc.setCircleVectorPitch(ui->sbCircleVectorPointsPitch->value());

    QStringList stringList;

    stringList << "Tile size mm: " << QString::number(pc.getTileSizeMm()) << "\r\n";
    stringList << "Angle mrad: " << QString::number(pc.getAngleMRad()) << "\r\n";
    stringList << "Offset x mm: " << QString::number(pc.getOffsetXmm()) << "\r\n";
    stringList << "Offset y mm: " << QString::number(pc.getOffsetYmm()) << "\r\n";
    stringList << "Offset z mm: " << QString::number(pc.getOffsetZmm()) << "\r\n";
    stringList << "Tile scale x percent: " << QString::number(pc.getTileScaleXPercent()) << "\r\n";
    stringList << "Tile scale y percent: " << QString::number(pc.getTileScaleYPercent()) << "\r\n";
    stringList << "Wait time ms: " << QString::number(pc.getWaitTimeMs()) << "\r\n";
    stringList << "Wait time after y movement ms: " << QString::number(pc.getWaitTimeAfterYMovementMs()) << "\r\n";
    stringList << "Laser frequency: " << QString::number(pc.getLaserFrequency()) << "\r\n";

    // SCELTA ALGORITMO
    stringList << "Is random algorithm: " << QString::number(pc.getIsRandomAlgorithm()) << "\r\n";
    stringList << "Is neighborhood algorithm: " << QString::number(pc.getIsNeighborhoodAlgorithm()) << "\r\n";

    // CONFIGURAZIONE RANDOM
    stringList << "Random points per tile: " << QString::number(pc.getRandomPointsPerTile()) << "\r\n";
    stringList << "Random is shuffle row tiles: " << QString::number(pc.getRandomIsShuffleRowTiles()) << "\r\n";

    // CONFIGURAZIONE NEIGHBORHOOD
    stringList << "Neighborhood min distance um: " << QString::number(pc.getNeighborhoodMinDistanceUm()) << "\r\n";
    stringList << "Neighborhood is shuffle row tiles: " << QString::number(pc.getNeighborhoodIsShuffleRowTiles()) << "\r\n";
    stringList << "Neighborhood is reverse Y: " << QString::number(pc.getNeighborhoodIsReverseY()) << "\r\n";
    stringList << "Neighborhood is shuffle stacked tiles: " << QString::number(pc.getNeighborhoodIsShuffleStackedTiles()) << "\r\n";

    // SCELTA DEL PUNTO DI STAMPA
    stringList << "Point shape: " << getStringFromPointShapeEnum(pc.getPointShape()) << "\r\n";

    // POINT
    stringList << "Number of pulses: " << QString::number(pc.getNumberOfPulses()) << "\r\n";

    // CIRCLE POINTS
    stringList << "Circle points radius um: " << QString::number(pc.getCirclePointsRadiusUm()) << "\r\n";
    stringList << "Circle points number of sides: " << QString::number(pc.getCirclePointsNumberOfSides()) << "\r\n";
    stringList << "Circle points number of pulses: " << QString::number(pc.getCirclePointsNumberOfPulses()) << "\r\n";

    // CIRCLE VECTOR
    stringList << "Circle vector radius um: " << QString::number(pc.getCircleVectorRadiusUm()) << "\r\n";
    stringList << "Circle vector number of revolutions: " << QString::number(pc.getCircleVectorNumberOfRevolutions()) << "\r\n";
    stringList << "Circle vector number of sides: " << QString::number(pc.getCircleVectorNumberOfSides()) << "\r\n";
    stringList << "Circle vector pitch: " << QString::number(pc.getCircleVectorPitch()) << "\r\n";

    ui->lConfigurationDetail->setText(stringList.join(""));

    traceExit;

}

void TestFrame::restorePrintConfiguration() {

    traceEnter;

    PrintConfiguration currentConfiguration = dPtr->currentConfiguration;

    ui->sbTileSize->setValue(currentConfiguration.getTileSizeMm());
    ui->dsbAngleMrad->setValue(currentConfiguration.getAngleMRad());
    ui->dsbOffsetX->setValue(currentConfiguration.getOffsetXmm());
    ui->dsbOffsetY->setValue(currentConfiguration.getOffsetYmm());
    ui->dsbOffsetZ->setValue(currentConfiguration.getOffsetZmm());
    ui->dsbScaleX->setValue(currentConfiguration.getTileScaleXPercent());
    ui->dsbScaleY->setValue(currentConfiguration.getTileScaleYPercent());
    ui->sbTileTime->setValue(currentConfiguration.getWaitTimeMs());
    ui->sbWaitTimeYMovement->setValue(currentConfiguration.getWaitTimeAfterYMovementMs());
    ui->sbLaserFrequency->setValue(currentConfiguration.getLaserFrequency());

    ui->rbRandomChoice->setChecked(currentConfiguration.getIsRandomAlgorithm());
    ui->rbNHChoice->setChecked(currentConfiguration.getIsNeighborhoodAlgorithm());

    ui->sbRPointsPerTile->setValue(currentConfiguration.getRandomPointsPerTile());
    ui->cbRShuffleRowTiles->setChecked(currentConfiguration.getRandomIsShuffleRowTiles());

    ui->sbNHMinDistance->setValue(currentConfiguration.getNeighborhoodMinDistanceUm());
    ui->rbNHNormal->setChecked(!currentConfiguration.getNeighborhoodIsShuffleRowTiles() && !currentConfiguration.getNeighborhoodIsReverseY());
    ui->rbNHShuffleRowTiles->setChecked(currentConfiguration.getNeighborhoodIsShuffleRowTiles());
    ui->rbNHReverseY->setChecked(currentConfiguration.getNeighborhoodIsReverseY());
    ui->cbNHShuffleStackedTiles->setChecked(currentConfiguration.getNeighborhoodIsShuffleStackedTiles());

    switch (currentConfiguration.getPointShape()) {
    case PointShapeEnum::POINT:
        ui->rbPoint->setChecked(true);
        ui->swPointShapeDetails->setCurrentIndex(0); break;
        break;
    case PointShapeEnum::CIRCLE_POINTS:
        ui->rbCirclePoints->setChecked(true);
        ui->swPointShapeDetails->setCurrentIndex(1); break;
        break;
    case PointShapeEnum::CIRCLE_VECTOR:
        ui->rbCircleVector->setChecked(true);
        ui->swPointShapeDetails->setCurrentIndex(2); break;
        break;
    }

    ui->sbPointPulses->setValue(currentConfiguration.getNumberOfPulses());

    ui->sbCirclePointsRadius->setValue(currentConfiguration.getCirclePointsRadiusUm());
    ui->sbCirclePointsNumberSides->setValue(currentConfiguration.getCirclePointsNumberOfSides());
    ui->sbCirclePointsPulses->setValue(currentConfiguration.getCirclePointsNumberOfPulses());

    ui->sbCircleVectorRadius->setValue(currentConfiguration.getCircleVectorRadiusUm());
    ui->sbCircleVectorNumberRevolutions->setValue(currentConfiguration.getCircleVectorNumberOfRevolutions());
    ui->sbCircleVectorNumberSides->setValue(currentConfiguration.getCircleVectorNumberOfSides());
    ui->sbCircleVectorPointsPitch->setValue(currentConfiguration.getCircleVectorPitch());

    traceExit;

}

void TestFrame::saveConfiguration() {

    traceEnter;

    Settings& s = Settings::instance();

    QFileDialog fd;
    fd.setDirectory(QDir(s.getMarkingConfigurationPath()));
    fd.setNameFilters(QStringList("configuration (*.json)"));
    fd.setAcceptMode(QFileDialog::AcceptSave);
    fd.setViewMode(QFileDialog::Detail);
    if (fd.exec() == QDialog::Accepted) {
        QString filePath = fd.selectedFiles().at(0);

        QFile file(filePath);

        if (!file.open(QIODevice::ReadWrite)) {
            traceWarn() << "Impossibile aprire il file in scrittura";
            DialogAlert diag(this);
            diag.setupLabels("Errore apertura file", QString("Errore nell'apertura del file %1").arg(filePath));
            diag.exec();
            return;
        }

        QScopedPointer<IAbstractJsonParser> parser(new ConfigurationJsonParser());
        QByteArray res;
        if (parser->encodeJson(&dPtr->currentConfiguration, res) != JSON_PARSER_NO_ERROR) {
            traceWarn() << "Errore nel parser del file";
            DialogAlert diag;
            diag.setupLabels("Errore parsing file", QString("Il file di configurazione non ' valido").arg(filePath));
            diag.exec();
            return;
        }

        file.write(res);
    }

    traceExit;

}

void TestFrame::loadConfiguration() {

    traceEnter;

    Settings& s = Settings::instance();

    QFileDialog fd;
    fd.setDirectory(QDir(s.getMarkingConfigurationPath()));
    fd.setNameFilters(QStringList("configuration (*.json)"));
    fd.setAcceptMode(QFileDialog::AcceptOpen);
    fd.setViewMode(QFileDialog::Detail);

    if (fd.exec() == QDialog::Accepted) {
        QString filePath = fd.selectedFiles().at(0);

        QFile file(filePath);

        if (!file.open(QIODevice::ReadOnly)) {
            traceWarn() << "Impossibile aprire il file in lettura";
            DialogAlert diag(this);
            diag.setupLabels("Errore apertura file", QString("Errore nell'apertura del file %1").arg(filePath));
            diag.exec();
            return;
        }

        QByteArray saveData = file.readAll();

        QScopedPointer<IAbstractJsonParser> parser(new ConfigurationJsonParser());
        PrintConfiguration pc;
        if (parser->decodeJson(saveData, &pc) != JSON_PARSER_NO_ERROR) {
            traceWarn() << "Errore nel parser del file";
            DialogAlert diag;
            diag.setupLabels("Errore parsing file", QString("Il file di configurazione non ' valido").arg(filePath));
            diag.exec();
            return;
        }

        dPtr->currentConfiguration = pc;

        restorePrintConfiguration();
        updatePrintConfiguration();
    }

    traceExit;

}

void TestFrame::updateUi() {

    traceEnter;
    bool isStartEnabled = !dPtr->hasErrors && !dPtr->motionBean.getNeedResetAxis() && dPtr->machineStatusReceiver->getCurrentStatus() == MachineStatus::IDLE;
    bool isStopEnabled = !dPtr->hasErrors && !dPtr->motionBean.getNeedResetAxis() && (
                (dPtr->machineStatusReceiver->getCurrentStatus() == MachineStatus::PRINTING) ||
                (dPtr->machineStatusReceiver->getCurrentStatus() == MachineStatus::PAUSE) ||
                (dPtr->machineStatusReceiver->getCurrentStatus() == MachineStatus::STOP_RESUMABLE)
                );
    bool isResumeEnabled = !dPtr->hasErrors && !dPtr->motionBean.getNeedResetAxis() && (
                (dPtr->machineStatusReceiver->getCurrentStatus() == MachineStatus::PAUSE) ||
                (dPtr->machineStatusReceiver->getCurrentStatus() == MachineStatus::STOP_RESUMABLE)
                );

    this->ui->pbStartProcess->setEnabled(isStartEnabled);
    this->ui->pbStopProcess->setEnabled(isStopEnabled);
    this->ui->pbResumeProcess->setEnabled(isResumeEnabled);
    traceExit;

}

void TestFrame::setupUi() {

    traceEnter;

    ui->setupUi(this);

    auto&& settings = Settings::instance();

    ui->tabWidget->tabBar()->installEventFilter(this);
    ui->tabWidget->setCurrentIndex(0);

    ui->leFilePath->setReadOnly(true);
    ui->leStatus->setReadOnly(true);
    ui->leStackTimeMeasure->setReadOnly(true);
    ui->leTileTimeMeasure->setReadOnly(true);
    ui->leEstimatedResidualTime->setReadOnly(true);

    ui->leFilePath->setEnabled(false);
    ui->leStatus->setEnabled(false);
    ui->leStackTimeMeasure->setEnabled(false);
    ui->leTileTimeMeasure->setEnabled(false);
    ui->leEstimatedResidualTime->setEnabled(false);

    ui->sbTileSize->setRange(TEST_FRAME_TILE_SIZE_MIN, TEST_FRAME_TILE_SIZE_MAX);

    int minTileTime = qMax(TEST_FRAME_WAIT_TIME_MS_MIN, settings.getUiSwLimitsMinTileTimeMs());
    ui->sbTileTime->setRange(minTileTime, TEST_FRAME_WAIT_TIME_MS_MAX);
    ui->sbWaitTimeYMovement->setRange(TEST_FRAME_Y_MOVEMENTS_WAIT_TIME_MS_MIN, TEST_FRAME_Y_MOVEMENTS_WAIT_TIME_MS_MAX);

    ui->dsbOffsetX->setRange(TEST_FRAME_OFFSET_X_MIN, TEST_FRAME_OFFSET_X_MAX);
    ui->dsbOffsetX->setSingleStep(TEST_FRAME_DSB_STEP);
    ui->dsbOffsetX->setDecimals(3);

    ui->dsbOffsetY->setRange(TEST_FRAME_OFFSET_Y_MIN, TEST_FRAME_OFFSET_Y_MAX);
    ui->dsbOffsetY->setSingleStep(TEST_FRAME_DSB_STEP);
    ui->dsbOffsetY->setDecimals(3);

    ui->dsbOffsetZ->setRange(TEST_FRAME_OFFSET_Z_MIN, TEST_FRAME_OFFSET_Z_MAX);
    ui->dsbOffsetZ->setSingleStep(TEST_FRAME_DSB_STEP);
    ui->dsbOffsetZ->setDecimals(3);

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

    ui->rbRandomChoice->setChecked(false);
    ui->rbNHChoice->setChecked(true);
    drillingAlgorithm = new QButtonGroup(this);
    drillingAlgorithm->addButton(ui->rbRandomChoice);
    drillingAlgorithm->addButton(ui->rbNHChoice);

    ui->rbNHNormal->setChecked(true);
    ui->rbNHShuffleRowTiles->setChecked(false);
    ui->rbNHReverseY->setChecked(false);
    nhRowTilesOrder = new QButtonGroup(this);
    nhRowTilesOrder->addButton(ui->rbNHNormal);
    nhRowTilesOrder->addButton(ui->rbNHShuffleRowTiles);
    nhRowTilesOrder->addButton(ui->rbNHReverseY);

    ui->pbStartProcess->setEnabled(false);
    ui->pbStopProcess->setEnabled(false);
    ui->pbPauseProcess->setEnabled(false);
    ui->pbResumeProcess->setEnabled(false);

    // point shape tab
    pointShapeGroup = new QButtonGroup(this);
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
    ui->swPointShapeDetails->setCurrentIndex(0);

    // laser tab
    ui->sbLaserPower->setRange(TEST_FRAME_LASER_MIN_POWER, TEST_FRAME_LASER_MAX_POWER);
    ui->hsLaserPower->setRange(TEST_FRAME_LASER_MIN_POWER, TEST_FRAME_LASER_MAX_POWER);
    ui->hsLaserPower->setSingleStep(TEST_FRAME_LASER_POWER_STEP);

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
#else
    ui->cbGuideLaser->setEnabled(true);
    ui->cbLaserInitialized->setEnabled(true);
    ui->hsLaserPower->setEnabled(true);
    ui->sbLaserFrequency->setEnabled(true);
    ui->hsLaserFrequency->setEnabled(true);
    ui->cbLaserPulseWidth->setEnabled(true);
    ui->sbLaserPower->setEnabled(true);
    ui->pbGuideLaser->setEnabled(true);
    ui->pbSet->setEnabled(true);
    ui->pbReset->setEnabled(true);
#endif

    traceExit;

}

void TestFrame::setupSignalsAndSlots() {

    traceEnter;

    auto&& errorManager = DeviceFactoryInstance.getErrorManager();
    auto&& motionAnalizer = DeviceFactoryInstance.getMotionAnalizer();

    connect(errorManager.data(), &ErrorManager::notifyMaxErrorType, [&](ErrorType type) {
        this->updateHasErrors(type == ErrorType::ERROR || type == ErrorType::FATAL);
    });

    connect(motionAnalizer.data(), &IMotionAnalizer::motionBeanSignal, this, &TestFrame::updateMotionBean);

    connect(dPtr->machineStatusReceiver.data(), &PROGRAM_NAMESPACE::MachineStatusReceiver::statusChanged, this, &TestFrame::updateMachineStatus);
    connect(dPtr, &TestFrameLogic::enablePauseSignal, this, [&](bool enable) {
        this->ui->pbPauseProcess->setEnabled(enable);
    });

    connect(ui->pbStartProcess, &QPushButton::clicked, dPtr, &TestFrameLogic::startWork);
    connect(ui->pbStopProcess, &QPushButton::clicked, dPtr, &TestFrameLogic::stopWork);
    connect(ui->pbPauseProcess, &QPushButton::clicked, dPtr, &TestFrameLogic::pauseWork);
    connect(ui->pbResumeProcess, &QPushButton::clicked, dPtr, &TestFrameLogic::resumeWork);


    // signals/slots della tab laser
    connect(ui->sbLaserPower, static_cast<void (MDSpinBox::*)(int)>(&MDSpinBox::valueChanged), [&](int value) {
        this->ui->hsLaserPower->setValue(value);
        dPtr->laserParametersChanged = true;
        this->updateTabLaserLabel(true);
    });
    connect(ui->hsLaserPower, &QSlider::valueChanged, [&](int value) {
        this->ui->sbLaserPower->setValue(value);
        dPtr->laserParametersChanged = true;
        this->updateTabLaserLabel(true);
    });
    connect(ui->sbLaserFrequency, static_cast<void (MDSpinBox::*)(int)>(&MDSpinBox::valueChanged), [&](int value) {
        this->ui->hsLaserFrequency->setValue(value);
        dPtr->laserParametersChanged = true;
        this->updateTabLaserLabel(true);
    });
    connect(ui->hsLaserFrequency, &QSlider::valueChanged, [&](int value) {
        this->ui->sbLaserFrequency->setValue(value);
        dPtr->laserParametersChanged = true;
        this->updateTabLaserLabel(true);
    });
    connect(ui->cbLaserPulseWidth, static_cast<void (MDComboBox::*)(int)>(&MDComboBox::currentIndexChanged), [&](int index) {
        IpgYLPNLaserConfiguration& laserConfiguration = IpgYLPNLaserConfiguration::instance();
        IpgYLPNLaserConfiguration::Mode currentMode = laserConfiguration.getMode(index);
        ui->sbLaserFrequency->setRange(currentMode.minFrequency, currentMode.maxFrequency);
        ui->sbLaserFrequency->setValue(currentMode.nominalFrequency);
        ui->hsLaserFrequency->setRange(currentMode.minFrequency, currentMode.maxFrequency);
        ui->hsLaserFrequency->setValue(currentMode.nominalFrequency);
        dPtr->laserParametersChanged = true;
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
        dPtr->laserParametersChanged = false;
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

        dPtr->laserParametersChanged = false;
        this->updateTabLaserLabel(false);

    });

    connect(ui->pbGuideLaser, &QPushButton::clicked, dPtr, &TestFrameLogic::changeGuideLaserState);

    connect(dPtr, &TestFrameLogic::laserIpgYLPNinitializedSignal, this, &TestFrame::laserIpgYLPNConfigurationReady);

    // signals/slots della tab points
    connect(pointShapeGroup, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), [&](int id) {

        switch (id) {
        case PointShapeEnum::POINT: ui->swPointShapeDetails->setCurrentIndex(0); break;
        case PointShapeEnum::CIRCLE_POINTS: ui->swPointShapeDetails->setCurrentIndex(1); break;
        case PointShapeEnum::CIRCLE_VECTOR: ui->swPointShapeDetails->setCurrentIndex(2); break;
        default: break;
        }

    });

    connect(ui->tabWidget, &QTabWidget::currentChanged, [&](int index) {
        if (index == TEST_FRAME_SAVE_LOAD_TAB_INDEX)
            this->updatePrintConfiguration();
    });

    connect(ui->pbSave, &QPushButton::clicked, this, &TestFrame::saveConfiguration);
    connect(ui->pbLoad, &QPushButton::clicked, this, &TestFrame::loadConfiguration);

    traceExit;

}

void TestFrame::updateTabLaserLabel(bool setAsterisk) {

    QString tabText = ui->tabWidget->tabText(TEST_FRAME_LASER_TAB_INDEX);

    if (!(tabText.right(1).compare("*") == 0) && setAsterisk)
        ui->tabWidget->setTabText(TEST_FRAME_LASER_TAB_INDEX, tabText.append("*"));

    if ((tabText.right(1).compare("*") == 0) && !setAsterisk)
        ui->tabWidget->setTabText(TEST_FRAME_LASER_TAB_INDEX, tabText.left(tabText.size()-1));

}

bool TestFrame::eventFilter(QObject* object, QEvent* event) {

    // NOTE NIC 04/04/2019 - filtro gli eventi del cambio tab;
    // se modifico qualche parametro e non lo imposto, informo l'utente che i dati
    // saranno resettati con una dialog
    if (qobject_cast<QTabBar*>(object)) {

        // abilito solamente il click del mouse
        if (event->type() == QEvent::Wheel || event->type() == QEvent::KeyPress)
            return true;

        if (dPtr->laserParametersChanged) {

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
                            dPtr->laserParametersChanged = false;
                            this->updateTabLaserLabel(false);

                        }
                    }

                }

            }

        }

    }

    return QFrame::eventFilter(object, event);

}

JsonParserError ConfigurationJsonParser::encodeJson(PrintConfiguration::ConstPtr obj, QByteArray& output) {

    QJsonDocument doc;
    QJsonObject jsonObj;

    jsonObj[CONFIGURATION_JSON_TILE_SIZE_MM_KEY] = obj->getTileSizeMm();
    jsonObj[CONFIGURATION_JSON_ANGLE_MRAD_KEY] = obj->getAngleMRad();
    jsonObj[CONFIGURATION_JSON_OFFSET_X_MM_KEY] = obj->getOffsetXmm();
    jsonObj[CONFIGURATION_JSON_OFFSET_Y_MM_KEY] = obj->getOffsetYmm();
    jsonObj[CONFIGURATION_JSON_OFFSET_Z_MM_KEY] = obj->getOffsetZmm();
    jsonObj[CONFIGURATION_JSON_TILE_SCALE_X_PERCENT_KEY] = obj->getTileScaleXPercent();
    jsonObj[CONFIGURATION_JSON_TILE_SCALE_Y_PERCENT_KEY] = obj->getTileScaleYPercent();
    jsonObj[CONFIGURATION_JSON_WAIT_TIME_MS_KEY] = obj->getWaitTimeMs();
    jsonObj[CONFIGURATION_JSON_WAIT_TIME_AFTER_Y_MOVEMENT_MS_KEY] = obj->getWaitTimeAfterYMovementMs();
    jsonObj[CONFIGURATION_JSON_LASER_FREQUENCY_KEY] = obj->getLaserFrequency();
    jsonObj[CONFIGURATION_JSON_IS_RANDOM_ALGORITHM_KEY] = obj->getIsRandomAlgorithm();
    jsonObj[CONFIGURATION_JSON_IS_NEIGHBORHOOD_ALGORITHM_KEY] = obj->getIsNeighborhoodAlgorithm();
    jsonObj[CONFIGURATION_JSON_RANDOM_POINTS_PER_TILE_KEY] = obj->getRandomPointsPerTile();
    jsonObj[CONFIGURATION_JSON_RANDOM_IS_SHUFFLE_ROW_TILES_KEY] = obj->getRandomIsShuffleRowTiles();
    jsonObj[CONFIGURATION_JSON_NEIGHBORHOOD_MIN_DISTANCE_UM_KEY] = obj->getNeighborhoodMinDistanceUm();
    jsonObj[CONFIGURATION_JSON_NEIGHBORHOOD_IS_SHUFFLE_STACKED_TILES_KEY] = obj->getNeighborhoodIsShuffleStackedTiles();
    jsonObj[CONFIGURATION_JSON_NEIGHBORHOOD_IS_SHUFFLE_ROW_TILES_KEY] = obj->getNeighborhoodIsShuffleRowTiles();
    jsonObj[CONFIGURATION_JSON_NEIGHBORHOOD_IS_REVERSE_Y_KEY] = obj->getNeighborhoodIsReverseY();
    jsonObj[CONFIGURATION_JSON_POINT_SHAPE_KEY] = getStringFromPointShapeEnum(obj->getPointShape());
    jsonObj[CONFIGURATION_JSON_NUMBER_OF_PULSES_KEY] = obj->getNumberOfPulses();
    jsonObj[CONFIGURATION_JSON_CIRCLE_POINTS_RADIUS_UM_KEY] = obj->getCirclePointsRadiusUm();
    jsonObj[CONFIGURATION_JSON_CIRCLE_POINTS_NUMBER_OF_SIDES_KEY] = obj->getCirclePointsNumberOfSides();
    jsonObj[CONFIGURATION_JSON_CIRCLE_POINTS_NUMBER_OF_PULSES_KEY] = obj->getCirclePointsNumberOfPulses();
    jsonObj[CONFIGURATION_JSON_CIRCLE_VECTOR_RADIUS_UM_KEY] = obj->getCircleVectorRadiusUm();
    jsonObj[CONFIGURATION_JSON_CIRCLE_VECTOR_NUMBER_OF_REVOLUTIONS_KEY] = obj->getCircleVectorNumberOfRevolutions();
    jsonObj[CONFIGURATION_JSON_CIRCLE_VECTOR_NUMBER_OF_SIDES_KEY] = obj->getCircleVectorNumberOfSides();
    jsonObj[CONFIGURATION_JSON_CIRCLE_VECTOR_PITCH_KEY] = obj->getCircleVectorPitch();

    doc.setObject(jsonObj);

    output = doc.toJson();

    return JSON_PARSER_NO_ERROR;

}

JsonParserError ConfigurationJsonParser::decodeJson(const QByteArray& input, PrintConfiguration::Ptr obj) {

    traceEnter;
    QJsonDocument doc = QJsonDocument::fromJson(input);
    if (doc.isNull()) {
        traceErr() << "Errore nella chiamata fromJson";
        return JSON_PARSER_ERROR_DOCUMENT_PARSER;
    }

    QJsonObject jsonObj = doc.object();

    QJsonValue tileSizeMm = jsonObj.value(CONFIGURATION_JSON_TILE_SIZE_MM_KEY);
    if (tileSizeMm.isUndefined()) {
        traceErr() << "Chiave tileSizeMm non presente nel file json";
        return JSON_PARSER_ERROR_KEY_NOT_FOUND;
    }

    QJsonValue angleMRad = jsonObj.value(CONFIGURATION_JSON_ANGLE_MRAD_KEY);
    if (angleMRad.isUndefined()) {
        traceErr() << "Chiave angleMRad non presente nel file json";
        return JSON_PARSER_ERROR_KEY_NOT_FOUND;
    }

    QJsonValue offsetXmm = jsonObj.value(CONFIGURATION_JSON_OFFSET_X_MM_KEY);
    if (offsetXmm.isUndefined()) {
        traceErr() << "Chiave offsetXmm non presente nel file json";
        return JSON_PARSER_ERROR_KEY_NOT_FOUND;
    }

    QJsonValue offsetYmm = jsonObj.value(CONFIGURATION_JSON_OFFSET_Y_MM_KEY);
    if (offsetYmm.isUndefined()) {
        traceErr() << "Chiave offsetYmm non presente nel file json";
        return JSON_PARSER_ERROR_KEY_NOT_FOUND;
    }

    QJsonValue offsetZmm = jsonObj.value(CONFIGURATION_JSON_OFFSET_Z_MM_KEY);
    if (offsetZmm.isUndefined()) {
        traceErr() << "Chiave offsetZmm non presente nel file json";
        return JSON_PARSER_ERROR_KEY_NOT_FOUND;
    }

    QJsonValue tileScaleXPercent = jsonObj.value(CONFIGURATION_JSON_TILE_SCALE_X_PERCENT_KEY);
    if (tileScaleXPercent.isUndefined()) {
        traceErr() << "Chiave tileScaleXPercent non presente nel file json";
        return JSON_PARSER_ERROR_KEY_NOT_FOUND;
    }

    QJsonValue tileScaleYPercent = jsonObj.value(CONFIGURATION_JSON_TILE_SCALE_Y_PERCENT_KEY);
    if (tileScaleYPercent.isUndefined()) {
        traceErr() << "Chiave tileScaleYPercent non presente nel file json";
        return JSON_PARSER_ERROR_KEY_NOT_FOUND;
    }

    QJsonValue waitTimeMs = jsonObj.value(CONFIGURATION_JSON_WAIT_TIME_MS_KEY);
    if (waitTimeMs.isUndefined()) {
        traceErr() << "Chiave waitTimeMs non presente nel file json";
        return JSON_PARSER_ERROR_KEY_NOT_FOUND;
    }

    QJsonValue waitTimeAfterYMovementMs = jsonObj.value(CONFIGURATION_JSON_WAIT_TIME_AFTER_Y_MOVEMENT_MS_KEY);
    if (waitTimeAfterYMovementMs.isUndefined()) {
        traceErr() << "Chiave waitTimeAfterYMovementMs non presente nel file json";
        return JSON_PARSER_ERROR_KEY_NOT_FOUND;
    }

    QJsonValue laserFrequency = jsonObj.value(CONFIGURATION_JSON_LASER_FREQUENCY_KEY);
    if (laserFrequency.isUndefined()) {
        traceErr() << "Chiave laserFrequency non presente nel file json";
        return JSON_PARSER_ERROR_KEY_NOT_FOUND;
    }

    QJsonValue isRandomAlgorithm = jsonObj.value(CONFIGURATION_JSON_IS_RANDOM_ALGORITHM_KEY);
    if (isRandomAlgorithm.isUndefined()) {
        traceErr() << "Chiave isRandomAlgorithm non presente nel file json";
        return JSON_PARSER_ERROR_KEY_NOT_FOUND;
    }

    QJsonValue isNeighborhoodAlgorithm = jsonObj.value(CONFIGURATION_JSON_IS_NEIGHBORHOOD_ALGORITHM_KEY);
    if (isNeighborhoodAlgorithm.isUndefined()) {
        traceErr() << "Chiave isNeighborhoodAlgorithm non presente nel file json";
        return JSON_PARSER_ERROR_KEY_NOT_FOUND;
    }

    QJsonValue randomPointsPerTile = jsonObj.value(CONFIGURATION_JSON_RANDOM_POINTS_PER_TILE_KEY);
    if (randomPointsPerTile.isUndefined()) {
        traceErr() << "Chiave randomPointsPerTile non presente nel file json";
        return JSON_PARSER_ERROR_KEY_NOT_FOUND;
    }

    QJsonValue randomIsShuffleRowTiles = jsonObj.value(CONFIGURATION_JSON_RANDOM_IS_SHUFFLE_ROW_TILES_KEY);
    if (randomIsShuffleRowTiles.isUndefined()) {
        traceErr() << "Chiave randomIsShuffleRowTiles non presente nel file json";
        return JSON_PARSER_ERROR_KEY_NOT_FOUND;
    }

    QJsonValue neighborhoodMinDistanceUm = jsonObj.value(CONFIGURATION_JSON_NEIGHBORHOOD_MIN_DISTANCE_UM_KEY);
    if (neighborhoodMinDistanceUm.isUndefined()) {
        traceErr() << "Chiave neighborhoodMinDistanceUm non presente nel file json";
        return JSON_PARSER_ERROR_KEY_NOT_FOUND;
    }

    QJsonValue neighborhoodIsShuffleStackedTiles = jsonObj.value(CONFIGURATION_JSON_NEIGHBORHOOD_IS_SHUFFLE_STACKED_TILES_KEY);
    if (neighborhoodIsShuffleStackedTiles.isUndefined()) {
        traceErr() << "Chiave neighborhoodIsShuffleStackedTiles non presente nel file json";
        return JSON_PARSER_ERROR_KEY_NOT_FOUND;
    }

    QJsonValue neighborhoodIsShuffleRowTiles = jsonObj.value(CONFIGURATION_JSON_NEIGHBORHOOD_IS_SHUFFLE_ROW_TILES_KEY);
    if (neighborhoodIsShuffleRowTiles.isUndefined()) {
        traceErr() << "Chiave neighborhoodIsShuffleRowTiles non presente nel file json";
        return JSON_PARSER_ERROR_KEY_NOT_FOUND;
    }

    QJsonValue neighborhoodIsReverseY = jsonObj.value(CONFIGURATION_JSON_NEIGHBORHOOD_IS_REVERSE_Y_KEY);
    if (neighborhoodIsReverseY.isUndefined()) {
        traceErr() << "Chiave neighborhoodIsReverseY non presente nel file json";
        return JSON_PARSER_ERROR_KEY_NOT_FOUND;
    }

    QJsonValue pointShape = jsonObj.value(CONFIGURATION_JSON_POINT_SHAPE_KEY);
    if (pointShape.isUndefined()) {
        traceErr() << "Chiave pointShape non presente nel file json";
        return JSON_PARSER_ERROR_KEY_NOT_FOUND;
    }

    QJsonValue numberOfPulses = jsonObj.value(CONFIGURATION_JSON_NUMBER_OF_PULSES_KEY);
    if (numberOfPulses.isUndefined()) {
        traceErr() << "Chiave numberOfPulses non presente nel file json";
        return JSON_PARSER_ERROR_KEY_NOT_FOUND;
    }

    QJsonValue circlePointsRadiusUm = jsonObj.value(CONFIGURATION_JSON_CIRCLE_POINTS_RADIUS_UM_KEY);
    if (circlePointsRadiusUm.isUndefined()) {
        traceErr() << "Chiave circlePointsRadiusUm non presente nel file json";
        return JSON_PARSER_ERROR_KEY_NOT_FOUND;
    }

    QJsonValue circlePointsNumberOfSides = jsonObj.value(CONFIGURATION_JSON_CIRCLE_POINTS_NUMBER_OF_SIDES_KEY);
    if (circlePointsNumberOfSides.isUndefined()) {
        traceErr() << "Chiave circlePointsNumberOfSides non presente nel file json";
        return JSON_PARSER_ERROR_KEY_NOT_FOUND;
    }

    QJsonValue circlePointsNumberOfPulses = jsonObj.value(CONFIGURATION_JSON_CIRCLE_POINTS_NUMBER_OF_PULSES_KEY);
    if (circlePointsNumberOfPulses.isUndefined()) {
        traceErr() << "Chiave circlePointsNumberOfPulses non presente nel file json";
        return JSON_PARSER_ERROR_KEY_NOT_FOUND;
    }

    QJsonValue circleVectorRadiusUm = jsonObj.value(CONFIGURATION_JSON_CIRCLE_VECTOR_RADIUS_UM_KEY);
    if (circleVectorRadiusUm.isUndefined()) {
        traceErr() << "Chiave circleVectorRadiusUm non presente nel file json";
        return JSON_PARSER_ERROR_KEY_NOT_FOUND;
    }

    QJsonValue circleVectorNumberOfRevolutions = jsonObj.value(CONFIGURATION_JSON_CIRCLE_VECTOR_NUMBER_OF_REVOLUTIONS_KEY);
    if (circleVectorNumberOfRevolutions.isUndefined()) {
        traceErr() << "Chiave circleVectorNumberOfRevolutions non presente nel file json";
        return JSON_PARSER_ERROR_KEY_NOT_FOUND;
    }

    QJsonValue circleVectorNumberOfSides = jsonObj.value(CONFIGURATION_JSON_CIRCLE_VECTOR_NUMBER_OF_SIDES_KEY);
    if (circleVectorNumberOfSides.isUndefined()) {
        traceErr() << "Chiave circleVectorNumberOfSides non presente nel file json";
        return JSON_PARSER_ERROR_KEY_NOT_FOUND;
    }

    QJsonValue circleVectorPitch = jsonObj.value(CONFIGURATION_JSON_CIRCLE_VECTOR_PITCH_KEY);
    if (circleVectorPitch.isUndefined()) {
        traceErr() << "Chiave circleVectorPitch non presente nel file json";
        return JSON_PARSER_ERROR_KEY_NOT_FOUND;
    }

    obj->setTileSizeMm(tileSizeMm.toInt());
    obj->setAngleMRad(angleMRad.toDouble());
    obj->setOffsetXmm(offsetXmm.toDouble());
    obj->setOffsetYmm(offsetYmm.toDouble());
    obj->setOffsetZmm(offsetZmm.toDouble());
    obj->setTileScaleXPercent(tileScaleXPercent.toDouble());
    obj->setTileScaleYPercent(tileScaleYPercent.toDouble());
    obj->setWaitTimeMs(waitTimeMs.toInt());
    obj->setWaitTimeAfterYMovementMs(waitTimeAfterYMovementMs.toInt());
    obj->setLaserFrequency(laserFrequency.toInt());

    obj->setIsRandomAlgorithm(isRandomAlgorithm.toBool());
    obj->setIsNeighborhoodAlgorithm(isNeighborhoodAlgorithm.toBool());

    obj->setRandomPointsPerTile(randomPointsPerTile.toInt());
    obj->setRandomIsShuffleRowTiles(randomIsShuffleRowTiles.toBool());

    obj->setNeighborhoodMinDistanceUm(neighborhoodMinDistanceUm.toInt());
    obj->setNeighborhoodIsShuffleStackedTiles(neighborhoodIsShuffleStackedTiles.toBool());
    obj->setNeighborhoodIsShuffleRowTiles(neighborhoodIsShuffleRowTiles.toBool());
    obj->setNeighborhoodIsReverseY(neighborhoodIsReverseY.toBool());

    obj->setPointShape(getPointShapeEnumFromString(pointShape.toString()));

    obj->setNumberOfPulses(numberOfPulses.toInt());

    obj->setCirclePointsRadiusUm(circlePointsRadiusUm.toInt());
    obj->setCirclePointsNumberOfSides(circlePointsNumberOfSides.toInt());
    obj->setCirclePointsNumberOfPulses(circlePointsNumberOfPulses.toInt());

    obj->setCircleVectorRadiusUm(circleVectorRadiusUm.toInt());
    obj->setCircleVectorNumberOfRevolutions(circleVectorNumberOfRevolutions.toInt());
    obj->setCircleVectorNumberOfSides(circleVectorNumberOfSides.toInt());
    obj->setCircleVectorPitch(circleVectorPitch.toInt());

    traceExit;
    return JSON_PARSER_NO_ERROR;

}
