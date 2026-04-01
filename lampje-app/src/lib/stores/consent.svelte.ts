import posthog from 'posthog-js';
import { browser } from '$app/environment';

type ConsentStatus = 'pending' | 'granted' | 'denied';

const STORAGE_KEY = 'lampje_consent';

function readPersistedChoice(): ConsentStatus {
	if (!browser) return 'pending';
	try {
		const stored = localStorage.getItem(STORAGE_KEY);
		if (stored === 'granted') return 'granted';
		if (stored === 'denied') return 'denied';
	} catch {
		// localStorage unavailable
	}
	return 'pending';
}

function persistChoice(choice: ConsentStatus) {
	try {
		localStorage.setItem(STORAGE_KEY, choice);
	} catch {
		// localStorage unavailable
	}
}

// Read consent eagerly so the banner never flashes on page load
let status = $state<ConsentStatus>(readPersistedChoice());
let sessionUser: { id: string; email?: string | null; name?: string | null } | null = $state(null);

export const consentStore = {
	get status() {
		return status;
	},

	init(user?: { id: string; email?: string | null; name?: string | null } | null) {
		sessionUser = user ?? null;

		// Sync PostHog with the persisted consent choice
		if (status === 'granted') {
			posthog.opt_in_capturing();
		} else if (status === 'denied') {
			posthog.opt_out_capturing();
		}
	},

	accept() {
		posthog.opt_in_capturing();
		status = 'granted';
		persistChoice('granted');

		if (sessionUser) {
			posthog.identify(sessionUser.id, {
				email: sessionUser.email,
				name: sessionUser.name
			});
		}
	},

	decline() {
		posthog.opt_out_capturing();
		status = 'denied';
		persistChoice('denied');
	},

	withdraw() {
		posthog.opt_out_capturing();
		posthog.reset();
		status = 'denied';
		persistChoice('denied');
	}
};
