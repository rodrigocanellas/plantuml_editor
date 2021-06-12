#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>
#include <QSettings>
#include <QTableWidgetItem>

#include "cfg.h"
#include "umlEditor.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;

  void keyPressEvent(QKeyEvent *p_ev) override;

  void showEvent(QShowEvent *ev) override;

private slots:
  void on_btnCfg_clicked();

  //  void on_jarPath_set(const QString&);

  void on_btnEdit_clicked();

  void on_tblEditors_cellDoubleClicked(int row, int column);

  void on_title_changed(int p_id, const QString &p_title);

  void on_editor_closed(int p_id);

  void on_tblEditors_itemChanged(QTableWidgetItem *item);

private:
  UmlEditor *find(int id);

private:
  Ui::MainWindow *ui;
  Cfg *m_cfg = nullptr;

  QString m_jarPath = "";
  QSettings m_settings;
  QList<UmlEditor *> m_editors;
};

#endif // MAINWINDOW_H
