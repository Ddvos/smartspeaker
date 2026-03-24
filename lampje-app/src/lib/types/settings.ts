export interface ApiKeyConfig {
	provider: string;
	label: string;
	key: string;
	status: 'connected' | 'error' | 'untested';
}

export interface AiModel {
	id: string;
	name: string;
	description: string;
	recommended?: boolean;
	badge?: string;
}

export interface VoiceOption {
	id: string;
	name: string;
	style: string;
}

export interface AppSettings {
	apiKeys: ApiKeyConfig[];
	selectedModel: string;
	selectedVoice: string;
	personality: number; // 0 (zakelijk) to 100 (speels)
	systemPrompt: string;
}
