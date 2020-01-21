#ifndef TESTFRAMELOGIC_HPP
#define TESTFRAMELOGIC_HPP

#include <QObject>

#include <NamedThread.hpp>
#include <MotionManager.hpp>
#include <IOManager.hpp>
#include <MotionManager.hpp>
#include <MotionBean.hpp>
#include <MachineStatusHandler.hpp>
#include <Error.hpp>
#include <core/image-processor/AbstractProcessor.hpp>
#include <laser-ipg-temporary/communication/ipg_sync_interface.hpp>
#include <json/AbstractJsonParser.hpp>
#include <third-party/ipg-marking-library-wrapper/include/Scanner.h>


class TestFrame;

enum class PointShapeEnum;
class PrintConfiguration;
class PrintStatus;
class MachineStatusReceiver;

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

enum class ExitCause : int {
    NOT_VALID = -1,
    PROCESS_COMPLETED_CORRECTLY,
    EXIT_BY_TIMEOUT,
    STOP_BY_USER,
    PAUSE_BY_USER,
    ERROR_CATCHED,
    INTERNAL_ERROR
};



/*
 * P R I N T S T A T U S
 */

/**
 * @brief The PrintStatus class
 * Questa classe indica lo stato dell'ultima stampa;
 * le informazioni contenute sono:
 * lastTileX: indica l'indice della riga in stampa;
 * tileYToPrint: lista di indici dei tile da stampare;
 * nota: se la lista e' vuota e l'indice di riga e' valorizzato,
 * significa che bisogna partire la stampa dalla riga successiva
 */
class PrintStatus {
public:
    using Ptr = PrintStatus*;
    using ConstPtr = const PrintStatus*;

private:
    int rowIndex;
    QList<int> tileYToPrint;

public:
    PrintStatus() : tileYToPrint() {
        rowIndex = -1;
    }

    ~PrintStatus() { }

    int getRowIndex() const { return rowIndex; }
    void setRowIndex(int value) { rowIndex = value; }

    QList<int> getTileYToPrint() const { return tileYToPrint; }
    void setTileYToPrint(const QList<int>& value)  { tileYToPrint = value; }
    void removeTileY(int indexY) { tileYToPrint.removeOne(indexY); }
    void reloadTileY(int maxNumTile) {
        for (int i=0; i<maxNumTile; ++i)
            tileYToPrint.append(i);
    }

    bool isTileYToPrint(int yTileIndex) { return tileYToPrint.contains(yTileIndex); }
    bool isValid() const { return rowIndex > -1; }

};



/*
 * P R I N T C O N F I G U R A T I O N
 */

class PrintConfiguration {
public:
    using Ptr = PrintConfiguration*;
    using ConstPtr = const PrintConfiguration*;

private:
    // percorso file
    QString filePath;

    // setup movimentazione
    int tileSizeMm;
    double angleMRad;
    double offsetXmm;
    double offsetYmm;
    double offsetZmm;
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
    bool neighborhoodIsShuffleRowTiles;
    bool neighborhoodIsReverseY;
    bool neighborhoodIsShuffleStackedTiles;

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
    double getOffsetZmm() const { return offsetZmm; }
    void setOffsetZmm(double value) { offsetZmm = value; }
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
    bool getNeighborhoodIsReverseY() const { return neighborhoodIsReverseY; }
    void setNeighborhoodIsReverseY(bool value) { neighborhoodIsReverseY = value; }

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



/*
 * C O N F I G U R A T I O N J S O N P A R S E R
 */

class ConfigurationJsonParser : public PROGRAM_NAMESPACE::AbstractJsonParser<PrintConfiguration> {
public:
    using Ptr = ConfigurationJsonParser*;
    using ConstPtr = const  ConfigurationJsonParser*;

private:
    static constexpr char* CONFIGURATION_JSON_FILE_PATH_KEY = "filePath";

    static constexpr char* CONFIGURATION_JSON_TILE_SIZE_MM_KEY = "tileSizeMm";
    static constexpr char* CONFIGURATION_JSON_ANGLE_MRAD_KEY = "angleMRad";
    static constexpr char* CONFIGURATION_JSON_OFFSET_X_MM_KEY = "offsetXmm";
    static constexpr char* CONFIGURATION_JSON_OFFSET_Y_MM_KEY = "offsetYmm";
    static constexpr char* CONFIGURATION_JSON_OFFSET_Z_MM_KEY = "offsetZmm";
    static constexpr char* CONFIGURATION_JSON_TILE_SCALE_X_PERCENT_KEY = "tileScaleXPercent";
    static constexpr char* CONFIGURATION_JSON_TILE_SCALE_Y_PERCENT_KEY = "tileScaleYPercent";
    static constexpr char* CONFIGURATION_JSON_WAIT_TIME_MS_KEY = "waitTimeMs";
    static constexpr char* CONFIGURATION_JSON_WAIT_TIME_AFTER_Y_MOVEMENT_MS_KEY = "waitTimeAfterYMovementMs";
    static constexpr char* CONFIGURATION_JSON_LASER_FREQUENCY_KEY = "laserFrequency";

    static constexpr char* CONFIGURATION_JSON_IS_RANDOM_ALGORITHM_KEY = "isRandomAlgorithm";
    static constexpr char* CONFIGURATION_JSON_IS_NEIGHBORHOOD_ALGORITHM_KEY = "isNeighborhoodAlgorithm";

    static constexpr char* CONFIGURATION_JSON_RANDOM_POINTS_PER_TILE_KEY = "randomPointsPerTile";
    static constexpr char* CONFIGURATION_JSON_RANDOM_IS_SHUFFLE_ROW_TILES_KEY = "randomIsShuffleRowTiles";

