/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * Razor - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2013 Razor team
 * Authors:
 *   Kuzma Shapran <kuzma.shapran@gmail.com>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#include "main_window.hpp"
#include "actions.hpp"
#include "default_model.hpp"

#include <QItemSelectionModel>
#include <QSortFilterProxyModel>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);

    mActions = new Actions(this);
    mDefaultModel = new DefaultModel(mActions, this);
    mSortFilterProxyModel = new QSortFilterProxyModel(this);

    mSortFilterProxyModel->setSourceModel(mDefaultModel);
    actions_TV->setModel(mSortFilterProxyModel);

    mSelectionModel = new QItemSelectionModel(actions_TV->model());
    actions_TV->setSelectionModel(mSelectionModel);

    connect(mSelectionModel, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(selectionChanged(QItemSelection,QItemSelection)));

    connect(mActions, SIGNAL(daemonDisappeared()), SLOT(daemonDisappeared()));
    connect(mActions, SIGNAL(daemonAppeared()), SLOT(daemonAppeared()));
    connect(mActions, SIGNAL(multipleActionsBehaviourChanged(MultipleActionsBehaviour)), SLOT(multipleActionsBehaviourChanged(MultipleActionsBehaviour)));
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::daemonDisappeared()
{
    add_PB->setEnabled(false);
    actions_TV->setEnabled(false);
    multipleActionsBehaviour_CB->setEnabled(false);
}

void MainWindow::daemonAppeared()
{
    add_PB->setEnabled(true);
    actions_TV->setEnabled(true);
    multipleActionsBehaviour_CB->setEnabled(true);

    multipleActionsBehaviour_CB->setCurrentIndex(mActions->multipleActionsBehaviour());

    actions_TV->horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);
}

void MainWindow::multipleActionsBehaviourChanged(MultipleActionsBehaviour behaviour)
{
    multipleActionsBehaviour_CB->setCurrentIndex(behaviour);
}

void MainWindow::on_multipleActionsBehaviour_CB_currentIndexChanged(int index)
{
    mActions->setMultipleActionsBehaviour(static_cast<MultipleActionsBehaviour>(index));
}

void MainWindow::selectionChanged(const QItemSelection &/*selected*/, const QItemSelection &/*deselected*/)
{
    QModelIndexList rows = mSelectionModel->selectedRows();

    modify_PB->setEnabled(rows.length() == 1);
    remove_PB->setEnabled(rows.length() != 0);

    bool enableSwap = (rows.length() == 2);
    if (enableSwap)
    {
        QPair<bool, GeneralActionInfo> info0 = mActions->actionById(mDefaultModel->id(rows[0]));
        QPair<bool, GeneralActionInfo> info1 = mActions->actionById(mDefaultModel->id(rows[1]));
        enableSwap = (info0.first && info1.first && (info0.second.shortcut == info1.second.shortcut));
    }
    swap_PB->setEnabled(enableSwap);
}

void MainWindow::on_add_PB_clicked()
{
}

void MainWindow::on_modify_PB_clicked()
{
}

void MainWindow::on_swap_PB_clicked()
{
    QModelIndexList rows = mSelectionModel->selectedRows();
    mActions->swapActions(mDefaultModel->id(rows[0]), mDefaultModel->id(rows[1]));
}

void MainWindow::on_remove_PB_clicked()
{
    foreach (QModelIndex rowIndex, mSelectionModel->selectedRows())
        mActions->removeAction(mDefaultModel->id(rowIndex));
}
