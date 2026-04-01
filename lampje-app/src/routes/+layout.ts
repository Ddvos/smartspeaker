import posthog from 'posthog-js';
import { browser } from '$app/environment';
import { consentStore } from '$lib/stores/consent.svelte';

export const load = async ({ data }) => {
	if (browser) {
		posthog.init('phc_3aUVIjKcHaatORIsyISwUjsGGEJdUyupj3MKGDFQrgy', {
			api_host: 'https://eu.i.posthog.com',
			capture_pageview: false,
			capture_pageleave: true,
			capture_exceptions: true,
			opt_out_capturing_by_default: true,
			cookieless_mode: 'on_reject'
		});

		const user = data.session?.user;
		consentStore.init(user?.id ? { id: user.id, email: user.email, name: user.name } : null);

		if (consentStore.status === 'granted' && user?.id) {
			posthog.identify(user.id, {
				email: user.email,
				name: user.name
			});
		} else if (!user) {
			posthog.reset();
		}
	}

	return data;
};
