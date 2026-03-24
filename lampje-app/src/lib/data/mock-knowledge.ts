import type { Topic, SearchResult, TimelineEntry } from '$lib/types';

const now = new Date();
const daysAgo = (d: number) => new Date(now.getTime() - d * 86400000);
const hoursAgo = (h: number) => new Date(now.getTime() - h * 3600000);

export const mockTopicCards: Topic[] = [
	{
		id: 'topic-1',
		name: 'Domotica & IoT',
		conversationCount: 12,
		lastActivity: hoursAgo(3),
		sparkline: [3, 5, 2, 7, 4, 8, 6]
	},
	{
		id: 'topic-2',
		name: 'Machine Learning',
		conversationCount: 9,
		lastActivity: daysAgo(2),
		sparkline: [2, 4, 6, 3, 5, 7, 4]
	},
	{
		id: 'topic-3',
		name: 'Koken & Recepten',
		conversationCount: 7,
		lastActivity: hoursAgo(8),
		sparkline: [1, 3, 2, 4, 2, 5, 3]
	},
	{
		id: 'topic-4',
		name: 'Muziek & Audio',
		conversationCount: 6,
		lastActivity: daysAgo(3),
		sparkline: [2, 1, 3, 2, 4, 2, 3]
	},
	{
		id: 'topic-5',
		name: 'Reisplannen',
		conversationCount: 4,
		lastActivity: daysAgo(4),
		sparkline: [0, 1, 0, 2, 1, 3, 2]
	},
	{
		id: 'topic-6',
		name: 'Programmeren',
		conversationCount: 8,
		lastActivity: daysAgo(6),
		sparkline: [4, 3, 5, 2, 6, 3, 4]
	},
	{
		id: 'topic-7',
		name: 'Gezondheid',
		conversationCount: 5,
		lastActivity: daysAgo(11),
		sparkline: [1, 2, 1, 3, 1, 2, 2]
	},
	{
		id: 'topic-8',
		name: 'Energie & Duurzaamheid',
		conversationCount: 3,
		lastActivity: daysAgo(9),
		sparkline: [0, 1, 2, 1, 0, 2, 1]
	},
	{
		id: 'topic-9',
		name: 'Elektronica',
		conversationCount: 6,
		lastActivity: daysAgo(7),
		sparkline: [2, 3, 1, 4, 2, 3, 2]
	}
];

export const mockSearchResults: SearchResult[] = [
	{
		id: 'sr-1',
		text: 'Voor Zigbee-lampen raad ik Zigbee2MQTT aan boven ZHA. Het is flexibeler en heeft betere ondersteuning voor de meeste apparaten.',
		conversationId: 'conv-1',
		conversationTitle: 'Slimme verlichting instellen',
		date: hoursAgo(2),
		relevance: 0.95
	},
	{
		id: 'sr-2',
		text: 'Een echte carbonara bevat geen room. Het geheim zit in de techniek: eidooiers met pecorino en de warmte van de pasta.',
		conversationId: 'conv-2',
		conversationTitle: 'Pasta carbonara recept',
		date: hoursAgo(7),
		relevance: 0.88
	},
	{
		id: 'sr-3',
		text: 'Bij supervised learning geef je het model gelabelde data. Bij unsupervised learning zoekt het model zelf structuur en patronen.',
		conversationId: 'conv-4',
		conversationTitle: 'Machine learning uitleg',
		date: daysAgo(2),
		relevance: 0.82
	},
	{
		id: 'sr-4',
		text: 'De Japan Rail Pass voor 14 dagen kost ongeveer 450 euro maar bespaart enorm op treinkosten, vooral de Shinkansen.',
		conversationId: 'conv-6',
		conversationTitle: 'Reisplanning Japan',
		date: daysAgo(5),
		relevance: 0.76
	},
	{
		id: 'sr-5',
		text: 'Nederlandse CSV-bestanden gebruiken vaak een puntkomma als scheidingsteken. Gebruik pd.read_csv met sep=";" en decimal=",".',
		conversationId: 'conv-7',
		conversationTitle: 'Python debugging hulp',
		date: daysAgo(6),
		relevance: 0.71
	}
];

export const mockTimeline: TimelineEntry[] = [
	{
		id: 'tl-1',
		date: hoursAgo(2),
		conversationId: 'conv-1',
		title: 'Slimme verlichting instellen',
		mood: 'focused',
		tags: ['domotica', 'zigbee']
	},
	{
		id: 'tl-2',
		date: hoursAgo(7),
		conversationId: 'conv-2',
		title: 'Pasta carbonara recept',
		mood: 'happy',
		tags: ['koken', 'italiaans']
	},
	{
		id: 'tl-3',
		date: daysAgo(1),
		conversationId: 'conv-3',
		title: 'Weer vandaag in Amsterdam',
		mood: 'neutral',
		tags: ['weer', 'amsterdam']
	},
	{
		id: 'tl-4',
		date: daysAgo(2),
		conversationId: 'conv-4',
		title: 'Machine learning uitleg',
		mood: 'curious',
		tags: ['ai', 'machine-learning']
	},
	{
		id: 'tl-5',
		date: daysAgo(3),
		conversationId: 'conv-5',
		title: 'Spotify afspeellijst maken',
		mood: 'playful',
		tags: ['muziek', 'spotify']
	},
	{
		id: 'tl-6',
		date: daysAgo(4),
		conversationId: 'conv-6',
		title: 'Reisplanning Japan',
		mood: 'curious',
		tags: ['reizen', 'japan']
	},
	{
		id: 'tl-7',
		date: daysAgo(6),
		conversationId: 'conv-7',
		title: 'Python debugging hulp',
		mood: 'focused',
		tags: ['programmeren', 'python']
	},
	{
		id: 'tl-8',
		date: daysAgo(7),
		conversationId: 'conv-8',
		title: 'Verjaardagscadeau ideeen',
		mood: 'happy',
		tags: ['cadeau', 'verjaardag']
	},
	{
		id: 'tl-9',
		date: daysAgo(9),
		conversationId: 'conv-9',
		title: 'Energiebesparing tips',
		mood: 'neutral',
		tags: ['energie', 'besparen']
	},
	{
		id: 'tl-10',
		date: daysAgo(11),
		conversationId: 'conv-10',
		title: 'Mediteren voor beginners',
		mood: 'playful',
		tags: ['gezondheid', 'meditatie']
	},
	{
		id: 'tl-11',
		date: daysAgo(10),
		conversationId: 'conv-1',
		title: 'Zigbee netwerk opzetten',
		mood: 'focused',
		tags: ['domotica', 'zigbee']
	},
	{
		id: 'tl-12',
		date: daysAgo(13),
		conversationId: 'conv-4',
		title: 'Wat is deep learning?',
		mood: 'curious',
		tags: ['ai', 'machine-learning']
	}
];
