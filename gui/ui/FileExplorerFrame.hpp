#ifndef FILEEXPLORERFRAME_HPP
#define FILEEXPLORERFRAME_HPP

#include <QFrame>

namespace Ui {
class FileExplorerFrame;
}

class FileExplorerFrame : public QFrame
{
    Q_OBJECT

public:
    explicit FileExplorerFrame(QWidget *parent = nullptr);
    ~FileExplorerFrame();

private:
    Ui::FileExplorerFrame *ui;
};

#endif // FILEEXPLORERFRAME_HPP
