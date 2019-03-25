#ifndef DIALOGRENAME_HPP
#define DIALOGRENAME_HPP

#include <QDialog>


namespace Ui {
class DialogRename;
}

class DialogRename : public QDialog {
    Q_OBJECT

public:
    explicit DialogRename(QWidget *parent = nullptr);
    ~DialogRename();

    void setupUi();
    void setupFilename(const QString& filename);
    void setupSignalsAndSlots();
    QString getFilename() const;

private:
    Ui::DialogRename *ui;
};

#endif // DIALOGRENAME_HPP
