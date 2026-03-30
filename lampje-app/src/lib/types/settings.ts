export interface UserSettings {
	hasGeminiApiKey: boolean;
	geminiApiKeyMasked: string;
	geminiApiKeyStatus: 'untested' | 'valid' | 'invalid';
	defaultModel: string;
	defaultVoice: string;
	defaultSystemPrompt: string;
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
