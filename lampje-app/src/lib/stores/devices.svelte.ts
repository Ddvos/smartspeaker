import type { Device, DeviceConfig } from '$lib/types';

let devices = $state<Device[]>([]);
let loading = $state(false);
let error = $state<string | null>(null);

export const deviceStore = {
	get devices() {
		return devices;
	},
	get loading() {
		return loading;
	},
	get error() {
		return error;
	},
	get primaryDevice(): Device | null {
		return devices[0] ?? null;
	},

	setDevices(data: Device[]) {
		devices = data;
	},

	async fetchDevices() {
		loading = true;
		error = null;
		try {
			const res = await fetch('/api/devices');
			if (!res.ok) throw new Error('Kon apparaten niet ophalen');
			devices = await res.json();
		} catch (e) {
			error = e instanceof Error ? e.message : 'Onbekende fout';
		} finally {
			loading = false;
		}
	},

	async pairDevice(code: string): Promise<Device | null> {
		loading = true;
		error = null;
		try {
			const res = await fetch('/api/devices/pair', {
				method: 'POST',
				headers: { 'Content-Type': 'application/json' },
				body: JSON.stringify({ code })
			});
			if (!res.ok) {
				const data = await res.json();
				throw new Error(data.error ?? 'Koppelen mislukt');
			}
			const device: Device = await res.json();
			devices = [...devices, device];
			return device;
		} catch (e) {
			error = e instanceof Error ? e.message : 'Onbekende fout';
			return null;
		} finally {
			loading = false;
		}
	},

	async updateConfig(deviceId: string, config: Partial<DeviceConfig>) {
		try {
			const res = await fetch(`/api/devices/${deviceId}/config`, {
				method: 'PUT',
				headers: { 'Content-Type': 'application/json' },
				body: JSON.stringify(config)
			});
			if (!res.ok) throw new Error('Configuratie opslaan mislukt');
			const updatedConfig: DeviceConfig = await res.json();

			devices = devices.map((d) =>
				d.id === deviceId ? { ...d, config: updatedConfig } : d
			);
		} catch (e) {
			error = e instanceof Error ? e.message : 'Onbekende fout';
		}
	}
};
