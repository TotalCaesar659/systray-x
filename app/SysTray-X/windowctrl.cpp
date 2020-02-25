/*
 *  Qt includes
 */
#include <QWidget>
#include <QWindow>

/*
 *  Main include
 */
#include "windowctrl.h"

/*
 *  System includes
 */
#include "preferences.h"

/*
 *  Constructor
 */
WindowCtrl::WindowCtrl( Preferences* pref, QObject *parent ) :
#ifdef Q_OS_UNIX
    WindowCtrlUnix( parent )
#elif defined Q_OS_WIN
    WindowCtrlWin( parent )
#else
    public QObject
#endif
{
    /*
     *  Store preferences
     */
    m_pref = pref;

    /*
     *  Initialize
     */
    m_minimize_hide = m_pref->getMinimizeHide();
    m_tb_container = nullptr;
}


void    WindowCtrl::slotWindowTest1()
{
    emit signalConsole("Test 1 started");

    // Do something.

//    findWindow( "- Mozilla Thunderbird" );
    displayWindowElements( "- Mozilla Thunderbird" );
//    findWindow( 4313 );

//    captureWindow( "- Mozilla Thunderbird" );

    emit signalConsole("Test 1 done");
}


void    WindowCtrl::slotWindowTest2()
{
    emit signalConsole("Test 2 started");

    // Do something.

//    foreach( quint64 win_id, getWinIds() )
//    {
//        closeWindow( (HWND)win_id );
//    }


    /*
     *  Disconnect container?
     */
/*
    m_tb_window->setParent( nullptr );

    delete m_tb_container;
    m_tb_container = nullptr;
*/
    emit signalConsole("Test 2 done");
}


void    WindowCtrl::slotWindowTest3()
{
    emit signalConsole("Test 3 started");

    // Do something.

    emit signalConsole("Test 3 done");
}


bool    WindowCtrl::captureWindow( const QString& title )
{
    emit signalConsole("Capture");

    if( !findWindow( title ) )
    {
        emit signalConsole("Capture error");
        return false;
    }

    /*
     *  Wrap Thunderbird window
     */
    m_tb_window = QWindow::fromWinId( getWinIds()[ 0 ] );
    m_tb_window->parent();

    m_tb_container = QWidget::createWindowContainer( m_tb_window );

    m_tb_container->show();

    emit signalConsole("Capture done");

    return true;
}


/*
 *  Handle window title signal
 */
void    WindowCtrl::slotWindowTitle( QString title )
{
    /*
     *  Store the window title
     */
    m_window_title = title;

    /*
     *  Get the window IDs
     */
    findWindow( title );
}


/*
 *  Handle change in minimizeHide state
 */
void    WindowCtrl::slotMinimizeHideChange()
{
    m_minimize_hide = m_pref->getMinimizeHide();
}


/*
 *  Handle change in window state
 */
void    WindowCtrl::slotWindowState( QString state )
{
    if( m_state != state )
    {
        m_state = state;

        if( state == "normal" )
        {
            foreach( quint64 win_id, getWinIds() )
            {
                hideWindow( win_id, false );
            }
        }
        else
        {
            foreach( quint64 win_id, getWinIds() )
            {
                hideWindow( win_id, m_minimize_hide );
            }
        }

        emit signalConsole( "New state: " + state );
    }
}


/*
 *  Handle show / hide signal
 */
void    WindowCtrl::slotShowHide()
{
    if( m_state == "minimized" )
    {
        m_state = "normal";

        foreach( quint64 win_id, getWinIds() )
        {
            normalizeWindow( win_id );

            emit signalConsole("Normalize");
        }

//        emit signalWindowNormal();    // TB window control

    } else {
        m_state = "minimized";

        foreach( quint64 win_id, getWinIds() )
        {
            minimizeWindow( win_id, m_minimize_hide );

            emit signalConsole("Minimize");
        }

//        emit signalWindowMinimize();  // TB window control

    }
}
