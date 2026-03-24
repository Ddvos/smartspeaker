export interface Stat {
	label: string;
	value: string;
	trend?: string;
	icon?: string;
}

export interface ActivityDay {
	label: string; // "Ma", "Di", "Wo", etc.
	minutes: number;
	conversations: number;
}

export interface TopTopic {
	name: string;
	count: number;
	color: string; // hex color for the bar
}
