import type { Device } from '$lib/types';

export const mockDevice: Device = {
	id: 'device-1',
	name: 'Lampje Proto-1',
	status: 'disconnected',
	model: 'Proto-1 (ESP32-S3)',
	voice: 'puck',
	sttProvider: 'Google Cloud STT',
	ttsProvider: 'ElevenLabs',
	ledColors: {
		idle: '#81ecff',
		listening: '#00d4ec',
		thinking: '#a68cff',
		speaking: '#ff7520',
		error: '#ff716c'
	},
	micSensitivity: 70,
	speakerVolume: 65,
	cameraEnabled: false,
	cameraResolution: '640x480',
	cliffSensors: true,
	firmwareVersion: '0.4.2-beta',
	lastSeen: '2 uur geleden',
	wifiStrength: 'Geen verbinding',
	battery: 'N.v.t.'
};
