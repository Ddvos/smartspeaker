import posthog from 'posthog-js';

type ConsentStatus = 'pending' | 'granted' | 'denied';

let status = $state<ConsentStatus>('pending');
let sessionUser: { id: string; email?: string | null; name?: string | null } | null = $state(null);

export const consentStore = {
	get status() {
		return status;
	},

	init(user?: { id: string; email?: string | null; name?: string | null } | null) {
		sessionUser = user ?? null;

		const explicitStatus = posthog.get_explicit_consent_status();
		if (explicitStatus === 'granted') {
			status = 'granted';
		} else if (explicitStatus === 'denied') {
			status = 'denied';
		} else {
			status = 'pending';
		}
	},

	accept() {
		posthog.opt_in_capturing();
		status = 'granted';

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
	},

	withdraw() {
		posthog.opt_out_capturing();
		posthog.reset();
		status = 'denied';
	}
};
