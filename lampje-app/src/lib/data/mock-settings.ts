import type { AppSettings, AiModel, VoiceOption } from '$lib/types';

export const mockSettings: AppSettings = {
	apiKeys: [
		{
			provider: 'gemini',
			label: 'Google Gemini',
			key: '',
			status: 'untested'
		},
		{
			provider: 'openai',
			label: 'OpenAI',
			key: '',
			status: 'untested'
		},
		{
			provider: 'elevenlabs',
			label: 'ElevenLabs',
			key: '',
			status: 'untested'
		}
	],
	selectedModel: 'gemini-2.5-flash',
	selectedVoice: 'puck',
	personality: 50,
	systemPrompt:
		'Je bent Lampje, een slimme en vriendelijke assistent in een klein nachtlampje. Je praat in het Nederlands en helpt graag met allerlei vragen. Je bent nieuwsgierig, geduldig en hebt een vleugje humor. Je onthoudt eerdere gesprekken en verwijst daar soms naar om de gebruiker beter te helpen.'
};

export const mockModels: AiModel[] = [
	{
		id: 'gemini-2.5-flash',
		name: 'Gemini 2.5 Flash',
		description: 'Snel en efficient, ideaal voor dagelijks gebruik en korte gesprekken.',
		recommended: true,
		badge: 'Snel'
	},
	{
		id: 'gemini-2.5-pro',
		name: 'Gemini 2.5 Pro',
		description:
			'Krachtig model met diepgaand begrip, geschikt voor complexe taken en lange gesprekken.'
	},
	{
		id: 'claude-haiku',
		name: 'Claude Haiku',
		description: 'Compact en snel, perfect voor eenvoudige taken en snelle antwoorden.',
		badge: 'Compact'
	},
	{
		id: 'claude-sonnet',
		name: 'Claude Sonnet',
		description:
			'Uitgebalanceerd model met sterke redeneervaardigheden en creatieve mogelijkheden.'
	}
];

export const mockVoices: VoiceOption[] = [
	{
		id: 'puck',
		name: 'Puck',
		style: 'Warm en vriendelijk'
	},
	{
		id: 'kore',
		name: 'Kore',
		style: 'Helder en professioneel'
	},
	{
		id: 'fenrir',
		name: 'Fenrir',
		style: 'Diep en kalm'
	},
	{
		id: 'aoede',
		name: 'Aoede',
		style: 'Zacht en rustgevend'
	}
];
