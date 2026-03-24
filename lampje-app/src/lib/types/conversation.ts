export type Mood = 'focused' | 'happy' | 'curious' | 'playful' | 'neutral';
export type MessageRole = 'user' | 'assistant';

export interface Message {
	id: string;
	role: MessageRole;
	content: string;
	timestamp: Date;
	audioUrl?: string;
}

export interface Conversation {
	id: string;
	title: string;
	createdAt: Date;
	updatedAt: Date;
	mood: Mood;
	duration: number; // seconds
	messages: Message[];
	summary?: string;
	tags: string[];
}
