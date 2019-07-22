#ifndef TESTFRAMELOGIC_HPP
#define TESTFRAMELOGIC_HPP

#include <QObject>

#include <NamedThread.hpp>
#include <MotionManager.hpp>
#include <IOManager.hpp>
#include <MotionManager.hpp>
#include <core/image-processor/AbstractProcessor.hpp>
#include <laser-ipg-temporary/communication/ipg_sync_interface.hpp>
#include <third-party/ipg-marking-library-wrapper/include/Scanner.h>


class TestFrame;

enum class PointShapeEnum;

enum class PrintCommandExecuted : int {
    IDLE,
    START,
    CYCLE,
    PROCESSOR_THREAD_RUN,
    IO_ACTIVATED,
    LASER_ON,
    SCANNER_HEAD_SETUP,
    PRINT_LOOP
};

class PrintConfiguration {
private:
    // percorso file
    QString filePath;

    // setup movimentazione
    int tileSizeMm;
    double angleMRad;
    double offsetXmm;
    double offsetYmm;
    float tileScaleXPercent;
    float tileScaleYPercent;
    int waitTimeMs;
    int waitTimeAfterYMovementMs;
    int laserFrequency;

    // scelta algoritmo
    bool isRandomAlgorithm;
    bool isNeighborhoodAlgorithm;

    // configurazione random
    int randomPointsPerTile;
    bool randomIsShuffleRowTiles;

    // configurazione neighborhood
    int neighborhoodMinDistanceUm;
    bool neighborhoodIsShuffleStackedTiles;
    bool neighborhoodIsShuffleRowTiles;

    // scelta del punto di stampa
    PointShapeEnum pointShape;

    // point
    int numberOfPulses;

    // circle points
    int circlePointsRadiusUm;
    int circlePointsNumberOfSides;
    int circlePointsNumberOfPulses;

    // circle vector
    int circleVectorRadiusUm;
    int circleVectorNumberOfRevolutions;
    int circleVectorNumberOfSides;
    int circleVectorPitch;

public:
    QString getFilePath() const { return filePath; }
    void setFilePath(const QString& value) { filePath = value; }

    int getTileSizeMm() const { return tileSizeMm; }
    void setTileSizeMm(int value) { tileSizeMm = value; }
    double getAngleMRad() const { return angleMRad; }
    void setAngleMRad(double value) { angleMRad = value; }
    double getOffsetXmm() const { return offsetXmm; }
    void setOffsetXmm(double value) { offsetXmm = value; }
    double getOffsetYmm() const { return offsetYmm; }
    void setOffsetYmm(double value) { offsetYmm = value; }
    float getTileScaleXPercent() const { return tileScaleXPercent; }
    void setTileScaleXPercent(float value) { tileScaleXPercent = value; }
    float getTileScaleYPercent() const { return tileScaleYPercent; }
    void setTileScaleYPercent(float value) { tileScaleYPercent = value; }
    int getWaitTimeMs() const { return waitTimeMs; }
    void setWaitTimeMs(int value) { waitTimeMs = value; }
    int getWaitTimeAfterYMovementMs() const { return waitTimeAfterYMovementMs; }
    void setWaitTimeAfterYMovementMs(int value) { waitTimeAfterYMovementMs = value; }
    int getLaserFrequency() const { return laserFrequency; }
    void setLaserFrequency(int value) { laserFrequency = value; }

    bool getIsRandomAlgorithm() const { return isRandomAlgorithm; }
    void setIsRandomAlgorithm(bool value) { isRandomAlgorithm = value; }
    bool getIsNeighborhoodAlgorithm() const { return isNeighborhoodAlgorithm; }
    void setIsNeighborhoodAlgorithm(bool value) { isNeighborhoodAlgorithm = value; }

    int getRandomPointsPerTile() const { return randomPointsPerTile; }
    void setRandomPointsPerTile(int value) { randomPointsPerTile = value; }
    bool getRandomIsShuffleRowTiles() const { return randomIsShuffleRowTiles; }
    void setRandomIsShuffleRowTiles(bool value) { randomIsShuffleRowTiles = value; }

    int getNeighborhoodMinDistanceUm() const { return neighborhoodMinDistanceUm; }
    void setNeighborhoodMinDistanceUm(int value) { neighborhoodMinDistanceUm = value; }
    bool getNeighborhoodIsShuffleStackedTiles() const { return neighborhoodIsShuffleStackedTiles; }
    void setNeighborhoodIsShuffleStackedTiles(bool value) { neighborhoodIsShuffleStackedTiles = value; }
    bool getNeighborhoodIsShuffleRowTiles() const { return neighborhoodIsShuffleRowTiles; }
    void setNeighborhoodIsShuffleRowTiles(bool value) { neighborhoodIsShuffleRowTiles = value; }

    PointShapeEnum getPointShape() const { return pointShape; }
    void setPointShape(const PointShapeEnum& value) { pointShape = value; }

