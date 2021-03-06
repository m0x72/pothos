// Copyright (c) 2014-2014 Josh Blum
// SPDX-License-Identifier: BSL-1.0

#pragma once
#include <Pothos/Config.hpp>
#include <QWidget>
#include <QColor>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>
#include <Pothos/System/NumaInfo.hpp>
#include <vector>
#include <map>

class HostExplorerDock;
class CpuSelectionWidget;
class QtColorPicker;
class QComboBox;
class QLineEdit;
class QSpinBox;
class QComboBox;
class QVBoxLayout;

//! Editor panel for affinity zone settings
class AffinityZoneEditor : public QWidget
{
    Q_OBJECT
public:
    AffinityZoneEditor(QWidget *parent);

    //! load the settings from a JSON object
    void loadFromConfig(const Poco::JSON::Object::Ptr &config);

    //! query the settings as a JSON object
    Poco::JSON::Object::Ptr getCurrentConfig(void) const;

    //! Get the current color set on this editor
    QColor color(void) const;

signals:
    void settingsChanged(void);

private slots:

    //! user edited the host list in another widget, modify the combo box to match
    void handleHostListChanged(void);

    void handleUriChanged(int)
    {
        this->updateCpuSelection();
        emit this->settingsChanged();
    }

    void handleColorChanged(const QColor &)
    {
        emit this->settingsChanged();
    }

    void handleProcessNameChanged(void)
    {
        emit this->settingsChanged();
    }

    void handleComboChanged(int)
    {
        emit this->settingsChanged();
    }

    void handleSpinSelChanged(void)
    {
        emit this->settingsChanged();
    }

private:

    void selectThisUri(const QString &uri);

    void updateCpuSelection(void);

    HostExplorerDock *_hostExplorerDock;
    QtColorPicker *_colorPicker;
    QComboBox *_hostsBox;
    QLineEdit *_processNameEdit;
    QSpinBox *_numThreadsSpin;
    QSpinBox *_prioritySpin;
    CpuSelectionWidget *_cpuSelection;
    QVBoxLayout *_cpuSelectionContainer;
    QComboBox *_yieldModeBox;

    std::map<QString, std::vector<Pothos::System::NumaInfo>> _uriToNumaInfo;
};
