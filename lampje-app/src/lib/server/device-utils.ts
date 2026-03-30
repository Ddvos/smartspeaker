import type { Device, DeviceConfig } from '$lib/types';

const HEARTBEAT_TIMEOUT_MS = 2 * 60 * 1000; // 2 minutes

export function deriveDeviceStatus(
	device: { userId: string | null; lastHeartbeat: Date | null }
): 'online' | 'offline' | 'pairing' {
	if (!device.userId) return 'pairing';
	if (!device.lastHeartbeat) return 'offline';

	const elapsed = Date.now() - device.lastHeartbeat.getTime();
	return elapsed < HEARTBEAT_TIMEOUT_MS ? 'online' : 'offline';
}

export function formatDeviceRow(
	device: {
		id: string;
		chipId: string;
		userId: string | null;
		name: string;
		firmwareVersion: string | null;
		lastHeartbeat: Date | null;
		ipAddress: string | null;
		wifiRssi: number | null;
		createdAt: Date | null;
	},
	config: {
		ledColors: Record<string, string>;
		micSensitivity: number;
		speakerVolume: number;
		displayBrightness: number;
		voice: string;
		systemPrompt: string | null;
		geminiModel: string;
		sttProvider: string;
		ttsProvider: string;
	}
): Device {
	return {
		id: device.id,
		chipId: device.chipId,
		userId: device.userId,
		name: device.name,
		status: deriveDeviceStatus(device),
		firmwareVersion: device.firmwareVersion,
		lastHeartbeat: device.lastHeartbeat?.toISOString() ?? null,
		ipAddress: device.ipAddress,
		wifiRssi: device.wifiRssi,
		createdAt: device.createdAt?.toISOString() ?? new Date().toISOString(),
		config: {
			ledColors: config.ledColors,
			micSensitivity: config.micSensitivity,
			speakerVolume: config.speakerVolume,
			displayBrightness: config.displayBrightness,
			voice: config.voice,
			systemPrompt: config.systemPrompt,
			geminiModel: config.geminiModel,
			sttProvider: config.sttProvider,
			ttsProvider: config.ttsProvider
		}
	};
}
