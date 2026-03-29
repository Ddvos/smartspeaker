export interface DeviceConfig {
	ledColors: Record<string, string>;
	micSensitivity: number; // 0-100
	speakerVolume: number; // 0-100
	displayBrightness: number; // 0-100
	voice: string;
	sttProvider: string;
	ttsProvider: string;
}

export interface Device {
	id: string;
	chipId: string;
	userId: string | null;
	name: string;
	status: 'online' | 'offline' | 'pairing';
	firmwareVersion: string | null;
	lastHeartbeat: string | null;
	ipAddress: string | null;
	wifiRssi: number | null;
	createdAt: string;
	config: DeviceConfig;
}

export interface DevicePairingCode {
	id: string;
	deviceId: string;
	code: string;
	expiresAt: string;
	used: boolean;
}
