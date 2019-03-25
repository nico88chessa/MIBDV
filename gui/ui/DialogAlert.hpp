#ifndef DIALOGALERT_HPP
#define DIALOGALERT_HPP

#include <QDialog>


namespace Ui {
class DialogAlert;
}

class DialogAlert : public QDialog {
    Q_OBJECT

public:
    explicit DialogAlert(QWidget *parent = nullptr);
    ~DialogAlert();

    void setupUi();
    void setupLabels(const QString& title, const QString& content);
    void setupSignalsAndSlots();

private:
    Ui::DialogAlert *ui;
};

#endif // DIALOGALERT_HPP
