#ifndef CFG_H
#define CFG_H

#include <QDialog>

namespace Ui {
class Cfg;
}

class Cfg : public QDialog
{
  Q_OBJECT

public:
  explicit Cfg(const QString& p_jarPath, QWidget* parent = nullptr);
  ~Cfg();

  QString getJarPath() const;

signals:
  void jarPath_set(const QString&);

private slots:
  void on_btnJarPath_clicked();

private:
  Ui::Cfg* ui;
};

#endif // CFG_H
