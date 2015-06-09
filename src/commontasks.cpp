#include "commontasks.h"
#include "taskbase.h"

CommonTasks::CommonTasks(QObject *parent)
    : QObject(parent)
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

