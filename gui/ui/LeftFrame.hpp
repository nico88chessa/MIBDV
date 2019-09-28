#ifndef LEFTFRAME_HPP
#define LEFTFRAME_HPP

#include <QFrame>


namespace Ui {
class LeftFrame;
}


class LeftFrameLogic;

enum class PageItem : int {
    INVALID_PAGE = -1,
    ALERT_PAGE,
    IO_PAGE,
    MOTION_PAGE,
    FILE_EXPLORER_PAGE
};

class LeftFrame : public QFrame {
    Q_OBJECT

    friend class LeftFrameLogic;

private:
    Ui::LeftFrame *ui;
    LeftFrameLogic* dPtr;

public:
    explicit LeftFrame(QWidget *parent = nullptr);
    ~LeftFrame();

private:
    void setupUi();
    void setupSignalsAndSlots();

public slots:
    void setPageItem(PageItem i);

signals:
    void pageItemChanged(PageItem i);

};

Q_DECLARE_METATYPE(PageItem)

#endif // LEFTFRAME_HPP
