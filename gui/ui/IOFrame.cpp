#include "IOFrame.hpp"
#include "IOFrameLogic.hpp"
#include "ui_IOFrame.h"

#include <Settings.hpp>
#include <gui/custom-widgets/MDSwitchLine.hpp>
#include <gui/custom-widgets/MDLineEdit.hpp>

#include <Logger.hpp>

using namespace PROGRAM_NAMESPACE;

/*******************************************
 *       I O   F R A M E   L O G I C
 *******************************************/

IOFrameLogic::IOFrameLogic(QObject* parent) : QObject (parent) {

    traceEnter;
    traceExit;

}

IOFrameLogic::~IOFrameLogic() {

    traceEnter;
    traceExit;

}

/*******************************************
 *            I O   F R A M E
 *******************************************/

int IOFrame::getRowMinHeight() const { return rowMinHeight; }
void IOFrame::setRowMinHeight(int value) { rowMinHeight = value; }

IOFrame::IOFrame(QWidget *parent) :
    QFrame(parent), ui(new Ui::IOFrame), dPtr(new IOFrameLogic(parent)) {

    traceEnter;

    this->setupUi();
    dPtr->qPtr = this;

    traceExit;

}

IOFrame::~IOFrame() {

    traceEnter;
    delete ui;
    traceExit;

}

void IOFrame::setupUi() {

    traceEnter;

    ui->setupUi(this);

    Settings& s = Settings::instance();

    // tab IO digitali
    QWidget* tabDigitalIO = ui->tabDigitalIO;
    QGridLayout* glDigitalIO = ui->glDigitalIOContents;

    const int colDigitalInput = 0;
    const int colDigitalOutput = 1;

    // ingressi digitali
    auto spacerDI = ui->vsDigitalInput;
    int startRow = 0;
    glDigitalIO->removeItem(spacerDI);

    for (const DigitalInput& digitalInput : s.getDigitalInputs()) {

        QCheckBox* switchLine = new QCheckBox(tabDigitalIO);
        switchLine->setProperty("isDigitalIO", true);
        switchLine->setText(digitalInput.getName());
        glDigitalIO->addWidget(switchLine, ++startRow, colDigitalInput);

    }

    glDigitalIO->addItem(spacerDI, ++startRow, colDigitalInput);

    // uscite digitali
    auto spacerDo = ui->vsDigitalOutput;
    startRow = 0;
    glDigitalIO->removeItem(spacerDo);

    for (const DigitalOutput& digitalOutput : s.getDigitalOutputs()) {

        MDSwitchLine* switchLine = new MDSwitchLine(tabDigitalIO);
        switchLine->setText(digitalOutput.getName());
        glDigitalIO->addWidget(switchLine, ++startRow, colDigitalOutput);

    }

    glDigitalIO->addItem(spacerDo, ++startRow, colDigitalOutput);


    // tab IO analogici
    QWidget* tabAnalogIO = ui->tabAnalogIO;
    QGridLayout* glAnalogIO = ui->glAnalogIOContents;

    // ingressi analogici

    const int colAnalogInput = 0;

    auto spacerAi = ui->vsAnalogInput;
    startRow = 0;
    glAnalogIO->removeItem(spacerAi);

    for (const AnalogInput& analogInput : s.getAnalogInputs()) {

        MDLineEdit* switchLine = new MDLineEdit(tabAnalogIO);
        switchLine->setLabel(analogInput.getName());
        glAnalogIO->addWidget(switchLine, ++startRow, colAnalogInput);
    }

    glAnalogIO->addItem(spacerAi, ++startRow, colAnalogInput);

    traceExit;

}

void IOFrame::setupSignalsAndSlots() {

    traceEnter;

    traceExit;

}

void IOFrame::paintEvent(QPaintEvent* event) {

    QFrame::paintEvent(event);

    QGridLayout* glDigitalIO = ui->glDigitalIOContents;
    for (int i=0; i<glDigitalIO->rowCount(); ++i)
        glDigitalIO->setRowMinimumHeight(i, rowMinHeight);

    QGridLayout* glAnalogIO = ui->glAnalogIOContents;
    for (int i=0; i<glAnalogIO->rowCount(); ++i)
        glAnalogIO->setRowMinimumHeight(i, rowMinHeight);


}
