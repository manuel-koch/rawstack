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

void CommonTasks::setUfraw(TaskBase *ufraw)
{
   if( m_ufraw != ufraw )
   {
       m_ufraw = ufraw;
       emit ufrawChanged(m_ufraw);
   }
}

void CommonTasks::setFinal(TaskBase *final)
{
    if( m_final != final )
    {
        m_final = final;
        emit finalChanged(m_final);
    }
}

