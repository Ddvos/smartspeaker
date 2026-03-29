import type { Device } from '$lib/types';

export const mockDevice: Device = {
	id: 'device-1',
	chipId: 'AA:BB:CC:DD:EE:FF',
	userId: 'user-1',
	name: 'Lampje Proto-1',
	status: 'offline',
	firmwareVersion: '0.1.0',
	lastHeartbeat: null,
	ipAddress: null,
	wifiRssi: null,
	createdAt: new Date().toISOString(),
	config: {
		ledColors: {
			idle: '#81ecff',
			listening: '#00d4ec',
			thinking: '#a68cff',
			speaking: '#ff7520',
			error: '#ff716c'
		},
		micSensitivity: 70,
		speakerVolume: 65,
		displayBrightness: 80,
		voice: 'puck',
		sttProvider: 'whisper',
		ttsProvider: 'elevenlabs'
	}
};
