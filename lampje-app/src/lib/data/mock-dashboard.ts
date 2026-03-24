import type { Stat, ActivityDay, TopTopic } from '$lib/types';

export const mockStats: Stat[] = [
	{
		label: 'GESPREKKEN',
		value: '47',
		trend: '+12 deze week',
		icon: 'message-square'
	},
	{
		label: 'SPREEKTIJD',
		value: '8u 23m',
		trend: 'gem. 11 min/gesprek',
		icon: 'clock'
	},
	{
		label: 'ONDERWERPEN',
		value: '15',
		trend: '+3 deze week',
		icon: 'hash'
	},
	{
		label: 'HERINNERINGEN',
		value: '284',
		trend: '98% doorzoekbaar',
		icon: 'brain'
	}
];

export const mockActivity: ActivityDay[] = [
	{ label: 'Ma', minutes: 45, conversations: 6 },
	{ label: 'Di', minutes: 32, conversations: 4 },
	{ label: 'Wo', minutes: 67, conversations: 8 },
	{ label: 'Do', minutes: 28, conversations: 3 },
	{ label: 'Vr', minutes: 52, conversations: 7 },
	{ label: 'Za', minutes: 15, conversations: 2 },
	{ label: 'Zo', minutes: 38, conversations: 5 }
];

export const mockTopics: TopTopic[] = [
	{ name: 'Domotica & IoT', count: 12, color: '#81ecff' },
	{ name: 'Machine Learning', count: 9, color: '#00d4ec' },
	{ name: 'Koken & Recepten', count: 7, color: '#ff7520' },
	{ name: 'Muziek', count: 6, color: '#a68cff' },
	{ name: 'Reisplannen', count: 4, color: '#ff716c' }
];
