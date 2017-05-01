#include "configsettingsstore.h"
#include "configsetting.h"

#include <QDebug>

ConfigSettingsStore::ConfigSettingsStore(QObject *parent)
    : QAbstractListModel(parent)
    , m_nofSettings(0)
    , m_nofSelectedSettings(0)
{
    m_rolemap = QAbstractListModel::roleNames();
    m_rolemap[NameRole]     = "name";
    m_rolemap[ValueRole]    = "value";
    m_rolemap[SelectedRole] = "selected";
}

ConfigSettingsStore::~ConfigSettingsStore()
{
    // EMPTY
}

int ConfigSettingsStore::rowCount(const QModelIndex &parent) const
{
    if( parent.isValid() )
        return 0;
    qDebug() << "ConfigSettingsStore::rowCount()" << m_nofSettings;
    return m_nofSettings;
}

QVariant ConfigSettingsStore::data(const QModelIndex &index, int role) const
{
    if( index.row() < 0 || index.row() >= m_nofSettings )
        return QVariant();

    qDebug() << "ConfigSettingsStore::data()" << index.row() << m_rolemap[role];
    switch( role )
    {
        case Qt::DisplayRole:
        case NameRole:
        {
            return m_settings.getSetting(index.row())->fullname();
        }
        case ValueRole:
        {
            return m_settings.getSetting(index.row())->value();
        }
        case SelectedRole:
        {
            return m_selected.at(index.row());
        }
    }
    return QVariant();
}

bool ConfigSettingsStore::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if( index.row() < 0 || index.row() >= m_nofSettings || !value.isValid() )
        return false;

    qDebug() << "ConfigSettingsStore::setData()" << index.row() << m_rolemap[role];
    switch( role )
    {
        case SelectedRole:
        {
             bool sel = value.toBool();
             if( m_selected[index.row()] != sel )
             {
                 m_selected[index.row()] = sel;
                 if( sel )
                     setNofSelectedSettings( m_nofSelectedSettings+1 );
                 else
                     setNofSelectedSettings( m_nofSelectedSettings-1 );
                 dataChanged( index, index, QVector<int>()<<SelectedRole);
                 return true;
             }
        }
    }

    return false;
}

void ConfigSettingsStore::setNofSettings(int nof)
{
    if( m_nofSettings == nof )
        return;

    m_nofSettings = nof;
    emit nofSettingsChanged(m_nofSettings);
}

void ConfigSettingsStore::setNofSelectedSettings(int nof)
{
    if( m_nofSelectedSettings == nof )
        return;

    m_nofSelectedSettings = nof;
    emit nofSelectedSettingsChanged(m_nofSelectedSettings);
}

void ConfigSettingsStore::copyFrom(ConfigDbEntry *config)
{
    if( !config || !config->settings() )
        return;

    qDebug() << "ConfigSettingsStore::copyFrom()" << config->config();

    beginResetModel();
    m_settings = *config->settings();
    setNofSettings( m_settings.settings().size() );
    setNofSelectedSettings( m_nofSettings );
    m_selected.clear();
    for( int i=0; i<m_nofSettings; i++ )
        m_selected.append(true);
    endResetModel();
}

void ConfigSettingsStore::applyTo(ConfigDbEntry *config)
{
    if( !config || !config->settings() )
        return;

    qDebug() << "ConfigSettingsStore::applyTo()" << config->config();

    for( int idx=0; idx<m_nofSettings; idx++ )
    {
        if( m_selected[idx] )
        {
            ConfigSetting *s = m_settings.getSetting(idx);
            ConfigSetting *o = config->settings()->getSetting(s->fullname());
            o->setValue( s->value() );
        }
    }
}
