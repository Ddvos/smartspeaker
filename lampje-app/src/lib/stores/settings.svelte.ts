import type { UserSettings } from '$lib/types';

const defaultSettings: UserSettings = {
	hasGeminiApiKey: false,
	geminiApiKeyMasked: '',
	geminiApiKeyStatus: 'untested',
	defaultModel: 'gemini-2.0-flash-live',
	defaultVoice: 'puck',
	defaultSystemPrompt: ''
};

let settings = $state<UserSettings>({ ...defaultSettings });
let loading = $state(false);
let saving = $state(false);
let error = $state<string | null>(null);

let debounceTimer: ReturnType<typeof setTimeout> | null = null;

function debouncedSave(updates: Record<string, unknown>) {
	if (debounceTimer) clearTimeout(debounceTimer);
	debounceTimer = setTimeout(() => save(updates), 300);
}

async function save(updates: Record<string, unknown>) {
	saving = true;
	error = null;
	try {
		const res = await fetch('/api/settings', {
			method: 'PUT',
			headers: { 'Content-Type': 'application/json' },
			body: JSON.stringify(updates)
		});
		if (!res.ok) throw new Error('Failed to save settings');
		const data: UserSettings = await res.json();
		settings = data;
	} catch (e) {
		error = e instanceof Error ? e.message : 'Unknown error';
	} finally {
		saving = false;
	}
}

export const settingsStore = {
	get current() {
		return settings;
	},
	get loading() {
		return loading;
	},
	get saving() {
		return saving;
	},
	get error() {
		return error;
	},

	init(data: UserSettings) {
		settings = data;
	},

	async load() {
		loading = true;
		error = null;
		try {
			const res = await fetch('/api/settings');
			if (!res.ok) throw new Error('Failed to load settings');
			settings = await res.json();
		} catch (e) {
			error = e instanceof Error ? e.message : 'Unknown error';
		} finally {
			loading = false;
		}
	},

	async saveApiKey(key: string) {
		await save({ geminiApiKey: key });
	},

	async testApiKey(): Promise<boolean> {
		saving = true;
		error = null;
		try {
			const res = await fetch('/api/settings/test-key', { method: 'POST' });
			if (!res.ok) throw new Error('Failed to test key');
			const data = await res.json();
			// Reload settings to get updated status
			await this.load();
			return data.valid;
		} catch (e) {
			error = e instanceof Error ? e.message : 'Unknown error';
			return false;
		} finally {
			saving = false;
		}
	},

	updateModel(modelId: string) {
		settings = { ...settings, defaultModel: modelId };
		save({ defaultModel: modelId });
	},

	updateVoice(voiceId: string) {
		settings = { ...settings, defaultVoice: voiceId };
		save({ defaultVoice: voiceId });
	},

	updateSystemPrompt(prompt: string) {
		settings = { ...settings, defaultSystemPrompt: prompt };
		debouncedSave({ defaultSystemPrompt: prompt });
	}
};
