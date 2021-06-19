#include "umlDisplayer.h"
#include "ui_umlDisplayer.h"

#include <chrono>
#include <cstdlib>
#include <thread>

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QPixmap>
#include <QSize>

UmlDisplayer::UmlDisplayer(const QString &p_jarPath, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::UmlDisplayer), m_jarPath(p_jarPath) {
  ui->setupUi(this);

  //  QScreen* screen = QApplication::screenAt(QPoint(this->x(), this->y()));

  //  this->setMaximumSize(screen->availableSize());

  //  QDesktopWidget desktopWidget;
  //  this->setMaximumSize(
  //    QSize(desktopWidget.width(), desktopWidget.height() * .8));

  //  setWindowState(Qt::WindowMaximized);
}

UmlDisplayer::~UmlDisplayer() { delete ui; }

void UmlDisplayer::update(const QString &p_fileName) {

  QFileInfo fileInfo(p_fileName);
  QString pngFile =
      fileInfo.dir().path() + "/" + fileInfo.completeBaseName() + ".png";

  QString command = "java -jar " + m_jarPath + " " + p_fileName;
  ui->staDisplayer->showMessage("generating " + pngFile, 5000);
  int _result = std::system(command.toStdString().c_str());
  if (_result) {
    QMessageBox msgBox;
    //      msgBox.setWindowTitle(tr(""));
    QString msg("Error executing '" + command + "'");
    msgBox.setText(tr(msg.toStdString().c_str()));
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
    return;
  }

  while (!QFile(pngFile).exists()) {
    qDebug() << pngFile << " still does not exist";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  m_pixMap = QPixmap(pngFile);
  ui->lblImage->setPixmap(
      m_pixMap.scaled(ui->lblImage->width(), ui->lblImage->height(),
                      Qt::KeepAspectRatio, Qt::SmoothTransformation));

  //    setWindowState(Qt::WindowMaximized);
  //    ui->lblImage->setMaximumSize(pngWidth,
  //                                 static_cast<int>(this->height() * .9));
  //    ui->lblImage->repaint();
  this->setWindowTitle("Displayer - " + p_fileName);
  // ui->btnBack->setFocus();
}

void UmlDisplayer::showEvent(QShowEvent *) { ui->btnBack->setFocus(); }

void UmlDisplayer::resizeEvent(QResizeEvent *) {
  if (!m_pixMap.isNull()) {
    ui->lblImage->setPixmap(
        m_pixMap.scaled(ui->lblImage->width(), ui->lblImage->height(),
                        Qt::KeepAspectRatio, Qt::SmoothTransformation));
  }
  ui->btnBack->setFocus();
}

void UmlDisplayer::on_btnBack_clicked() {
  QWidget *parent = parentWidget();
  if (parent != nullptr) {
    parent->setWindowState((parent->windowState() & ~Qt::WindowMinimized) |
                           Qt::WindowActive);
    parent->raise();
    //    parent->setFocus();
    parent->activateWindow();
  }
}
