#include "mainWindow.h"
#include "ui_mainwindow.h"

#include <QListWidgetItem>
#include <QString>
#include <QVariant>

#define COMPANY "Stone"
#define APP "PlantUML Editor"
#define JAR_PATH_ITEM "jarPath"

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , m_jarPath("")
  , m_settings(COMPANY, APP)

{
  ui->setupUi(this);
  m_jarPath = m_settings.value(JAR_PATH_ITEM, "").toString();
  ui->tblEditors->hideColumn(0);
  if (m_jarPath.isEmpty()) {
    on_btnCfg_clicked();
  }
}

MainWindow::~MainWindow()
{
  delete ui;
}

void
MainWindow::on_btnCfg_clicked()
{
  if (m_cfg == nullptr) {
    m_cfg = new Cfg(m_jarPath, this);
    QObject::connect(m_cfg,
                     SIGNAL(jarPath_set(const QString&)),
                     this,
                     SLOT(on_jarPath_set(const QString&)));
  }
  m_cfg->open();
}

void
MainWindow::on_jarPath_set(const QString& p_jarPath)
{
  m_jarPath = p_jarPath;

  m_settings.setValue(JAR_PATH_ITEM, m_jarPath);
}

void
MainWindow::on_btnEdit_clicked()
{
  UmlEditor* editor = new UmlEditor(m_jarPath, this);

  QObject::connect(editor,
                   SIGNAL(title_changed(int, const QString&)),
                   this,
                   SLOT(on_title_changed(int, const QString&)));

  QObject::connect(
    editor, SIGNAL(editor_closed(int)), this, SLOT(on_editor_closed(int)));

  m_editors.push_back(editor);

  ui->tblEditors->insertRow(ui->tblEditors->rowCount());
  ui->tblEditors->setItem(
    ui->tblEditors->rowCount() - 1,
    0,
    new QTableWidgetItem(QString::number(editor->getId())));
  ui->tblEditors->setItem(ui->tblEditors->rowCount() - 1,
                          1,
                          new QTableWidgetItem(editor->windowTitle()));

  editor->show();
}

void
MainWindow::on_tblEditors_cellDoubleClicked(int row, int /*column*/)
{
  QString idStr = ui->tblEditors->item(row, 0)->text();
  int id = idStr.toInt();
  UmlEditor* editor = find(id);
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

void
MainWindow::on_title_changed(int p_id, const QString& p_title)
{
  int rowCount = ui->tblEditors->rowCount();
  for (int row = 0; row < rowCount; ++row) {
    QString idStr = ui->tblEditors->item(row, 0)->text();
    int id = idStr.toInt();
    if (id == p_id) {
      ui->tblEditors->item(row, 1)->setText(p_title);
    }
  }
}

void
MainWindow::on_editor_closed(int p_id)
{
  int rowCount = ui->tblEditors->rowCount();
  for (int row = 0; row < rowCount; ++row) {
    QString idStr = ui->tblEditors->item(row, 0)->text();
    int id = idStr.toInt();
    if (id == p_id) {
      ui->tblEditors->removeRow(row);
    }
  }
}

UmlEditor*
MainWindow::find(int id)
{
  for (UmlEditor* editor : m_editors) {
    if (editor->getId() == id) {
      return editor;
    }
  }
  return nullptr;
}

void
MainWindow::on_tblEditors_itemChanged(QTableWidgetItem*)
{
  ui->tblEditors->setColumnWidth(1, ui->tblEditors->width() - 20);
}

void
MainWindow::keyPressEvent(QKeyEvent* p_ev)
{
  //    setText("You Pressed Key " + ev->text());
  if ((p_ev->key() == Qt::Key_E) &&
      (QGuiApplication::queryKeyboardModifiers() == Qt::ControlModifier)) {
    int currentRow = ui->tblEditors->currentRow();
    if ((currentRow >= 0) && (currentRow < ui->tblEditors->rowCount())) {
      on_tblEditors_cellDoubleClicked(currentRow, 0);
    }
  }
}

void
MainWindow::showEvent(QShowEvent*)
{
  ui->tblEditors->setFocus();
}
