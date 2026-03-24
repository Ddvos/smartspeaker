export interface Topic {
	id: string;
	name: string;
	conversationCount: number;
	lastActivity: Date;
	sparkline: number[]; // 7 data points
}

export interface SearchResult {
	id: string;
	text: string;
	conversationId: string;
	conversationTitle: string;
	date: Date;
	relevance: number; // 0-1
}

export interface TimelineEntry {
	id: string;
	date: Date;
	conversationId: string;
	title: string;
	mood: import('./conversation').Mood;
	tags: string[];
}
