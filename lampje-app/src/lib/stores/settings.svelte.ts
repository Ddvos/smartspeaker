import type { AppSettings } from '$lib/types';
import { mockSettings } from '$lib/data/mock-settings';

let settings = $state<AppSettings>({ ...mockSettings });

export const settingsStore = {
	get current() {
		return settings;
	},

	updateModel(modelId: string) {
		settings = { ...settings, selectedModel: modelId };
	},
	updateVoice(voiceId: string) {
		settings = { ...settings, selectedVoice: voiceId };
	},
	updatePersonality(value: number) {
		settings = { ...settings, personality: value };
	},
	updateSystemPrompt(prompt: string) {
		settings = { ...settings, systemPrompt: prompt };
	},

	updateApiKey(provider: string, key: string) {
		settings = {
			...settings,
			apiKeys: settings.apiKeys.map((k) =>
				k.provider === provider ? { ...k, key, status: 'untested' as const } : k
			)
		};
	},

	async testApiKey(provider: string): Promise<boolean> {
		// Simulate API test
		return new Promise((resolve) => {
			setTimeout(() => {
				const success = Math.random() > 0.3;
				settings = {
					...settings,
					apiKeys: settings.apiKeys.map((k) =>
						k.provider === provider
							? { ...k, status: success ? ('connected' as const) : ('error' as const) }
							: k
					)
				};
				resolve(success);
			}, 1200);
		});
	}
};
