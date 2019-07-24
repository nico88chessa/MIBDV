#ifndef TESTFRAME_HPP
#define TESTFRAME_HPP

#include <QButtonGroup>
#include <QFrame>
#include <QTabWidget>

#include <IOManager.hpp>
#include <MotionManager.hpp>
#include <MotionBean.hpp>
#include <DigitalInputValue.hpp>
#include <laser-ipg-temporary/communication/ipg_sync_interface.hpp>
#include <json/AbstractJsonParser.hpp>


class TestFrameLogic;

enum class PointShapeEnum : int {
    UNDEFINED = -1,
    POINT,
    CIRCLE_POINTS,
    CIRCLE_VECTOR
};

static inline QString getStringFromPointShapeEnum(PointShapeEnum pointShape) {

    switch (pointShape) {
        case PointShapeEnum::UNDEFINED: return "UNDEFINED"; break;
        case PointShapeEnum::POINT: return "POINT"; break;
        case PointShapeEnum::CIRCLE_POINTS: return "CIRCLE_POINTS"; break;
        case PointShapeEnum::CIRCLE_VECTOR: return "CIRCLE_VECTOR"; break;
    }

    return "UNDEFINED";

}

static inline PointShapeEnum getPointShapeEnumFromString(const QString& str) {

    if (str.compare("UNDEFINED", Qt::CaseInsensitive)==0)
        return PointShapeEnum::UNDEFINED;
    if (str.compare("POINT", Qt::CaseInsensitive)==0)
        return PointShapeEnum::POINT;
    if (str.compare("CIRCLE_POINTS", Qt::CaseInsensitive)==0)
        return PointShapeEnum::CIRCLE_POINTS;
    if (str.compare("CIRCLE_VECTOR", Qt::CaseInsensitive)==0)
        return PointShapeEnum::CIRCLE_VECTOR;

    return PointShapeEnum::UNDEFINED;

}

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
    static constexpr char* CONFIGURATION_JSON_NEIGHBORHOOD_IS_SHUFFLE_STACKED_TILES_KEY = "neighborhoodIsShuffleStackedTiles";
    static constexpr char* CONFIGURATION_JSON_NEIGHBORHOOD_IS_SHUFFLE_ROW_TILES_KEY = "neighborhoodIsShuffleRowTiles";

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

namespace Ui {
class TestFrame;
}

class TestFrame : public QFrame {
    Q_OBJECT

    friend class TestFrameLogic;

public:
    using Ptr = TestFrame *;
    using ConstPtr = const TestFrame *;

public:
    static constexpr int TEST_FRAME_PULSE_ENERGY_DFLT = 1;

private:
    static constexpr int TEST_FRAME_POINTS_PER_TILE_MIN = 1;
    static constexpr int TEST_FRAME_POINTS_PER_TILE_MAX = 10000;
    static constexpr int TEST_FRAME_TILE_SIZE_MIN = 1;
    static constexpr int TEST_FRAME_TILE_SIZE_MAX = 200;
    static constexpr int TEST_FRAME_MIN_FREQUENCY = 0;
    static constexpr int TEST_FRAME_MAX_FREQUENCY = 3000;
    static constexpr int TEST_FRAME_WAIT_TIME_MS_MIN = 50;
    static constexpr int TEST_FRAME_WAIT_TIME_MS_MAX = 10000;
    static constexpr int TEST_FRAME_Y_MOVEMENTS_WAIT_TIME_MS_MIN = 1000;
    static constexpr int TEST_FRAME_Y_MOVEMENTS_WAIT_TIME_MS_MAX = 10000;
    static constexpr int TEST_FRAME_OFFSET_X_MIN = 0;
    static constexpr int TEST_FRAME_OFFSET_X_MAX = 1200;
    static constexpr int TEST_FRAME_OFFSET_Y_MIN = 0;
    static constexpr int TEST_FRAME_OFFSET_Y_MAX = 1200;
    static constexpr double TEST_FRAME_SCALE_X_DFLT = 100;
    static constexpr double TEST_FRAME_SCALE_X_MIN = 80;
    static constexpr double TEST_FRAME_SCALE_X_MAX = 120;
    static constexpr double TEST_FRAME_SCALE_Y_DFLT = 100;
    static constexpr double TEST_FRAME_SCALE_Y_MIN = 80;
    static constexpr double TEST_FRAME_SCALE_Y_MAX = 120;
    static constexpr int TEST_FRAME_POINTS_DISTANCE_UM_MIN = 100;
    static constexpr int TEST_FRAME_POINTS_DISTANCE_UM_MAX = 100000;
    static constexpr int TEST_FRAME_ANGLE_MRAD_MIN = -200;
    static constexpr int TEST_FRAME_ANGLE_MRAD_MAX = 200;
    static constexpr double TEST_FRAME_DSB_STEP = 0.1;
    static constexpr double TEST_FRAME_DSB_SCALE_STEP = 0.01;
    static constexpr double TEST_FRAME_ANGLE_STEP = 0.001;

