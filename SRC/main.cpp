#include "COBRA2_mainwindow.h"
#include <QApplication>
#include <QString>
#include <QLockFile>
#include <QDir>
#include <QMessageBox>
#include "main.h"
#include "Common/SettingsFile.h"

int main(int argc, char *argv[])
{
    QStringList paths = QCoreApplication::libraryPaths();
    paths.append(".");
    paths.append("platforms");
    QCoreApplication::setLibraryPaths(paths);
    QApplication a(argc, argv);
    QLockFile lockFile(QDir::temp().absoluteFilePath("Server.lock"));
    if(!lockFile.tryLock(100))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Приложение уже запущено !!!");
        msgBox.exec();
        return 1;
    }
    TSettingsFile *settings = new TSettingsFile();
    COBRA2_MainWindow *window = new COBRA2_MainWindow;
    if(settings->WindowMinimize()) window->hide(); else window->show();
    return a.exec();
}