    int getNumberOfPulses() const { return numberOfPulses; }
    void setNumberOfPulses(int value) { numberOfPulses = value; }

    int getCirclePointsRadiusUm() const { return circlePointsRadiusUm; }
    void setCirclePointsRadiusUm(int value) { circlePointsRadiusUm = value; }
    int getCirclePointsNumberOfSides() const { return circlePointsNumberOfSides; }
    void setCirclePointsNumberOfSides(int value) { circlePointsNumberOfSides = value; }
    int getCirclePointsNumberOfPulses() const { return circlePointsNumberOfPulses; }
    void setCirclePointsNumberOfPulses(int value) { circlePointsNumberOfPulses = value; }

    int getCircleVectorRadiusUm() const { return circleVectorRadiusUm; }
    void setCircleVectorRadiusUm(int value) { circleVectorRadiusUm = value; }
    int getCircleVectorNumberOfRevolutions() const { return circleVectorNumberOfRevolutions; }
    void setCircleVectorNumberOfRevolutions(int value) { circleVectorNumberOfRevolutions = value; }
    int getCircleVectorNumberOfSides() const { return circleVectorNumberOfSides; }
    void setCircleVectorNumberOfSides(int value) { circleVectorNumberOfSides = value; }
    int getCircleVectorPitch() const { return circleVectorPitch; }
    void setCircleVectorPitch(int value) { circleVectorPitch = value; }

};

class Worker : public QObject {
    Q_OBJECT

public:
    using Ptr = Worker*;
    using ConstPtr = const Worker*;

    static constexpr unsigned int WAIT_THREAD_STARTED_TIME_MS = 10000;
    static constexpr unsigned int MOTION_CHECK_TIME_MS = 200;
    static constexpr unsigned int COUNTDOWN_INTERVAL_MS = 1000;
    static constexpr unsigned int MAX_COUNTDOWN_TICKS = 10;

    // questo parametro serve per far si che lo stato della testa vada a BUSY;
    // infatti se faccio un close() poco dopo aver fatto un open(), il close viene ignorato
    static constexpr int TEST_FRAME_SCANNER_WAIT_TIME_AFTER_CONNECTION_MS = 2000;

    friend class TestFrameLogic;

private:
    TestFrameLogic* tfl;
    QSharedPointer<PROGRAM_NAMESPACE::IOManager> ioManager;
    QSharedPointer<PROGRAM_NAMESPACE::MotionManager> motionManager;
    QScopedPointer<PROGRAM_NAMESPACE::ProcessorThread> fileProcessorThread;
    QScopedPointer<ipg_marking_library_wrapper::Scanner> scanner;
    PrintConfiguration printConfiguration;
    bool hasToStop;
    PrintCommandExecuted commandsExecuted;

public:
    explicit Worker(QObject* parent = nullptr);

    const PrintConfiguration& getPrintConfiguration() const { return printConfiguration; }
    void setPrintConfiguration(const PrintConfiguration& value) { printConfiguration = value; }

private:
    void setupSignalsAndSlot();
    inline void updateLastCommandExecute(PrintCommandExecuted c) { this->commandsExecuted = c; }

public slots:
    void startJob();

private:
    bool beforeProcess();
    bool process();
    bool afterProcess();
    inline void showDialogAsync(const QString& err, const QString& descr);
    inline void updateStatusAsync(const QString& status);
    inline void updateTileTimeAsync(unsigned int tileTimeMs);
    inline void updateStackedTimeAsync(unsigned int stackedTimeMs);
    inline void updateEstimatedResidualTimeAsync(unsigned int stackedTimeMs);

    bool setupLaserOn();
    bool setupLaserOff();
    bool getPulseEnergy(float& energyJoule);

signals:
    void finished();
    void stopRequest();

};

class TestFrameLogic : public QObject {
    Q_OBJECT

    friend class TestFrame;
    friend class Worker;

public:
    using Ptr = TestFrameLogic *;
    using ConstPtr = const TestFrameLogic *;

private:
    TestFrame* qPtr;
    NamedThread* workerThread;

    bool isProcessStopped;
    bool isLaserInitialized;

public:
    TestFrameLogic();
    ~TestFrameLogic();

private slots:
    void startProcess();
    void stopProcess();

    void changeGuideLaserState();
    void updateStatus(const QString& status);
    void updateTileTime(unsigned int tileTimeMs);
    void updateStackedTime(unsigned int stackedTimeMs);
    void updateEstimatedResidualTime(unsigned int stackedTimeMs);

    // WARNING NIC 03/04/2019 - inizio gestione temporanea laser YLPN (da rifare)
    void initIpgYLPNLaser();

signals:
    // WARNING NIC 03/04/2019 - inizio gestione temporanea laser YLPN (da rifare)
    void laserIpgYLPNinitializedSignal();
    void stopRequest();

};

#endif // TESTFRAMELOGIC_HPP
