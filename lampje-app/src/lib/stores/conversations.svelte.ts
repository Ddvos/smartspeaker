import type { Conversation } from '$lib/types';
import { mockConversations } from '$lib/data/mock-conversations';

let conversations = $state<Conversation[]>(mockConversations);
let activeId = $state<string | null>(null);
let searchTerm = $state('');
let activeFilter = $state<string>('all'); // 'all', 'today', 'week', 'favorites'
let activeTagFilter = $state<string | null>(null);

export const conversationsStore = {
	get all() {
		return conversations;
	},
	get activeId() {
		return activeId;
	},
	get searchTerm() {
		return searchTerm;
	},
	get activeFilter() {
		return activeFilter;
	},
	get activeTagFilter() {
		return activeTagFilter;
	},

	get filtered() {
		let result = conversations;

		if (searchTerm) {
			const term = searchTerm.toLowerCase();
			result = result.filter(
				(c) =>
					c.title.toLowerCase().includes(term) ||
					c.tags.some((t) => t.toLowerCase().includes(term))
			);
		}

		if (activeFilter === 'today') {
			const today = new Date();
			today.setHours(0, 0, 0, 0);
			result = result.filter((c) => c.updatedAt >= today);
		} else if (activeFilter === 'week') {
			const weekAgo = new Date();
			weekAgo.setDate(weekAgo.getDate() - 7);
			result = result.filter((c) => c.updatedAt >= weekAgo);
		}

		if (activeTagFilter) {
			result = result.filter((c) => c.tags.includes(activeTagFilter!));
		}

		return result;
	},

	get active(): Conversation | null {
		if (!activeId) return null;
		return conversations.find((c) => c.id === activeId) ?? null;
	},

	setActive(id: string | null) {
		activeId = id;
	},
	setSearch(term: string) {
		searchTerm = term;
	},
	setFilter(filter: string) {
		activeFilter = filter;
	},
	setTagFilter(tag: string | null) {
		activeTagFilter = tag;
	}
};
