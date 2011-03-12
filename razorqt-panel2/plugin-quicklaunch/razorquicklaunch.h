#ifndef RAZORQUICKLAUNCH_H
#define RAZORQUICKLAUNCH_H

#include "razorpanelplugin.h"
#include "razorpanel.h"

class XdgDesktopFile;
class ReadSettings;

/*! \brief Loader for "quick launcher" icons in the panel.
\author Petr Vanek <petr@scribus.info>
*/
class RazorQuickLaunch : public RazorPanelPlugin
{
    Q_OBJECT

public:
    RazorQuickLaunch(RazorPanel* panel, const QString& configId, QWidget *parent = 0);
    ~RazorQuickLaunch();

private:
    ReadSettings * cfg;
    QString m_configId;

    QMap<QString,XdgDesktopFile*> m_xdgFiles;

private slots:
    //! run command in the action
    void execAction(QAction*);
};

/*! \brief Special action representation for RazorQuickLaunch plugin.
It supports XDG desktop files or "legacy" launching of specified apps.
All process management is handled internally.
\author Petr Vanek <petr@scribus.info>
*/
class RazorQuickLaunchAction : public QAction
{
    Q_OBJECT

public:
    /*! Constructor for "legacy" launchers.
        \warning The XDG way is preferred this is only for older or non-standard apps
        \param name a name to display in tooltip
        \param exec a executable with path
        \param icon a valid QIcon
     */
    RazorQuickLaunchAction(const QString & name,
                           const QString & exec,
                           const QIcon & icon,
                           QWidget * parent);
    /*! Constructor for XDG desktop handlers.
        \param desktop name of a XDG desktop file. With or without path.
     */
    RazorQuickLaunchAction(const QString & desktop, QWidget * parent);

    //! Returns true if the action is valid (contains all required properties).
    bool isValid() { return m_valid; }

public slots:
    void execAction();

private:
    enum ActionType { ActionLegacy, ActionXdg };
    ActionType m_type;
    QString m_data;
    bool m_valid;
};

EXPORT_RAZOR_PANEL_PLUGIN_H

#endif
