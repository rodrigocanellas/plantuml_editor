#ifndef UMLDISPLAYER_H
#define UMLDISPLAYER_H

#include <QMainWindow>
#include <QProcess>
#include <QResizeEvent>
#include <QShowEvent>
#include <QString>

namespace Ui {
class UmlDisplayer;
}

class UmlDisplayer : public QMainWindow
{
  Q_OBJECT

public:
  explicit UmlDisplayer(const QString& p_jarPath, QWidget* parent = nullptr);
  ~UmlDisplayer() override;

  void update(const QString& p_fileName);

  void showEvent(QShowEvent*) override;
  void resizeEvent(QResizeEvent*) override;

private slots:
  void on_btnBack_clicked();

private:
  Ui::UmlDisplayer* ui;

  QString m_jarPath;

  QPixmap m_pixMap;
};

#endif // UMLDISPLAYER_H
