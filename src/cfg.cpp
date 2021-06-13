#include "cfg.h"
#include "ui_cfg.h"

#include <QFileDialog>

#include "events.h"

#include <tenacitas.lib/async.h>

using namespace tenacitas;
using namespace tenacitas::plantuml_editor;

Cfg::Cfg(const QString &p_jarPath, QWidget *parent)
    : QDialog(parent), ui(new Ui::Cfg) {
  ui->setupUi(this);
  ui->txtJarPath->setText(p_jarPath);
}

Cfg::~Cfg() { delete ui; }

QString Cfg::getJarPath() const { return ui->txtJarPath->text(); }

void Cfg::on_btnJarPath_clicked() {

  ui->txtJarPath->setText(QFileDialog::getOpenFileName(
      this, tr(""), "./", tr("Jar Files (*.jar)")));

  async::dispatch(evt::jar_path_set{ui->txtJarPath->text().toStdString()});

  //  emit jarPath_set(ui->txtJarPath->text());

  this->close();
}
