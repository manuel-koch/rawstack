#include "commontasks.h"
#include "taskbase.h"

#include <QDebug>

CommonTasks::CommonTasks(QObject *parent)
    : QObject(parent)
    , m_ufraw(NULL)
    , m_final(NULL)
{
    // EMPTY
}

CommonTasks::~CommonTasks()
{
    // EMPTY
}

QUrl CommonTasks::image() const
{
    bool preview = m_ufraw ? m_ufraw->worker()->cycle() == 0 : false;
    qDebug() << "CommonTasks::image()" << (preview ? "preview" : "final");
    if( preview && m_ufraw )
        return m_ufraw->images()->preview();
    else if( m_final )
        return m_final->images()->image();
    else
        return QUrl();
}

void CommonTasks::setUfraw(TaskBase *ufraw)
{
   if( m_ufraw != ufraw )
   {
       m_ufraw = ufraw;
       emit ufrawChanged(m_ufraw);
       if( m_ufraw )
           emit imageChanged( m_ufraw->images()->image() );
   }
}

void CommonTasks::setFinal(TaskBase *final)
{
    if( m_final != final )
    {
        if( m_final )
            disconnect( m_final->images(), SIGNAL(imageChanged(QUrl)), this, SIGNAL(imageChanged(QUrl)) );

        m_final = final;
        emit finalChanged(m_final);

        if( m_final )
        {
            emit imageChanged( m_final->images()->image() );
            connect( m_final->images(), SIGNAL(imageChanged(QUrl)), this, SIGNAL(imageChanged(QUrl)) );
        }
    }
}

