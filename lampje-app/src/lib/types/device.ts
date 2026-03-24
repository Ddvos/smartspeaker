export interface Device {
	id: string;
	name: string;
	status: 'connected' | 'disconnected';
	model: string;
	voice: string;
	sttProvider: string;
	ttsProvider: string;
	ledColors: Record<string, string>;
	micSensitivity: number; // 0-100
	speakerVolume: number; // 0-100
	cameraEnabled: boolean;
	cameraResolution: string;
	cliffSensors: boolean;
	firmwareVersion: string;
	lastSeen: string;
	wifiStrength: string;
	battery: string;
}
