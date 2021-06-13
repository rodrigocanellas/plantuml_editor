#include "findDialog.h"
#include "ui_findDialog.h"

FindDialog::FindDialog(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::FindDialog)
{
  ui->setupUi(this);
  ui->rdbForward->toggle();
  ui->rdbNonCase->toggle();
  ui->rdbPart->toggle();
}

FindDialog::~FindDialog()
{
  delete ui;
}

void
FindDialog::on_buttonBox_accepted()
{
  m_textToFind = ui->txtTextToFind->text();
  m_textReplaceFor = ui->txtReplaceFor->text();
}

void
FindDialog::on_buttonBox_rejected()
{
  m_textToFind = "";
  m_textReplaceFor = "";
}

const QString&
FindDialog::getTextToFind() const
{
  return m_textToFind;
}

const QString&
FindDialog::getTextReplaceFor() const
{
  return m_textReplaceFor;
}

bool
FindDialog::forward() const
{
  return ui->rdbForward->isChecked();
}

bool
FindDialog::backward() const
{
  return ui->rdbBackward->isChecked();
}

bool
FindDialog::wholeWord() const
{
  return ui->rdbWhole->isChecked();
}

bool
FindDialog::wordPart() const
{
  return ui->rdbPart->isChecked();
}

bool
FindDialog::caseSensitive() const
{
  return ui->rdbCase->isChecked();
}

bool
FindDialog::nonCaseSensitive() const
{
  return ui->rdbNonCase->isChecked();
}
