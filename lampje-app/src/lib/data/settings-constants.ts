import type { AiModel, VoiceOption } from '$lib/types';

export const DEFAULT_SYSTEM_PROMPT =
	'Je bent Lampje, een vriendelijke en behulpzame slimme lamp. Je spreekt Nederlands en helpt de gebruiker met vragen, taken en gezellig kletsen. Houd je antwoorden kort en natuurlijk.';

export const models: AiModel[] = [
	{
		id: 'gemini-2.0-flash-live',
		name: 'Gemini 2.0 Flash',
		description: 'Snel en efficient, ideaal voor dagelijks gebruik en korte gesprekken.',
		recommended: true,
		badge: 'Snel'
	}
];

export const voices: VoiceOption[] = [
	{ id: 'zephyr', name: 'Zephyr', style: 'Helder' },
	{ id: 'puck', name: 'Puck', style: 'Vrolijk' },
	{ id: 'charon', name: 'Charon', style: 'Informatief' },
	{ id: 'kore', name: 'Kore', style: 'Vastberaden' },
	{ id: 'fenrir', name: 'Fenrir', style: 'Enthousiast' },
	{ id: 'leda', name: 'Leda', style: 'Jeugdig' },
	{ id: 'orus', name: 'Orus', style: 'Vastberaden' },
	{ id: 'aoede', name: 'Aoede', style: 'Luchtig' },
	{ id: 'callirrhoe', name: 'Callirrhoe', style: 'Ontspannen' },
	{ id: 'autonoe', name: 'Autonoe', style: 'Helder' },
	{ id: 'enceladus', name: 'Enceladus', style: 'Zacht' },
	{ id: 'iapetus', name: 'Iapetus', style: 'Duidelijk' },
	{ id: 'umbriel', name: 'Umbriel', style: 'Ontspannen' },
	{ id: 'algieba', name: 'Algieba', style: 'Vloeiend' },
	{ id: 'despina', name: 'Despina', style: 'Vloeiend' },
	{ id: 'erinome', name: 'Erinome', style: 'Duidelijk' },
	{ id: 'algenib', name: 'Algenib', style: 'Schor' },
	{ id: 'rasalgethi', name: 'Rasalgethi', style: 'Informatief' },
	{ id: 'laomedeia', name: 'Laomedeia', style: 'Vrolijk' },
	{ id: 'achernar', name: 'Achernar', style: 'Zacht' },
	{ id: 'alnilam', name: 'Alnilam', style: 'Vastberaden' },
	{ id: 'schedar', name: 'Schedar', style: 'Gelijkmatig' },
	{ id: 'gacrux', name: 'Gacrux', style: 'Volwassen' },
	{ id: 'pulcherrima', name: 'Pulcherrima', style: 'Vooruitstrevend' },
	{ id: 'achird', name: 'Achird', style: 'Vriendelijk' },
	{ id: 'zubenelgenubi', name: 'Zubenelgenubi', style: 'Casual' },
	{ id: 'vindemiatrix', name: 'Vindemiatrix', style: 'Zachtaardig' },
	{ id: 'sadachbia', name: 'Sadachbia', style: 'Levendig' },
	{ id: 'sadaltager', name: 'Sadaltager', style: 'Deskundig' },
	{ id: 'sulafat', name: 'Sulafat', style: 'Warm' }
];