    static constexpr int TEST_FRAME_PULSES_MIN = 1;
    static constexpr int TEST_FRAME_PULSES_MAX = 20000;
    static constexpr int TEST_FRAME_CIRCLE_RADIUS_MIN = 50;
    static constexpr int TEST_FRAME_CIRCLE_RADIUS_MAX = 100000; // TODO NIC 13/05/2019 - rimettere 2000
    static constexpr int TEST_FRAME_CIRCLE_RADIUS_STEP = 1;
    static constexpr int TEST_FRAME_CIRCLE_NUM_REVOLUTIONS_MIN = 1;
    static constexpr int TEST_FRAME_CIRCLE_NUM_REVOLUTIONS_MAX = 2000;
    static constexpr int TEST_FRAME_CIRCLE_NUM_REVOLUTIONS_STEP = 1;
    static constexpr int TEST_FRAME_CIRCLE_NUM_SIDES_MIN = 3;
    static constexpr int TEST_FRAME_CIRCLE_NUM_SIDES_MAX = 20; // TODO NIC 13/05/2019 - rimettere 10
    static constexpr int TEST_FRAME_CIRCLE_NUM_SIDES_STEP = 1;
    static constexpr int TEST_FRAME_CIRCLE_POINTS_PITCH_MIN = 1;
    static constexpr int TEST_FRAME_CIRCLE_POINTS_PITCH_MAX = TEST_FRAME_CIRCLE_RADIUS_MAX/20; // parametro da verificare
    static constexpr int TEST_FRAME_CIRCLE_POINTS_PITCH_STEP = 1;


    static constexpr int TEST_FRAME_LASER_MIN_POWER = 0;
    static constexpr int TEST_FRAME_LASER_MAX_POWER = 100;
    static constexpr int TEST_FRAME_LASER_POWER_STEP = 1;

    static constexpr int TEST_FRAME_LASER_TAB_INDEX = 2;
    static constexpr int TEST_FRAME_SAVE_LOAD_TAB_INDEX = 3;


    Ui::TestFrame *ui;
    QButtonGroup* pointShapeGroup;
    TestFrameLogic* dPtr;
    PROGRAM_NAMESPACE::MotionBean motionBean;
    PROGRAM_NAMESPACE::DigitalInputStatus digitalInputStatus;
    bool laserParametersChanged;
    PrintConfiguration currentConfiguration;

public:
    explicit TestFrame(QWidget *parent = nullptr);
    ~TestFrame();

public slots:
    void updateMotionBean(const PROGRAM_NAMESPACE::MotionBean& b);
    void updateDigitalInputStatus(const PROGRAM_NAMESPACE::DigitalInputStatus& i);
    void setFilePath(const QString& filePath);
    void laserIpgYLPNConfigurationReady();
    void showPopup(const QString& err, const QString& descr);

private slots:
    void updatePrintConfiguration();
    void restorePrintConfiguration();
    void saveConfiguration();
    void loadConfiguration();

private:
    void setupUi();
    void setupSignalsAndSlots();
    void updateTabLaserLabel(bool setAsterisk);

    bool eventFilter(QObject *object, QEvent *event);

};

#endif // TESTFRAME_HPP
