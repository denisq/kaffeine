/*
 * dvbmanager.h
 *
 * Copyright (C) 2008 Christoph Pfister <christophpfister@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
 */

#ifndef DVBMANAGER_H
#define DVBMANAGER_H

#include <QDate>
#include <QMap>
#include <QPair>
#include <QStringList>

class DvbChannelModel;
class DvbConfig;
class DvbDevice;
class DvbDeviceConfig;
class DvbDeviceManager;
class DvbScanData;
class DvbTransponder;

class DvbDeviceConfig
{
public:
	DvbDeviceConfig(const QString &deviceId_, const QString &frontendName_, DvbDevice *device_)
		: deviceId(deviceId_), frontendName(frontendName_), device(device_), used(false) { }
	~DvbDeviceConfig() { }

	QString deviceId;
	QString frontendName;
	DvbDevice *device;
	QList<DvbConfig> configs;
	bool used;
};

class DvbManager : public QObject
{
	Q_OBJECT
public:
	enum TransmissionType
	{
		DvbC = 0,
		DvbS = 1,
		DvbT = 2,
		Atsc = 3,
		TransmissionTypeMax = Atsc
	};

	explicit DvbManager(QObject *parent);
	~DvbManager();

	QStringList getSources() const
	{
		return sources;
	}

	DvbChannelModel *getChannelModel() const
	{
		return channelModel;
	}

	QList<DvbDevice *> getDevices() const;
	DvbDevice *requestDevice(const QString &source);
	void releaseDevice(DvbDevice *device);

	QList<DvbDeviceConfig> getDeviceConfigs() const;
	void setDeviceConfigs(const QList<QList<DvbConfig> > &configs);

	QString getScanFileDate(); // returns the formatted short date of the last scan file update
	QStringList getScanSources(TransmissionType type);
	QList<DvbTransponder> getTransponders(const QString &source);

private slots:
	void deviceAdded(DvbDevice *device);
	void deviceRemoved(DvbDevice *device);

private:
	void readChannelList();
	void writeChannelList();

	void readDeviceConfigs();
	void writeDeviceConfigs();

	void readScanFile();

	DvbChannelModel *channelModel;
	QList<DvbDeviceConfig> deviceConfigs;
	QMap<QString, QString> sourceMapping;
	QStringList sources;

	DvbDeviceManager *deviceManager;

	QDate scanFileDate;
	DvbScanData *scanData;
	QStringList scanSources[TransmissionTypeMax + 1];
	QList<int> scanOffsets[TransmissionTypeMax + 1];
};

#endif /* DVBMANAGER_H */