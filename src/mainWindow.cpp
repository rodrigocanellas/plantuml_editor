#include "mainWindow.h"
#include "ui_mainwindow.h"

#include <chrono>

#include <QListWidgetItem>
#include <QString>
#include <QVariant>

#include "events.h"
#include <tenacitas.lib/async.h>
#include <tenacitas.lib/logger.h>
#include <tenacitas.lib/type.h>

using namespace std::chrono_literals;
using namespace tenacitas;
using namespace tenacitas::plantuml_editor;

#define COMPANY "tenacitas"
#define APP "PlantUML Editor"
#define JAR_PATH_ITEM "jarPath"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_jarPath(""),
      m_settings(COMPANY, APP)

{
  logger::set_debug_level();
  ui->setupUi(this);
  m_jarPath = m_settings.value(JAR_PATH_ITEM, "").toString();
  ui->tblEditors->hideColumn(0);
  if (m_jarPath.isEmpty()) {
    on_btnCfg_clicked();
  }

  // defining handlers for the events
  async::add_handler<evt::jar_path_set>(
      [this](type::ptr<bool>, evt::jar_path_set &&p_evt) -> void {
        DEB("jar_path_set = ", p_evt);
        m_jarPath = QString(p_evt.value.c_str());
        m_settings.setValue(JAR_PATH_ITEM, m_jarPath);
      },
      1s);

  async::add_handler<evt::title_changed>(
      [this](type::ptr<bool>, evt::title_changed &&p_evt) -> void {
        DEB("title_changed = ", p_evt);
        int rowCount = ui->tblEditors->rowCount();
        for (int row = 0; row < rowCount; ++row) {
          QString idStr = ui->tblEditors->item(row, 0)->text();
          int id = idStr.toInt();
          if (id == p_evt.id) {
            ui->tblEditors->item(row, 1)->setText(p_evt.title.c_str());
          }
        }
      },
      1s);

  async::add_handler<evt::editor_closed>(
      [this](type::ptr<bool>, evt::editor_closed &&p_evt) -> void {
        DEB("editor_closed  = ", p_evt);
        int rowCount = ui->tblEditors->rowCount();
        for (int row = 0; row < rowCount; ++row) {
          QString idStr = ui->tblEditors->item(row, 0)->text();
          int id = idStr.toInt();
          if (id == p_evt.id) {
            ui->tblEditors->removeRow(row);
          }
        }
      },
      1s);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_btnCfg_clicked() {
  if (m_cfg == nullptr) {
    m_cfg = new Cfg(m_jarPath, this);
  }
  m_cfg->open();
}

void MainWindow::on_btnEdit_clicked() {
  UmlEditor *editor = new UmlEditor(m_jarPath, this);

  m_editors.push_back(editor);

  ui->tblEditors->insertRow(ui->tblEditors->rowCount());
  ui->tblEditors->setItem(
      ui->tblEditors->rowCount() - 1, 0,
      new QTableWidgetItem(QString::number(editor->getId())));
  ui->tblEditors->setItem(ui->tblEditors->rowCount() - 1, 1,
                          new QTableWidgetItem(editor->windowTitle()));

  editor->show();
}

void MainWindow::on_tblEditors_cellDoubleClicked(int row, int /*column*/) {
  QString idStr = ui->tblEditors->item(row, 0)->text();
  int id = idStr.toInt();
  UmlEditor *editor = find(id);
  if (editor != nullptr) {
    //    this->lower();
    editor->setWindowState((editor->windowState() & ~Qt::WindowMinimized) |
                           Qt::WindowActive);

    //    editor->setFocus();
    editor->raise();
    editor->activateWindow();
    //    editor->show();
  }
}

UmlEditor *MainWindow::find(int id) {
  for (QList<UmlEditor *>::iterator _ite = m_editors.begin();
       _ite != m_editors.end(); ++_ite) {
    if ((*_ite)->getId() == id) {
      return *_ite;
    }
  }
  return nullptr;
}

void MainWindow::on_tblEditors_itemChanged(QTableWidgetItem *) {
  ui->tblEditors->setColumnWidth(1, ui->tblEditors->width() - 20);
}

void MainWindow::keyPressEvent(QKeyEvent *p_ev) {
  //    setText("You Pressed Key " + ev->text());
  if ((p_ev->key() == Qt::Key_E) &&
      (QGuiApplication::queryKeyboardModifiers() == Qt::ControlModifier)) {
    int currentRow = ui->tblEditors->currentRow();
    if ((currentRow >= 0) && (currentRow < ui->tblEditors->rowCount())) {
      on_tblEditors_cellDoubleClicked(currentRow, 0);
    }
  }
}

void MainWindow::showEvent(QShowEvent *) { ui->tblEditors->setFocus(); }
