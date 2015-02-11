#include "recent_files_dialog.h"

RecentFilesDialog::RecentFilesDialog(QWidget *parent , QStringList& list)
        :QDialog(parent)
{
    setupUi(this);
    listRecent->addItems(list);
}

void RecentFilesDialog::onAcceptLocal()
{
    if(selection.length() == 0)
        selection = (listRecent->currentItem())->text();
    QDialog::accept();
}

void RecentFilesDialog::onSelected(QListWidgetItem *item)
{
    selection = item->text();
}