    static constexpr char* CONFIGURATION_JSON_NEIGHBORHOOD_MIN_DISTANCE_UM_KEY = "neighborhoodMinDistanceUm";
    static constexpr char* CONFIGURATION_JSON_NEIGHBORHOOD_IS_SHUFFLE_ROW_TILES_KEY = "neighborhoodIsShuffleRowTiles";
    static constexpr char* CONFIGURATION_JSON_NEIGHBORHOOD_IS_REVERSE_Y_KEY = "neighborhoodIsReverseY";
    static constexpr char* CONFIGURATION_JSON_NEIGHBORHOOD_IS_SHUFFLE_STACKED_TILES_KEY = "neighborhoodIsShuffleStackedTiles";

    static constexpr char* CONFIGURATION_JSON_POINT_SHAPE_KEY = "pointShape";

    static constexpr char* CONFIGURATION_JSON_NUMBER_OF_PULSES_KEY = "numberOfPulses";

    static constexpr char* CONFIGURATION_JSON_CIRCLE_POINTS_RADIUS_UM_KEY = "circlePointsRadiusUm";
    static constexpr char* CONFIGURATION_JSON_CIRCLE_POINTS_NUMBER_OF_SIDES_KEY = "circlePointsNumberOfSides";
    static constexpr char* CONFIGURATION_JSON_CIRCLE_POINTS_NUMBER_OF_PULSES_KEY = "circlePointsNumberOfPulses";

    static constexpr char* CONFIGURATION_JSON_CIRCLE_VECTOR_RADIUS_UM_KEY = "circleVectorRadiusUm";
    static constexpr char* CONFIGURATION_JSON_CIRCLE_VECTOR_NUMBER_OF_REVOLUTIONS_KEY = "circleVectorNumberOfRevolutions";
    static constexpr char* CONFIGURATION_JSON_CIRCLE_VECTOR_NUMBER_OF_SIDES_KEY = "circleVectorNumberOfSides";
    static constexpr char* CONFIGURATION_JSON_CIRCLE_VECTOR_PITCH_KEY = "circleVectorPitch";

public:
    ConfigurationJsonParser() : AbstractJsonParser() { }

protected:
    mibdv::JsonParserError encodeJson(PrintConfiguration::ConstPtr obj, QByteArray& output);

    mibdv::JsonParserError decodeJson(const QByteArray& input, PrintConfiguration::Ptr obj);

};



/*
 * W O R K E R
 */

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
    QSharedPointer<PROGRAM_NAMESPACE::MachineStatusNotifier> machineStatusNotifier;
    QScopedPointer<PROGRAM_NAMESPACE::ProcessorThread> fileProcessorThread;
    QScopedPointer<ipg_marking_library_wrapper::Scanner> scanner;
    PrintConfiguration printConfiguration;
    PrintStatus printStatus;

    bool hasToStop;
    bool hasToPause;
    bool hasErrors;
    QList<PROGRAM_NAMESPACE::Error> errorList;

    PrintCommandExecuted commandsExecuted;

public:
    explicit Worker(QObject* parent = nullptr);

    const PrintConfiguration& getPrintConfiguration() const { return printConfiguration; }
    void setPrintConfiguration(const PrintConfiguration& value) { printConfiguration = value; }

    PrintStatus getPrintStatus() const { return printStatus; }
    void setPrintStatus(const PrintStatus& value) { printStatus = value; }

private:
    void setupSignalsAndSlots();
    inline void updateLastCommandExecute(PrintCommandExecuted c) { this->commandsExecuted = c; }

public slots:
    void startProcess();

private:
    bool beforeProcess();
    ExitCause process();
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
    void finishedSignal();
    void stopRequestSignal();
    void pauseRequestSignal();
    void enablePauseSignal(bool value);

    /* NOTE NIC 25/07/2019 - gestione segnali errori
     * sono obbligato ad usare altri segnali perche' nel processo di stampa, non e' detto che sia
     * sempre in attesa di eventi esterni (quando chiamo processEvents()); allora, per evitare una perdita di un errore,
     * imposto la variabile di istanza hasErrors attraverso uno slot fatto ad-hoc (vedere metodo setupSignalsAndSlots)
     */
    void hasErrorsSignal(QList<PROGRAM_NAMESPACE::Error> errors);
    void hasFatalsSignal(QList<PROGRAM_NAMESPACE::Error> fatals);
    void stopProcessByErrorSignal(); // uso private alla classe Worker

};



/*
 * T E S T F R A M E L O G I C
 */

class TestFrameLogic : public QObject {
    Q_OBJECT

    friend class TestFrame;
    friend class Worker;

public:
    using Ptr = TestFrameLogic *;
    using ConstPtr = const TestFrameLogic *;

private:
    TestFrame* qPtr;
    QScopedPointer<PROGRAM_NAMESPACE::MachineStatusReceiver> machineStatusReceiver;
    PrintStatus printStatus;

    PROGRAM_NAMESPACE::MotionBean motionBean;
    bool laserParametersChanged;
    bool hasErrors;
    PROGRAM_NAMESPACE::MachineStatus machineStatus;
    PrintConfiguration currentConfiguration;

    bool isProcessStopped;
    bool isLaserInitialized;

public:
    explicit TestFrameLogic(QObject* parent = Q_NULLPTR);
    ~TestFrameLogic();

private slots:
    void startWork();
    void stopWork();
    void resumeWork();
    void pauseWork();

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

    void stopRequestSignal();
    void pauseRequestSignal();
    void resumeRequestSignal();
    void enablePauseSignal(bool value);

};

#endif // TESTFRAMELOGIC_HPP
